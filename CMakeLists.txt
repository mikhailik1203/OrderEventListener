cmake_minimum_required(VERSION 3.10)
project(OrderEventListener)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${PROJECT_SOURCE_DIR}
        $ENV{BOOST_INCLUDE})

LINK_DIRECTORIES(
        $ENV{BOOST_LIB})


add_executable(OrderEventListener
        main.cpp Listener.h OrderEventListenerDef.h EventListener.cpp EventListener.h test/testEventListener.cpp InstrumentStatistics.cpp InstrumentStatistics.h)