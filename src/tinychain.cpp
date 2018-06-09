#include <tinychain/tinychain.hpp>


namespace tinychain {

sha256_t to_sha256(Json::Value jv){
    Json::StreamWriterBuilder builder;
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(jv, &oss);
    return sha256(oss.str());
}

} //tinychain
