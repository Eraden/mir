cmake_minimum_required(VERSION 3.13)
project(mir)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall -Wpedantic)

find_package(Threads)
find_package(PostgreSQL)
find_package(Boost 1.69.0 COMPONENTS system filesystem thread unit_test_framework REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
include_directories(src)

message(INFO " Psql version ${PostgreSQL_VERSION_STRING}")
message(INFO " Boost.Test libraries ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")

set(
        DB_SOURCE
        src/db/Connection.cpp src/db/Connection.hpp
        src/db/Query.hpp
        src/db/Url.cpp src/db/Url.hpp
)

set(
        UTILS_SOURCE
        src/utils/Logger.cpp src/utils/Logger.hpp
)

set(
        MIR_SOURCE
        src/mir/Migrator.cpp src/mir/Migrator.hpp
        src/mir/CommandLineInterface.cpp src/mir/CommandLineInterface.hpp
        src/mir/UpMigrator.cpp src/mir/UpMigrator.hpp
        src/mir/DownMigrator.cpp src/mir/DownMigrator.hpp
        src/mir/SetupMigrator.cpp src/mir/SetupMigrator.hpp
        src/mir/MigrationDirectoryReader.cpp src/mir/MigrationDirectoryReader.hpp
        src/mir/RunMigrationQuery.cpp src/mir/RunMigrationQuery.hpp
        src/mir/LoadMigrationsQuery.cpp src/mir/LoadMigrationsQuery.hpp
        src/mir/ConfigReader.hpp
)

set(
        HTTP_SOURCE
        src/http/server/Server.cpp src/http/server/Server.hpp
        src/http/server/Connection.cpp src/http/server/Connection.hpp
        src/http/server/ConnectionManager.cpp src/http/server/ConnectionManager.hpp
        src/http/server/Header.cpp src/http/server/Header.hpp
        src/http/server/MimeTypes.cpp src/http/server/MimeTypes.hpp
        src/http/server/Reply.cpp src/http/server/Reply.hpp
        src/http/server/RequestHandler.cpp src/http/server/RequestHandler.hpp
        src/http/server/Request.cpp src/http/server/Request.hpp
        src/http/server/RequestParser.cpp src/http/server/RequestParser.hpp
        src/http/server/PathHandler.cpp src/http/server/PathHandler.hpp
        src/http/handlers/VisitorHandler.cpp src/http/handlers/VisitorHandler.hpp
)

add_library(db STATIC ${DB_SOURCE})
add_library(utils STATIC ${UTILS_SOURCE})

set(
        LIBS
        db utils
)

add_executable(mir ${MIR_SOURCE} src/mir/main.cpp src/mir/ConfigReader.cpp src/mir/Generator.cpp src/mir/Generator.hpp)

# tests
add_executable(mir_test ${MIR_SOURCE} src/mir/test.cpp)
add_executable(db_test ${DB_SOURCE} src/db/test.cpp)

target_compile_definitions(mir_test PRIVATE UNIT_TESTING=1)
target_compile_definitions(db_test PRIVATE UNIT_TESTING=1)

target_link_libraries(mir ${LIBS})
target_link_libraries(mir ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mir ${Boost_LIBRARIES})
target_link_libraries(mir ${PostgreSQL_LIBRARIES})

target_link_libraries(db_test utils)
target_link_libraries(db_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(db_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(db_test ${Boost_LIBRARIES})
target_link_libraries(db_test ${PostgreSQL_LIBRARIES})

target_link_libraries(mir_test ${LIBS})
target_link_libraries(mir_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mir_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(mir_test ${Boost_LIBRARIES})
target_link_libraries(mir_test ${PostgreSQL_LIBRARIES})

enable_testing()

add_test(NAME MirTest COMMAND ./mir_test)
add_test(NAME DbTest COMMAND ./db_test)

install(
        TARGETS mir db utils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/mir
        ARCHIVE DESTINATION lib/mir
        INCLUDES DESTINATION include/mir
)
