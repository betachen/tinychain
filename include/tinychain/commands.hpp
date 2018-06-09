#pragma once
#include <jsoncpp/json/json.h>
#include <tinychain/tinychain.hpp>
#include <tinychain/node.hpp>

namespace tinychain
{

class commands
{
public:
    typedef std::vector<std::string> vargv_t;
    commands(const vargv_t& vargv, blockchain& chain):vargv_(vargv), chain_(chain) {
    }

    commands(const commands&)  = default;
    commands(commands&&)  = default;
    commands& operator=(commands&&)  = default;
    commands& operator=(const commands&)  = default;

    bool exec(Json::Value& out);

    static const vargv_t commands_list;

private:
    const vargv_t& vargv_;
    blockchain& chain_;
};



}// tinychain
