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
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/organizer/blockchain.hpp>

namespace tinychain
{

class miner
{
public:
    miner(blockchain& chain, std::atomic<bool>& isMining)
      :chain_{chain},
        isMining_{isMining}
    {
    }

    miner(const miner&) = default;
    miner(miner&&) = default;
    miner& operator=(miner&&) = default;
    miner& operator=(const miner&) = default;

    void print(){ std::cout<<"class miner"<<std::endl; }

    //开始挖矿
    void start(address_t addr);
    inline bool pow_once(block& new_block, address_t& addr);

    // 填写自己奖励——coinbase
    tx create_coinbase_tx(address_t& addr);

private:
    blockchain& chain_;
    std::atomic<bool>& isMining_;
};


bool validate_tx(const tx& new_tx) ;

bool validate_block(const tx& new_block) ;

}// tinychain
