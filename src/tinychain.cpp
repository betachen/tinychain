#include <tinychain/tinychain.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace tinychain {

uint64_t get_now_timestamp() {
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::second_clock::universal_time() - epoch;
    return time_from_epoch.total_seconds();
}

sha256_t to_sha256(Json::Value jv){
    Json::StreamWriterBuilder builder;
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(jv, &oss);
    return sha256(oss.str());
}

tx::tx(address_t address, uint64_t amount) {
    //get_balance_from blokchain
    //TODO
    auto&& input_item = std::make_pair(sha256(address), 0);
    inputs_.push_back(input_item);

    // build tx
    auto&& ouput_item = std::make_pair(address, amount);
    outputs_.push_back(ouput_item);

    // hash
    to_json();
}

} //tinychain
