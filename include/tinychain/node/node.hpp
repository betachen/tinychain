#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace node
{

class node
{
public:
    node() noexcept = default;
    node(const node&) noexcept = default;
    node(node&&) noexcept = default;
    node& operator=(node&&) noexcept = default;
    node& operator=(const node&) noexcept = default;

    void print(){ std::cout<<"class node"<<std::endl; }
    void test();

private:
};

}// node
}// tinychain
