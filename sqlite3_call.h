//
//  created by coding sailor on 2023/9/15.
//  lilulu@siptimes.com
//

#ifndef sql_sts_hpp
#define sql_sts_hpp

#include <sqlite3.h>
#include <string>
#include <list>
#include <vector>

using namespace std;

struct sql_column
{
    int column_type;
    uint8_t* data;
};

void extract_data(vector<string> name, vector<int> type, list<uint8_t*> data);

long now();

bool connect(sqlite3* &db, string db_file_path);

bool exec(sqlite3* db,
          string sql,
          string& sz_err,
          vector<string>&,
          vector<int>&,
          list<uint8_t*>&);

bool disconnect(sqlite3* db);

#endif /* sql_sts_hpp */
