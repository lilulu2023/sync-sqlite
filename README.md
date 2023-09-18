# sync-sqlite-native
A Node lib calling sqlite3 with node c++ addon

# known issue
1. As passing data to c function, only text int and float data type supported for present
2. Statements should not proceed 512 characters

# usage
```
    const sync_sql_native = require('sync-sqlite-native');
    
    let connect_success = sync_sql_native.connect(':memory:'); //You can also use your path to db file
    
    let data_array = sync_sql_native.exec(':memory:', 'SELECT * FROM sqlite_master');
    
    let error_msg_obj = sync_sql_native.exec(':memory:', 'some bad statements');
    
    let disconnect_result =  sync_sql_native.disconnect(':memory:');
    
```
