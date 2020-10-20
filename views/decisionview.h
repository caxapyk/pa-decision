#ifndef DECISIONVIEW_H
#define DECISIONVIEW_H

#include "views/view.h"

#include <QWidget>

namespace Ui {
class DecisionView;
}

class DecisionView : public View
{
    Q_OBJECT

public:
    explicit DecisionView(QWidget *parent = nullptr);
    ~DecisionView();

    void restoreViewState() override;
    void saveViewState() override;

private:
    Ui::DecisionView *ui;

    void initialize() override;
};

#endif // DECISIONVIEW_H
