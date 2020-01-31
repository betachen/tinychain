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

#pragma once

#include <algorithm>
#include <tinychain/elements/tinychain.hpp>
#include <sqlite3pp/sqlite3pp.h>
#include <boost/filesystem.hpp>
//#include <sqlite3pp/sqlite3ppext.h>

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

    virtual void print();
    void init();
    void create_genesis_block();

protected:
    const char* db_name_ = "tinychain.db";
};

class chain_database: public database
{
public:
    
    chain_database()  {};
    chain_database(const chain_database&)  = default;
    chain_database(chain_database&&)  = default;
    chain_database& operator=(chain_database&&)  = default;
    chain_database& operator=(const chain_database&)  = default;

    void print(){
        //for (auto& each : queue_ ) {
        //    log::info("block")<<each.to_string();
        //};
    }
    void test();

    // inesert block into database
    void push(const block& newblock);

    uint64_t height();

    block get_last_block();

    bool get_block (const sha256_t block_hash, block& b) {
        return true;
    }

    bool get_tx (const sha256_t tx_hash, tx& t) {
        return true;
    }

    virtual ~chain_database(){
        db_conn_.disconnect();
    }

private:
    sqlite3pp::database db_conn_{db_name_};
};

// 相当于是本地钱包的私钥管理
class key_pair_database:public database
{
public:
    key_pair_database() {};
    key_pair_database(const key_pair_database&)  = default;
    key_pair_database(key_pair_database&&)  = default;
    key_pair_database& operator=(key_pair_database&&)  = default;
    key_pair_database& operator=(const key_pair_database&)  = default;

    key_pair get_new_key_pair();
    bool list_keys(Json::Value& root);

private:
    sqlite3pp::database db_conn_{db_name_};
};

}// tinychain
