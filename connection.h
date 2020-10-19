#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

struct ConnectionExeption {
    const char* p;
    ConnectionExeption (const char* q) { p=q; }
};

class Connection
{
public:
    Connection();
    void connect(QString host, QString db, QString username, QString pass);
};

#endif // CONNECTION_H
