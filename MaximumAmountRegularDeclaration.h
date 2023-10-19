#ifndef MAXIMUMAMOUNTREGULARDECLARATION_H
#define MAXIMUMAMOUNTREGULARDECLARATION_H

#include <QWidget>

namespace Ui {
class MaximumAmountRegularDeclaration;
}

class MaximumAmountRegularDeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit MaximumAmountRegularDeclaration(QWidget *parent = nullptr);
    ~MaximumAmountRegularDeclaration();

public:
    Ui::MaximumAmountRegularDeclaration *ui;
};

#endif // MAXIMUMAMOUNTREGULARDECLARATION_H
