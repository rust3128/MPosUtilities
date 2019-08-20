#-------------------------------------------------
#
# Project created by QtCreator 2019-08-14T15:55:39
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPosUtilites
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# If there is no version tag in git this one will be used
VERSION = 0.1.0

# Need to discard STDERR so get path to NULL device
win32 {
    NULL_DEVICE = NUL # Windows doesn't have /dev/null but has NUL
} else {
    NULL_DEVICE = /dev/null
}

# Need to call git with manually specified paths to repository
BASE_GIT_COMMAND = git --git-dir $$PWD/.git --work-tree $$PWD

# Trying to get version from git tag / revision
GIT_VERSION = $$system($$BASE_GIT_COMMAND describe --always --tags 2> $$NULL_DEVICE)

# Check if we only have hash without version number
!contains(GIT_VERSION,\d+\.\d+\.\d+) {
    # If there is nothing we simply use version defined manually
    isEmpty(GIT_VERSION) {
        GIT_VERSION = $$VERSION
    } else { # otherwise construct proper git describe string
        GIT_COMMIT_COUNT = $$system($$BASE_GIT_COMMAND rev-list HEAD --count 2> $$NULL_DEVICE)
        isEmpty(GIT_COMMIT_COUNT) {
            GIT_COMMIT_COUNT = 0
        }
        GIT_VERSION = $$VERSION-$$GIT_COMMIT_COUNT-g$$GIT_VERSION
    }
}

# Turns describe output like 0.1.5-42-g652c397 into "0.1.5.42.652c397"
GIT_VERSION ~= s/-/"."
GIT_VERSION ~= s/g/""

# Now we are ready to pass parsed version to Qt
VERSION = $$GIT_VERSION
win32 { # On windows version can only be numerical so remove commit hash
    VERSION ~= s/\.\d+\.[a-f0-9]{6,}//
}

# Adding C preprocessor #DEFINE so we can use it in C++ code
# also here we want full version on every system so using GIT_VERSION
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"

win32: {
        BUILD_DATE = $$system( date /t ) $$system( time /t )
        BUILD_DATE = $$member(BUILD_DATE,0)_$$member(BUILD_DATE,1)
}
unix: BUILD_DATE = $$system( date "+%Y%m%d_%H%M" )

DEFINES += BUILD_DATE=\\\"$$BUILD_DATE\\\"

CONFIG += file_copies
COPIES += scriptsql

scriptsql.path = $$OUT_PWD/SQL
scriptsql.files = $$files($$PWD/SQL/*.sql)

CONFIG += c++11

SOURCES += \
        ConnectionSettingDialog/connectionsettingdialog.cpp \
        DataBases/databases.cpp \
        DataBases/databasesettings.cpp \
        DataBases/options.cpp \
        LoggingCategories/loggingcategories.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        ConnectionSettingDialog/connectionsettingdialog.h \
        DataBases/databases.h \
        DataBases/databasesettings.h \
        DataBases/options.h \
        LoggingCategories/loggingcategories.h \
        mainwindow.h

FORMS += \
        ConnectionSettingDialog/connectionsettingdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    SQL/programm.sql

RESOURCES += \
    resourses.qrc
