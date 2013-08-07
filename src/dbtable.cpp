//
//  dbtable.cpp
//  mysync
//
//  Created by Mario Mueller on 07.08.13.
//
//

#include "dbtable.h"

DbTable::DbTable(std::string table, std::string statement) {
    select_statement = statement;
    table_name = table;
    limit = DEFAULT_BATCH_SIZE;
}

void DbTable::setSourceConnection(sql::Connection *c) {
    source_conn = c;
}

void DbTable::setTargetConnection(sql::Connection *c) {
    target_conn = c;
}

void DbTable::setBatchSize(int bsize) {
    limit = bsize;
}