//
//  dbtable.h
//  mysync
//
//  Created by Mario Mueller on 07.08.13.
//
//

#ifndef __mysync__dbtable__
#define __mysync__dbtable__

#include <iostream>
#include "mysql_connection.h"

#define DEFAULT_BATCH_SIZE 1000

class DbTable {
    
public:
    std::string table_name;

    DbTable(std::string table_name, std::string select_statement);
    void setSourceConnection(sql::Connection *conn);
    void setTargetConnection(sql::Connection *conn);
    void setBatchSize(int bsize);
    
private:
    int limit;
    sql::Connection *source_conn;
    sql::Connection *target_conn;
    std::string select_statement;
};

#endif /* defined(__mysync__dbtable__) */
