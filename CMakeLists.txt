cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)

FetchContent_Declare(
        tabulate
        GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
        GIT_TAG        master
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(nlohmann_json tabulate)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        src/Exceptii.h
        src/Exceptii.cpp
        src/Guard.cpp
        src/Guard.h
        src/Forward.cpp
        src/Forward.h
        src/Center.cpp
        src/Center.h
        src/Stats.cpp
        src/Stats.h
        src/TwoWayPlayer.cpp
        src/TwoWayPlayer.h
)
target_sources(${MAIN_EXECUTABLE_NAME}
        PRIVATE
        src/Contract.cpp
        src/Player.cpp
        src/Echipa.cpp
        src/Sezon.cpp
        src/SezonLoader.cpp
        src/Utilitati.cpp
        src/Exceptii.cpp
        src/Guard.cpp
        src/Forward.cpp
        src/Center.cpp
        src/Stats.cpp
        src/TwoWayPlayer.cpp
)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
        ext
)
target_link_libraries(${MAIN_EXECUTABLE_NAME}
        nlohmann_json::nlohmann_json
        tabulate::tabulate
)

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets data COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})