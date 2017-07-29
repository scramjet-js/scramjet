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

using namespace BloombergLP;

namespace sjtu {

bdld::Datum
InterpretUtil::interpret(Allocator             *allocator,
                         const sjtt::Function&  function,
                         const Datum           *args) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != args || 0 == function.argCount());

    const sjtt::Function *nextFun  = &function;
    int bottom;
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
/*
                                 const sjtt::Bytecode *codes) {
    BSLS_ASSERT(0 != codes);

    bsl::vector<Datum> stack(sjtt::Bytecode::s_MinInitialStackSize,
                             sjtd::DatumUdtUtil::s_Undefined);
    bsl::vector<sjtt::Frame> frames;
    frames.emplace_back(0, codes, codes);
    sjtt::Frame *frame = &frames.back();
    while (true) {
        const sjtt::Bytecode& code = *frame->pc();
        switch (code.opcode()) {

          case sjtt::Bytecode::e_Push: {

            stack.push_back(code.data());
          } break;

          case sjtt::Bytecode::e_Load: {

            BSLS_ASSERT(code.data().isInteger());
            stack.push_back(frame->getValue(&stack, code.data().theInteger()));
          } break;

          case sjtt::Bytecode::e_Store: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(stack.size() > frame->bottom());
            frame->getValue(&stack, code.data().theInteger()) = stack.back();
            stack.pop_back();
          } break;

          case sjtt::Bytecode::e_Jump: {

            BSLS_ASSERT(code.data().isInteger());
            frame->jump(code.data().theInteger());
            continue;
          } break;

          case sjtt::Bytecode::e_If: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(stack.size() > frame->bottom());
            BSLS_ASSERT(stack.back().isBoolean());
            const bool cond = stack.back().theBoolean();
            stack.pop_back();
            if (cond) {
                frame->jump(code.data().theInteger());
                continue;
            }
          } break;

          case sjtt::Bytecode::e_IfEqInts: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(stack.size() - frame->bottom() >= 2);
            BSLS_ASSERT(stack.back().isInteger());
            const int first = stack.back().theInteger();
            stack.pop_back();
            BSLS_ASSERT(stack.back().isInteger());
            const bool cond = stack.back().theInteger() == first;
            stack.pop_back();
            if (cond) {
                frame->jump(code.data().theInteger());
                continue;
            }
          } break;

          case sjtt::Bytecode::e_EqInts: {

            BSLS_ASSERT(stack.size() - frame->bottom() >= 2);
            BSLS_ASSERT(stack.back().isInteger());
            BSLS_ASSERT(stack[stack.size() - 2].isInteger());

            const int l = stack.back().theInteger();
            stack.pop_back();
            bdld::Datum& back = stack.back();
            back = bdld::Datum::createBoolean(l == back.theInteger());
          } break;

          case sjtt::Bytecode::e_IncInt: {
            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(stack.size() - frame->bottom() >
                        code.data().theInteger());
            BSLS_ASSERT(
                frame->getValue(&stack, code.data().theInteger()).isInteger());
            bdld::Datum& value = frame->getValue(&stack,
                                                 code.data().theInteger());
            value = bdld::Datum::createInteger(value.theInteger() + 1);
          } break;

          case sjtt::Bytecode::e_AddDoubles: {

            BSLS_ASSERT(stack.size() - frame->bottom() >= 2);
            BSLS_ASSERT(stack.back().isDouble());
            BSLS_ASSERT(stack[stack.size() - 2].isDouble());

            const double l = stack.back().theDouble();
            stack.pop_back();
            bdld::Datum& back = stack.back();
            back = bdld::Datum::createDouble(l + back.theDouble());
          } break;

          case sjtt::Bytecode::e_AddInts: {

            BSLS_ASSERT(stack.size() - frame->bottom() >= 2);
            BSLS_ASSERT(stack.back().isInteger());
            BSLS_ASSERT(stack[stack.size() - 2].isInteger());

            const int l = stack.back().theInteger();
            stack.pop_back();
            bdld::Datum& back = stack.back();
            back = bdld::Datum::createInteger(l + back.theInteger());
          } break;

          case sjtt::Bytecode::e_Call: {
            BSLS_ASSERT(stack.size() > frame->bottom());
            BSLS_ASSERT(stack.back().isInteger());
            BSLS_ASSERT(code.data().isInteger());

            const int argCount = stack.back().theInteger();
            stack.pop_back();
            const int newBottom  = stack.size() - argCount;
            BSLS_ASSERT(stack.size() - argCount >= frame->bottom());
            const int numToAdd =
                              sjtt::Bytecode::s_MinInitialStackSize - argCount;
            if (0 < numToAdd) {
                stack.insert(stack.end(),
                             numToAdd,
                             sjtd::DatumUdtUtil::s_Undefined);
            }
            frames.emplace_back(newBottom,
                                frame->firstCode(),
                                frame->firstCode() + code.data().theInteger());
            frame = &frames.back();
            continue;                             // skip past normal increment
          } break;

          case sjtt::Bytecode::e_Execute: {

            BSLS_ASSERT(stack.size() - frame->bottom() >= 2);
            BSLS_ASSERT(sjtd::DatumUdtUtil::isExternalFunction(stack.back()));

            const sjtd::DatumUdtUtil::ExternalFunction f =
                         sjtd::DatumUdtUtil::getExternalFunction(stack.back());
            stack.pop_back();
            BSLS_ASSERT(stack.back().isInteger());
            const int numArgs = stack.back().theInteger();
            stack.pop_back();
            BSLS_ASSERT(stack.size() - frame->bottom() >= numArgs);
            const Datum *end = stack.end();
            const Datum *firstArg = end - numArgs;
            const Datum result =
                       f(sjtt::ExecutionContext(allocator, firstArg, numArgs));
            stack.erase(firstArg, end);
            stack.push_back(result);
          } break;

          case sjtt::Bytecode::e_Exit: {

            BSLS_ASSERT(stack.size() > frame->bottom());

            const Datum value = stack.back();
            if (1 == frames.size()) {
                // If last frame, return the value.

                return value.clone(allocator);                // RETURN
            }
            else {

                // Pop all the values for the current frame off the stack.

                BSLS_ASSERT(stack.size() > frame->bottom());

                // pop back to the bottom of the current frame; this will
                // remove the arguments pushed on before calling

                stack.erase(stack.begin() + frame->bottom(), stack.end());

                // pop the frame and set the last one as current

                frames.pop_back();
                frame = &frames.back();

                // push on the return value

                stack.push_back(value);
            }
          } break;

          case sjtt::Bytecode::e_Resize: {
            BSLS_ASSERT(code.data().isInteger());
            stack.resize(frame->bottom() + code.data().theInteger(),
                         sjtd::DatumUdtUtil::s_Undefined);
          } break;
        }
        frame->incrementPc();
    }
}
}
*/
