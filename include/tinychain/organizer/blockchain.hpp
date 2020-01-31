/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
/**
 * Part of:
 * Comments:
 *
**/
#pragma once
#include <queue>
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/database/database.hpp>
#include <tinychain/network/network.hpp>

namespace tinychain
{

class blockchain
{
public:
    typedef block::tx_list_t memory_pool_t;

    blockchain(uint16_t id = 1991):id_(id) {
        id_ = id;
    }
    blockchain(const blockchain&)  = default;
    blockchain(blockchain&&)  = default;
    blockchain& operator=(blockchain&&)  = default;
    blockchain& operator=(const blockchain&)  = default;

    void print(){
        log::info("blockchain")<<"--------begin--------";
        chain_.print();
        log::info("blockchain")<<"--------end--------";
    }
    void test();

    void push_block(const block& new_block){
        chain_.push(new_block);
    }

    uint64_t get_height();

    block get_last_block(); 

    bool get_block(sha256_t block_hash, block& out);

    bool get_tx(sha256_t tx_hash, tx& out);

    bool get_balance(address_t address, uint64_t balance);

    auto id() {return id_;}

    memory_pool_t pool() { return pool_; }
    void pool_reset(size_t times) { 
        //TO FIX, dirty impl
        while(times--)
            pool_.erase(pool_.begin());
    }

    void collect(tx& tx) {
        pool_.push_back(tx);
        log::info("blockchain-pool")<<"new tx:"<<tx.to_json().toStyledString();
    }

    key_pair get_new_key_pair(){
        return key_pair_database_.get_new_key_pair();
    }

    Json::Value list_keys(){
        Json::Value root;
        key_pair_database_.list_keys(root);
        return root;
    }

    Json::Value send(address_t addr, uint64_t amount){
        Json::Value root;
        tx sent{addr, amount};

        //本地pool
        collect(sent);
        
        //广播
        //ws_send(target_tx.to_json().toStyledString());

        return sent.to_json();
    }

private:
    uint16_t id_;
    block genesis_block_;
    chain_database chain_; 
    key_pair_database key_pair_database_;
    memory_pool_t pool_;
};

}// tinychain
