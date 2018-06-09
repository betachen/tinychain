#pragma once
#include <tinychain/tinychain.hpp>
#include <metaverse/mgbubble/utility/Queue.hpp>
#include <deque>

namespace tinychain
{

class database
{
public:
    database()  {};
    database(const database&)  = default;
    database(database&&)  = default;
    database& operator=(database&&)  = default;
    database& operator=(const database&)  = default;

    virtual void print(){ std::cout<<"class database"<<std::endl; }
    virtual void test();

    virtual bool flush() = 0;

protected:

};

class chain_database: public Queue<block>
{
public:
    // thread safty
    typedef Queue<block> chain_database_t;
    
    chain_database()  {};
    chain_database(const chain_database&)  = default;
    chain_database(chain_database&&)  = default;
    chain_database& operator=(chain_database&&)  = default;
    chain_database& operator=(const chain_database&)  = default;

    void print(){ std::cout<<"class chain_database"<<std::endl; }
    void test();

    uint64_t height() { return count(); }

    bool get_block (const sha256_t block_hash, block& b) {
        auto iter = std::find_if(queue_.begin(), queue_.end(), [&block_hash](const block& b){
                return b.hash() == block_hash;
                });
        if (iter == queue_.end()) {
            return false;
        }
        b = *iter;
        return true;
    }

    bool get_tx (const sha256_t tx_hash, tx& t) {
        auto iter = std::find_if(queue_.begin(), queue_.end(), [&tx_hash, &t](const block& b){
                auto&& tl = b.tx_list();
                auto iter2 = std::find_if(tl.begin(), tl.end(), [&tx_hash, &t](const tx& t){
                return t.hash() == tx_hash;
                });
                if (iter2 == tl.end()) { 
                    return false;
                } else {
                    t = *iter2;
                    return true;
                }

                });
        if (iter == queue_.end()) {
            return false;
        }
        return true;
    }


private:
//    chain_database_t chain_database_;
};

class key_pair_database
{
public:
    typedef std::vector<key_pair> key_pair_database_t;

    key_pair_database()  {};
    key_pair_database(const key_pair_database&)  = default;
    key_pair_database(key_pair_database&&)  = default;
    key_pair_database& operator=(key_pair_database&&)  = default;
    key_pair_database& operator=(const key_pair_database&)  = default;

    void print(){ std::cout<<"class key_pair_database"<<std::endl; }
    void test();

private:
    key_pair_database_t key_pair_database_;
};

}// tinychain
