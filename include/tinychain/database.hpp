#pragma once
#include <tinychain/tinychain.hpp>
#include <metaverse/mgbubble/utility/Queue.hpp>

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

class chain_database
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

    uint64_t height() { return chain_database_.count(); }
    void push_block(const block& new_block){
        chain_database_.push(new_block);
    }

private:
    chain_database_t chain_database_;
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
