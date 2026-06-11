QT       += core gui
QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/AbstractActivity.cpp \
    Sources/ActivityManager.cpp \
    Sources/Dialog/AddDialog.cpp \
    Sources/HourMinute.cpp \
    Sources/JSONformat.cpp \
    Sources/UI/Forms/EventForm.cpp \
    Sources/UI/Forms/ProjectForm.cpp \
    Sources/UI/Forms/ReminderForm.cpp \
    Sources/UI/Forms/RoutineForm.cpp \
    Sources/UI/Forms/TaskForm.cpp \
    Sources/UI/TaskWidget.cpp \
    #Sources/UI/addDialog.cpp \
    Sources/UI/calendar.cpp \
    Sources/UI/month.cpp \
    Sources/UI/navBar.cpp \
    Sources/UI/week.cpp \
    Sources/date.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/project.cpp \
    Sources/tag.cpp \
    Sources/task.cpp \
    Sources/tagManager.cpp \
    Sources/Routine.cpp \
    Sources/reminder.cpp \
    Sources/event.cpp \
    Sources/xmlFormat.cpp \
    Sources/Dialog/TaskDialog.cpp \
    Sources/UI/Forms/ActivityForm.cpp \
    Sources/UI/TagComboBox.cpp

HEADERS += \
    Headers/AbstractActivity.h \
    Headers/ActivityManager.h \
    Headers/ActivityVisitor.h \
    Headers/Dialog/AddDialog.h \
    Headers/DisplayVisitor.h \
    Headers/HourMinute.h \
    Headers/JSONformat.h \
    #Headers/UI/addDialog.h \
    Headers/UI/Forms/EventForm.h \
    Headers/UI/Forms/ProjectForm.h \
    Headers/UI/Forms/ReminderForm.h \
    Headers/UI/Forms/RoutineForm.h \
    Headers/UI/Forms/TaskForm.h \
    Headers/UI/calendar.h \
    Headers/UI/month.h \
    Headers/UI/navBar.h \
    Headers/UI/week.h \
    Headers/UI/DayWidget.h \
    Headers/Visitor/AddSubtaskVisitor.h \
    Headers/config.h \
    Headers/date.h \
    Headers/event.h \
    Headers/mainwindow.h \
    Headers/project.h \
    Headers/reminder.h \
    Headers/routine.h \
    Headers/tag.h \
    Headers/tagManager.h \
    Headers/task.h \
    Headers/xmlFormat.h \
    Headers/Visitor/ActivityVisitor.h \
    Headers/Visitor/DisplayVisitor.h \
    Headers/UI/TaskWidget.h \
    Headers/Dialog/TaskDialog.h \
    Headers/UI/Forms/ActivityForm.h \
    Headers/UI/TagComboBox.h \



FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
