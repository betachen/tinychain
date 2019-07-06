/**
 * Part of:
 * Comments:
 *
**/
#pragma once
#include <tinychain/tinychain.hpp>
#include <tinychain/lib/SimpleWeb.hpp>

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


private:
};

}// tinychain
