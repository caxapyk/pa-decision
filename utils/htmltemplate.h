#ifndef HTMLTEMPLATE_H
#define HTMLTEMPLATE_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QTextDocument>
#include <QVariant>

class HtmlTemplate : public QObject
{
    Q_OBJECT
public:
    explicit HtmlTemplate(const QString &path, QObject *parent = nullptr);
    ~HtmlTemplate();

    void set(const QString &key, const QVariant &value);
    void print();

    QTextDocument *render();

private:
    QTextDocument *m_doc;

    QString m_html;
    QString m_stylesheet;

    QMap<QString, QVariant> m_vars;
};

#endif // HTMLTEMPLATE_H
