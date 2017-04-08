# llvm-dbas
[![Build Status](https://travis-ci.org/thestr4ng3r/llvm-dbas.svg?branch=master)](https://travis-ci.org/thestr4ng3r/llvm-dbas)

llvm-dbas is an LLVM Assembler adding Debug Information at IR Level. 
This version is based on **LLVM 3.8**.

When compiling LLVM IR code to machine code, there usually is afterwards no
easy way to keep track of which native instructions resulted from which LLVM
instruction.

This tool works very similar to llvm-as, which converts a given .ll file containing
LLVM IR in text form and converts it to the equivalent bitcode. In addition to that,
llvm-dbas also adds Debug Information Metadata to the IR containing information such as
the original line number in the .ll file.

This enables debugging on IR level, as shown in this example:

```
$ llvm-dbas test.ll -o test_debug.bc

$ clang test_debug.bc -O0 -o test_debug

$ lldb ./test_debug
(lldb) target create "./test_debug"
Current executable set to './test_debug' (x86_64).
(lldb) b main
Breakpoint 1: where = test_debug`main + 4, address = 0x0000000000400504
(lldb) r
Process 2523 launched: './test_debug' (x86_64)
Process 2523 stopped
* thread #1, name = 'test_debug', stop reason = breakpoint 1.1
    frame #0: 0x0000000000400504 test_debug`main at test.ll:13
   10  	define i32 @main() #0 {
   11  	  %1 = alloca i32, align 4
   12  	  %a = alloca i32, align 4
-> 13  	  store i32 0, i32* %1, align 4
   14  	  store i32 42, i32* %a, align 4
   15  	  %2 = load i32, i32* %a, align 4
   16  	  %3 = icmp slt i32 %2, 30
```

## Building
llvm-dbas uses CMake as its build system. It only depends on LLVM 3.8 and can be build like this:
```
$ mkdir build && cd build
$ cmake ..
$ make
```

## Usage
llvm-dbas can be used exactly the same way as llvm-as:
```
OVERVIEW: llvm .ll -> .bc assembler adding debug information on IR level

USAGE: llvm-dbas [options] <input .ll file>

OPTIONS:

General options:

  -disable-output                      - Disable output
  -f                                   - Enable binary output on terminals
  -filter-print-funcs=<function names> - Only print IR for functions whose name match this for all print-[before|after][-all] options
  -function-summary                    - Emit function summary index
  -o=<filename>                        - Override output filename
  -print-after-all                     - Print IR after each pass
  -print-before-all                    - Print IR before each pass
  -rng-seed=<seed>                     - Seed for the random number generator
  -time-passes                         - Time each pass, printing elapsed time for each on exit
  -verify-debug-info                   - 
  -verify-dom-info                     - Verify dominator info (time consuming)

Generic Options:

  -help                                - Display available options (-help-hidden for more)
  -help-list                           - Display list of available options (-help-list-hidden for more)
  -version                             - Display the version of this program
```

## Changes from llvm-as
The source of llvm-dbas is mostly identical to the parts of LLVM used for llvm-as.

Changes include:
* LLLexer keeps track of the current line number.
* LLDebugInfo is a class which is responsible for creating and attaching the Debug Information.
* LLParser uses LLDebugInfo while parsing the code.

## About
Created by Florian Märkl: https://www.metallic.software

This project is distributed under the University of Illinois Open Source License.
See [LICENSE.TXT](LICENSE.TXT) for details.