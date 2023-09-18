{
  "targets": [
    {
      "target_name": "sync_sql",
      "sources": ["src/*.cc" ],
      "link_settings":
      {
        "libraries": [
            "-lsqlite3",
         ]
      }
    }
  ]
} 
