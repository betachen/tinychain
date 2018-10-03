/**
 * Part of:
 * Comments:
 *
**/
﻿#pragma once
#include <tinychain/tinychain.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

class miner
{
public:
    miner(blockchain& chain):chain_(chain) {};
    miner(const miner&) = default;
    miner(miner&&) = default;
    miner& operator=(miner&&) = default;
    miner& operator=(const miner&) = default;

    void print(){ std::cout<<"class miner"<<std::endl; }

    //开始挖矿
    void start(address_t& addr);
    inline bool pow_once(block& new_block, address_t& addr);

    // 填写自己奖励——coinbase
    tx create_coinbase_tx(address_t& addr);

private:
    blockchain& chain_;
};


bool validate_tx(const tx& new_tx) ;

bool validate_block(const tx& new_block) ;

}// tinychain
