QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chart_adapters.cpp \
    charts.cpp \
    directory_item.cpp \
    directory_model.cpp \
    group_by_strategies.cpp \
    list_adapter.cpp \
    main.cpp \
    window.cpp

HEADERS += \
    chart_adapters.h \
    charts.h \
    directory_item.h \
    directory_model.h \
    directory_observer.h \
    group_by_strategies.h \
    list_adapter.h \
    window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
