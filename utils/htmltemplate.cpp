#include "htmltemplate.h"

#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QDebug>
#include <QFile>

HtmlTemplate::HtmlTemplate(const QString &path, QObject *parent) :
    QObject(parent)
{
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

void HtmlTemplate::setVars(const QMap<QString, QVariant> &vars)
{
    for(int i = 0; i < vars.size(); ++i) {
        m_html.replace(vars.keys().at(i), vars.values().at(i).toString());
    }
}

void HtmlTemplate::print()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);

    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument td;
        td.setPageSize(QSizeF(printer.pageRect().size()));
        td.setDefaultStyleSheet(m_stylesheet);
        td.setHtml(m_html);

        td.print(&printer);
    }
}
