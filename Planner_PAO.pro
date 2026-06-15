QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    Sources/Model/AbstractActivity.cpp \
    Sources/Model/ActivityManager.cpp \
    Sources/Model/HourMinute.cpp \
    Sources/Model/date.cpp \
    Sources/Model/event.cpp \
    Sources/Model/project.cpp \
    Sources/Model/reminder.cpp \
    Sources/Model/Routine.cpp \
    Sources/Model/tag.cpp \
    Sources/Model/tagManager.cpp \
    Sources/Model/task.cpp \
    Sources/Model/Persistence/JSONformat.cpp \
    Sources/Model/Persistence/xmlFormat.cpp \
    Sources/UI/Forms/ActivityForm.cpp \
    Sources/UI/Forms/EventForm.cpp \
    Sources/UI/Forms/ProjectForm.cpp \
    Sources/UI/Forms/ReminderForm.cpp \
    Sources/UI/Forms/RoutineForm.cpp \
    Sources/UI/Forms/TaskForm.cpp \
    Sources/UI/Widgets/ActivityDelete.cpp \
    Sources/UI/Widgets/ActivityModify.cpp \
    Sources/UI/Widgets/ActivitySearch.cpp \
    Sources/UI/Widgets/AddWidget.cpp \
    Sources/UI/Widgets/DayWidget.cpp \
    Sources/UI/Widgets/TagComboBox.cpp \
    Sources/UI/Widgets/TagWidget.cpp \
    Sources/UI/Widgets/TaskWidget.cpp \
    Sources/UI/Widgets/calendar.cpp \
    Sources/UI/Widgets/month.cpp \
    Sources/UI/Widgets/navBar.cpp \
    Sources/Visitor/DayWidgetVisitor.cpp \
    Sources/Visitor/DisplayVisitor.cpp \
    Sources/Visitor/FormSelectVisitor.cpp \
    Sources/Visitor/TaskWidgetVisitor.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp

HEADERS += \
    Headers/Model/AbstractActivity.h \
    Headers/Model/ActivityManager.h \
    Headers/Model/HourMinute.h \
    Headers/Model/config.h \
    Headers/Model/date.h \
    Headers/Model/event.h \
    Headers/Model/project.h \
    Headers/Model/rec_ev.h \
    Headers/Model/reminder.h \
    Headers/Model/routine.h \
    Headers/Model/tag.h \
    Headers/Model/tagManager.h \
    Headers/Model/task.h \
    Headers/Model/Persistence/JSONformat.h \
    Headers/Model/Persistence/xmlFormat.h \
    Headers/UI/Forms/ActivityForm.h \
    Headers/UI/Forms/EventForm.h \
    Headers/UI/Forms/ProjectForm.h \
    Headers/UI/Forms/ReminderForm.h \
    Headers/UI/Forms/RoutineForm.h \
    Headers/UI/Forms/TaskForm.h \
    Headers/UI/Widgets/ActivityDelete.h \
    Headers/UI/Widgets/ActivityModel.h \
    Headers/UI/Widgets/ActivityModify.h \
    Headers/UI/Widgets/ActivitySearch.h \
    Headers/UI/Widgets/AddWidget.h \
    Headers/UI/Widgets/DayWidget.h \
    Headers/UI/Widgets/TagComboBox.h \
    Headers/UI/Widgets/TagWidget.h \
    Headers/UI/Widgets/TaskWidget.h \
    Headers/UI/Widgets/calendar.h \
    Headers/UI/Widgets/month.h \
    Headers/UI/Widgets/navBar.h \
    Headers/Visitor/ActivityVisitor.h \
    Headers/Visitor/AddSubtaskVisitor.h \
    Headers/Visitor/DayWidgetVisitor.h \
    Headers/Visitor/DisplayVisitor.h \
    Headers/Visitor/FormSelectVisitor.h \
    Headers/Visitor/TaskWidgetVisitor.h \
    Headers/mainwindow.h


