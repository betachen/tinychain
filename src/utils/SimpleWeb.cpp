/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <tinychain/rest/rest.hpp>
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

