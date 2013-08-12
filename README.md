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

Please find the documentation of the config in the test.conf file and refer to the libconfig site for further information about the syntax.

## Usage
mysync has four operation modes:

### update
This mode selects all present IDs from the target table and selects only those from the source table.

### insert
This mode selects all present IDs from the target table and selects only IDs from the source table, that are NOT IN the selected bunch of IDs.

### truncinsert
Runns a truncate before using the insert mode.

### upsert
Combines a update and insert run.