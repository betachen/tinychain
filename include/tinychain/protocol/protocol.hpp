#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace protocol
{

class protocol
{
public:
    protocol() noexcept = default;
    protocol(const protocol&) noexcept = default;
    protocol(protocol&&) noexcept = default;
    protocol& operator=(protocol&&) noexcept = default;
    protocol& operator=(const protocol&) noexcept = default;

    void print(){ std::cout<<"class protocol"<<std::endl; }
    void test();

private:
};

}// protocol
}// tinychain
