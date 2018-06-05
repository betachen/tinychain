#include <tinychain/tinychain.hpp>
#include <tinychain/blockchain.hpp>
#include <tinychain/consensus.hpp>
#include <metaverse/mgbubble.hpp>

using namespace tinychain;

int main(int argc, char* argv[])
{

    consensus::consensus t1;
    blockchain::blockchain t2;

    // bind
    mgbubble::RestServ rest_server_("html");
    auto& conn = rest_server_.bind("0.0.0.0:8000");

    // init for websocket and seesion control
    mg_set_protocol_http_websocket(&conn);
    mg_set_timer(&conn, mg_time() + mgbubble::RestServ::session_check_interval);

    // run
    for (;;)
        rest_server_.poll(1000);

    t1.print();
    t2.print();

    return 0;
}

