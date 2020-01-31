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
#include <thread>
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/database/database.hpp>
#include <tinychain/organizer/consensus.hpp>
#include <tinychain/organizer/blockchain.hpp>
#include <tinychain/network/network.hpp>

namespace tinychain
{

class Node
{
public:
    Node()  noexcept {
        log::info("Node")<<"Node started";
    }

    Node(const Node&)  = default;
    Node(Node&&)  = default;
    Node& operator=(Node&&)  = default;
    Node& operator=(const Node&)  = default;

    void test();
    bool check();

    bool is_mining() const { return isMining_.load(); }

    void miner_stop() { isMining_ = false; }

    void miner_run(address_t& address) {
        isMining_ = true;

        if (address.empty()){
            address = blockchain_.get_new_key_pair().address();
        }

        std::thread miner_service(std::bind(&miner::start, &miner_, address));
        miner_service.detach();
    }

    blockchain& chain() { return blockchain_; }
    network& p2p() { return network_; }

private:

    network network_;
    blockchain blockchain_;

    std::atomic<bool> isMining_{false};
    miner miner_{blockchain_, isMining_};
};



}// tinychain
