#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace network
{

class network
{
public:
    network() noexcept = default;
    network(const network&) noexcept = default;
    network(network&&) noexcept = default;
    network& operator=(network&&) noexcept = default;
    network& operator=(const network&) noexcept = default;

    void print(){ std::cout<<"class network"<<std::endl; }
    void test();

private:
};

}// network
}// tinychain
