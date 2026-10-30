QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = CareerNavigator
TEMPLATE = app

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    User.cpp \
    Career.cpp \
    CareerDatabase.cpp \
    RecommendationEngine.cpp \
    Roadmap.cpp \
    Step.cpp

HEADERS += \
    MainWindow.h \
    ThemeManager.h \
    FileUtils.h \
    User.h \
    Career.h \
    CareerDatabase.h \
    RecommendationEngine.h \
    CareerPath.h \
    DegreeCareerPath.h \
    SkillBasedCareerPath.h \
    Roadmap.h \
    Step.h

DISTFILES += \
    careers.csv \
    roadmap_data.csv \
    README.md
