QT += core
QT -= gui

TARGET = QIcsMediaInfo
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

QMAKE_CXXFLAGS += -DUNICODE
#QMAKE_LFLAGS += -stdlib=libc++

MEDIA_INFO_DIR=/Users/rkrause/src/cpp/MediaInfo/MediaInfoLib/Source
ZENLIB_DIR=/Users/rkrause/src/cpp/MediaInfo/ZenLib/Source

INCLUDEPATH += $${ZENLIB_DIR}
INCLUDEPATH += $${MEDIA_INFO_DIR}

LIBS += -L$${MEDIA_INFO_DIR}/Library -lMediaInfo
LIBS += -L$${ZENLIB_DIR}/Library -lZenLib -lz
