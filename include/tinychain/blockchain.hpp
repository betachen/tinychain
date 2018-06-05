#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace blockchain
{

class tx
{
public:
    typedef std::vector<std::string> input_t;
    typedef std::vector<std::string> output_t;

    tx() noexcept = default;
    tx(const tx&) noexcept = default;
    tx(tx&&) noexcept = default;
    tx& operator=(tx&&) noexcept = default;
    tx& operator=(const tx&) noexcept = default;

    void print(){ std::cout<<"class tx"<<std::endl; }
    void test();

private:
    input_t input_;
    output_t output_;
};

class block
{
public:
    typedef std::vector<tx> tx_t;

    block() noexcept = default;
    block(const block&) noexcept = default;
    block(block&&) noexcept = default;
    block& operator=(block&&) noexcept = default;
    block& operator=(const block&) noexcept = default;

    void print(){ std::cout<<"class block"<<std::endl; }
    void test();

    struct blockheader {
        uint64_t nonce{0};
        uint64_t height{0};
        uint64_t timestamp{0};
        uint64_t tx_count{0};
        std::string hash;
        std::string merkel_root_hash;
        std::string prev_hash;
        
    };

private:
    blockheader header_;
    tx_t tx_;
};


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
        std::string genesis_hash;
    };

private:
    chain_id id_;
    chain_t chain_;
    //TODO storage
};

}// blockchain
}// tinychain
