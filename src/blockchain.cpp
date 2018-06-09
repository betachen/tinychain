#include <tinychain/tinychain.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

void blockchain::test(){}

block blockchain::get_last_block() {
    return *(chain_.get_last_block());
}

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

void blockchain::create_genesis_block() {

    genesis_block_.header_.prev_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    genesis_block_.header_.timestamp = get_now_timestamp();
    genesis_block_.header_.tx_count = 1;
    genesis_block_.header_.difficulty = 1;

    genesis_block_.header_.hash = to_sha256(genesis_block_.to_json());

    push_block(genesis_block_);
}

} //tinychain

