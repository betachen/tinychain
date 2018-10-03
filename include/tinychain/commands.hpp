/**
 * Part of:
 * Comments:
 *
**/
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
    commands(const vargv_t& vargv, node& node):vargv_(vargv), node_(node) {
    }

    commands(const commands&)  = default;
    commands(commands&&)  = default;
    commands& operator=(commands&&)  = default;
    commands& operator=(const commands&)  = default;

    bool exec(Json::Value& out);

    static const vargv_t commands_list;

private:
    const vargv_t& vargv_;
    node& node_;
};



}// tinychain
