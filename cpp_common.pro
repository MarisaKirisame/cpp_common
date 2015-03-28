TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    can_call.hpp \
    combinator.hpp \
    expansion.hpp \
    iterator.hpp \
    restriction.hpp \
    value_less.hpp
QMKAE_CXXFLAGS += -std=c++1z