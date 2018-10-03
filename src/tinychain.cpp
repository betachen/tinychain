/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/tinychain.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace tinychain {

// 从公钥推导地址的函数
address_t key_to_address(const public_key_t& public_key) {
    std::string encoded_pubk;
    public_key.Save(CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded_pubk)).Ref());
    // 截取0~41位公钥用作地址
    return (encoded_pubk.substr(0, 41));
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


// #################### 以下类的成员函数实现 ##################
//
tx::tx(address_t& address) {
    auto&& input_item = std::make_pair("00000000000000000000000000000000", 0);
    inputs_.push_back(input_item);

    // build tx
    auto&& ouput_item = std::make_pair(address, 1000);
    outputs_.push_back(ouput_item);

    // hash
    to_json();
}



tx::tx(address_t& address, uint64_t amount) {
    //get_balance_from blokchain
    //TODO
    auto&& input_item = std::make_pair(sha256(address), 0);
    inputs_.push_back(input_item);

    // build tx
    auto&& ouput_item = std::make_pair(address, amount);
    outputs_.push_back(ouput_item);

    // hash
    to_json();
}

} //tinychain
