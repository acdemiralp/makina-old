#include <makina/network/websocket_server.hpp>

#include <nlohmann/json.hpp>

#include <makina/core/logger.hpp>
#include <makina/utility/string.hpp>

namespace mak
{
websocket_server::websocket_server (const std::uint16_t port) : port_(port)
{

}

std::vector<uWS::WebSocket<true>*> websocket_server::sockets()
{
  std::vector<uWS::WebSocket<true>*> sockets;
  hub_.getDefaultGroup<true>().forEach([&sockets] (uWS::WebSocket<true>* socket) { sockets.push_back(socket); });
  return sockets;
}

void websocket_server::send        (uWS::WebSocket<true>* target, const std::string& id, const std::string& content) const
{
  nlohmann::json message;
  message["id"     ] = id     ;
  message["content"] = content;
  auto message_string = base64_encode(compress(message.dump()));
  target ? target->send(message_string.c_str()) : logger->error("Failed to send message: Socket is nullptr.");
}
void websocket_server::broadcast   (                              const std::string& id, const std::string& content)
{
  hub_.getDefaultGroup<true>().forEach([&] (uWS::WebSocket<true>* socket) { send(socket, id, content); });
}
void websocket_server::set_callback(                              const std::string& id, const std::function<void(uWS::WebSocket<true>*, const std::string&)>& callback)
{
  if (callback) callbacks_[id] = callback;
  else          callbacks_.erase(id);
}

void websocket_server::prepare     (                             scene* scene)
{
  hub_.onConnection   ([&] (uWS::WebSocket<true>* socket, uWS::HttpRequest request)
  {
    logger->info("Client {} connected.", socket->getAddress().address);

    const auto event_name = "connection";
    if (callbacks_.count(event_name))
      callbacks_.at(event_name)(socket, nlohmann::json());
  });
  hub_.onMessage      ([&] (uWS::WebSocket<true>* socket, char*            data   , std::size_t length , uWS::OpCode op_code)
  {
    logger->info("Client {} sent message.", socket->getAddress().address);
    
    const auto message = nlohmann::json::parse(decompress(base64_decode(std::string(data, length))));
    const auto id      = message["id"     ].get<std::string>();
    const auto content = message["content"].get<std::string>();
    if (callbacks_.count(id)) 
      callbacks_.at(id)(socket, content);
  });
  hub_.onDisconnection([&] (uWS::WebSocket<true>* socket, std::int32_t     code   , char*       message, std::size_t length )
  {
    logger->info("Client {} disconnected.", socket->getAddress().address);

    const auto event_name = "disconnection";
    if (callbacks_.count(event_name))
      callbacks_.at(event_name)(socket, nlohmann::json());
  });
  hub_.listen         (port_);
  logger->info("Server listening at port {}.", port_);
}
void websocket_server::update      (frame_timer::duration delta, scene* scene)
{
  hub_.poll();
}
}