QT += core gui
QT += network
QT += sql
QT += xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AShareDividendDifferentiatedDeclaration.cpp \
    CheckBoxHeaderView.cpp \
    MaximumAmountRegularDeclaration.cpp \
    RTGS_instructions.cpp \
    SecuritiesRegistrationtop.cpp \
    dashboard.cpp \
    deliveryManager.cpp \
    fundbalancequery.cpp \
    fundmanagement.cpp \
    globaldata.cpp \
    main.cpp \
    riskManagement.cpp \
    rtgsdeclaration.cpp \
    toast.cpp \
    user.cpp \
    widget.cpp

HEADERS += \
    AShareDividendDifferentiatedDeclaration.h \
    CheckBoxHeaderView.h \
    MaximumAmountRegularDeclaration.h \
    SecuritiesRegistrationtop.h \
    dashboard.h \
    deliveryManager.h \
    fundbalancequery.h \
    fundmanagement.h \
    globaldata.h \
    riskManagement.h \
    rtgsdeclaration.h \
    toast.h \
    widget.h

FORMS += \
    AShareDividendDifferentiatedDeclaration.ui \
    MaximumAmountRegularDeclaration.ui \
    SecuritiesRegistrationtop.ui \
    dashboard.ui \
    deliveryManager.ui \
    fundbalancequery.ui \
    fundmanagement.ui \
    riskManagement.ui \
    rtgsdeclaration.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
