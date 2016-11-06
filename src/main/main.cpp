#include <tinychain/tinychain.hpp>
#include <tinychain/tinychain/tinychain.hpp>
#include <tinychain/blockchain/blockchain.hpp>
#include <tinychain/consensus/consensus.hpp>

using namespace tinychain;

int main(int argc, char* argv[])
{
    consensus::consensus t1;
    blockchain::blockchain t2;
    tinychain::tinychain t3;

    t1.print();
    t2.print();
    t3.print();

    return 0;
}
