#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace blockchain
{

class blockchain
{
public:
    typedef std::vector<block> chain_t;

    blockchain() noexcept = default;
    blockchain(const blockchain&) noexcept = default;
    blockchain(blockchain&&) noexcept = default;
    blockchain& operator=(blockchain&&) noexcept = default;
    blockchain& operator=(const blockchain&) noexcept = default;

    void print(){ std::cout<<"class blockchain"<<std::endl; }
    void test();

    struct chain_id {
        uint64_t id{0};
        hash256_t genesis_hash;
    };

private:
    chain_id id_;
    chain_t chain_;
    //TODO storage
};

}// blockchain
}// tinychain
