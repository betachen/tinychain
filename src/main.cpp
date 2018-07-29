#include <tinychain/tinychain.hpp>
#include <tinychain/node.hpp>
#include <metaverse/mgbubble.hpp>

using namespace tinychain;
using namespace mgbubble;

// global logger
Logger logger;

int main(int argc, char* argv[])
{

    database d;
    d.init();
    d.print();

#if 0
    log::info("main")<<"started";
    try {
        sqlite3pp::database db("test.db");
        sqlite3pp::database db2("test.db");
        {
          sqlite3pp::command cmd(db, "create table if not EXISTS block ( \
      number bigint primary KEY , \
      hash char(64) not null, \
      bits bigint, \
      transaction_count INTEGER , \
      mixhash  VARCHAR (128), \
      version char(8) , \
      merkle_tree_hash char(64), \
      previous_block_hash CHAR (64), \
      nonce varchar(128) , \
      time_stamp bigint);");
          std::cout << cmd.execute() << std::endl;
          //sqlite3pp::transaction xct(db);
          {
              sqlite3pp::command cmd(db, "INSERT INTO block(number, hash, bits) VALUES ('0','AAAA', '1234')");
              sqlite3pp::command cmd1(db, "INSERT INTO block(number, hash, bits) VALUES ('1','AAAB', '1235')");
              sqlite3pp::command cmd2(db2, "INSERT INTO block(number, hash, bits) VALUES ('2','AAAC', '1236')");
              sqlite3pp::command cmd3(db, "INSERT INTO block(number, hash, bits) VALUES ('3','AAAD', '1237')");
              sqlite3pp::command cmd4(db2, "INSERT INTO block(number, hash, bits) VALUES ('4','AAAE', '1238')");
              std::cout <<"cmd:"<< cmd.execute() << std::endl;
              std::cout <<"cmd1:"<< cmd1.execute() << std::endl;
              std::cout <<"cmd2:"<< cmd2.execute() << std::endl;
              std::cout <<"cmd3:"<< cmd3.execute() << std::endl;
              std::cout <<"cmd4:"<< cmd4.execute() << std::endl;
          }
        }
        sqlite3pp::query qry(db2, "SELECT number, hash, bits FROM block");
            std::cout << "column:"<< qry.column_count()<<std::endl;
        for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
	        int id;
	        char const* name, *phone;
	        std::tie(id, name, phone) = (*i).get_columns<int, char const*, char const*>(0, 1, 2);
            std::cout << id << "\t" << name << "\t" << phone << std::endl;
         }
        std::cout << "disconnect db:"<<db.disconnect() << std::endl;

    } catch (std::exception& ex) {
           std::cout << ex.what() << std::endl;
    }
    return  0;
#endif

    //std::string input = "grape";
    //auto&& output1 = sha256(input);
    //log::info("main") << "sha256('"<< input << "'):" << output1;
    //

#if 0
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

    // server setup
    node my_node;
    mgbubble::RestServ Server{"webroot", my_node};
    auto& conn = Server.bind("0.0.0.0:8000");
    mg_set_protocol_http_websocket(&conn);
    mg_set_timer(&conn, mg_time() + mgbubble::RestServ::session_check_interval);

    log::info("main")<<"httpserver started";
    Server.run();

    return 0;
}

