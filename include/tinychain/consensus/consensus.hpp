#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace consensus
{

class consensus
{
public:
    consensus() noexcept = default;
    consensus(const consensus&) noexcept = default;
    consensus(consensus&&) noexcept = default;
    consensus& operator=(consensus&&) noexcept = default;
    consensus& operator=(const consensus&) noexcept = default;

    void print(){ std::cout<<"class consensus"<<std::endl; }
    void test();

private:
};

}// consensus
}// tinychain
