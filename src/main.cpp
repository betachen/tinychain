#include <tinychain/tinychain.hpp>
#include <tinychain/node.hpp>

using namespace tinychain;

// global logger
Logger logger;

int main(int argc, char* argv[])
{

    log::info("main")<<"started";

    //std::string input = "grape";
    //auto&& output1 = sha256(input);
    //log::info("main") << "sha256('"<< input << "'):" << output1;
    //

    tx tx1("tx1_address", 100);
    tx tx2("tx2_address", 200);

    tx tx3("tx3_address", 300);
    tx tx4("tx4_address", 400);

    blockchain::memory_pool_t p;
    p.push_back(tx1);
    p.push_back(tx2);

    block block1(1);
    block1.setup(p);

    p.push_back(tx3);
    block block2(2);
    block2.setup(p);

    block block3(3);
    blockchain blockchain1;
    blockchain1.push_block(block1);
    blockchain1.push_block(block2);
    blockchain1.push_block(block3);
    blockchain1.print();

#if 0
    // bind
    mgbubble::RestServ rest_server_("webroot");
    auto& conn = rest_server_.bind("0.0.0.0:8000");

    // init for websocket and seesion control
    mg_set_protocol_http_websocket(&conn);
    mg_set_timer(&conn, mg_time() + mgbubble::RestServ::session_check_interval);

    // run
    for (;;)
        rest_server_.poll(1000);
#endif

    node my_node;
    my_node.run();

    return 0;
}

