/**
 * Part of:
 * Comments:
 *
**/
﻿#pragma once

#include <tinychain/logging.hpp>
#include <jsoncpp/json/json.h>
#include <tinychain/sha256.hpp>

#include <string>
#include <array>
#include <random>
#include <sstream>

// Digital Signatue Algorithm
#include <cryptopp/rsa.h> 
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/whrlpool.h>

namespace tinychain
{

namespace tc = tinychain;

// ---------------------------- typedef ----------------------------
typedef std::string sha256_t;
typedef std::string address_t;
typedef CryptoPP::RSA::PublicKey public_key_t;
typedef CryptoPP::RSA::PrivateKey private_key_t;

// ---------------------------- ulitity ----------------------------
uint64_t get_now_timestamp();
sha256_t to_sha256(Json::Value jv);
address_t key_to_address(const public_key_t& public_key);
address_t key_to_address(const private_key_t& private_key);

// ---------------------------- class ----------------------------
class key_pair
{

public:
	// new key pair
    key_pair()  { 
        private_key_.GenerateRandomWithKeySize(rng, 1024);
    }
	// Copy
    key_pair(const key_pair& rk)  {
        private_key_ = rk.private_key(); 
    }
	// =
    key_pair& operator=(const key_pair& rk) {
        private_key_ = rk.private_key(); 
        return *this;
    }

    // 从base64直接构造RSA私钥
    key_pair(std::string& encoded_prik) {

        log::debug("key_pair-in")<<encoded_prik;
	    // decode base64 into private key
        CryptoPP::StringSource prik_ss(encoded_prik, true, new CryptoPP::Base64Decoder());
        private_key_.BERDecode(prik_ss); 
        log::debug("key_pair-out")<<to_json();
    }

    // TODO
    key_pair(key_pair&&) = default;
    key_pair& operator=(key_pair&&)  = default;

    void print(){log::info("key_pair")<<to_json();}
    void test();

    address_t address() const { 
	    return key_to_address(private_key_);
	}

    private_key_t private_key() const { return private_key_; }

    auto encode_pair() const { 
        // get public key
        public_key_t public_key(private_key_);

        // encode with base64
        std::string encoded_prik, encoded_pubk;
        CryptoPP::Base64Encoder prik_slink(new CryptoPP::StringSink(encoded_prik), false);//false for no '\n'
        CryptoPP::Base64Encoder pubk_slink(new CryptoPP::StringSink(encoded_pubk), false);
        private_key_.DEREncode(prik_slink); 
        public_key.DEREncode(pubk_slink);
        prik_slink.MessageEnd();//base64 编码补足=
        pubk_slink.MessageEnd();
        //log::debug("key_pair-0")<<encoded_prik;

        return std::make_pair(encoded_prik, encoded_pubk);
    }

    Json::Value to_json() const {
        Json::Value root;
        auto&& keypair = encode_pair();
        root["address"] = address();
        root["public_key"] = keypair.second;
        root["private_key"] = keypair.first;
        return root;
    }

public:
    CryptoPP::AutoSeededRandomPool rng;

private:
    private_key_t private_key_;
};

class tx
{
public:
    typedef std::pair<sha256_t, uint8_t> input_item_t;
    typedef std::pair<address_t, uint64_t> output_item_t;

    typedef std::vector<input_item_t> input_t;
    typedef std::vector<output_item_t> output_t;

    tx() {}
    tx(address_t& address); //coinbase
    tx(address_t& address, uint64_t amount); 

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

    Json::Value item_to_json (input_item_t in) {
        Json::Value root;
        root["hash"] = in.first;
        root["index"] = in.second;
        return root;
    }
    Json::Value item_to_json (output_item_t out) {
        Json::Value root;
        root["address"] = out.first;
        root["value"] = out.second;
        return root;
    }

    Json::Value to_json(){
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

    input_t inputs() const { return inputs_; }
    output_t outputs() const { return outputs_; }
    sha256_t hash() const { return hash_; }

private:
    input_t inputs_;
    output_t outputs_;
    sha256_t hash_;
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
        uint64_t difficulty{0};
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

    Json::Value to_json(){
        Json::Value root;
        Json::Value bheader;

        bheader["nonce"] = header_.nonce;
        bheader["height"] = header_.height;
        bheader["timestamp"] = header_.timestamp;
        bheader["tx_count"] = header_.tx_count;
        bheader["difficulty"] = header_.difficulty;
        bheader["hash"] = header_.hash;
        bheader["merkel_header_hash"] = header_.merkel_root_hash;
        bheader["prev_hash"] = header_.prev_hash;

        root["header"] = bheader;

        Json::Value txs;
        for (auto& tx : tx_list_) {
            txs.append(tx.to_json());
        }
        root["txs"] = txs;

        return root;
    }

    std::string to_string() {
        auto&& j = to_json();
        return j.toStyledString();
    }

    sha256_t hash() const { return header_.hash; }

    void setup(tx_list_t& txs) {tx_list_.swap(txs);}

    blockheader header_;
private:
    tx_list_t tx_list_;
};


}// tinychain
