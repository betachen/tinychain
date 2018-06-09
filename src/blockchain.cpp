#include <tinychain/tinychain.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

void blockchain::test(){}

bool blockchain::get_block(sha256_t block_hash, block& b) {
    if (!chain_.get_block(block_hash, b)) {
        return false;
    }
    return true;
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

