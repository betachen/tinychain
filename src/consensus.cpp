#include <tinychain/tinychain.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

void miner::test(){}


bool validate_tx(blockchain& chain, const tx& new_tx) {
    // input exsited?
    auto&& inputs = new_tx.inputs();
    for (auto& each : inputs) {

        tx pt;
        if (!chain.get_tx(each.first, pt)) {
            return false;
        }

        log::info("consensus")<<pt.to_json().toStyledString();
        //auto&& tl = pt.outputs();
    }

    return true;
}

bool validate_block(const tx& new_block) {

    return true;
}


} //tinychain

