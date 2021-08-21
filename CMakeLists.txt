cmake_minimum_required(VERSION 3.13)
project(zgws)

set(CMAKE_C_STANDARD 99)

add_definitions(-DCA=\"${PROJECT_SOURCE_DIR}/ca.pem\")
add_definitions(-DWWW=\"${PROJECT_SOURCE_DIR}/www\")
add_definitions(-DMG_ENABLE_OPENSSL=1)

find_package(OpenSSL REQUIRED)

SET(MONGOOSE mongoose.c mongoose.h)
file(GLOB ZNP znp/*.h znp/*.c)
file(GLOB ZCL zcl/*.h zcl/*.c)
file(GLOB APP app/*.h app/*.c)

add_executable(zgws main.c mt_uart.h mt_uart.c mt_frame.h mt_frame.c ${ZNP} ${ZCL} ${APP} ${MONGOOSE})
target_link_libraries(zgws OpenSSL::SSL json-c)