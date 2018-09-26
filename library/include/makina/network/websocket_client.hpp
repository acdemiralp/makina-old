#ifndef MAKINA_NETWORK_WEBSOCKET_CLIENT_HPP
#define MAKINA_NETWORK_WEBSOCKET_CLIENT_HPP

#include <cstdint>
#include <functional>
#include <map>
#include <string>

#include <uWS.h>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT websocket_client : public system
{
public:
  explicit websocket_client  (const std::string& ip, std::uint16_t port);
  websocket_client           (const websocket_client&  that) = delete ;
  websocket_client           (      websocket_client&& temp) = default;
  virtual ~websocket_client()                                = default;
  websocket_client& operator=(const websocket_client&  that) = delete ;
  websocket_client& operator=(      websocket_client&& temp) = default;

  void send        (const std::string& id, const std::string&                             content  = std::string()) const;
  void set_callback(const std::string& id, const std::function<void(const std::string&)>& callback = nullptr      );

protected:
  void prepare     (                             scene* scene) override;
  void update      (frame_timer::duration delta, scene* scene) override;

  std::string                                                    uri_      ;
  uWS::Hub                                                       hub_      ;
  uWS::WebSocket<false>*                                         socket_   = nullptr;
  std::map<std::string, std::function<void(const std::string&)>> callbacks_;
};
}

#endif