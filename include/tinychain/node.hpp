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
        miner_run();

        log::info("node")<<"node started";
        blockchain_.print();

        log::info("node")<<"httpserver started";
        for (;;)
            rest_server_.poll(1000);
    }

    void miner_run() {
        // miner
        std::thread miner_service(std::bind(&miner::start, &miner_));
        miner_service.detach();
    }

private:
    mgbubble::RestServ rest_server_{"webroot"};

    blockchain blockchain_;
    network network_;
    miner miner_{blockchain_};
    key_pair_database key_pair_database_;
};



}// tinychain
