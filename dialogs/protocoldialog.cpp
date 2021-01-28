#include "protocoldialog.h"

#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDebug>
#include <QDate>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>
#include <QHeaderView>

ProtocolDialog::ProtocolDialog(QWidget *parent) :
    ChoiceDialog(parent)
{
    m_view = new ProtocolView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Protocols"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    connect(pB_details, &QPushButton::clicked, this, [=] {
        m_view->details();
    });

    buttonLayout()->addWidget(pB_details);
}

ProtocolDialog::~ProtocolDialog()
{
    saveDialogState();

    delete pB_details;
    delete m_view;
}

void ProtocolDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("ProtocolDialog/geometry").toByteArray());
}

void ProtocolDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("ProtocolDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

int ProtocolDialog::exec()
{
    m_view->refresh();
    return ChoiceDialog::exec();
}

void ProtocolDialog::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    pB_details->setEnabled(current.isValid());
    ChoiceDialog::onCurrentChanged(current, previous);
}

bool ProtocolDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || !m_view->selectionModel()->selection().isEmpty();
}

QVariant ProtocolDialog::choice(const QModelIndex &current) const
{
    if(current.isValid()) {
        return m_view->proxyModel()->mapToSource(current.siblingAtColumn(0)).data();
    }

    return QVariant();
}
