/**
 * Part of:
 * Comments:
 *
**/
#pragma once
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/node/node.hpp>
#include <tinychain/utils/SimpleWeb.hpp>

namespace tinychain
{

class Rest : public RestServer
{
public:
    Rest(size_t cores) noexcept
    :RestServer{cores}
    {
    }

    Rest(const Rest&) noexcept = delete;
    Rest(Rest&&) noexcept = delete;

    Rest& operator=(Rest&&) noexcept = delete;
    Rest& operator=(const Rest&) noexcept = delete;

    ~Rest() override;

    void installHandlers() override;

    void print(){ std::cout<<"class network"<<std::endl; }
    void test();

    void getnewkey(RestResponsePtr rep, RestRequestPtr req);
    void listkeys(RestResponsePtr rep, RestRequestPtr req);
    void getheight(RestResponsePtr rep, RestRequestPtr req);
    void getlastblock(RestResponsePtr rep, RestRequestPtr req);
    void getbalance(RestResponsePtr rep, RestRequestPtr req);
    void startmining(RestResponsePtr rep, RestRequestPtr req);
    void stopmining(RestResponsePtr rep, RestRequestPtr req);
    void send(RestResponsePtr rep, RestRequestPtr req);

private:
    Node node_;
};

}// tinychain
