#ifndef SIMPLE_WEB_CRYPTO_HPP
#define SIMPLE_WEB_CRYPTO_HPP

#include <contrib/compat.hpp>
#include <contrib/crypto/base64/base64.hpp>
#include <contrib/crypto/sha1/sha1.hpp>

namespace SimpleWeb {

namespace Crypto {

    constexpr const char* ws_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    inline std::string sha1(const std::string& input) {
        unsigned char buffer[64];
        memset(buffer, 0x00, 64);
        websocketpp::sha1::calc(input.c_str(), input.size(), buffer);
        return reinterpret_cast<char*>(buffer);
    }

namespace Base64 {
    inline std::string encode(const std::string& input) {
        return websocketpp::base64_encode(input);
    }

    inline std::string decode(const std::string& input) {
        return websocketpp::base64_decode(input);
    }

} //Base64

} // Crypto

} //SimpleWeb
#endif /* SIMPLE_WEB_CRYPTO_HPP */
