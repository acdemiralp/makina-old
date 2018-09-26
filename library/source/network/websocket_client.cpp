#include <makina/network/websocket_client.hpp>

#include <nlohmann/json.hpp>

#include <makina/core/logger.hpp>
#include <makina/utility/string.hpp>

namespace mak
{
websocket_client::websocket_client (const std::string& ip, const std::uint16_t port) : uri_("ws://" + ip + ":" + std::to_string(port))
{

}

void websocket_client::send        (const std::string& id, const std::string&                             content ) const
{
  nlohmann::json message;
  message["id"     ] = id     ;
  message["content"] = content;
  auto message_string = base64_encode(compress(message.dump()));
  socket_ ? socket_->send(message_string.c_str()) : logger->error("Failed to send message: Socket is nullptr.");
}
void websocket_client::set_callback(const std::string& id, const std::function<void(const std::string&)>& callback)
{
  if (callback) callbacks_[id] = callback;
  else          callbacks_.erase(id);
}

void websocket_client::prepare     (                             scene* scene)
{
  hub_.onConnection   ([&] (uWS::WebSocket<false>* socket, uWS::HttpRequest request)
  {
    logger->info("Connected to server.");

    socket_ = socket;

    const auto event_name = "connection";
    if (callbacks_.count(event_name))
      callbacks_.at(event_name)(nlohmann::json());
  });
  hub_.onMessage      ([&] (uWS::WebSocket<false>* socket, char*            data   , std::size_t length , uWS::OpCode op_code)
  {
    logger->info("Received message from server.");

    const auto message = nlohmann::json::parse(decompress(base64_decode(std::string(data, length))));
    const auto id      = message["id"     ].get<std::string>();
    const auto content = message["content"].get<std::string>();
    if (callbacks_.count(id)) 
      callbacks_.at(id)(content);
  });
  hub_.onDisconnection([&] (uWS::WebSocket<false>* socket, std::int32_t     code   , char*       message, std::size_t length )
  {
    logger->info("Disconnected from server.");

    socket_ = nullptr;

    const auto event_name = "disconnection";
    if (callbacks_.count(event_name))
      callbacks_.at(event_name)(nlohmann::json());

    hub_.connect(uri_);
  });
  hub_.connect        (uri_);
}
void websocket_client::update      (frame_timer::duration delta, scene* scene)
{
  hub_.poll();
}
}