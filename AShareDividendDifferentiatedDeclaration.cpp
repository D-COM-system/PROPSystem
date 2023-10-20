#include "AShareDividendDifferentiatedDeclaration.h"
#include "ui_AShareDividendDifferentiatedDeclaration.h"

AShareDividendDifferentiatedDeclaration::AShareDividendDifferentiatedDeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AShareDividendDifferentiatedDeclaration)
{
    ui->setupUi(this);
}

AShareDividendDifferentiatedDeclaration::~AShareDividendDifferentiatedDeclaration()
{
    delete ui;
}
