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
    sql::Connection *src_con = sql_driver->connect(config["source"]["host"].as<std::string>(),
                                                   config["source"]["user"].as<std::string>(),
                                                   config["source"]["pass"].as<std::string>());
    
    // go over the tables
    YAML::Node tables = config["tables"];
    
    std::cout << "Gathering tables to process" << std::endl;
    for (std::size_t i = 0; i < tables.size(); i++) {
        std::cout << "\t Found: " << tables[i].as<std::string>() << std::endl;
    }
    
    return 0;
}