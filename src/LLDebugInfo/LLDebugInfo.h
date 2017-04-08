//===-- LLDebugInfo.h - Debug Info Builder for LLParser ----------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// These classes are implemented in LLDebugInfo/LLDebugInfo.cpp.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DBAS_LLDEBUGINFO_LLDEBUGINFO_H
#define LLVM_DBAS_LLDEBUGINFO_LLDEBUGINFO_H

#include "llvm/IR/Module.h"
#include "llvm/IR/DIBuilder.h"

namespace llvm {
  class LLDebugInfo
  {
  private:
    Module *M;

    DIBuilder DIB;
    DICompileUnit *CU;
    DIFile *File;

  public:
    LLDebugInfo(llvm::Module *M, StringRef File, StringRef Directory);

    DISubprogram *addFunction(Function *F);
    void addInstruction(Instruction *I, DISubprogram *SP, unsigned int line);
    void finalize();
  };
}
#endif //LLVM_IRDBG_LLDEBUGINFO_H
