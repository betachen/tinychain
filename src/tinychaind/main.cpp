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
 
#include <tinychain/elements/tinychain.hpp>
#include <tinychain/node/node.hpp>
#include <tinychain/rest/rest.hpp>

using namespace tinychain;

// global logger
//Logger logger;

int main(int argc, char* argv[])
{

    // 初始化本地数据库
    database d;
    d.init();
    d.print();
    // 初始化本地服务
    // TODO

    // 启动HTTP服务
    Rest rest{2};
    rest.bind(8000);
    rest.start();
    log::info("main")<<"Rest Server started";

    // 启动WebSocket服务
    WebSocketServer ws{2};
    ws.bind(8001);
    ws.start();
    log::info("main")<<"WebSocket Server started";

    rest.join();
    ws.join();

#if 0 //测试用代码 后续会移除
    //std::string input = "grape";
    //auto&& output1 = sha256(input);
    //log::info("main") << "sha256('"<< input << "'):" << output1;
    
    tx tx1("tx1_address", 100);
    tx tx2("tx2_address", 200);

    tx tx3("tx3_address", 300);
    tx tx4("tx4_address", 400);

    blockchain::memory_pool_t p;
    p.push_back(tx1);
    p.push_back(tx2);

    block block1(1);
    block1.setup(p);

    p.push_back(tx3);
    block block2(2);
    block2.setup(p);

    block block3(3);
    blockchain blockchain1;
    blockchain1.push_block(block1);
    blockchain1.push_block(block2);
    blockchain1.push_block(block3);
    blockchain1.print();
#endif

    return 0;
}

