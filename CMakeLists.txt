cmake_minimum_required(VERSION 3.2)
project(llvm-dbas)

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCE_FILES
		${SRC_DIR}/AsmParser/LLLexer.h
		${SRC_DIR}/AsmParser/LLLexer.cpp
		${SRC_DIR}/AsmParser/LLParser.h
		${SRC_DIR}/AsmParser/LLParser.cpp
		${SRC_DIR}/AsmParser/LLToken.h
		${SRC_DIR}/AsmParser/Parser.h
		${SRC_DIR}/AsmParser/Parser.cpp

		${SRC_DIR}/LLDebugInfo/LLDebugInfo.cpp
		${SRC_DIR}/LLDebugInfo/LLDebugInfo.h

		${SRC_DIR}/main.cpp)


find_package(LLVM 3.8 REQUIRED)
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#include_directories("${SRC_DIR}/AsmParser")
include_directories("${SRC_DIR}")

if(NOT LLVM_ENABLE_RTTI)
	add_definitions(-fno-rtti)
endif()

add_executable(llvm-dbas ${SOURCE_FILES})

target_link_libraries(llvm-dbas
		LLVMBitWriter
		LLVMCore
		LLVMSupport)