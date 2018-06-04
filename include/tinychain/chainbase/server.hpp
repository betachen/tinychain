/*
 * Copyright (C) 2016 hao.chen.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#pragma once

#include "restful-cpp/common.hpp"

namespace rfcpp {
namespace server {

template <typename DerivedT>
class MgrSvr {
public:
    // Copy.
    MgrSvr(const MgrSvr&) = delete;
    MgrSvr& operator=(const MgrSvr&) = delete;

    // Move.
    MgrSvr(MgrSvr&&) = delete;
    MgrSvr& operator=(MgrSvr&&) = delete;

    // Bind
    mg_connection& bind(const char* addr)
    {
        auto* conn = mg_bind(&mgr_, addr, handler);
        if (!conn)
          throw std::runtime_error{"mg_bind() failed"};
        conn->user_data = this;
        return *conn;
    }

    // poll
    inline time_t poll(int milli) { return mg_mgr_poll(&mgr_, milli); }

protected:
    MgrSvr() noexcept { mg_mgr_init(&mgr_, this); }
    ~MgrSvr() noexcept { mg_mgr_free(&mgr_); }

private:

    static void handler(mg_connection* conn, int event, void* data)
    {
       http_message* hm = static_cast<http_message*>(data);
       websocket_message* ws = static_cast<websocket_message*>(data);
       auto* self = static_cast<DerivedT*>(conn->user_data);

       switch (event) {
       case MG_EV_CLOSE:{
            if (conn->flags & MG_F_IS_WEBSOCKET) {
                //self->websocketBroadcast(*conn, "left", 4);
            }else{
                conn->user_data = nullptr;
            }
            break;
        }
       case MG_EV_HTTP_REQUEST:{
            // logined, http request process
            if (mg_ncasecmp((&hm->uri)->p, "/api", 4u) == 0) {
                //self->httpRequest(*conn, hm);
            }else{
                //self->httpStatic(*conn, hm);
                conn->flags |= MG_F_SEND_AND_CLOSE;
            }
            break;
        }
        case MG_EV_WEBSOCKET_HANDSHAKE_DONE:{
            //self->websocketSend(conn, "connected", 9);
            break;
        }
        case MG_EV_WEBSOCKET_FRAME:{
            //self->websocketSend(*conn, ws);
            break;
        }
        case MG_EV_SSI_CALL:{
        }
        case MG_EV_TIMER:{
            //self->check_sessions();
            mg_set_timer(conn, mg_time() + 60.0);
            break;
        }
       }// switch
    }// handler

    mg_mgr mgr_;
};


class MyServer : public MgrSvr<MyServer>
{
public:
    explicit MyServer(const char* webroot)
    {
        memset(&httpoptions_, 0x00, sizeof(httpoptions_));
        httpoptions_.document_root = webroot;
    }
    ~MyServer() noexcept {}

    // Copy.
    MyServer(const MyServer& rhs) = delete;
    MyServer& operator=(const MyServer& rhs) = delete;

    // Move.
    MyServer(MyServer&&) = delete;
    MyServer& operator=(MyServer&&) = delete;

    //reset
    void reset(HttpMessage& data) noexcept;

    //request
    void httpStatic (mg_connection& nc, HttpMessage data);
    void httpRequest (mg_connection& nc, HttpMessage data);
    void httpRpcRequest (mg_connection& nc, HttpMessage data);
    void websocketBroadcast (mg_connection& nc, const char* msg, size_t len);
    void websocketSend(mg_connection* nc, const char* msg, size_t len);
    void websocketSend(mg_connection& nc, WebsocketMessage ws);

    std::pair<std::string_view, std::string_view> user_check(HttpMessage &data)
    {
       auto user = data.header("user");
       auto password = data.header("pass");
    
       if (user.empty() || password.empty()) {
      	    throw std::logic_error("authorisation required");
       }
       return std::make_pair(user, password);
    }
    
private:
    enum : int {
        // Method values are represented as powers of two for simplicity.
        MethodGet = 1 << 0,
        MethodPost = 1 << 1,
        MethodPut = 1 << 2,
        MethodDelete = 1 << 3,
        // Method value mask.
        MethodMask = MethodGet | MethodPost | MethodPut | MethodDelete,
        
        // Subsequent bits represent matching components.
        MatchMethod = 1 << 4,
        MatchUri = 1 << 5,
        // Match result mask.
        MatchMask = MatchMethod | MatchUri
    };

    bool isSet(int bs) const noexcept { return (state_ & bs) == bs; }

    // http
    mg_serve_http_opts httpoptions_;

    // config
    OStream out_;
    Tokeniser<'/'> uri_;
    int state_{0};
};


} // server
} // rfcpp


