#pragma once
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

    void print(){ std::cout<<"class node"<<std::endl; }
    void test();
    bool check();
    void run() {
        for (;;)
            rest_server_.poll(1000);
    }

private:
    mgbubble::RestServ rest_server_{"webroot"};

    blockchain blockchain_;
    network network_;
    consensus consensus_;
    key_pair_database key_pair_database_;
};



}// tinychain
