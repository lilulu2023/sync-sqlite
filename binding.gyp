{
  "targets": [
    {
      "target_name": "sync_sql",
      "sources": ["src/sqlite3_call.cc","src/sync_sql.cc" ],
      "link_settings":
      {
        "libraries": [
            "-lsqlite3",
         ]
      }
    }
  ]
} 
