// Encryption Algorithm from cryptopp
#include <cryptopp/rsa.h> 
#include <cryptopp/base64.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 //ignore #warning from comipler for MD5
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/whrlpool.h>

#pragma once

namespace tinychain {

using public_key_t = CryptoPP::RSA::PublicKey;
using private_key_t = CryptoPP::RSA::PrivateKey;

}
