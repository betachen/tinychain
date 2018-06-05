#pragma once
#include <tinychain/tinychain.hpp>
#include <tinychain/database.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/network.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

class node
{
public:
    node() noexcept = default;
    node(const node&) noexcept = default;
    node(node&&) noexcept = default;
    node& operator=(node&&) noexcept = default;
    node& operator=(const node&) noexcept = default;

    void print(){ std::cout<<"class node"<<std::endl; }
    void test();
    bool check();
    bool run();

private:
    blockchain blockchain_;
    network network_;
    consensus consensus_;
    key_pair_database key_pair_database_;
};

}// tinychain
