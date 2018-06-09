#pragma once
#include <tinychain/tinychain.hpp>
#include <tinychain/database.hpp>
#include <queue>

namespace tinychain
{

class blockchain
{
public:
    typedef std::vector<block> memory_pool_t;

    blockchain(uint16_t id = 3721):id_(id) {
        id_ = id;
    }
    blockchain(const blockchain&)  = default;
    blockchain(blockchain&&)  = default;
    blockchain& operator=(blockchain&&)  = default;
    blockchain& operator=(const blockchain&)  = default;

    void print(){
        log::info("blockchain")<<"---begin---";
        block b;
        while(chain_.pop(b)) {
            log::info("block")<<b.to_string();
        }
        log::info("blockchain")<<"---end---";
    }
    void test();

    void push_block(const block& new_block){
        chain_.push(new_block);
    }

    bool get_block(sha256_t block_hash, block& out);

    bool get_tx(sha256_t tx_hash, tx& out);

    bool get_balance(address_t address, uint64_t balance);

    auto id() {return id_;}

private:
    uint16_t id_;
    sha256_t genesis_hash_{"0F78FCC486F5315418FBF095E71C0675EE07D318E5AC4D150050CD8E57966496"};
    chain_database chain_; 
    memory_pool_t pool_;
};

}// tinychain
