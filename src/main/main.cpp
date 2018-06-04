#include <tinychain/tinychain.hpp>
#include <tinychain/blockchain/blockchain.hpp>
#include <tinychain/consensus/consensus.hpp>

using namespace tinychain;

int main(int argc, char* argv[])
{
    consensus::consensus t1;
    blockchain::blockchain t2;

    t1.print();
    t2.print();

    return 0;
}
