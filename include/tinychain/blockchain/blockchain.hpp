#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace blockchain
{

class blockchain
{
public:
    blockchain() noexcept = default;
    blockchain(const blockchain&) noexcept = default;
    blockchain(blockchain&&) noexcept = default;
    blockchain& operator=(blockchain&&) noexcept = default;
    blockchain& operator=(const blockchain&) noexcept = default;

    void print(){ std::cout<<"class blockchain"<<std::endl; }
    void test();

private:
};

}// blockchain
}// tinychain
