const sql_native = require('node-gyp-build')(__dirname);

class Sql {
    constructor() {
        this.db = null;
    }

    connect(path_like) {
        this.db = path_like
        return sql_native.connect(this.db)
    }

    exec(statement) {
        return sql_native.exec(this.db, statement)
    }

    run(statement, params) {
        if (params && params.length > 0) {
            for (let param of params) {
                statement = statement.replace(/\?/, param)
            }
        }
        return this.exec(statement)
    }

    insert(table, param) {
        let keys = [];
        let values = [];
        for (let key in param) {
            keys.push(key);
            values.push("'" + param[key] + "'")
        }
        let sql = "INSERT INTO " + table + " (" + keys.join(',') + ") VALUES (" + values.join(",") + ")";
        return this.exec(sql);
    }

    update(table, param, where) {
        let changes = []
        let clauses = []
        for (let key in param) {
            changes.push(key + '=' + "'" + param[key] + "'")
        }
        for (let key in where) {
            clauses.push(key + '=' + "'" + where[key] + "'")
        }
        let sql = "UPDATE " + table + " SET " + changes.join(',') + (clauses.length > 0 ? " WHERE " + clauses.join(" AND ") : "");
        return this.exec(sql);
    }

    disconnect() {
        return sql_native.disconnect(this.db)
    }
}

module.exports = Sql


