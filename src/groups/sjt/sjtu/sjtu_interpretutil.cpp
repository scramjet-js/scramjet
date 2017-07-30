// sjtu_interpretutil.cpp
#include <sjtu_interpretutil.h>

#include <bdlma_localsequentialallocator.h>

#include <bsl_set.h>
#include <bsl_vector.h>
#include <bsls_assert.h>

#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtd_datumudtutil.h>
#include <sjtt_frame.h>

#include <bsl_iostream.h>

using namespace BloombergLP;

namespace sjtu {

bdld::Datum
InterpretUtil::interpret(Allocator             *allocator,
                         const sjtt::Function&  function,
                         const Datum           *args) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != args || 0 == function.argCount());

    const sjtt::Function *nextFun  = &function;
    int bottom = 0;
    sjtt::Frame *frame;

    bsl::vector<bdld::Datum> stack(function.argCount() + function.numLocals());
    ::memcpy(stack.data(), args, sizeof(bdld::Datum) * function.argCount());
    bsl::vector<sjtt::Frame> frames;

    typedef sjtt::Bytecode BC;

call:
    frames.emplace_back(bottom, nextFun->code(), nextFun->code());
    frame = &frames.back();
next_code:
    const sjtt::Bytecode& code = *frame->pc();
    switch (code.opcode()) {
      case BC::e_Allocate:
      case BC::e_AllocateI32:
      case BC::e_AllocateDouble: {
        const int curSize = stack.size();
        stack.resize(curSize + 1);
        frame->getValue(&stack, code.dest()) = Datum::createInteger(curSize);
      } break;
      case BC::e_Store:
      case BC::e_StoreI32:
      case BC::e_StoreDouble: {
        const Datum& xIndex = frame->getValue(&stack, code.x());
        BSLS_ASSERT(xIndex.isInteger());
        const Datum& y = frame->getValue(&stack, code.y());
        BSLS_ASSERT(BC::e_StoreI32 != code.opcode() || y.isInteger());
        BSLS_ASSERT(BC::e_StoreDouble != code.opcode() || y.isDouble());
        frame->getValue(&stack, xIndex.theInteger()) = y;
      } break;
      case BC::e_Load:
      case BC::e_LoadI32:
      case BC::e_LoadDouble: {
        const Datum& xIndex = frame->getValue(&stack, code.x());
        BSLS_ASSERT(xIndex.isInteger());
        const Datum& x = frame->getValue(&stack, xIndex.theInteger());
        BSLS_ASSERT(BC::e_LoadI32 != code.opcode() || x.isInteger());
        BSLS_ASSERT(BC::e_LoadDouble != code.opcode() || x.isDouble());
        frame->getValue(&stack, code.dest()) = x;
      } break;
      case BC::e_EqI32: {
        const Datum& x = frame->getValue(&stack, code.x());
        BSLS_ASSERT(x.isInteger());
        const Datum& y = frame->getValue(&stack, code.y());
        BSLS_ASSERT(y.isInteger());
        const int result = x.theInteger() == y.theInteger() ? 1 : 0;
        frame->getValue(&stack, code.dest()) = Datum::createInteger(result);
      } break;
      case BC::e_Const:
      case BC::e_ConstI32:
      case BC::e_ConstDouble: {
        BSLS_ASSERT(code.opcode() != BC::e_ConstI32 ||
                    code.value().isInteger());
        BSLS_ASSERT(code.opcode() != BC::e_ConstDouble ||
                    code.value().isDouble());
        frame->getValue(&stack, code.dest()) = code.value();
      } break;
      case BC::e_ExtractI32:
      case BC::e_ExtractDouble: {
        const Datum& x = frame->getValue(&stack, code.x());
        BSLS_ASSERT(code.opcode() != BC::e_ExtractI32 || x.isInteger());
        BSLS_ASSERT(code.opcode() != BC::e_ExtractDouble || x.isDouble());
        frame->getValue(&stack, code.dest()) = x;
      } break;
      case BC::e_Jump: {
        frame->jump(code.offset());
        goto next_code;
      } break;
      case BC::e_IfI32: {
        const Datum& x = frame->getValue(&stack, code.x());
        BSLS_ASSERT(x.isInteger());
        if (0 != x.theInteger()) {
            frame->jump(code.offset());
            goto next_code;
        }
      } break;
      case BC::e_AddI32: {
        const Datum& x = frame->getValue(&stack, code.x());
        BSLS_ASSERT(x.isInteger());
        const Datum& y = frame->getValue(&stack, code.y());
        BSLS_ASSERT(y.isInteger());
        const int result = x.theInteger() + y.theInteger();
        frame->getValue(&stack, code.dest()) = Datum::createInteger(result);
      } break;
      case BC::e_Call: {
        bottom = stack.size();
        const sjtt::Function& fun = code.function();
        const short *args = code.args();
        stack.resize(stack.size() + fun.argCount() + fun.numLocals());
        for (int i = 0; i < fun.argCount(); ++i) {
            stack[bottom + i] = stack[args[i]];
        }
        nextFun = &fun;
        goto call;
      } break;
      case BC::e_Return: {
        // read the result

        const bdld::Datum result = frame->getValue(&stack, code.x());

        // If only one frame we're done; return from function.

        if (1 == frames.size()) {
            return result;
        }

        // clear data from the returning frame off the stack

        stack.resize(frame->bottom());

        // remove the current frame and restore the calling frame

        frames.pop_back();
        frame = &frames.back();

        // store the result in the location indicated by the current code on
        // the calling frame -- we didn't increment it after calling, but will
        // after we fall out from the switch

        frame->getValue(&stack, frame->pc()->dest()) = result;
      }
    }
    frame->incrementPc();
    goto next_code;
}
}
