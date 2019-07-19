#ifndef SIMPLE_WEB_CLIENT_HTTP_HPP
#define SIMPLE_WEB_CLIENT_HTTP_HPP

#include "asio_compatibility.hpp"
#include "mutex.hpp"
#include "utility.hpp"
#include <limits>
#include <random>
#include <unordered_set>
#include <vector>

namespace SimpleWeb {
  class HeaderEndMatch {
    int crlfcrlf = 0;
    int lflf = 0;

  public:
    std::pair<asio::buffers_iterator<asio::const_buffers_1>, bool> operator()(asio::buffers_iterator<asio::const_buffers_1> begin, asio::buffers_iterator<asio::const_buffers_1> end) {
      auto it = begin;
      for(; it != end; ++it) {
        if(*it == '\n') {
          if(crlfcrlf == 1)
            ++crlfcrlf;
          else if(crlfcrlf == 2)
            crlfcrlf = 0;
          else if(crlfcrlf == 3)
            return {++it, true};
          if(lflf == 0)
            ++lflf;
          else if(lflf == 1)
            return {++it, true};
        }
        else if(*it == '\r') {
          if(crlfcrlf == 0)
            ++crlfcrlf;
          else if(crlfcrlf == 2)
            ++crlfcrlf;
          else
            crlfcrlf = 0;
          lflf = 0;
        }
        else {
          crlfcrlf = 0;
          lflf = 0;
        }
      }
      return {it, false};
    }
  };
} // namespace SimpleWeb
#ifndef USE_STANDALONE_ASIO
namespace boost {
#endif
  namespace asio {
    template <> struct is_match_condition<SimpleWeb::HeaderEndMatch> : public std::true_type {};
  } // namespace asio
#ifndef USE_STANDALONE_ASIO
} // namespace boost
#endif

namespace SimpleWeb {
  template <class socket_type>
  class Client;

  template <class socket_type>
  class ClientBase {
  public:
    class Content : public std::istream {
      friend class ClientBase<socket_type>;

    public:
      std::size_t size() noexcept {
        return streambuf.size();
      }
      /// Convenience function to return std::string. The stream buffer is consumed.
      std::string string() noexcept {
        try {
          std::string str;
          auto size = streambuf.size();
          str.resize(size);
          read(&str[0], static_cast<std::streamsize>(size));
          return str;
        }
        catch(...) {
          return std::string();
        }
      }

      /// CHENHAO FIX
      std::string_view toStringView() noexcept {
        return {boost::asio::buffer_cast<const char*>(streambuf.data()), streambuf.size()};
      }

    private:
      asio::streambuf &streambuf;
      Content(asio::streambuf &streambuf) noexcept : std::istream(&streambuf), streambuf(streambuf) {}
    };

    class Response {
      friend class ClientBase<socket_type>;
      friend class Client<socket_type>;

      asio::streambuf streambuf;

      Response(std::size_t max_response_streambuf_size) noexcept : streambuf(max_response_streambuf_size), content(streambuf) {}

    public:
      std::string http_version, status_code;

      Content content;

      CaseInsensitiveMultimap header;
    };

    class Config {
      friend class ClientBase<socket_type>;

    private:
      Config() noexcept {}

    public:
      /// Set timeout on requests in seconds. Default value: 0 (no timeout).
      long timeout = 0;
      /// Set connect timeout in seconds. Default value: 0 (Config::timeout is then used instead).
      long timeout_connect = 0;
      /// Maximum size of response stream buffer. Defaults to architecture maximum.
      /// Reaching this limit will result in a message_size error code.
      std::size_t max_response_streambuf_size = std::numeric_limits<std::size_t>::max();
      /// Set proxy server (server:port)
      std::string proxy_server;
    };

  protected:
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
      template <typename... Args>
      Connection(std::shared_ptr<ScopeRunner> handler_runner_, long timeout, Args &&... args) noexcept
          : handler_runner(std::move(handler_runner_)), timeout(timeout), socket(new socket_type(std::forward<Args>(args)...)) {}

      std::shared_ptr<ScopeRunner> handler_runner;
      long timeout;

      std::unique_ptr<socket_type> socket; // Socket must be unique_ptr since asio::ssl::stream<asio::ip::tcp::socket> is not movable
      bool in_use = false;
      bool event_stream = false;
      bool attempt_reconnect = true;

      std::unique_ptr<asio::steady_timer> timer;

      void close() noexcept {
        error_code ec;
        socket->lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        socket->lowest_layer().cancel(ec);
      }

      void set_timeout(long seconds = 0) noexcept {
        if(seconds == 0)
          seconds = timeout;
        if(seconds == 0) {
          timer = nullptr;
          return;
        }
        timer = std::unique_ptr<asio::steady_timer>(new asio::steady_timer(get_socket_executor(*socket), std::chrono::seconds(seconds)));
        std::weak_ptr<Connection> self_weak(this->shared_from_this()); // To avoid keeping Connection instance alive longer than needed
        timer->async_wait([self_weak](const error_code &ec) {
          if(!ec) {
            if(auto self = self_weak.lock())
              self->close();
          }
        });
      }

      void cancel_timeout() noexcept {
        if(timer) {
          try {
            timer->cancel();
          }
          catch(...) {
          }
        }
      }
    };

    class Session {
    public:
      Session(std::size_t max_response_streambuf_size, std::shared_ptr<Connection> connection_, std::unique_ptr<asio::streambuf> request_streambuf_) noexcept
          : connection(std::move(connection_)), request_streambuf(std::move(request_streambuf_)), response(new Response(max_response_streambuf_size)) {}

      std::shared_ptr<Connection> connection;
      std::unique_ptr<asio::streambuf> request_streambuf;
      std::shared_ptr<Response> response;
      std::function<void(const error_code &)> callback;
    };

  public:
    /// Set before calling request
    Config config;

    /// If you have your own asio::io_service, store its pointer here before calling request().
    /// When using asynchronous requests, running the io_service is up to the programmer.
    std::shared_ptr<io_context> io_service;

    /// Convenience function to perform synchronous request. The io_service is run within this function.
    /// If reusing the io_service for other tasks, use the asynchronous request functions instead.
    /// Do not use concurrently with the asynchronous request functions.
    /// When requesting Server-Sent Events: will throw on error::eof, please use asynchronous request functions instead.
    std::shared_ptr<Response> request(const std::string &method, const std::string &path = {"/"}, string_view content = {}, const CaseInsensitiveMultimap &header = CaseInsensitiveMultimap()) {
      std::shared_ptr<Response> response;
      error_code ec;
      request(method, path, content, header, [&response, &ec](std::shared_ptr<Response> response_, const error_code &ec_) {
        response = response_;
        ec = ec_;
      });

      {
        LockGuard lock(concurrent_synchronous_requests_mutex);
        ++concurrent_synchronous_requests;
      }
      io_service->run();
      {
        LockGuard lock(concurrent_synchronous_requests_mutex);
        --concurrent_synchronous_requests;
        if(!concurrent_synchronous_requests)
          restart(*io_service);
      }

      if(ec)
        throw system_error(ec);

      return response;
    }

    /// Convenience function to perform synchronous request. The io_service is run within this function.
    /// If reusing the io_service for other tasks, use the asynchronous request functions instead.
    /// Do not use concurrently with the asynchronous request functions.
    /// When requesting Server-Sent Events: will throw on error::eof, please use asynchronous request functions instead.
    std::shared_ptr<Response> request(const std::string &method, const std::string &path, std::istream &content, const CaseInsensitiveMultimap &header = CaseInsensitiveMultimap()) {
      std::shared_ptr<Response> response;
      error_code ec;
      request(method, path, content, header, [&response, &ec](std::shared_ptr<Response> response_, const error_code &ec_) {
        response = response_;
        ec = ec_;
      });

      {
        LockGuard lock(concurrent_synchronous_requests_mutex);
        ++concurrent_synchronous_requests;
      }
      io_service->run();
      {
        LockGuard lock(concurrent_synchronous_requests_mutex);
        --concurrent_synchronous_requests;
        if(!concurrent_synchronous_requests)
          restart(*io_service);
      }

      if(ec)
        throw system_error(ec);

      return response;
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, const std::string &path, string_view content, const CaseInsensitiveMultimap &header,
                 std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      auto session = std::make_shared<Session>(config.max_response_streambuf_size, get_connection(), create_request_header(method, path, header));
      std::weak_ptr<Session> session_weak(session); // To avoid keeping session alive longer than needed
      auto request_callback = std::make_shared<std::function<void(std::shared_ptr<Response>, const error_code &)>>(std::move(request_callback_));
      session->callback = [this, session_weak, request_callback](const error_code &ec) {
        if(auto session = session_weak.lock()) {
          {
            LockGuard lock(this->connections_mutex);
            if(!session->connection->event_stream)
              session->connection->in_use = false;

            // Remove unused connections, but keep one open for HTTP persistent connection:
            std::size_t unused_connections = 0;
            for(auto it = this->connections.begin(); it != this->connections.end();) {
              if(ec && session->connection == *it)
                it = this->connections.erase(it);
              else if((*it)->in_use)
                ++it;
              else {
                ++unused_connections;
                if(unused_connections > 1)
                  it = this->connections.erase(it);
                else
                  ++it;
              }
            }
          }

          if(*request_callback)
            (*request_callback)(session->response, ec);
        }
      };

      std::ostream write_stream(session->request_streambuf.get());
      if(content.size() > 0) {
        auto header_it = header.find("Content-Length");
        if(header_it == header.end()) {
          header_it = header.find("Transfer-Encoding");
          if(header_it == header.end() || header_it->second != "chunked")
            write_stream << "Content-Length: " << content.size() << "\r\n";
        }
      }
      write_stream << "\r\n";
      write_stream.write(content.data(), static_cast<std::streamsize>(content.size()));

      connect(session);
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, const std::string &path, string_view content,
                 std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      request(method, path, content, CaseInsensitiveMultimap(), std::move(request_callback_));
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, const std::string &path,
                 std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      request(method, path, std::string(), CaseInsensitiveMultimap(), std::move(request_callback_));
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      request(method, std::string("/"), std::string(), CaseInsensitiveMultimap(), std::move(request_callback_));
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, const std::string &path, std::istream &content, const CaseInsensitiveMultimap &header,
                 std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      auto session = std::make_shared<Session>(config.max_response_streambuf_size, get_connection(), create_request_header(method, path, header));
      std::weak_ptr<Session> session_weak(session); // To avoid keeping session alive longer than needed
      auto request_callback = std::make_shared<std::function<void(std::shared_ptr<Response>, const error_code &)>>(std::move(request_callback_));
      session->callback = [this, session_weak, request_callback](const error_code &ec) {
        if(auto session = session_weak.lock()) {
          {
            LockGuard lock(this->connections_mutex);
            if(!session->connection->event_stream)
              session->connection->in_use = false;

            // Remove unused connections, but keep one open for HTTP persistent connection:
            std::size_t unused_connections = 0;
            for(auto it = this->connections.begin(); it != this->connections.end();) {
              if(ec && session->connection == *it)
                it = this->connections.erase(it);
              else if((*it)->in_use)
                ++it;
              else {
                ++unused_connections;
                if(unused_connections > 1)
                  it = this->connections.erase(it);
                else
                  ++it;
              }
            }
          }

          if(*request_callback)
            (*request_callback)(session->response, ec);
        }
      };

      content.seekg(0, std::ios::end);
      auto content_length = content.tellg();
      content.seekg(0, std::ios::beg);
      std::ostream write_stream(session->request_streambuf.get());
      if(content_length > 0) {
        auto header_it = header.find("Content-Length");
        if(header_it == header.end()) {
          header_it = header.find("Transfer-Encoding");
          if(header_it == header.end() || header_it->second != "chunked")
            write_stream << "Content-Length: " << content_length << "\r\n";
        }
      }
      write_stream << "\r\n";
      if(content_length > 0)
        write_stream << content.rdbuf();

      connect(session);
    }

    /// Asynchronous request where setting and/or running Client's io_service is required.
    /// Do not use concurrently with the synchronous request functions.
    /// When requesting Server-Sent Events: request_callback might be called more than twice, first call with empty contents on open, and with ec = error::eof on last call
    void request(const std::string &method, const std::string &path, std::istream &content,
                 std::function<void(std::shared_ptr<Response>, const error_code &)> &&request_callback_) {
      request(method, path, content, CaseInsensitiveMultimap(), std::move(request_callback_));
    }

    /// Close connections
    void stop() noexcept {
      LockGuard lock(connections_mutex);
      for(auto it = connections.begin(); it != connections.end();) {
        (*it)->close();
        it = connections.erase(it);
      }
    }

    virtual ~ClientBase() noexcept {
      handler_runner->stop();
      stop();
    }

  protected:
    bool internal_io_service = false;

    std::string host;
    unsigned short port;
    unsigned short default_port;

    std::unique_ptr<std::pair<std::string, std::string>> host_port;

    Mutex connections_mutex;
    std::unordered_set<std::shared_ptr<Connection>> connections GUARDED_BY(connections_mutex);

    std::shared_ptr<ScopeRunner> handler_runner;

    Mutex concurrent_synchronous_requests_mutex;
    std::size_t concurrent_synchronous_requests GUARDED_BY(concurrent_synchronous_requests_mutex) = 0;

    ClientBase(const std::string &host_port, unsigned short default_port) noexcept : default_port(default_port), handler_runner(new ScopeRunner()) {
      auto parsed_host_port = parse_host_port(host_port, default_port);
      host = parsed_host_port.first;
      port = parsed_host_port.second;
    }

    std::shared_ptr<Connection> get_connection() noexcept {
      std::shared_ptr<Connection> connection;
      LockGuard lock(connections_mutex);

      if(!io_service) {
        io_service = std::make_shared<io_context>();
        internal_io_service = true;
      }

      for(auto it = connections.begin(); it != connections.end(); ++it) {
        if(!(*it)->in_use) {
          connection = *it;
          break;
        }
      }
      if(!connection) {
        connection = create_connection();
        connections.emplace(connection);
      }
      connection->attempt_reconnect = true;
      connection->in_use = true;

      if(!host_port) {
        if(config.proxy_server.empty())
          host_port = std::unique_ptr<std::pair<std::string, std::string>>(new std::pair<std::string, std::string>(host, std::to_string(port)));
        else {
          auto proxy_host_port = parse_host_port(config.proxy_server, 8080);
          host_port = std::unique_ptr<std::pair<std::string, std::string>>(new std::pair<std::string, std::string>(proxy_host_port.first, std::to_string(proxy_host_port.second)));
        }
      }

      return connection;
    }

    std::pair<std::string, unsigned short> parse_host_port(const std::string &host_port, unsigned short default_port) const noexcept {
      std::pair<std::string, unsigned short> parsed_host_port;
      std::size_t host_end = host_port.find(':');
      if(host_end == std::string::npos) {
        parsed_host_port.first = host_port;
        parsed_host_port.second = default_port;
      }
      else {
        parsed_host_port.first = host_port.substr(0, host_end);
        parsed_host_port.second = static_cast<unsigned short>(stoul(host_port.substr(host_end + 1)));
      }
      return parsed_host_port;
    }

    virtual std::shared_ptr<Connection> create_connection() noexcept = 0;
    virtual void connect(const std::shared_ptr<Session> &) = 0;

    std::unique_ptr<asio::streambuf> create_request_header(const std::string &method, const std::string &path, const CaseInsensitiveMultimap &header) const {
      auto corrected_path = path;
      if(corrected_path == "")
        corrected_path = "/";
      if(!config.proxy_server.empty() && std::is_same<socket_type, asio::ip::tcp::socket>::value)
        corrected_path = "http://" + host + ':' + std::to_string(port) + corrected_path;

      std::unique_ptr<asio::streambuf> streambuf(new asio::streambuf());
      std::ostream write_stream(streambuf.get());
      write_stream << method << " " << corrected_path << " HTTP/1.1\r\n";
      write_stream << "Host: " << host;
      if(port != default_port)
        write_stream << ':' << std::to_string(port);
      write_stream << "\r\n";
      for(auto &h : header)
        write_stream << h.first << ": " << h.second << "\r\n";
      return streambuf;
    }

    void write(const std::shared_ptr<Session> &session) {
      session->connection->set_timeout();
      asio::async_write(*session->connection->socket, session->request_streambuf->data(), [this, session](const error_code &ec, std::size_t /*bytes_transferred*/) {
        session->connection->cancel_timeout();
        auto lock = session->connection->handler_runner->continue_lock();
        if(!lock)
          return;
        if(!ec)
          this->read(session);
        else
          session->callback(ec);
      });
    }

    void read(const std::shared_ptr<Session> &session) {
      session->connection->set_timeout();
      asio::async_read_until(*session->connection->socket, session->response->streambuf, HeaderEndMatch(), [this, session](const error_code &ec, std::size_t bytes_transferred) {
        session->connection->cancel_timeout();
        auto lock = session->connection->handler_runner->continue_lock();
        if(!lock)
          return;
        if(session->response->streambuf.size() == session->response->streambuf.max_size()) {
          session->callback(make_error_code::make_error_code(errc::message_size));
          return;
        }

        if(!ec) {
          session->connection->attempt_reconnect = true;
          std::size_t num_additional_bytes = session->response->streambuf.size() - bytes_transferred;

          if(!ResponseMessage::parse(session->response->content, session->response->http_version, session->response->status_code, session->response->header)) {
            session->callback(make_error_code::make_error_code(errc::protocol_error));
            return;
          }

          auto header_it = session->response->header.find("Content-Length");
          if(header_it != session->response->header.end()) {
            auto content_length = stoull(header_it->second);
            if(content_length > num_additional_bytes) {
              session->connection->set_timeout();
              asio::async_read(*session->connection->socket, session->response->streambuf, asio::transfer_exactly(content_length - num_additional_bytes), [session](const error_code &ec, std::size_t /*bytes_transferred*/) {
                session->connection->cancel_timeout();
                auto lock = session->connection->handler_runner->continue_lock();
                if(!lock)
                  return;
                if(session->response->streambuf.size() == session->response->streambuf.max_size()) {
                  session->callback(make_error_code::make_error_code(errc::message_size));
                  return;
                }

                if(!ec)
                  session->callback(ec);
                else
                  session->callback(ec);
              });
            }
            else
              session->callback(ec);
          }
          else if((header_it = session->response->header.find("Transfer-Encoding")) != session->response->header.end() && header_it->second == "chunked") {
            auto chunks_streambuf = std::make_shared<asio::streambuf>(this->config.max_response_streambuf_size);

            // Copy leftover bytes
            std::ostream ostream(chunks_streambuf.get());
            auto size = session->response->streambuf.size();
            std::unique_ptr<char[]> buffer(new char[size]);
            session->response->content.read(buffer.get(), static_cast<std::streamsize>(size));
            ostream.write(buffer.get(), static_cast<std::streamsize>(size));

            this->read_chunked_transfer_encoded(session, chunks_streambuf);
          }
          else if(session->response->http_version < "1.1" || ((header_it = session->response->header.find("Session")) != session->response->header.end() && header_it->second == "close")) {
            session->connection->set_timeout();
            asio::async_read(*session->connection->socket, session->response->streambuf, [this, session](const error_code &ec, std::size_t /*bytes_transferred*/) {
              session->connection->cancel_timeout();
              auto lock = session->connection->handler_runner->continue_lock();
              if(!lock)
                return;
              if(session->response->streambuf.size() == session->response->streambuf.max_size()) {
                session->callback(make_error_code::make_error_code(errc::message_size));
                return;
              }

              if(!ec) {
                {
                  LockGuard lock(this->connections_mutex);
                  this->connections.erase(session->connection);
                }
                session->callback(ec);
              }
              else
                session->callback(ec == error::eof ? error_code() : ec);
            });
          }
          else if(((header_it = session->response->header.find("Content-Type")) != session->response->header.end() && header_it->second == "text/event-stream")) {
            session->connection->event_stream = true;

            auto events_streambuf = std::make_shared<asio::streambuf>(this->config.max_response_streambuf_size);

            // Copy leftover bytes
            std::ostream ostream(events_streambuf.get());
            auto size = session->response->streambuf.size();
            std::unique_ptr<char[]> buffer(new char[size]);
            session->response->content.read(buffer.get(), static_cast<std::streamsize>(size));
            ostream.write(buffer.get(), static_cast<std::streamsize>(size));

            session->callback(ec); // Connection to a Server-Sent Events resource is opened

            this->read_server_sent_event(session, events_streambuf);
          }
          else
            session->callback(ec);
        }
        else {
          if(session->connection->attempt_reconnect && ec != error::operation_aborted) {
            LockGuard lock(connections_mutex);
            auto it = connections.find(session->connection);
            if(it != connections.end()) {
              connections.erase(it);
              session->connection = create_connection();
              session->connection->attempt_reconnect = false;
              session->connection->in_use = true;
              session->response = std::shared_ptr<Response>(new Response(this->config.max_response_streambuf_size));
              connections.emplace(session->connection);
              lock.unlock();
              this->connect(session);
            }
            else {
              lock.unlock();
              session->callback(ec);
            }
          }
          else
            session->callback(ec);
        }
      });
    }

    void read_chunked_transfer_encoded(const std::shared_ptr<Session> &session, const std::shared_ptr<asio::streambuf> &chunks_streambuf) {
      session->connection->set_timeout();
      asio::async_read_until(*session->connection->socket, *chunks_streambuf, "\r\n", [this, session, chunks_streambuf](const error_code &ec, size_t bytes_transferred) {
        session->connection->cancel_timeout();
        auto lock = session->connection->handler_runner->continue_lock();
        if(!lock)
          return;
        if(chunks_streambuf->size() == chunks_streambuf->max_size()) {
          session->callback(make_error_code::make_error_code(errc::message_size));
          return;
        }

        if(!ec) {
          std::istream istream(chunks_streambuf.get());
          std::string line;
          getline(istream, line);
          bytes_transferred -= line.size() + 1;
          line.pop_back();
          unsigned long length = 0;
          try {
            length = stoul(line, 0, 16);
          }
          catch(...) {
            session->callback(make_error_code::make_error_code(errc::protocol_error));
            return;
          }

          auto num_additional_bytes = chunks_streambuf->size() - bytes_transferred;

          if((2 + length) > num_additional_bytes) {
            session->connection->set_timeout();
            asio::async_read(*session->connection->socket, *chunks_streambuf, asio::transfer_exactly(2 + length - num_additional_bytes), [this, session, chunks_streambuf, length](const error_code &ec, size_t /*bytes_transferred*/) {
              session->connection->cancel_timeout();
              auto lock = session->connection->handler_runner->continue_lock();
              if(!lock)
                return;
              if(chunks_streambuf->size() == chunks_streambuf->max_size()) {
                session->callback(make_error_code::make_error_code(errc::message_size));
                return;
              }

              if(!ec)
                this->read_chunked_transfer_encoded_chunk(session, chunks_streambuf, length);
              else
                session->callback(ec);
            });
          }
          else
            this->read_chunked_transfer_encoded_chunk(session, chunks_streambuf, length);
        }
        else
          session->callback(ec);
      });
    }

    void read_chunked_transfer_encoded_chunk(const std::shared_ptr<Session> &session, const std::shared_ptr<asio::streambuf> &chunks_streambuf, unsigned long length) {
      std::istream istream(chunks_streambuf.get());
      if(length > 0) {
        std::ostream ostream(&session->response->streambuf);
        std::unique_ptr<char[]> buffer(new char[length]);
        istream.read(buffer.get(), static_cast<std::streamsize>(length));
        ostream.write(buffer.get(), static_cast<std::streamsize>(length));
        if(session->response->streambuf.size() == session->response->streambuf.max_size()) {
          session->callback(make_error_code::make_error_code(errc::message_size));
          return;
        }
      }

      // Remove "\r\n"
      istream.get();
      istream.get();

      if(length > 0)
        read_chunked_transfer_encoded(session, chunks_streambuf);
      else
        session->callback(error_code());
    }

    void read_server_sent_event(const std::shared_ptr<Session> &session, const std::shared_ptr<asio::streambuf> &events_streambuf) {
      session->connection->set_timeout();
      asio::async_read_until(*session->connection->socket, *events_streambuf, HeaderEndMatch(), [this, session, events_streambuf](const error_code &ec, std::size_t /*bytes_transferred*/) {
        session->connection->cancel_timeout();
        auto lock = session->connection->handler_runner->continue_lock();
        if(!lock)
          return;
        if(events_streambuf->size() == events_streambuf->max_size()) {
          session->callback(make_error_code::make_error_code(errc::message_size));
          return;
        }

        if(!ec) {
          std::istream istream(events_streambuf.get());
          std::ostream ostream(&session->response->streambuf);
          std::string line;
          while(std::getline(istream, line) && !line.empty() && !(line.back() == '\r' && line.size() == 1)) {
            ostream.write(line.data(), static_cast<std::streamsize>(line.size() - (line.back() == '\r' ? 1 : 0)));
            ostream.put('\n');
          }

          session->callback(ec);
          read_server_sent_event(session, events_streambuf);
        }
        else
          session->callback(ec);
      });
    }
  };

  template <class socket_type>
  class Client : public ClientBase<socket_type> {};

  using HTTP = asio::ip::tcp::socket;

  template <>
  class Client<HTTP> : public ClientBase<HTTP> {
  public:
    Client(const std::string &server_port_path) noexcept : ClientBase<HTTP>::ClientBase(server_port_path, 80) {}

  protected:
    std::shared_ptr<Connection> create_connection() noexcept override {
      return std::make_shared<Connection>(handler_runner, config.timeout, *io_service);
    }

    void connect(const std::shared_ptr<Session> &session) override {
      if(!session->connection->socket->lowest_layer().is_open()) {
        auto resolver = std::make_shared<asio::ip::tcp::resolver>(*io_service);
        session->connection->set_timeout(config.timeout_connect);
        async_resolve(*resolver, *host_port, [this, session, resolver](const error_code &ec, resolver_results results) {
          session->connection->cancel_timeout();
          auto lock = session->connection->handler_runner->continue_lock();
          if(!lock)
            return;
          if(!ec) {
            session->connection->set_timeout(config.timeout_connect);
            asio::async_connect(*session->connection->socket, results, [this, session, resolver](const error_code &ec, async_connect_endpoint /*endpoint*/) {
              session->connection->cancel_timeout();
              auto lock = session->connection->handler_runner->continue_lock();
              if(!lock)
                return;
              if(!ec) {
                asio::ip::tcp::no_delay option(true);
                error_code ec;
                session->connection->socket->set_option(option, ec);
                this->write(session);
              }
              else
                session->callback(ec);
            });
          }
          else
            session->callback(ec);
        });
      }
      else
        write(session);
    }
  };
} // namespace SimpleWeb

#endif /* SIMPLE_WEB_CLIENT_HTTP_HPP */
