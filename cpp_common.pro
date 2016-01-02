TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    can_call.hpp \
    expansion.hpp \
    iterator.hpp \
    restriction.hpp \
    value_less.hpp \
    scope.hpp \
    named_parameter.hpp \
    misc.hpp
QMKAE_CXXFLAGS += -std=c++1z
