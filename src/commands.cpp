#include <tinychain/tinychain.hpp>
#include <tinychain/commands.hpp>

namespace tinychain
{


bool commands::exec(Json::Value& out){
    if (*(vargv_.begin()) == "getnewkey") {
        auto&& key = chain_.get_new_key_pair();
        out = key.to_json();
    } else if  (*(vargv_.begin()) == "listkeys") {
        out = chain_.list_keys();
    } else if  (*(vargv_.begin()) == "send") {
        out = "send-ret-not-yet";
    } else if  (*(vargv_.begin()) == "getbalance") {
        out = "getbalance-ret-not-yet";
    } else if  (*(vargv_.begin()) == "startmining") {
        //TODO
        if (vargv_.size() < 2) {
            std::string cmd;
            //node_.miner_run(cmd);
            out["result"] = "start mining on a your random address";
        }
        //node_->miner_run(vargv_[1]);
        out["result"] = "start mining on address" + vargv_[1];
    } else {
        out = "<getnewkey>  <listkeys>  <getbalance>  <send>  <startmining>";
        return false;
    }

    chain_.id();
    return true;
}

const commands::vargv_t command_list = {"getnewkey","send","getbalance", "startmining"};


} //tinychain

