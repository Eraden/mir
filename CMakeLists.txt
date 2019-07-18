cmake_minimum_required(VERSION 3.13)
project(mir)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall -Wpedantic)

set(Boost_USE_STATIC_LIBS ON)

find_package(PostgreSQL)
find_package(Boost 1.69.0 COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
include_directories(src includes)

message(INFO " Psql version ${PostgreSQL_VERSION_STRING}")
message(INFO " Boost.Test libraries ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")

file(GLOB DB_INCLUDES_FILES "includes/mir/db/*.hpp")
file(GLOB MIR_INCLUDES_FILES "includes/mir/mir/*.hpp")
file(GLOB UTILS_INCLUDES_FILES "includes/mir/utils/*.hpp")

set(
        INCLUDES_FILES
        ${DB_INCLUDES_FILES}
        ${MIR_INCLUDES_FILES}
        ${UTILS_INCLUDES_FILES}
)

set(
        DB_SOURCE
        src/db/Connection.cpp
        src/db/Url.cpp
)

set(
        UTILS_SOURCE
        src/utils/Logger.cpp
        src/utils/SqlPrinter.cpp
)

set(
        MIR_SOURCE
        ${INCLUDES_FILES}
        src/mir/CommandLineInterface.cpp
        src/mir/UpMigrator.cpp
        src/mir/DownMigrator.cpp
        src/mir/SetupMigrator.cpp
        src/mir/MigrationDirectoryReader.cpp
        src/mir/RunMigrationQuery.cpp
        src/mir/LoadMigrationsQuery.cpp
)

add_library(mirdb STATIC ${DB_SOURCE})
add_library(mirutils STATIC ${UTILS_SOURCE})

set(LIBS mirdb mirutils)

add_executable(mir ${MIR_SOURCE} src/mir/main.cpp src/mir/ConfigReader.cpp src/mir/Generator.cpp src/mir/CheckRunner.cpp)

# tests
add_executable(mir_test ${MIR_SOURCE} src/mir/test.cpp)
add_executable(db_test ${DB_SOURCE} src/db/test.cpp)

target_compile_definitions(mir_test PRIVATE UNIT_TESTING=1)
target_compile_definitions(db_test PRIVATE UNIT_TESTING=1)

target_link_libraries(mir ${LIBS})
target_link_libraries(mir ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(mir ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(mir ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(mir ${PostgreSQL_LIBRARIES})

target_link_libraries(db_test mirutils)
target_link_libraries(db_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(db_test ${Boost_LIBRARIES})
target_link_libraries(db_test ${PostgreSQL_LIBRARIES})

target_link_libraries(mir_test ${LIBS})
target_link_libraries(mir_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(mir_test ${Boost_LIBRARIES})
target_link_libraries(mir_test ${PostgreSQL_LIBRARIES})

enable_testing()

add_test(NAME MirTest COMMAND ./mir_test)
add_test(NAME DbTest COMMAND ./db_test)

install(
        TARGETS mir mirdb mirutils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/mir
        ARCHIVE DESTINATION lib/mir
        INCLUDES DESTINATION includes/mir
)

message(INFO " includes ${INCLUDES_FILES}")

install(FILES ${DB_INCLUDES_FILES} DESTINATION include/mir/db)
install(FILES ${MIR_INCLUDES_FILES} DESTINATION include/mir/mir)
install(FILES ${UTILS_INCLUDES_FILES} DESTINATION include/mir/utils)
install(FILES includes/mir/mir.hpp DESTINATION include/mir)
