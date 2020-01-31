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

