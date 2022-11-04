cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("VideoManager" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS}")

# Flags
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lboost_system -lboost_thread")
set(CMAKE_THREAD_LIBS_INIT "-lpthread")

# Source
file(GLOB VIDEO_MANAGER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(SRC ${SRC} ${VIDEO_MANAGER_SRC})
set(INC ${INC} ${CMAKE_CURRENT_SOURCE_DIR})

# Target
add_library(${PROJECT_NAME} SHARED ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB})
target_compile_definitions(${PROJECT_NAME} PUBLIC VIDEO_MANAGER_VERSION=1)

# Install
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
