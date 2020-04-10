TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ../Téléchargements/semaphoresDoctor-master/Reprography.cpp
LIBS += -lpthread -lrt -lboost_system -lboost_thread
