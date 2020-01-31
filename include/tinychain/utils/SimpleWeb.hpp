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
 
#pragma once

#include <functional>
#include <memory>
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/utils/threadpool.hpp>
#include <contrib/simpleweb/simpleweb.hpp>

namespace tinychain {

//HTTP
using HttpServer      = SimpleWeb::Server<SimpleWeb::HTTP> ;
using HttpClient      = SimpleWeb::Client<SimpleWeb::HTTP> ;
using HttpStatusCode  = SimpleWeb::StatusCode;
using RestRequestPtr  = std::shared_ptr<HttpServer::Request> ;
using RestResponsePtr = std::shared_ptr<HttpServer::Response> ;

//WS
using WsServer      = SimpleWeb::SocketServer<SimpleWeb::WS> ;
using WsClient      = SimpleWeb::SocketClient<SimpleWeb::WS> ;
using WsConnectionPtr = std::shared_ptr<WsServer::Connection>;
using WsInMessagePtr = std::shared_ptr<WsServer::InMessage>;

////////////////////////////////////////////////////////////////
template <class T>
class SimpleServer {

public:
    explicit SimpleServer(size_t poolSize, bool enableInnerIo = false) noexcept
      : enableInnerIo_{enableInnerIo}, isRunning_{false}
    {
      server_.config.port = 8000; //default port

      if (enableInnerIo_) {
        server_.config.thread_pool_size = poolSize;
      } else {
        //simpleweb default to 1 thread, set up as 0 to disable it.
        server_.config.thread_pool_size = 0;
        // 将io_service从simpleweb复制一份到此处则可用自己控制io_service
        // https://gitlab.com/eidheim/Simple-Web-Server/blob/master/server_http.hpp#L395
        server_.io_service = wokerPool_.service();
        wokerPool_.spawn(poolSize);
      }
    }

    //move
    SimpleServer(SimpleServer&&) = delete;
    SimpleServer& operator=(SimpleServer&&) = delete;
   
    //copy
    SimpleServer(const SimpleServer&) = delete;
    SimpleServer& operator=(const SimpleServer&) = delete;

    virtual ~SimpleServer() noexcept { stop(); }

    void bind(uint16_t port)
    {
      server_.config.port = port;
      installHandlers();
    }

    bool start()
    {
      if(isRunning_) return false;

      if (!enableInnerIo_) {
        // 直接回收,因为禁用了simpleWeb本身的io_service->run
        //simpleBooter_->detach();
        server_.start();
        isRunning_ = true;
        return isRunning_;

      } else {
        simpleBooter_ = std::make_unique<std::thread>([this]() {
          server_.start();
          isRunning_ = true;
          log::info(__func__) << "SimpleServer started.";
        });
        if(!simpleBooter_) {
          log::warning(__func__) << "Join failed, stop SimpleServer...";
          server_.stop();
          isRunning_ = false;
        }
        return isRunning_;
      }
    }

    void join()
    {
      if (!enableInnerIo_) {
        wokerPool_.join();
      } else {
        assert(simpleBooter_);
        if (simpleBooter_ && simpleBooter_->joinable()) {
          simpleBooter_->join();
        }
      }
    }

    void stop()
    {
      bool tmp = isRunning_;
      isRunning_ = false;
      if (tmp) {
        log::info(__func__) << "stop SimpleServer socket connections...";
        server_.stop();
        log::info(__func__) << "shutdown SimpleServer io_service...";
        wokerPool_.shutdown();
        log::info(__func__) << "SimpleServer status(stopped): " << isRunning_.load();
      }
    }

    virtual void installHandlers() = 0;

protected:
    mutable std::atomic<bool> isRunning_;
    T server_;
    threadpool wokerPool_;

    const bool enableInnerIo_;
    std::unique_ptr<std::thread> simpleBooter_;
};

class RestServer : public SimpleServer<HttpServer>
{
public:
    RestServer(size_t poolSize, bool enableInnerIo = false) noexcept
      :SimpleServer{poolSize, enableInnerIo}
    {
      installDefaultHandlers();
    }

    //move
    RestServer(RestServer&&) = delete;
    RestServer& operator=(RestServer&&) = delete;

    //copy
    RestServer(const RestServer&) = delete;
    RestServer& operator=(const RestServer&) = delete;

    ~RestServer() noexcept override;

    void installDefaultHandlers();

    void installHandlers() override {} // FIXME: should be 0
};


class WebSocketServer : public SimpleServer<WsServer>
{
public:
    WebSocketServer(size_t poolSize, bool enableInnerIo = false) noexcept
      :SimpleServer{poolSize, enableInnerIo}
    {
      installDefaultHandlers();
    }

    //move
    WebSocketServer(WebSocketServer&&) = delete;
    WebSocketServer& operator=(WebSocketServer&&) = delete;

    //copy
    WebSocketServer(const WebSocketServer&) = delete;
    WebSocketServer& operator=(const WebSocketServer&) = delete;

    ~WebSocketServer() noexcept override;

    void installDefaultHandlers();

    void installHandlers() override {} // FIXME: should be 0
};

} // swirly
