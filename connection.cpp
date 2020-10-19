#include "connection.h"
#include "application.h"

Connection::Connection() {}

void Connection::connect(QString host, QString dbname, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=10;MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1");

    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);

    db.open();

    if (!db.isDriverAvailable("QMYSQL")) {
        throw ConnectionExeption("QMYSQL driver not avaible");
    } else if (!db.isOpen()){
        throw ConnectionExeption("Could not connect to database");
    }
}
