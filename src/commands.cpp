#include <tinychain/tinychain.hpp>
#include <tinychain/commands.hpp>

namespace tinychain
{


bool commands::exec(Json::Value& out){
    if (*(vargv_.begin()) == "getnewkey") {
        auto&& key = node_.chain().get_new_key_pair();
        out = key.to_json();
    } else if  (*(vargv_.begin()) == "listkeys") {
        out = node_.chain().list_keys();
    } else if  (*(vargv_.begin()) == "send") {
        if (vargv_.size() >= 3) {
            uint64_t amount = std::stoul(vargv_[2]);
            auto ret = node_.chain().send(vargv_[1], amount);
            out = ret.toStyledString();
        } else {
            out = "incorrect send paramas";
        }

    } else if  (*(vargv_.begin()) == "getbalance") {
        out = "getbalance-ret-not-yet";
    } else if  (*(vargv_.begin()) == "startmining") {
        std::string addr;
        if (vargv_.size() >= 2) {
            addr = vargv_[1];
            node_.miner_run(addr);
            out["result"] = "start mining on address" + addr;
        } else {
            node_.miner_run(addr);
            out["result"] = "start mining on your random address: " + addr;
        }
    } else {
        out = "<getnewkey>  <listkeys>  <getbalance>  <send>  <startmining>";
        return false;
    }

    return true;
}

const commands::vargv_t command_list = {"getnewkey","send","getbalance", "startmining"};


} //tinychain

