cmake_minimum_required(VERSION 3.16)
project(CareerNavigator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# If Qt6 is not in default search paths, look in /Users/gunjan/Qt/6.11.1/macos
if(EXISTS "/Users/gunjan/Qt/6.11.1/macos")
    list(PREPEND CMAKE_PREFIX_PATH "/Users/gunjan/Qt/6.11.1/macos")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Backend Core Sources
set(BACKEND_SOURCES
    User.cpp
    Career.cpp
    CareerDatabase.cpp
    RecommendationEngine.cpp
    Roadmap.cpp
    Step.cpp
)

set(BACKEND_HEADERS
    User.h
    Career.h
    CareerDatabase.h
    RecommendationEngine.h
    CareerPath.h
    DegreeCareerPath.h
    SkillBasedCareerPath.h
    Roadmap.h
    Step.h
    FileUtils.h
)

# GUI Application Executable
add_executable(CareerNavigator
    main_gui.cpp
    MainWindow.h
    MainWindow.cpp
    ThemeManager.h
    ${BACKEND_SOURCES}
    ${BACKEND_HEADERS}
)

target_link_libraries(CareerNavigator PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Copy CSV files to the output directory alongside binary
add_custom_command(TARGET CareerNavigator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/careers.csv"
        "$<TARGET_FILE_DIR:CareerNavigator>/careers.csv"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/roadmap_data.csv"
        "$<TARGET_FILE_DIR:CareerNavigator>/roadmap_data.csv"
)

# Also support building the original CLI tool for evaluation comparison
add_executable(career_navigator_cli
    main.cpp
    ${BACKEND_SOURCES}
    ${BACKEND_HEADERS}
)
