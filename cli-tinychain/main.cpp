
#include <contrib/jsoncpp/json/json.h>
#include <tinychain/lib/SimpleWeb.hpp>

using namespace tinychain;
using namespace std;

int main(int argc, char* argv[])
{
    std::string url{"127.0.0.1:8000"};

    SimpleWeb::CaseInsensitiveMultimap header;
    header.emplace("content-type", "application/json");

    HttpClient client{url};

    std::string uri{"/"};
    char** pp = argv;
    if (argc > 1) {
      pp++; // argv[0] => argv[1]
      uri += *pp++;
    }

    std::stringstream ss;
    bool isFirst{true};
    int i{1};
    for (; i < argc - 1; ++i) {

      if (!isFirst) ss << ' ';
      ss << *pp++;
      isFirst = false;

    }

    //std::cout<< ss.str() << std::endl;
    try {
      auto resp = client.request("POST", uri, ss.str(), header);
      std::cout << resp->content.toStringView() << std::endl << std::flush;

      return 0;
    } catch (boost::system::system_error& e) {
      std::cout <<e.what()<<std::endl;

      return -1;
    }

}
