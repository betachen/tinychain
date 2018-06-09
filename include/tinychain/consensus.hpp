#pragma once
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
    void test();

    void start();

private:
    blockchain& chain_;
};


bool validate_tx(const tx& new_tx) ;

bool validate_block(const tx& new_block) ;

}// tinychain
