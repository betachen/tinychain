
#include <contrib/jsoncpp/json/json.h>
#include <tinychain/lib/SimpleWeb.hpp>

using namespace tinychain;
using namespace std;

int main(int argc, char* argv[])
{
    const std::string url{"127.0.0.1:8000"};

    SimpleWeb::CaseInsensitiveMultimap header;
    header.emplace("content-type", "application/json");

    HttpClient client{url};


    char** pp = argv;
    pp++; // argv[0] => argv[1]
    string params{"/"};
    params += *pp;
#if 0
    Json::Value jsonvar;
    Json::Value jsonopt;
    jsonvar["jsonrpc"] = "2.0";
    jsonvar["id"] = 1;
    jsonvar["method"] = (argc > 1) ? argv[1] : "help";
    jsonvar["params"] = Json::arrayValue;

    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            jsonvar["params"].append(argv[i]);
        }
    }
#endif

    auto resp = client.request("GET", params);
    std::cout<< resp->content.toStringView() << std::endl << std::flush;

    return 0;
}
