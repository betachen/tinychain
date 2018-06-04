#include <tinychain/tinychain.hpp>

namespace tinychain
{
namespace database
{

class database
{
public:
    database() noexcept = default;
    database(const database&) noexcept = default;
    database(database&&) noexcept = default;
    database& operator=(database&&) noexcept = default;
    database& operator=(const database&) noexcept = default;

    void print(){ std::cout<<"class database"<<std::endl; }
    void test();

private:
};

}// database
}// tinychain
