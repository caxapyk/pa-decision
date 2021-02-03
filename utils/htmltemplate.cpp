#include "htmltemplate.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QDebug>
#include <QFile>

HtmlTemplate::HtmlTemplate(const QString &path, QObject *parent) :
    QObject(parent)
{
    m_doc = new QTextDocument;

    QFile tmpl(path + ".html");
    QFile stylesheet(path + ".css");

    if (tmpl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream tmpl_stream(&tmpl);
        tmpl_stream.setCodec("UTF-8");
        m_html = tmpl_stream.readAll();

        tmpl.close();
    }

    if (stylesheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stylesheet_stream(&stylesheet);
        stylesheet_stream.setCodec("UTF-8");
        m_stylesheet = stylesheet_stream.readAll();

        stylesheet.close();
    }
}

HtmlTemplate::~HtmlTemplate()
{
    delete m_doc;
}

void HtmlTemplate::set(const QString &key, const QVariant &value)
{
    m_vars.insert(key, value);
}

QTextDocument *HtmlTemplate::render()
{
    for(int i = 0; i < m_vars.size(); ++i) {
        m_html.replace(QString("{{%1}}").arg(m_vars.keys().at(i)), m_vars.values().at(i).toString());
    }

    m_doc->setDefaultStyleSheet(m_stylesheet);
    m_doc->setHtml(m_html);

    return m_doc;
}

void HtmlTemplate::print()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);

    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument *td = render();
        td->print(&printer);
        td->setPageSize(QSizeF(printer.pageRect().size()));
        td->print(&printer);
    }
}
