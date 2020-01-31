/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

#include <contrib/jsoncpp/json/json.h>
#include <tinychain/utils/SimpleWeb.hpp>

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
