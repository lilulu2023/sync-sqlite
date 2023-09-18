# sync-sqlite-native
A Node lib calling sqlite3 with node c++ addon

# known issue
1. As passing data to c function, only text int and float data type supported for present
2. Statements should not proceed 512 characters

# usage
```
    const Sql = require('sync-sqlite-native');
    
    let db = new Sql(':memory:');
    
    let data_array = db.run('SELECT * FROM sqlite_master');

    console.log(data_array);
    
    db.close();
    
```
