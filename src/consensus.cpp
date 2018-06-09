#include <algorithm>
#include <tinychain/tinychain.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/blockchain.hpp>

namespace tinychain
{

void miner::start(){
    for(;;) {
        block new_block;

        // 未找到，继续找
        if (!pow_once(new_block)) {
            continue;
        }

        // pool已经被清空
        chain_.pool_reset();

        // 调用网络广播

        // 本地存储
        chain_.push_block(new_block);
    }
}

bool miner::pow_once(block& new_block) {

    auto&& pool = chain_.pool();

    auto&& prev_block = chain_.get_last_block();

    // 填充新块
    new_block.header_.height = prev_block.header_.height + 1;
    new_block.header_.prev_hash = prev_block.header_.hash;

    new_block.header_.timestamp = get_now_timestamp();

    new_block.header_.tx_count = pool.size();

    // 难度调整: 
    // 不可以是0，最小200000, 想修改挖矿速度，请修改这两个数字
    // 前者控制每块速度，后者控制最快速度，大约6~10秒
    new_block.header_.difficulty = std::max((new_block.header_.timestamp - prev_block.header_.timestamp) * 18000, 200000ull); // 10 seconds
    // 计算挖矿目标值,最大值除以难度就目标值
    uint64_t target = 0xffffffffffffffff / prev_block.header_.difficulty;

    // 装载交易
    new_block.setup(pool);

    // 计算目标值
    for ( uint64_t n = 0; ; ++n) {
        //尝试候选目标值
        new_block.header_.nonce = n;
        auto&& jv_block = new_block.to_json();
        auto&& can = to_sha256(jv_block);
        uint64_t ncan = std::stoull(can.substr(0, 16), 0, 16); //截断前16位，转换uint64 后进行比较

        // 找到了
        if (ncan < target) {
            //log::info("consensus") << "target:" << ncan;
            //log::info("consensus") << "hash  :" << to_sha256(jv_block);
            new_block.header_.hash = can;
            log::info("consensus") << "new block :" << can;
            return true;
        }
    }

    return false;
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

