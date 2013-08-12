# MySync
MySync is a tool that is able to syncronize identical tables between two MySQL databases.

## History
The tool was initially developed in my time at [trivago](http://www.trivago.com) to solve the need of syncing special tables from the live database to a shared development or local database. 

## Installation
MySync uses cmake for it's build. This means you need at least a working cmake and a C++ compiler like g++ or clang++. The dependency list is rather small. You will need the [mysql-connector-c++](http://dev.mysql.com/downloads/connector/cpp/), which is included in most distro-repositories. We use no special features, so even a bit outdated versions should work fine. The second dependency is [libconfig](http://www.hyperrealm.com/libconfig/), which is also available in most repos.

The steps are quite easy:

```
    $> cd mysync
    $> cmake .
    $> make
    $> ./mysync /path/to/config.conf
```

Please find the documentation of the config in the test.conf file and refer to the libconfig site for further information about the syntax. The original config file example is also included further down this readme.

## Usage
mysync has four operation methods. You can configure different methods per table.

### update
This mode selects all present IDs from the target table and selects only those from the source table.

### insert
This mode selects all present IDs from the target table and selects only IDs from the source table, that are NOT IN the selected bunch of IDs.

### truncinsert
Runns a truncate before using the insert mode.

### upsert
Combines a update and insert run.

```
# Tables is an array of strings like in ("foo", "bar", "baz")
# Every table mentioned here must appear in the table {} list later on.
tables = ("foo");

# This defines the source database.
source = {
  # Format is host:port, sorry no sockets yet.
  host      = "127.0.0.1:3306";
  user      = "test";
  database  = "test";
  pass      = "test";
};

# This defines the taget database.
target = {
  host      = "127.0.0.1:3306";
  user      = "root";
  database  = "test";
  # Leave password commented if not used.
  #pass:    = "";
};

# Detail information for each table.
table = {
  
  # the table name is the key for this map. Repeat this section for every table.
  foo = {
    # This is the primary key. We do not support multi-col PKs yet.
    key         = "foo_id";
    # The query to select the data from the source table. 
    # You CAN use a where statement or select single cols, give them different names, etc.
    # You can NOT use a LIMIT statement here.
    # There is a check that ensures the same col count on the source and the target tables.
    statement   = "SELECT * FROM item";
    # See README.md for a description of each method.
    # Possible options are: update,insert,truncinsert,upsert
    method      = "insert";
  };
};
```