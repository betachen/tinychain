/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/rest.hpp>
#include <sstream>

// defualt
#define URI_PING ("^/ping(/?$)")
#define URI_ECHO ("^/echo(/?$)")

static const char* notfound = "{\"status\":404, \"reason\":\"URI or Method not found.\"}";

namespace tinychain
{

using namespace SimpleWeb;

RestServer::~RestServer() noexcept = default;

WebSocketServer::~WebSocketServer() noexcept = default;

void RestServer::installDefaultHandlers() {
  server_.resource[URI_PING]["GET"] = [this](RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream ss;
    ss << "Got ping from " << req->remote_endpoint_address() << ':' << req->remote_endpoint_port();
    rep->write(HttpStatusCode::success_ok, ss);
  };

  server_.resource[URI_ECHO]["POST"] = [this](RestResponsePtr rep, RestRequestPtr req) {
    rep->write(HttpStatusCode::success_ok, req->content.toStringView());
  };

  auto default_handler = [](RestResponsePtr rep, RestRequestPtr req) {
    rep->write(HttpStatusCode::client_error_not_found, notfound);
  };

  server_.default_resource["GET"] = default_handler;
  server_.default_resource["POST"] = default_handler;
}

void WebSocketServer::installDefaultHandlers() {

  auto& echo = server_.endpoint["^/echo/?$"];

  echo.on_message = [this](WsConnectionPtr conn, WsInMessagePtr inMsg) {
    auto out_message = inMsg->toStringView();
    conn->send(out_message, [](const SimpleWeb::error_code& ec) {
      if(ec) {
      log::info(__func__) <<"Server: Error sending message."<<"Error: "<<ec<<", error message:" << ec.message();
      }
    });
  };
}

} //tinychain

