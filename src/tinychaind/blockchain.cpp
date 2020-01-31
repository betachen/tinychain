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
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/organizer/blockchain.hpp>

namespace tinychain
{

void blockchain::test(){}

block blockchain::get_last_block() {
    return chain_.get_last_block();
}

bool blockchain::get_block(sha256_t block_hash, block& b) {
    if (!chain_.get_block(block_hash, b)) {
        return false;
    }
    return true;
}

uint64_t blockchain::get_height(){
    return chain_.height();
}

bool blockchain::get_balance(address_t address, uint64_t balance){
    return true;
}

bool blockchain::get_tx(sha256_t tx_hash, tx& t) {
    if (!chain_.get_tx(tx_hash, t)) {
        return false;
    }
    return true;
}

} //tinychain

