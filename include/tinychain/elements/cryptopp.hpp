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
