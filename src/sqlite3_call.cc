//
//  created by coding sailor on 2023/9/15.
//  lilulu@siptimes.com
//


#include <sqlite3.h>
#include <cstring>
#include "sqlite3_call.h"

bool connect(sqlite3* &db, string db_file_path)
{
    return !sqlite3_open(db_file_path.c_str(), &db);
}

bool exec(sqlite3* db, string sql, string& sz_err,
          vector<string>& column_names,
          vector<int>& column_types,
          list<uint8_t*> &column_value){
    int ret = 0;
    sqlite3_stmt *stmt;
    ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if(ret)
    {
        sz_err = sqlite3_errstr(ret);
        return false;
    }
    
    int column_count = sqlite3_column_count(stmt);
    for(int i= 0;i<column_count;i++)
    {
        string column_name = sqlite3_column_name(stmt, i);
        column_names.push_back(column_name);
    }
    while (ret != SQLITE_DONE) {
        ret = sqlite3_step(stmt);
        if(ret == SQLITE_ROW)
        {
            for(int i = 0; i< column_count; i++)
            {
                int column_type = sqlite3_column_type(stmt, i);
                if(column_types.size() < column_count)
                {
                    column_types.push_back(column_type);
                }
                uint8_t* value = nullptr;
                switch (column_type) {
                    case SQLITE_TEXT:
                    {
                        int byte_count = sqlite3_column_bytes(stmt, i);
                        const uint8_t* bytes = sqlite3_column_text(stmt, i);
                        value = new uint8_t[byte_count];
                        memcpy(value, bytes, byte_count);
                        break;
                    }
                    case SQLITE_FLOAT:
                    {
                        value = (uint8_t*)new double;
                        *value = sqlite3_column_double(stmt, i);
                        break;
                    }
                    case SQLITE_INTEGER:
                    {
                        value = (uint8_t*)new int;
                        *value = sqlite3_column_int(stmt, i);
                        break;
                    }
                }
                column_value.push_back(value);
            }
        }
    }
    ret = sqlite3_finalize(stmt);
    if(ret != SQLITE_OK)
    {
        sz_err = sqlite3_errstr(ret);
        return false;
    }
    return true;
}

bool disconnect(sqlite3* db)
{
    int ret = sqlite3_close(db);
    return !ret;
}
