/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/rest.hpp>
#include <tinychain/lib/Tokeniser.hpp>
#include <sstream>

using namespace std;

namespace tinychain
{

Rest::~Rest() = default;

void Rest::getnewkey(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    auto&& key = node_.chain().get_new_key_pair();
    out << key.to_json();
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::listkeys(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    out << node_.chain().list_keys();
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::getheight(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    out << node_.chain().get_height();
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::getlastblock(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    out << node_.chain().get_last_block().to_json();
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::getbalance(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    out << "not implement yet";
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::stopmining(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    node_.miner_stop();
    Json::Value jv;
    jv["msg"] = "mining stopping";
    jv["is_mining"] = node_.is_mining();
    out << jv;
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::startmining(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    Tokeniser<' '> t{req->content.toStringView()};

    // 挖矿地址
    std::string_view param = t.top();
    std::string addr;
    addr.assign(param.data(), param.size());

    node_.miner_run(addr);

    Json::Value jv;
    jv["address"] = addr;
    jv["is_mining"] = node_.is_mining();
    out << jv;
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::send(RestResponsePtr rep, RestRequestPtr req) {
    std::stringstream out;
    Tokeniser<' '> t{req->content.toStringView()};

    // 接收方地址
    std::string_view param = t.top();
    std::string addr;
    addr.assign(param.data(), param.size());

    t.pop();

    // 金额
    param = t.top();
    std::string amountStr;
    amountStr.assign(param.data(), param.size());
    uint16_t amount = std::stoull(amountStr);

    // 发送
    out << node_.chain().send(addr, amount);
    rep->write(HttpStatusCode::success_ok, out);
}

void Rest::installHandlers() {
    server_.resource["^/getnewkey(/?$)"]["POST"] = std::bind(&Rest::getnewkey, this, placeholders::_1, placeholders::_2);
    server_.resource["^/listkeys(/?$)"]["POST"] = std::bind(&Rest::listkeys, this, placeholders::_1, placeholders::_2);
    server_.resource["^/getheight(/?$)"]["POST"] = std::bind(&Rest::getheight, this, placeholders::_1, placeholders::_2);
    server_.resource["^/getlastblock(/?$)"]["POST"] = std::bind(&Rest::getlastblock, this, placeholders::_1, placeholders::_2);

    server_.resource["^/getbalance(/?$)"]["POST"] = std::bind(&Rest::getbalance, this, placeholders::_1, placeholders::_2);
    server_.resource["^/startmining(/?$)"]["POST"] = std::bind(&Rest::startmining, this, placeholders::_1, placeholders::_2);
    server_.resource["^/stopmining(/?$)"]["POST"] = std::bind(&Rest::stopmining, this, placeholders::_1, placeholders::_2);
    server_.resource["^/send(/?$)"]["POST"] = std::bind(&Rest::send, this, placeholders::_1, placeholders::_2);
}

} //tinychain

