cmake_minimum_required (VERSION 3.10)

project (modbus-rs485pi VERSION 1.0.0 DESCRIPTION "Modbus RTU RS485 half duplex Raspberry PI library")

set(CMAKE_C_FLAGS "-O3 -Wall -pthread")
include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED src/modbus_rs485pi.c src/mailbox.c src/gpio.c)
target_link_libraries(${PROJECT_NAME} PUBLIC modbus PUBLIC pigpio)
set_target_properties(${PROJECT_NAME} PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	PUBLIC_HEADER src/modbus_rs485pi.h)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

enable_testing()

add_executable(test_client tests/test_client.c)
target_link_libraries(test_client ${PROJECT_NAME})
add_test(NAME test_client COMMAND test_client)
