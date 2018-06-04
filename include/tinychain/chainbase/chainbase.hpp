#include <tinychain/tinychain.hpp>

namespace tinychain
{

class chainbase
{
public:
    chainbase() noexcept = default;
    chainbase(const chainbase&) noexcept = default;
    chainbase(chainbase&&) noexcept = default;
    chainbase& operator=(chainbase&&) noexcept = default;
    chainbase& operator=(const chainbase&) noexcept = default;

    void print(){ std::cout<<"class chainbase"<<std::endl; }
    void test();

private:
};

}// chainbase
