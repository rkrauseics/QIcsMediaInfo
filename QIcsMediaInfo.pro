QT += core
QT -= gui

TARGET = QIcsMediaInfo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

QMAKE_CXXFLAGS += -DUNICODE
#QMAKE_LFLAGS += -stdlib=libc++

MEDIA_INFO_DIR=/Users/rkrause/Downloads/MediaInfo_CLI_GNU_FromSource

INCLUDEPATH += $${MEDIA_INFO_DIR}/ZenLib/Source
INCLUDEPATH += $${MEDIA_INFO_DIR}/MediaInfoLib/Source
LIBS += -L$${MEDIA_INFO_DIR}/MediaInfoLib/Project/GNU/Library -lmediainfo
LIBS += -L$${MEDIA_INFO_DIR}/ZenLib/Project/GNU/Library -lzen -lz
