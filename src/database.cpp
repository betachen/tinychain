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
          private_key char(64) primary key, \
          public_key char(64) not null, \
          account TEXT NOT NULL DEFAULT 'default');");
        cmd2.execute();

    } catch (std::exception& ex) {
        db_conn.disconnect();
        log::error("database")<<"disconnect db with error:"<< ex.what();
    }
    db_conn.disconnect();
}

key_pair key_pair_database::get_new_key_pair() {
    key_pair key;
    log::info("database")<<"getnewkey address:"<< key.address();
    sqlite3pp::transaction xct(db_conn_);
    sqlite3pp::command cmd(db_conn_, "INSERT INTO key_pairs (public_key, private_key) VALUES (?, ?)");
    //TODO change pub pri key algorithm
    cmd.binder() << key.public_key().c_str()<<std::to_string(key.private_key()).c_str();
    cmd.execute();
    xct.commit();
    return key;
}

bool key_pair_database::list_keys(key_pair_list_t& key_list) {
    sqlite3pp::query qry(db_conn_, "SELECT public_key, private_key FROM key_pairs");

    for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
        key_list.push_back(key_pair((*i).get<char const*>(0), (*i).get<int>(1)));
    }
    return true;
}

} //tinychain

