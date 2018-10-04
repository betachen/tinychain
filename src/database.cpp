/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/tinychain.hpp>
#include <tinychain/database.hpp>

namespace tinychain
{

// ----------------------- class databse -------------------
void database::print(){
    sqlite3pp::database db_conn{db_name_};
    log::debug("database")<<"====== Print begin =====";
    try{
        sqlite3pp::query qry(db_conn, "SELECT * FROM key_pairs");
        log::debug("database")<< "column count:"<< qry.column_count();

        for (int i = 0; i < qry.column_count(); ++i) {
	      log::debug("database") << "columm name:" << qry.column_name(i);
        }

        for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
	        for (int j = 0; j < qry.column_count(); ++j) {
                log::debug("database")<< (*i).get<char const*>(j);
	        }
        }
    } catch (std::exception& ex) {
        db_conn.disconnect();
        log::error("database")<<"disconnect db with error:"<< ex.what();
    }
    db_conn.disconnect();
    log::debug("database")<<"====== Print end =====";
}

void database::create_genesis_block() {

    block genesis_block;

    genesis_block.header_.prev_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    genesis_block.header_.timestamp = get_now_timestamp();
    genesis_block.header_.tx_count = 1;
    genesis_block.header_.difficulty = 1;

    genesis_block.header_.hash = to_sha256(genesis_block.to_json());

    // TO BE Improved. => Same one
    chain_database chain;
    chain.push(genesis_block);
}


void database::init(){

    namespace bf = boost::filesystem;

    auto db_path = bf::current_path() / db_name_;

    if (bf::exists(db_path)) {
        log::info("database")<<"Using "<<db_name_;
        return;
    }

    log::info("database")<<"Initializing "<<db_name_;
    sqlite3pp::database db_conn{db_name_};
    try {
        sqlite3pp::command cmd(db_conn, "create table if not EXISTS block ( \
          height bigint primary KEY , \
          hash char(64) not null, \
          bits bigint, \
          transaction_count INTEGER , \
          mixhash  VARCHAR (128), \
          version char(8) , \
          merkle_tree_hash char(64), \
          previous_block_hash CHAR (64), \
          nonce varchar(128) , \
          time_stamp bigint);");
        cmd.execute();

        sqlite3pp::command cmd2(db_conn, "create table if not EXISTS key_pairs( \
          address char(64) primary key, \
          private_key BLOB NOT NULL, \
          account TEXT NOT NULL DEFAULT 'default');");
        cmd2.execute();

    } catch (std::exception& ex) {
        db_conn.disconnect();
        log::error("database")<<"disconnect db with error:"<< ex.what();
    }

    db_conn.disconnect();

    log::info("database")<<"Creating genesis block";
    create_genesis_block();
}

// -------------------- class chain_database ------------------
void chain_database::push(const block& newblock) {

    sqlite3pp::transaction xct(db_conn_);
    sqlite3pp::command cmd(db_conn_, "INSERT INTO block (height, hash, bits, time_stamp) VALUES (?, ?, ?, ?)");
    cmd.binder() << static_cast<long long int>(newblock.header_.height) 
        << newblock.header_.hash << static_cast<long long int>(newblock.header_.difficulty) 
        << static_cast<long long int>(newblock.header_.timestamp);

    log::debug("database")<<"chenhao2:"<< newblock.header_.hash;

    cmd.execute();
    xct.commit();
}

block chain_database::get_last_block() {

    block cc;
    sqlite3pp::query qry(db_conn_, "SELECT height, hash, bits, time_stamp FROM block where height = (SELECT count(*) - 1 FROM block)");
    for (auto i = qry.begin(); i != qry.end(); ++i) {
        cc.header_.height = (*i).get<long long int>(0);
        cc.header_.hash = (*i).get<const char*>(1);
        cc.header_.difficulty = (*i).get<long long int>(2);
        cc.header_.timestamp = (*i).get<long long int>(3);
    }
    log::info("database")<<"last_block_hash:"<< cc.header_.hash;

    return cc;
}

uint64_t chain_database::height() {
    sqlite3pp::query qry(db_conn_, "SELECT count(*) - 1 FROM block");
    sqlite3pp::query::iterator i = qry.begin();
    return (*i).get<long long int>(0);
}

// -------------------- class key_pair_database ------------------
//使用CryptoPP的RSA库生成新的密钥对
key_pair key_pair_database::get_new_key_pair() {
    key_pair key;
    log::info("database")<<"getnewkey address:"<< key.address();
    sqlite3pp::transaction xct(db_conn_);
    sqlite3pp::command cmd(db_conn_, "INSERT INTO key_pairs (address, private_key) VALUES (?, ?)");
    auto&& keypair = key.encode_pair();
    cmd.binder() << key.address() << keypair.first;
    cmd.execute();
    xct.commit();
    return key;
}

// 从数据库中读取数据
bool key_pair_database::list_keys(Json::Value& root) {
    sqlite3pp::query qry(db_conn_, "SELECT private_key FROM key_pairs");

    for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
        std::string private_key{(*i).get<char const*>(0)};
        key_pair each(private_key);
        root.append(each.to_json());
    }
    return true;
}

} //tinychain

