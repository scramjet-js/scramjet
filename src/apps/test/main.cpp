#include <iostream>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/ToolOutputFile.h>

int main(int argc, char* argv[]) {
    llvm::LLVMContext c;
    llvm::Module m("sum.ll", c);
    m.setDataLayout("e-p: 64: 64: 64-i1: 8: 8-i8: 8: 8-i16: 16: 16-i32: "
                    "32: 32-i64: 64: 64-f32: 32: 32-f64: 64: 64-v64: 64: "
                    "64-v128: 128: 128-a0: 0: 64-s0: 64: 64-f80: 128: 128-n8: "
                    "16: 32: 64-S128");
    return 0;
}
