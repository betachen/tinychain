/**
 * Part of:
 * Comments:
 *
**/
#pragma once
#include <thread>
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
    node()  noexcept {
        log::info("node")<<"node started";
    }

    node(const node&)  = default;
    node(node&&)  = default;
    node& operator=(node&&)  = default;
    node& operator=(const node&)  = default;

    void test();
    bool check();

    void miner_run(address_t address) {
        // miner
        address_t miner_addr;
        if (address.empty()){
            miner_addr = blockchain_.get_new_key_pair().address();
        } else {
            miner_addr = address;
        }
        std::thread miner_service(std::bind(&miner::start, &miner_, miner_addr));
        miner_service.detach();
    }

    blockchain& chain() { return blockchain_; }
    network& p2p() { return network_; }

private:

    network network_;
    blockchain blockchain_;
    miner miner_{blockchain_};
};



}// tinychain
