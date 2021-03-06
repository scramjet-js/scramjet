// sjtu_interpretutil.cpp
#include <sjtu_interpretutil.h>

#include <bdlma_localsequentialallocator.h>

#include <bsl_vector.h>
#include <bsls_assert.h>

#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtd_datumudtutil.h>
#include <sjtt_frame.h>

using namespace BloombergLP;

namespace sjtu {

bdld::Datum
InterpretUtil::interpretBytecode(Allocator            *allocator,
                                 const sjtt::Bytecode *codes) {
    BSLS_ASSERT(0 != allocator);
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
