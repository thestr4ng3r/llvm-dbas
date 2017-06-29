//===- LLDebugInfo.cpp - Debug Info Builder for LLParser -------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the functionality defined in LLDebugInfo/LLDebugInfo.h
//
//===----------------------------------------------------------------------===//

#include "LLDebugInfo.h"
using namespace llvm;

LLDebugInfo::LLDebugInfo(llvm::Module *M, StringRef Filename, StringRef Directory)
        : M(M), DIB(*M) {
  M->addModuleFlag(Module::Error, "Debug Info Version", (uint32_t)DEBUG_METADATA_VERSION);
  M->addModuleFlag(Module::Error, "Dwarf Version", 4);

  DIFile *DebugSrcFile = DIB.createFile(Filename, Directory);
  CU = DIB.createCompileUnit(dwarf::DW_LANG_lo_user,
                             DebugSrcFile,
                             "llvm-dbas",
                             false,
                             "",
                             0);

  File = CU->getFile();
}

DISubprogram *LLDebugInfo::addFunction(Function *F) {
  DITypeRefArray a;
  DISubroutineType *ST = DIB.createSubroutineType(a);
  DISubprogram *SP = DIB.createFunction(File, F->getName(), F->getName(), File, 0, ST, true, true, 0);
  F->setSubprogram(SP);
  return SP;
}

void LLDebugInfo::addInstruction(Instruction *I, DISubprogram *SP, unsigned int Line) {
  DebugLoc loc = DebugLoc::get(Line, 0, SP);
  I->setDebugLoc(loc);
}

void LLDebugInfo::finalize() {
  DIB.finalize();
}