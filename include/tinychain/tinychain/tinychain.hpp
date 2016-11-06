#include <tinychain/tinychain.hpp>

namespace tinychain
{

class tinychain
{
public:
    tinychain() noexcept = default;
    tinychain(const tinychain&) noexcept = default;
    tinychain(tinychain&&) noexcept = default;
    tinychain& operator=(tinychain&&) noexcept = default;
    tinychain& operator=(const tinychain&) noexcept = default;

    void print(){ std::cout<<"class tinychain"<<std::endl; }
    void test();

private:
};

}// tinychain
