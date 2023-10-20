#ifndef ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H
#define ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H

#include <QWidget>

namespace Ui {
class AShareDividendDifferentiatedDeclaration;
}

class AShareDividendDifferentiatedDeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit AShareDividendDifferentiatedDeclaration(QWidget *parent = nullptr);
    ~AShareDividendDifferentiatedDeclaration();

public:
    Ui::AShareDividendDifferentiatedDeclaration *ui;
};

#endif // ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H
