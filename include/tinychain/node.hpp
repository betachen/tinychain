#pragma once
#include <thread>
#include <tinychain/tinychain.hpp>
#include <tinychain/database.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/network.hpp>
#include <tinychain/blockchain.hpp>
#include <metaverse/mgbubble.hpp>

namespace tinychain
{

class node
{
public:
    node()  noexcept {
        // http server
        auto& conn = rest_server_.bind("0.0.0.0:8000");
        mg_set_protocol_http_websocket(&conn);
        mg_set_timer(&conn, mg_time() + mgbubble::RestServ::session_check_interval);

        // blockchain
    }

    node(const node&)  = default;
    node(node&&)  = default;
    node& operator=(node&&)  = default;
    node& operator=(const node&)  = default;

    void test();
    bool check();
    void run() {
        log::info("node")<<"node started";
        log::info("node")<<"httpserver started";
        for (;;)
            rest_server_.poll(1000);
    }

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

private:

    network network_;
    blockchain blockchain_;
    mgbubble::RestServ rest_server_{"webroot", blockchain_};
    miner miner_{blockchain_};
};



}// tinychain
