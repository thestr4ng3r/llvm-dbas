
#include "ir-dbg.h"

#include <iostream>

using namespace std;
using namespace llvm;

void ProcessModule(llvm::Module *M)
{
	const auto &functions = M->getFunctionList();

	for(const Function &F : functions)
	{
		cout << F.getName().str() << endl;
	}
}