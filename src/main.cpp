//
// Mario Mueller <mario@xenji.com> 2013
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <libconfig.h++>
#include <sys/stat.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>

#include "method_proxy.h"
#include "update_method.h"
#include "insert_method.h"
#include "abort_exception.h"
#include "dbtable.h"

#define DEFAULT_BATCH_SIZE  1000

using namespace std;
using namespace libconfig;

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        cerr << "Please provide a config file as first and only parameter." << endl;
        return -1;
    }
    argv++;
    
    struct stat buf;
    int exists = stat(*argv, &buf);
    if (exists == -1) {
        cerr << "Given config file does not exist." << endl;
        return -1;
    }
    
    Config config;
    
    // Prepare threads
    try
    {
        // Read the file. If there is an error, report it and exit.
        config.readFile(*argv);
        
        
        // MySQL
        sql::Driver *sql_driver = get_driver_instance();
        
        // Source connection
        string src_password = "";
        if (config.exists("source.pass")) {
            src_password = config.lookup("source.pass").c_str();
        }
        sql::Connection *src_con = sql_driver->connect(config.lookup("source.host").c_str(),
                                                       config.lookup("source.user").c_str(),
                                                       src_password);
        src_con->setSchema(config.lookup("source.database").c_str());
        
        // target connection
        string trgt_password = "";
        if (config.exists("target.pass")) {
            trgt_password = config.lookup("target.pass").c_str();
        }
        
        sql::Connection *trgt_con = sql_driver->connect(config.lookup("target.host").c_str(),
                                                        config.lookup("target.user").c_str(),
                                                        trgt_password);
        trgt_con->setSchema(config.lookup("target.database").c_str());
        
        // go over the tables
        const Setting &tables = config.getRoot()["tables"];
        const Setting &table_root = config.getRoot()["table"];
        cout << "Gathering tables to process" << endl;
        
        for (size_t i = 0; i < tables.getLength(); i++) {
            const string table_name = tables[i].c_str();
            cout << "\tFound: " << table_name << endl;
            
            if (!config.getRoot()["table"].exists(table_name)) {
                cerr << "Table " << table_name << " is set in table list, but not described in the table section." << endl;
                cerr << "I skip it for now, but you definetly want to check this!" << endl;
                continue;
            }
            
            MySync::DbTable table = MySync::DbTable(table_name, table_root[table_name]["statement"].c_str());
            table.setSourceConnection(src_con);
            table.setTargetConnection(trgt_con);
            table.gatherTargetFields();
            table.gatherAndValidateSourceFields();
            
            string method = table_root[table_name]["method"].c_str();
            
            MySync::MethodProxy *mp = NULL;
            cout << "\tSelected method for processing: " << method << endl;
            if (method == "update") {
                mp = new MySync::UpdateMethod();
                mp->setKeyField(table_root[table_name]["key"].c_str());
                mp->setSourceConnection(src_con);
                mp->setTargetConnection(trgt_con);
                table.setMethodProxy(mp);
            }
            else if (method == "insert") {
                mp = new MySync::InsertMethod();
                mp->setKeyField(table_root[table_name]["key"].c_str());
                mp->setSourceConnection(src_con);
                mp->setTargetConnection(trgt_con);
                table.setMethodProxy(mp);
            }
            else if (method == "upsert") {
                mp = new MySync::UpdateMethod();
                mp->setKeyField(table_root[table_name]["key"].c_str());
                mp->setSourceConnection(src_con);
                mp->setTargetConnection(trgt_con);
                table.setMethodProxy(mp);
                
                mp = new MySync::InsertMethod();
                mp->setKeyField(table_root[table_name]["key"].c_str());
                mp->setSourceConnection(src_con);
                mp->setTargetConnection(trgt_con);
                table.setMethodProxy(mp);
            }
            else if (method == "truncinsert") {
                sql::Statement *stmt = trgt_con->createStatement();
                stringstream query;
                query << "TRUNCATE TABLE " << table_name;
                stmt->execute(query.str());
                
                mp = new MySync::InsertMethod();
                mp->setKeyField(table_root[table_name]["key"].c_str());
                mp->setSourceConnection(src_con);
                mp->setTargetConnection(trgt_con);
                table.setMethodProxy(mp);
            }
            else {
                cerr << "\tProvided method " << method << " for table " << table_name << " is unknown." << endl;
                cerr << "I skip it for now, but you definetly want to check this!" << endl;
                continue;
            }
            
            int batch_size = DEFAULT_BATCH_SIZE;
            cout << "\tDefault batch size by MySync is " << DEFAULT_BATCH_SIZE << "." << endl;
            if (table_root[table_name].exists("batch_size")) {
                table_root[table_name].lookupValue("batch_size", batch_size);
                cout << "\tSettings say the new batch size is " << batch_size << "." << endl;
            }
            cout << "\tEventhough the default and your custom batch size is recognized by MySync, the feature is not yet implemented. Sorry." << endl;
            table.setBatchSize(batch_size);
            try {
                table.run();
            }
            catch (AbortException &e) {
                cout << "# ERR: Error in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
                //cout << "# ERR: " << e.what() << endl;
            }
        }
    }
    catch(const FileIOException &fioex)
    {
        cerr << "I/O error while reading file." << endl;
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
        << " - " << pex.getError() << endl;
        return(EXIT_FAILURE);
    }
    catch (exception &e) {
        cerr << "# ERR: (Indicating the catch clause position) Error in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cerr << "# Exception: " << e.what();
    }
    
    return 0;
}
