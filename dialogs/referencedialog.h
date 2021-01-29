#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class ReferenceDialog : public QDialog
{
    Q_OBJECT
public:
    ReferenceDialog(QWidget *parent = nullptr);
    ~ReferenceDialog();

    virtual void restoreDialogState() {};
    virtual void saveDialogState() {};

    QVBoxLayout * _layout() { return m_layout; };

private:
    QVBoxLayout *m_layout;
    QDialogButtonBox *m_bbox;
};

#endif // REFERENCEDIALOG_H
