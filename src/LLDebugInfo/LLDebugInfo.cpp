
#include "LLDebugInfo.h"

using namespace llvm;

LLDebugInfo::LLDebugInfo(llvm::Module *M, StringRef Filename, StringRef Directory)
        : M(M), DIB(*M) {
  M->addModuleFlag(Module::Error, "Debug Info Version", (uint32_t)DEBUG_METADATA_VERSION);
  M->addModuleFlag(Module::Error, "Dwarf Version", 4);

  CU = DIB.createCompileUnit(dwarf::DW_LANG_lo_user,
                                           Filename,
                                           Directory,
                                           "llvm-irdbg",
                                           false,
                                           "",
                                           0);

  File = CU->getFile();
}

void LLDebugInfo::addFunction(Function *F) {
  DITypeRefArray a;
  DISubroutineType *ST = DIB.createSubroutineType(a);
  Subprogram = DIB.createFunction(File, F->getName(), F->getName(), File, 0, ST, true, true, 0);
  F->setMetadata("dbg", Subprogram);
}

void LLDebugInfo::addInstruction(Instruction *I, unsigned int Line) {
  DILocation *DebugLocation = DILocation::get(M->getContext(), Line + 1, 0, Subprogram);
  I->setMetadata("dbg", DebugLocation);
}

void LLDebugInfo::finalize() {
  DIB.finalize();
}