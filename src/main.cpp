//
// Mario Mueller <mario@xenji.com> 2013
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <sys/stat.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include "method_proxy.h"
#include "update_method.h"
#include "insert_method.h"

#include "dbtable.h"

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Please provide a config file as first and only parameter." << std::endl;
        return -1;
    }
    argv++;
    
    struct stat buf;
    int exists = stat(*argv, &buf);
    if (exists == -1) {
        std::cerr << "Given config file does not exist." << std::endl;
        return -1;
    }
    
    YAML::Node config = YAML::LoadFile(*argv);
  
    // MySQL
    sql::Driver *sql_driver = get_driver_instance();
    
    // Source connection
    std::string src_password = "";
    if (config["source"]["pass"].IsDefined()) {
        src_password = config["source"]["pass"].as<std::string>();
    }
    sql::Connection *src_con = sql_driver->connect(config["source"]["host"].as<std::string>(),
                                                   config["source"]["user"].as<std::string>(),
                                                   src_password);
    src_con->setSchema(config["source"]["database"].as<std::string>());
    
    // target connection
    std::string trgt_password = "";
    if (config["target"]["pass"].IsDefined()) {
        trgt_password = config["target"]["pass"].as<std::string>();
    }
    
    sql::Connection *trgt_con = sql_driver->connect(config["target"]["host"].as<std::string>(),
                                                    config["target"]["user"].as<std::string>(),
                                                    trgt_password);
    trgt_con->setSchema(config["target"]["database"].as<std::string>());
    
    // go over the tables
    YAML::Node tables = config["tables"];
    
    std::cout << "Gathering tables to process" << std::endl;
    for (std::size_t i = 0; i < tables.size(); i++) {
        const std::string table_name = tables[i].as<std::string>();
        std::cout << "\tFound: " << table_name << std::endl;
        
        if (!config["table"][table_name].IsDefined()) {
            std::cerr << "Table " << table_name << " is set in table list, but not described in the table section." << std::endl;
            std::cerr << "I skip it for now, but you definetly want to check this!" << std::endl;
            continue;
        }
        
        MySync::DbTable table = MySync::DbTable(table_name, config["table"][table_name]["statement"].as<std::string>());
        table.setSourceConnection(src_con);
        table.setTargetConnection(trgt_con);
        table.gatherTargetFields();
        table.gatherAndValidateSourceFields();
        
        std::string method = config["table"][table_name]["method"].as<std::string>();
        
        if (method == "update") {
            MySync::UpdateMethod mp = MySync::UpdateMethod();
            table.setMethodProxy(mp);
        }
        else if (method == "insert") {
            MySync::InsertMethod mp = MySync::InsertMethod();
            table.setMethodProxy(mp);
        }
        else {
            std::cerr << "\tProvided method " << method << " for table " << table_name << " is unknown." << std::endl;
            std::cerr << "I skip it for now, but you definetly want to check this!" << std::endl;
            continue;
        }
    }
    return 0;
}
