/**
 * Part of:
 * Comments:
 *
**/
#pragma once
#include <thread>
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/database/database.hpp>
#include <tinychain/organizer/consensus.hpp>
#include <tinychain/organizer/blockchain.hpp>
#include <tinychain/network/network.hpp>

namespace tinychain
{

class Node
{
public:
    Node()  noexcept {
        log::info("Node")<<"Node started";
    }

    Node(const Node&)  = default;
    Node(Node&&)  = default;
    Node& operator=(Node&&)  = default;
    Node& operator=(const Node&)  = default;

    void test();
    bool check();

    bool is_mining() const { return isMining_.load(); }

    void miner_stop() { isMining_ = false; }

    void miner_run(address_t& address) {
        isMining_ = true;

        if (address.empty()){
            address = blockchain_.get_new_key_pair().address();
        }

        std::thread miner_service(std::bind(&miner::start, &miner_, address));
        miner_service.detach();
    }

    blockchain& chain() { return blockchain_; }
    network& p2p() { return network_; }

private:

    network network_;
    blockchain blockchain_;

    std::atomic<bool> isMining_{false};
    miner miner_{blockchain_, isMining_};
};



}// tinychain
