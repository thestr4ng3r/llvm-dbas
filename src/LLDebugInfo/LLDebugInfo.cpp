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

#include <llvm/Support/raw_ostream.h>

using namespace llvm;

LLDebugInfo::LLDebugInfo(llvm::Module *M, StringRef Filename, StringRef Directory)
        : M(M), DIB(*M) {
  M->addModuleFlag(Module::Warning, "Debug Info Version", (uint32_t)DEBUG_METADATA_VERSION);
  M->addModuleFlag(Module::Warning, "Dwarf Version", 4);

  DIFile *DebugSrcFile = DIB.createFile(Filename, Directory);
  CU = DIB.createCompileUnit(dwarf::DW_LANG_lo_user,
                             DebugSrcFile,
                             "llvm-dbas",
                             false,
                             "",
                             0);

  File = CU->getFile();
}

DIType *LLDebugInfo::addType(Type *Ty, llvm::Module *Mod) {
  DataLayout layout = DataLayout(Mod);
  switch (Ty->getTypeID()) {
  case Type::HalfTyID:
    return DIB.createBasicType("half", 16, dwarf::DW_ATE_float);
  case Type::FloatTyID:
    return DIB.createBasicType("float", 32, dwarf::DW_ATE_float);
  case Type::DoubleTyID:
    return DIB.createBasicType("double", 64, dwarf::DW_ATE_float);
  case Type::X86_FP80TyID:
    return DIB.createBasicType("x86_fp80", 80, dwarf::DW_ATE_float);
  case Type::FP128TyID:
    return DIB.createBasicType("fp128", 128, dwarf::DW_ATE_float);
  case Type::PPC_FP128TyID:
    return DIB.createBasicType("ppc_fp128", 128, dwarf::DW_ATE_float);
  case Type::VoidTyID:
  case Type::LabelTyID:
  case Type::MetadataTyID:
  case Type::X86_MMXTyID:
  case Type::VectorTyID:
  case Type::TokenTyID: {
    std::string name;
    raw_string_ostream os(name);
    Ty->print(os);
    return DIB.createUnspecifiedType(name);
  }
  case Type::IntegerTyID: {
    IntegerType *IntTy = dyn_cast<IntegerType>(Ty);
    return DIB.createBasicType("i" + std::to_string(IntTy->getBitWidth()), IntTy->getBitWidth(), dwarf::DW_ATE_unsigned);
  }
  case Type::FunctionTyID: {
    FunctionType *FuncTy = dyn_cast<FunctionType>(Ty);
    std::vector<Metadata *> argTypes;
    for (Type *ArgType : FuncTy->params()) {
      argTypes.push_back(addType(ArgType, Mod));
    }
    return DIB.createSubroutineType(DIB.getOrCreateTypeArray(argTypes));
  }
  case Type::StructTyID: {
    StructType *StructTy = dyn_cast<StructType>(Ty);
    if (StructTy->isOpaque()) {
      return DIB.createUnspecifiedType(StructTy->getName());
    }
    std::vector<Metadata *> argTypes;
    for (Type *ArgType : StructTy->elements()) {
      argTypes.push_back(addType(ArgType, Mod));
    }
    return DIB.createStructType(File, StructTy->hasName() ? StructTy->getName() : "", File, 0, layout.getTypeSizeInBits(Ty), layout.getABITypeAlignment(Ty), DINode::FlagZero, nullptr, DIB.getOrCreateArray(argTypes));
  }
  case Type::ArrayTyID: {
    ArrayType *ArrayTy = dyn_cast<ArrayType>(Ty);
    std::vector<Metadata *> subscripts;
    subscripts.push_back(DIB.getOrCreateSubrange(0, ArrayTy->getNumElements()));
    return DIB.createArrayType(layout.getTypeSizeInBits(Ty), layout.getABITypeAlignment(Ty), addType(ArrayTy->getElementType(), Mod), DIB.getOrCreateArray(subscripts));
  }
  case Type::PointerTyID: {
    PointerType *PointerTy = dyn_cast<PointerType>(Ty);
    return DIB.createPointerType(addType(PointerTy->getElementType(), Mod), layout.getTypeSizeInBits(Ty));
  }
  }
}

DISubprogram *LLDebugInfo::addFunction(Function *F, unsigned int Line) {
  DISubroutineType *ST = dyn_cast<DISubroutineType>(addType(F->getFunctionType(), M));
  DISubprogram *SP = DIB.createFunction(File, F->getName(), F->getName(), File, Line, ST, true, true, 0);
  F->setSubprogram(SP);
  return SP;
}

void LLDebugInfo::addParameters(Function *F, DISubprogram *SP, unsigned int Line) {
  int i = 0;
  for (Argument &arg : F->args()) {
    DIType *Ty = addType(arg.getType(), M);
    DILocalVariable *V = DIB.createParameterVariable(SP, arg.getName(), i++, File, Line, Ty);
    DIB.insertDbgValueIntrinsic(&arg, 0, V, DIB.createExpression(), DebugLoc::get(Line, 0, SP).get(), F->getEntryBlock().getFirstNonPHI());
  }

}

void LLDebugInfo::addInstruction(Instruction *I, DISubprogram *SP, std::string NameStr, int NameID, unsigned int Line) {
  std::string name = NameStr.empty() ? std::to_string(NameID) : NameStr;
  DebugLoc loc = DebugLoc::get(Line, 0, SP);
  I->setDebugLoc(loc);
  if (!I->getType()->isVoidTy()) {
    DIType *Ty = addType(I->getType(), M);
    DILocalVariable *V = DIB.createAutoVariable(SP, name, File, Line, Ty);
    DIB.insertDbgValueIntrinsic(I, 0, V, DIB.createExpression(), loc.get(), I->getParent());
  }
}

void LLDebugInfo::finalize() {
  DIB.finalize();
}
