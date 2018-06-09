#pragma once
#include <tinychain/logging.hpp>
#include <jsoncpp/json/json.h>
#include <tinychain/sha256.hpp>
#include <string>
#include <array>
#include <random>

namespace tinychain
{

namespace tc = tinychain;

typedef std::string sha256_t;

class key_pair
{
public:
    static uint64_t pseudo_random()
    {
        std::random_device device;
        std::uniform_int_distribution<uint64_t> distribution;
        return distribution(device);
    }

    key_pair()  { 
        private_key_ = pseudo_random(); 
        public_key_ = sha256(std::to_string(private_key_));
    }
    key_pair(const key_pair& rk)  {
        private_key_ = rk.private_key(); 
        public_key_ = rk.public_key();
    }
    key_pair& operator=(const key_pair& rk) {
        private_key_ = rk.private_key(); 
        public_key_ = rk.public_key();
        return *this;
    }

    key_pair(key_pair&&)  = default;
    key_pair& operator=(key_pair&&)  = default;

    void print(){ std::cout<<"class key_pair"<<std::endl; }
    void test();

    sha256_t public_key() const { return public_key_; }
    uint64_t private_key() const { return private_key_; }

private:
    uint64_t private_key_;
    sha256_t public_key_;
};

class tx
{
public:
    typedef std::pair<key_pair, uint64_t> tx_item_t;
    typedef std::vector<tx_item_t> input_t;
    typedef std::vector<tx_item_t> output_t;

    tx()  = default;
    tx(const tx& rt) {
       inputs_ = rt.inputs(); 
       outputs_ = rt.outputs();
    }
    tx& operator=(const tx& rt) {
       inputs_ = rt.inputs(); 
       outputs_ = rt.outputs();
       return *this;
    }
    tx(tx&&)  = default;
    tx& operator=(tx&&)  = default;

    void print(){ std::cout<<"class tx"<<std::endl; }
    void test();
#if 0
    Json::Value to_json(){
        //TODO
    }
    std::string to_string() {
        auto&& j = to_json();
        return j.toStyledString();
    }
#endif
    input_t inputs() const { return inputs_; }
    output_t outputs() const { return outputs_; }

private:
    input_t inputs_;
    output_t outputs_;
};

class block
{
public:
    typedef std::vector<tx> tx_list_t;

    struct blockheader {
        uint64_t nonce{0};
        uint64_t height{0};
        uint64_t timestamp{0};
        uint64_t tx_count{0};
        uint64_t target{0};
        sha256_t hash;
        sha256_t merkel_root_hash; //TODO
        sha256_t prev_hash;
        
    };


    block() {}
    block(uint64_t h) {header_.height = h;}
    block(const block& rb) {
        header_ = rb.header();
        tx_list_ = rb.tx_list();
    }
    block& operator=(const block& rb) {
        header_ = rb.header();
        tx_list_ = rb.tx_list();
        return *this;
    }

    block(block&&)  = default;
    block& operator=(block&&)  = default;

    void print(){ std::cout<<"class block"<<std::endl; }
    void test();

    tx_list_t tx_list() const { return tx_list_; }
    block::blockheader header() const { return header_; }

    void getblocktemplate(block& new_block) const {
    }

    Json::Value to_json(){
        Json::Value root;
        root["nonce"] = header_.nonce;
        root["height"] = header_.height;
        root["timestamp"] = header_.timestamp;
        root["tx_count"] = header_.target;
        root["target"] = header_.target;
        root["hash"] = header_.hash;
        root["merkel_root_hash"] = header_.merkel_root_hash;
        root["prev_hash"] = header_.prev_hash;

        Json::Value txs;
        root["txs"].resize(1);
        txs.append("tx");
        for (auto& tx : tx_list_) {
            txs.append("tx");
        }

        return root;
    }

    std::string to_string() {
        auto&& j = to_json();
        return j.toStyledString();
    }


private:
    blockheader header_;
    tx_list_t tx_list_;
};


}// tinychain
