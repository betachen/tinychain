#include <tinychain/tinychain.hpp>
#include <tinychain/consensus.hpp>
#include <tinychain/blockchain.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace tinychain
{

void miner::test(){}

void miner::start() {
    auto pool = chain_.pool();
    block new_block;

    auto&& prev_block = chain_.get_last_block();

    new_block.header_.height = prev_block.header_.height + 1;
    new_block.header_.prev_hash = prev_block.header_.hash;

    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::second_clock::universal_time() - epoch;
    new_block.header_.timestamp = time_from_epoch.total_seconds();


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

