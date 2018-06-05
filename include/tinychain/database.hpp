#pragma once
#include <tinychain/tinychain.hpp>
#include <queue>

namespace tinychain
{

class database
{
public:
    database() noexcept {};
    database(const database&) noexcept = default;
    database(database&&) noexcept = default;
    database& operator=(database&&) noexcept = default;
    database& operator=(const database&) noexcept = default;

    virtual void print(){ std::cout<<"class database"<<std::endl; }
    virtual void test();

    virtual bool flush() = 0;

protected:

};

class chain_database
{
public:
    typedef std::queue<block> chain_database_t;
    
    chain_database() noexcept {};
    chain_database(const chain_database&) noexcept = default;
    chain_database(chain_database&&) noexcept = default;
    chain_database& operator=(chain_database&&) noexcept = default;
    chain_database& operator=(const chain_database&) noexcept = default;

    void print(){ std::cout<<"class chain_database"<<std::endl; }
    void test();

private:
    chain_database_t chain_database_;
};

class key_pair_database
{
public:
    typedef std::vector<key_pair> key_pair_database_t;

    key_pair_database() noexcept {};
    key_pair_database(const key_pair_database&) noexcept = default;
    key_pair_database(key_pair_database&&) noexcept = default;
    key_pair_database& operator=(key_pair_database&&) noexcept = default;
    key_pair_database& operator=(const key_pair_database&) noexcept = default;

    void print(){ std::cout<<"class key_pair_database"<<std::endl; }
    void test();

private:
    key_pair_database_t key_pair_database_;
};

}// tinychain
