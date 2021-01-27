#include "subjtypedialog.h"
#include "application.h"

#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QHeaderView>

SubjtypeDialog::SubjtypeDialog(QWidget *parent) :
    TreeDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Subjects"));
    setWindowIcon(QIcon(":/icons/icons/subject-16.png"));

    setInfoIconVisible(true);
    setInfoText(tr("Use subjects for grouping desicions!"));

    m_model = new SubjtypeModel;
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    treeView()->setModel(m_proxyModel);
    treeView()->hideColumn(0);

    setProxyModel(m_proxyModel);
}

SubjtypeDialog::~SubjtypeDialog()
{
    saveDialogState();

    delete m_model;
    delete m_proxyModel;
}

void SubjtypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SubjtypeDialog/geometry").toByteArray());
    treeView()->header()->restoreState(settings->value("DoctypeDialog/tV_itemView").toByteArray());
}

void SubjtypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("SubjtypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", treeView()->header()->saveState());
    settings->endGroup();
}
