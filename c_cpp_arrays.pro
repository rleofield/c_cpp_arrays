TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


#DESTDIR += bin
CONFIG  += x86_64


OBJECTS_DIR = .tmp
MOC_DIR = .tmp
UI_DIR = .tmp
RCC_DIR = .tmp


QMAKE_CXXFLAGS += -gdwarf-3


QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_DEBUG += -Wall
QMAKE_CXXFLAGS_DEBUG += -Wcast-align
QMAKE_CXXFLAGS_DEBUG += -Wcast-qual
QMAKE_CXXFLAGS_DEBUG += -Wconversion
QMAKE_CXXFLAGS_DEBUG += -Weffc++
QMAKE_CXXFLAGS_DEBUG += -Werror
QMAKE_CXXFLAGS_DEBUG += -Wextra
QMAKE_CXXFLAGS_DEBUG += -Wno-zero-as-null-pointer-constant
QMAKE_CXXFLAGS_DEBUG += -Wold-style-cast
QMAKE_CXXFLAGS_DEBUG += -Woverloaded-virtual
QMAKE_CXXFLAGS_DEBUG += -Wparentheses
QMAKE_CXXFLAGS_DEBUG += -Wpedantic
QMAKE_CXXFLAGS_DEBUG += -Wreturn-type
QMAKE_CXXFLAGS_DEBUG += -Wshadow
QMAKE_CXXFLAGS_DEBUG += -Wunused-but-set-variable
QMAKE_CXXFLAGS_DEBUG += -Wunused-function
QMAKE_CXXFLAGS_DEBUG += -Wunused-parameter
QMAKE_CXXFLAGS_DEBUG += -Wunused-variable
QMAKE_CXXFLAGS_DEBUG += -Wwrite-strings



QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -Wparentheses
QMAKE_CXXFLAGS_RELEASE += -Wreturn-type
QMAKE_CXXFLAGS_RELEASE += -Wshadow
QMAKE_CXXFLAGS_RELEASE += -Wextra
QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter
QMAKE_CXXFLAGS_RELEASE -= -Wwrite-strings
QMAKE_CXXFLAGS_RELEASE -= -Wunused-variable
QMAKE_CXXFLAGS_RELEASE += -msse
#QMAKE_CXXFLAGS_RELEASE += -Weffc++


message("qmake c_cpp_arrays")


SOURCES += \
    src/main.cpp


HEADERS +=



INCLUDEPATH += $$PWD/src



INCLUDEPATH += $$PWD/../libpngjpegz/png/h $$PWD/../libpngjpegz/zlib/h $$PWD/../libpngjpegz/jpeg
DEPENDPATH += $$PWD/../libpngjpegz
PRE_TARGETDEPS += $$PWD/../libpngjpegz/libpngjpegz.a

LIBS += -L$$PWD/../libpngjpegz/ -lpngjpegz

LIBS += -lboost_system
LIBS += -lboost_filesystem

OTHER_FILES += \
    README.md \
    lgpl-3.0.txt


