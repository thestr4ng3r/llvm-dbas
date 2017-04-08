//
// Created by florian on 08.04.17.
//

#ifndef LLVM_IRDBG_LLDEBUGINFO_H
#define LLVM_IRDBG_LLDEBUGINFO_H

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

    // TODO: do not save this as a member
    DISubprogram *Subprogram;

  public:
    LLDebugInfo(llvm::Module *M, StringRef File, StringRef Directory);

    void addFunction(Function *F);
    void addInstruction(Instruction *I, unsigned int line);
    void finalize();
  };
}
#endif //LLVM_IRDBG_LLDEBUGINFO_H
