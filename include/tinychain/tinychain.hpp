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

    key_pair() noexcept { 
        private_key_ = pseudo_random(); 
        public_key_ = sha256(std::to_string(private_key_));
    }
    key_pair(const key_pair& rk) noexcept {
        private_key_ = rk.private_key(); 
        public_key_ = rk.public_key();
    }
    key_pair(key_pair&&) noexcept = default;
    key_pair& operator=(key_pair&&) noexcept = default;
    key_pair& operator=(const key_pair&) noexcept = default;

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

    tx() noexcept = default;
    tx(const tx&) noexcept = default;
    tx(tx&&) noexcept = default;
    tx& operator=(tx&&) noexcept = default;
    tx& operator=(const tx&) noexcept = default;

    void print(){ std::cout<<"class tx"<<std::endl; }
    void test();

private:
    input_t input_;
    output_t output_;
};

class block
{
public:
    typedef std::vector<tx> tx_list_t;

    block() noexcept = default;
    block(const block&) noexcept = default;
    block(block&&) noexcept = default;
    block& operator=(block&&) noexcept = default;
    block& operator=(const block&) noexcept = default;

    void print(){ std::cout<<"class block"<<std::endl; }
    void test();

    struct blockheader {
        uint64_t nonce{0};
        uint64_t height{0};
        uint64_t timestamp{0};
        uint64_t tx_count{0};
        sha256_t hash;
        sha256_t merkel_root_hash; //TODO
        sha256_t prev_hash;
        
    };

private:
    blockheader header_;
    tx_list_t tx_list_;
};


}// tinychain
