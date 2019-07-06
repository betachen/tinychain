/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/tinychain.hpp>
#include <tinychain/node.hpp>
#include <tinychain/rest.hpp>

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
    node myNode;
    Rest rest{2};
    rest.bind(8000);
    rest.start();

    // 启动本地服务
    log::info("main")<<"Rest Server started";
    rest.join();

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

