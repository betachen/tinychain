#include <tinychain/tinychain.hpp>
#include <tinychain/database.hpp>

namespace tinychain
{

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

void database::init(){
    log::info("database")<<"Initializing...";
    sqlite3pp::database db_conn{db_name_};
    try {
        sqlite3pp::command cmd(db_conn, "create table if not EXISTS block ( \
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
}

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

