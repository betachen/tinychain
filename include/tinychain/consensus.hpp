#pragma once
#include <tinychain/tinychain.hpp>

namespace tinychain
{

class miner
{
public:
    miner() noexcept = default;
    miner(const miner&) noexcept = default;
    miner(miner&&) noexcept = default;
    miner& operator=(miner&&) noexcept = default;
    miner& operator=(const miner&) noexcept = default;

    void print(){ std::cout<<"class miner"<<std::endl; }
    void test();

private:
};


bool validate_tx(const tx& new_tx) ;

bool validate_block(const tx& new_block) ;

}// tinychain
