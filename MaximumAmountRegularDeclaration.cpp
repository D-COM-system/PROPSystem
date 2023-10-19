#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"

MaximumAmountRegularDeclaration::MaximumAmountRegularDeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaximumAmountRegularDeclaration)
{
    ui->setupUi(this);
}

MaximumAmountRegularDeclaration::~MaximumAmountRegularDeclaration()
{
    delete ui;
}
