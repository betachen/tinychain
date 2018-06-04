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

#include <functional>
#include <experimental/string_view>
#include "restful-cpp/contrib/mongoose.h"

// ------------------------- definations -----------------------------

#define RFCPP_API __attribute__((visibility("default")))

// ------------------------- string_view -----------------------------
namespace std {
    using experimental::string_view;                                                
} 

constexpr std::string_view operator""_sv(const char* str, std::size_t len) noexcept
{                                                                               
    return {str, len};                                                            
}

// ------------------------- utilities -----------------------------
namespace rfcpp {

inline std::string_view operator+(const mg_str& str) noexcept
{
    return {str.p, str.len};
}

inline std::string_view operator+(const websocket_message& msg) noexcept
{
    return {reinterpret_cast<char*>(msg.data), msg.size};
}


// ------------------------- Message -----------------------------
class HttpMessage
{
public:
    HttpMessage(http_message* impl) noexcept : impl_{impl} {}
    ~HttpMessage() noexcept = default;
    
    // Copy.
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1778
    HttpMessage(const HttpMessage&) = default;
    HttpMessage& operator=(const HttpMessage&) = default;
    
    // Move.
    HttpMessage(HttpMessage&&) = default;
    HttpMessage& operator=(HttpMessage&&) = default;
    
    auto get() const noexcept { return impl_; }
    auto method() const noexcept { return +impl_->method; }
    auto uri() const noexcept { return +impl_->uri; }
    auto proto() const noexcept { return +impl_->proto; }
    auto queryString() const noexcept { return +impl_->query_string; }
    auto header(const char* name) const noexcept
    {
      auto* val = mg_get_http_header(impl_, name);
      return val ? +*val : std::string_view{};
    }
    auto body() const noexcept { return +impl_->body; }

private:

    http_message* impl_;
};

class WebsocketMessage
{ 
public:
    WebsocketMessage(websocket_message* impl) noexcept : impl_{impl} {}
    ~WebsocketMessage() noexcept = default;
    
    // Copy.
    WebsocketMessage(const WebsocketMessage&) = default;
    WebsocketMessage& operator=(const WebsocketMessage&) = default;
    
    // Move.
    WebsocketMessage(WebsocketMessage&&) = default;
    WebsocketMessage& operator=(WebsocketMessage&&) = default;
    
    auto get() const noexcept { return impl_; }
    auto data() const noexcept { return reinterpret_cast<char*>(impl_->data); }
    auto size() const noexcept { return impl_->size; }
   
private:
    websocket_message* impl_;
};

// ------------------------- Session -----------------------------
struct Session
{
        Session() = default;
        Session(uint64_t a1, double a2, double a3, 
                std::string&& a4, std::string a5):
            id(a1), created(a2), last_used(a3), user(a4), pass(a5){}
        ~Session() = default;

        uint64_t        id;
        double          created;
        double          last_used;
        std::string     user;
        std::string     pass;
};

} // rfcpp


// ------------------------- Streambuf -----------------------------
#include <ostream>
//#include "restful-cpp/contrib/mongoose.h"

namespace rfcpp {

class StreamBuf : public std::streambuf {
public:
    explicit StreamBuf(mbuf& buf) throw(std::bad_alloc);
    ~StreamBuf() noexcept override;
  
    // Copy.
    StreamBuf(const StreamBuf& rhs) = delete;
    StreamBuf& operator=(const StreamBuf& rhs) = delete;
  
    // Move.
    StreamBuf(StreamBuf&&) = delete;
    StreamBuf& operator=(StreamBuf&&) = delete;
  
    const char_type* data() const noexcept { return buf_.buf; }
    std::streamsize size() const noexcept { return buf_.len; }
    void reset() noexcept;
    void setContentLength(size_t pos, size_t len) noexcept;
  
protected:
    int_type overflow(int_type c) noexcept override;
  
    std::streamsize xsputn(const char_type* s, std::streamsize count) noexcept override;
  
private:
    mbuf& buf_;
};

class OStream : public std::ostream {
public:
    OStream(): std::ostream{nullptr} {}
    ~OStream() noexcept override = default;
  
    // Copy.
    OStream(const OStream& rhs) = delete;
    OStream& operator=(const OStream& rhs) = delete;
  
    // Move.
    OStream(OStream&&) = delete;
    OStream& operator=(OStream&&) = delete;
  
    StreamBuf* rdbuf() const noexcept { return static_cast<StreamBuf*>(std::ostream::rdbuf()); }
    const char_type* data() const noexcept { return rdbuf()->data(); }
    std::streamsize size() const noexcept { return rdbuf()->size(); }
    StreamBuf* rdbuf(StreamBuf* sb) noexcept
    {
      return static_cast<StreamBuf*>(std::ostream::rdbuf(sb));
    }
    void reset(int status, const char* reason,const char *content_type = "text/plain",const char *charset = "utf-8") noexcept;
    void setContentLength() noexcept;

private:
    size_t headSize_{0};
    size_t lengthAt_{0};
};

} // rfcpp



// ------------------------- Tokeniser -----------------------------
#include <algorithm>

namespace rfcpp {

template <char DelimN>
class Tokeniser {
public:
    explicit Tokeniser(std::string_view buf) noexcept { reset(buf); }
    explicit Tokeniser() noexcept { reset(""); }
    ~Tokeniser() noexcept = default;
  
    // Copy.
    Tokeniser(const Tokeniser& rhs) noexcept = default;
    Tokeniser& operator=(const Tokeniser& rhs) noexcept = default;
  
    // Move.
    Tokeniser(Tokeniser&&) noexcept = default;
    Tokeniser& operator=(Tokeniser&&) noexcept = default;
  
    std::string_view top() const noexcept { return buf_.substr(i_ - buf_.cbegin(), j_ - i_); }
    bool empty() const noexcept { return i_ == buf_.cend(); }
    void reset(std::string_view buf) noexcept
    {
      buf_ = buf;
      i_ = buf_.cbegin();
      j_ = std::find(i_, buf_.cend(), DelimN);
    }
    void pop() noexcept
    {
      if (j_ != buf_.cend()) {
        i_ = j_ + 1;
        j_ = std::find(i_, buf_.cend(), DelimN);
      } else {
        i_ = j_;
      }
    }
  
private:
    std::string_view buf_;
    std::string_view::const_iterator i_, j_;
};

} // rfcpp

