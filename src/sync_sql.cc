#include <node_api.h>
#include "sqlite3_call.h"

#define MAX_SQL_LEN 1024

#define NAPI_CALL(env, call)                                      \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      const char* err_message = error_info->error_message;        \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (err_message == NULL)               \
            ? "empty error message"                               \
            : err_message;                                        \
        napi_throw_error((env), NULL, message);                   \
        return NULL;                                              \
      }                                                           \
    }                                                             \
  } while(0)


sqlite3* db;

static napi_value
db_connect(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    size_t str_size_read = 0;
    char* buff = new char[MAX_SQL_LEN];
    napi_value *argv = new napi_value[1];
    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    napi_get_value_string_utf8(env, argv[0], buff, MAX_SQL_LEN, &str_size_read);
    auto ret = connect(db, buff);
    free(buff);
    napi_value return_value;
    napi_create_int32(env, ret, &return_value);
    return return_value;
}

static napi_value
db_exec(napi_env env, napi_callback_info info)
{
    napi_value result;
    size_t argc = 1;
    size_t str_size_read = 0;
    napi_value argv[1];
    char* buff = new char[MAX_SQL_LEN];
    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    napi_get_value_string_utf8(env, argv[0], buff, MAX_SQL_LEN, &str_size_read);
   
    vector<string> column_names;
    vector<int> column_types;
    list<uint8_t*> column_values;
    string sql = string(buff, str_size_read);
    string err = string();
    free(buff);
    
    bool ret = exec(db, sql, err, column_names, column_types, column_values);

    if(!ret)
    {
        napi_value error_result;
        napi_value err_msg;
        napi_create_object(env, &error_result);
        napi_create_string_utf8(env, err.c_str(), err.length(), &err_msg);
        napi_set_named_property(env, error_result, "error", err_msg);
        return error_result;
    }else{
        long column_count = min(column_names.size(), column_types.size());
        long row_count = column_values.size() / column_count;
        auto it = column_values.begin();
        napi_value result_array;
        napi_create_array_with_length(env, row_count, &result_array);
        for(int row = 0; row < row_count; row++ )
        {
            napi_value row_value;
            napi_create_object(env, &row_value);
            for(int column = 0; column < column_count; column++)
            {
                string column_name = column_names[column];
                int column_type = column_types[column];
                uint8_t* value = *it;
                if(column_type == SQLITE_TEXT)
                {
                    string s = reinterpret_cast<char*>(value);
                    napi_value text_value ;
                    napi_create_string_utf8(env, s.c_str(), s.length(), &text_value);
                    napi_set_named_property(env, row_value, column_name.c_str(), text_value);
                }
                if(column_type == SQLITE_FLOAT)
                {
                    double f = *value;
                    napi_value float_value ;
                    napi_create_double(env, f, &float_value);
                    napi_set_named_property(env, row_value, column_name.c_str(), float_value);
                }
                if(column_type == SQLITE_INTEGER)
                {
                    int n = *value;
                    napi_value int_value ;
                    napi_create_double(env, n, &int_value);
                    napi_set_named_property(env, row_value, column_name.c_str(), int_value);
                }
                it++;
            }
            napi_set_element(env, result_array, row, row_value);
        }
        return result_array;
    }
}

static napi_value
db_disconnect(napi_env env, napi_callback_info info)
{
    auto ret = disconnect(db);
    napi_value result;
    napi_create_int32(env, ret, &result);
    return result;
}




napi_value create_sync_sql(napi_env env) {
  napi_value result;
  NAPI_CALL(env, napi_create_object(env, &result));

  napi_value connect_function;
  napi_value exec_function;
  napi_value disconnect_function;
  NAPI_CALL(env, napi_create_function(env,
                                      "connect",
                                      NAPI_AUTO_LENGTH,
                                      db_connect,
                                      NULL,
                                      &connect_function));
    
    NAPI_CALL(env, napi_create_function(env,
                                        "exec",
                                        NAPI_AUTO_LENGTH,
                                        db_exec,
                                        NULL,
                                        &exec_function));
    
    NAPI_CALL(env, napi_create_function(env,
                                        "disconnect",
                                        NAPI_AUTO_LENGTH,
                                        db_disconnect,
                                        NULL,
                                        &disconnect_function));

  NAPI_CALL(env, napi_set_named_property(env,
                                         result,
                                         "connect",
                                         connect_function));
    
    NAPI_CALL(env, napi_set_named_property(env,
                                           result,
                                           "disconnect",
                                           disconnect_function));
    
    NAPI_CALL(env, napi_set_named_property(env,
                                           result,
                                           "exec",
                                           exec_function));

  return result;
}


NAPI_MODULE_INIT() {
  return create_sync_sql(env);
}
