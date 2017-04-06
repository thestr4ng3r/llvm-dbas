
#include "ir-dbg.h"

#include <iostream>

using namespace std;
using namespace llvm;

void ProcessModule(llvm::Module *M)
{
	const auto &functions = M->getFunctionList();

	for(const Function &F : functions)
	{
		if(F.isDeclaration())
			continue;

		cout << F.getName().str() << endl;

		for(const BasicBlock &B : F.getBasicBlockList())
		{
			for(const Instruction &I : B.getInstList())
			{
				cout << "\t" << Instruction::getOpcodeName(I.getOpcode()) << endl;
			}
			cout << "\t-----------------\n";
		}
	}
}