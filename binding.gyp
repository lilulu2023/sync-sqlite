{
  "targets": [
    {
      "target_name": "sync_sql",
      "sources": [ "sync_sql.cc", "sqlite3_call.cc" ],
      "link_settings":
      {
        "libraries": [
            "-lsqlite3",
         ]
      }
    }
  ]
} 
