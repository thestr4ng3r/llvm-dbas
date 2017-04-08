//===- Parser.cpp - Main dispatch module for the Parser library -----------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the functionality defined in AsmParser/Parser.h
//
//===----------------------------------------------------------------------===//

#include "Parser.h"
#include "LLParser.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/FileSystem.h"
using namespace llvm;

bool llvm::parseAssemblyInto(StringRef File, MemoryBufferRef F, Module &M, SMDiagnostic &Err,
							 SlotMapping *Slots) {
	SourceMgr SM;
	std::unique_ptr<MemoryBuffer> Buf = MemoryBuffer::getMemBuffer(F);
	SM.AddNewSourceBuffer(std::move(Buf), SMLoc());

  SmallString<256> FileV(File);
  sys::fs::make_absolute(FileV);
  sys::path::remove_filename(FileV);

  StringRef Filename = sys::path::filename(File);
  StringRef Directory = Twine(FileV).getSingleStringRef();

	return LLParser(Filename, Directory, F.getBuffer(), SM, Err, &M, Slots).Run();
}

std::unique_ptr<Module> llvm::parseAssembly(StringRef File, MemoryBufferRef F,
											SMDiagnostic &Err,
											LLVMContext &Context,
											SlotMapping *Slots) {
	std::unique_ptr<Module> M =
			make_unique<Module>(F.getBufferIdentifier(), Context);

	if (parseAssemblyInto(File, F, *M, Err, Slots))
		return nullptr;

	return M;
}

std::unique_ptr<Module> llvm::parseAssemblyFile(StringRef Filename,
												SMDiagnostic &Err,
												LLVMContext &Context,
												SlotMapping *Slots) {
	ErrorOr<std::unique_ptr<MemoryBuffer>> FileOrErr =
			MemoryBuffer::getFileOrSTDIN(Filename);
	if (std::error_code EC = FileOrErr.getError()) {
		Err = SMDiagnostic(Filename, SourceMgr::DK_Error,
						   "Could not open input file: " + EC.message());
		return nullptr;
	}

	return parseAssembly(Filename, FileOrErr.get()->getMemBufferRef(), Err, Context, Slots);
}
