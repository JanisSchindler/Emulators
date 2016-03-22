TEMPLATE = subdirs

SUBDIRS += \
    Main

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/Input/release/ -lInput
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/Input/debug/ -lInput
else:unix: LIBS += -L$$OUT_PWD/Input/ -lInput

INCLUDEPATH += $$PWD/Input
DEPENDPATH += $$PWD/Input

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/Input/release/libInput.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/Input/debug/libInput.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/Input/release/Input.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/Input/debug/Input.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/Input/libInput.a
