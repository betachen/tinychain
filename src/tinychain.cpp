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
 
/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/elements/tinychain.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace tinychain {

// 从公钥推导地址的函数
address_t key_to_address(const public_key_t& public_key) {
	// 首先获取标准格式的公钥
    std::string x509_pubk; //binary pubk as x.509
	CryptoPP::StringSink ss(x509_pubk);
	public_key.Save(ss);

	// 进行一次MD5作为地址
	return to_md5(x509_pubk);

#if 0
    CryptoPP::Base64Encoder pubk_slink(new CryptoPP::StringSink(encoded_pubk), false);
    public_key.DEREncode(pubk_slink);
    pubk_slink.MessageEnd();
    // sha256 public key 作为地址，和sha256区别开来，并且简短
	auto&& sha256_pubk = sha256(encoded_pubk);
    return sha256_pubk;
#endif
}

address_t key_to_address(const private_key_t& private_key) {
    public_key_t public_key(private_key);
    return key_to_address(public_key);
}

// 获取时间戳的工具函数
uint64_t get_now_timestamp() {
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::second_clock::universal_time() - epoch;
    return time_from_epoch.total_seconds();
}

// 获取随机数的工具函数
uint64_t pseudo_random(){
    std::random_device device;
    std::uniform_int_distribution<uint64_t> distribution;
    return distribution(device);
}

sha256_t to_sha256(Json::Value jv){
    Json::StreamWriterBuilder builder;
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(jv, &oss);
    return sha256(oss.str());
}

md5_t to_md5(const std::string& message){
	uint8_t digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];
	
	CryptoPP::Weak::MD5 hash;
	hash.CalculateDigest( digest, (const uint8_t*)message.c_str(), message.length() );
	
	CryptoPP::HexEncoder encoder;
	std::string output;
	
	encoder.Attach( new CryptoPP::StringSink( output ) );
	encoder.Put( digest, sizeof(digest) );
	encoder.MessageEnd();

	return output;
}

// #################### 以下类的成员函数实现 ##################

Json::Value tx::item_to_json (input_item_t in) {
    Json::Value root;
    root["hash"] = std::get<0>(in);
    root["index"] = std::get<1>(in);
    root["script_sign"] = std::get<2>(in);
    return root;
}
Json::Value tx::item_to_json (output_item_t out) {
    Json::Value root;
    root["address"] = std::get<0>(out);
    root["value"] = std::get<1>(out);
    root["script_pubkey"] = std::get<2>(out);
    return root;
}

Json::Value tx::to_json(){
    Json::Value root;

    Json::Value inputs;
    for (auto& each: inputs_) {
        inputs.append(item_to_json(each));
    }
    root["inputs"] = inputs;

    Json::Value outputs;
    for (auto& each: outputs_) {
        outputs.append(item_to_json(each));
    }
    root["outputs"] = outputs;
    hash_ = to_sha256(root);
    root["hash"] = hash_;

    return root;
}

tx::tx(address_t& address) {
    // coinbase tx: TODO
    auto&& input_item = std::make_tuple("00000000000000000000000000000000", 0, "0ffffffffffffff");
    inputs_.push_back(input_item);

    // build tx
    auto&& ouput_item = std::make_tuple(address, 1000, "1ffffffffffffffff");
    outputs_.push_back(ouput_item);

    // hash
    to_json();
}

tx::tx(address_t& address, uint64_t amount) {
    //get_balance_from blokchain, P2PKH
    //TODO
    auto&& input_item = std::make_tuple(to_md5(address), 0, "0ffffffffffffff");
    inputs_.push_back(input_item);

    // build tx
    auto&& ouput_item = std::make_tuple(address, amount, "1ffffffffffffffff");
    outputs_.push_back(ouput_item);

    // hash
    to_json();
}

} //tinychain
