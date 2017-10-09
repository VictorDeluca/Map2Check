#pragma once

#include <llvm/Pass.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Metadata.h>
#include <llvm/IR/Constants.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

class LibraryFunctions {
    Constant* map2check_init = NULL;
    Constant* map2check_exit = NULL;

public:
    Constant* getInitFunction() { return this->map2check_init; }
    Constant* getExitFunction() { return this->map2check_exit; }

    LibraryFunctions(Function *F, LLVMContext* Ctx) {
        this->map2check_init = F->getParent()->
         getOrInsertFunction("map2check_init",
                 Type::getVoidTy(*Ctx),
                 Type::getInt32Ty(*Ctx),
                 NULL);

        this->map2check_exit = F->getParent()->
          getOrInsertFunction("map2check_success",
                  Type::getVoidTy(*Ctx),
                  NULL);
    }

};