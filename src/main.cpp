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

