cmake_minimum_required(VERSION 3.26)
# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
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

# ----- SFML (nou) -----
set(SFML_WARNINGS_AS_ERRORS OFF)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        # renovate: datasource=github-tags depName=SFML/SFML versioning=loose
        GIT_TAG        3.0.2
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)
# ----------------------

FetchContent_MakeAvailable(nlohmann_json tabulate SFML)

###############################################################################
# external dependencies with find_package
find_package(Threads REQUIRED)               # nou (cerut de SFML)
if(APPLE)                                     # nou
elseif(UNIX)                                  # nou
    find_package(X11)                         # nou (SFML pe Linux)
endif()                                       # nou

###############################################################################
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
        src/StatLeader.h
        src/NBALeague.cpp
        src/NBALeague.h
        src/Observer.h
        src/SezonSimulator.cpp
        src/SezonSimulator.h
        src/Observatori.cpp
        src/Observatori.h
        src/TwoWayPlayer.cpp
        src/TwoWayPlayer.h
        main.cpp
        src/Center.cpp
        src/Echipa.cpp
        src/Utilitati.h
        src/SezonLoader.h
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

# am adaugat SFML + Threads (si X11 pe Linux); legare intr-un singur apel cu PRIVATE
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        nlohmann_json::nlohmann_json
        tabulate::tabulate
        SFML::Graphics SFML::Window SFML::System Threads::Threads
)
if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

###############################################################################
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets data COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})