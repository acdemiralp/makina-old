#ifndef MAKINA_NETWORK_WEBSOCKET_SERVER_HPP
#define MAKINA_NETWORK_WEBSOCKET_SERVER_HPP

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <uWS.h>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT websocket_server : public system
{
public:
  explicit websocket_server  (std::uint16_t port);
  websocket_server           (const websocket_server&  that) = delete ;
  websocket_server           (      websocket_server&& temp) = default;
  virtual ~websocket_server()                                = default;
  websocket_server& operator=(const websocket_server&  that) = delete ;
  websocket_server& operator=(      websocket_server&& temp) = default;

  std::vector<uWS::WebSocket<true>*> sockets();

  void send        (uWS::WebSocket<true>* target, const std::string& id, const std::string& content = std::string()) const;
  void broadcast   (                              const std::string& id, const std::string& content = std::string());
  void set_callback(                              const std::string& id, const std::function<void(uWS::WebSocket<true>*, const std::string&)>& callback = nullptr);

protected:
  void prepare     (                             scene* scene) override;
  void update      (frame_timer::duration delta, scene* scene) override;

  std::uint16_t                                                                         port_     ;
  uWS::Hub                                                                              hub_      ;
  std::map<std::string, std::function<void(uWS::WebSocket<true>*, const std::string&)>> callbacks_;
};
}

#endif