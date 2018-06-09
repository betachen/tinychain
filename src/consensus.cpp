#include <tinychain/tinychain.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

void miner::test(){}

void miner::start() {
    auto pool = chain_.pool();
    block new_block;

    auto&& prev_block = chain_.get_last_block();

    // 填充新块
    new_block.header_.height = prev_block.header_.height + 1;
    new_block.header_.prev_hash = prev_block.header_.hash;

    new_block.header_.timestamp = get_now_timestamp();

    new_block.header_.tx_count = pool.size();

    // 难度调整
    auto target = 0x0fffffffffffffff / prev_block.header_.difficulty;

    new_block.header_.difficulty = (new_block.header_.timestamp - prev_block.header_.timestamp) / 6; // 6 seconds

    // 设置区块体,pool已经被清空
    new_block.setup(pool);

    // 调用网络广播

    // 本地存储
    chain_.push_block(new_block);
}

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

