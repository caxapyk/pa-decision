#ifndef HTMLTEMPLATE_H
#define HTMLTEMPLATE_H

#include <QObject>

class HtmlTemplate : public QObject
{
    Q_OBJECT
public:
    explicit HtmlTemplate(const QString &path, QObject *parent = nullptr);
    void setVars(const QMap<QString, QVariant> &vars);
    void print();


private:
    QString m_html;
    QString m_stylesheet;
};

#endif // HTMLTEMPLATE_H
