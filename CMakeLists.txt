cmake_minimum_required(VERSION 3.16)
project(TSP_BranchAndBound VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 or Qt5
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Widgets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/graphwidget.cpp
    src/tsp_solver.cpp
    src/node.cpp
    src/edge.cpp
    src/graph.cpp
)

# Header files
set(HEADERS
    include/mainwindow.h
    include/graphwidget.h
    include/tsp_solver.h
    include/node.h
    include/edge.h
    include/graph.h
)

# Create executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Set Windows subsystem to avoid console window on Windows
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries
if(${QT_VERSION_MAJOR} EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
