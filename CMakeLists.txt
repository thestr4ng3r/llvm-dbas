cmake_minimum_required(VERSION 3.2)
project(llvm_dbinf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ir-dbg.cpp ir-dbg.h)


find_package(LLVM 3.8 REQUIRED)
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if(NOT LLVM_ENABLE_RTTI)
	add_definitions(-fno-rtti)
endif()

add_executable(llvm_dbinf ${SOURCE_FILES})

target_link_libraries(llvm_dbinf
		LLVMAsmParser
		LLVMBitWriter
		LLVMCore
		LLVMSupport)