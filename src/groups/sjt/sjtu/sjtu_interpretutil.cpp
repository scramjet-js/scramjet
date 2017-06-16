// sjtu_interpretutil.cpp
#include <sjtu_interpretutil.h>

#include <bdlma_localsequentialallocator.h>

#include <bsl_vector.h>
#include <bsls_assert.h>

#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtt_datumudtutil.h>
#include <sjtt_frame.h>

using namespace BloombergLP;

namespace sjtu {

bdld::Datum
InterpretUtil::interpretBytecode(Allocator            *allocator,
                                 const sjtt::Bytecode *codes) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != codes);

    bsl::vector<Datum>       stack;
    bsl::vector<sjtt::Frame> frames;
    frames.emplace_back(0, codes, codes);
    sjtt::Frame *frame = &frames.back();
    frame->reserve(&stack, sjtt::Bytecode::s_MinInitialStackSize);
    while (true) {
        const sjtt::Bytecode& code = *frame->pc();
        switch (code.opcode()) {

          case sjtt::Bytecode::e_Push: {

            stack.push_back(code.data());
          } break;

          case sjtt::Bytecode::e_Load: {

            BSLS_ASSERT(code.data().isInteger());
            stack.push_back(frame->getValue(stack, code.data().theInteger()));
          } break;

          case sjtt::Bytecode::e_Store: {

            BSLS_ASSERT(code.data().isInteger());
            frame->setValue(&stack, code.data().theInteger(), stack.back());
            frame->pop(&stack);
          } break;

          case sjtt::Bytecode::e_Jump: {

            BSLS_ASSERT(code.data().isInteger());
            frame->jump(code.data().theInteger());
            continue;
          } break;

          case sjtt::Bytecode::e_If: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(!stack.empty());
            BSLS_ASSERT(stack.back().isBoolean());
            const bool cond = stack.back().theBoolean();
            frame->pop(&stack);
            if (cond) {
                frame->jump(code.data().theInteger());
                continue;
            }
          } break;

          case sjtt::Bytecode::e_IfEqInts: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(2 <= stack.size());
            BSLS_ASSERT(stack.back().isInteger());
            const int first = stack.back().theInteger();
            frame->pop(&stack);
            BSLS_ASSERT(stack.back().isInteger());
            const bool cond = stack.back().theInteger() == first;
            frame->pop(&stack);
            if (cond) {
                frame->jump(code.data().theInteger());
                continue;
            }
          } break;

          case sjtt::Bytecode::e_AddDoubles: {

            BSLS_ASSERT(1 < stack.size());
            BSLS_ASSERT(stack.back().isDouble());
            BSLS_ASSERT(stack[stack.size() - 2].isDouble());

            const double l = stack.back().theDouble();
            frame->pop(&stack);
            const double result = l + stack.back().theDouble();
            frame->pop(&stack);
            stack.push_back(bdld::Datum::createDouble(result));
          } break;

          case sjtt::Bytecode::e_Call: {
            BSLS_ASSERT(!stack.empty());
            BSLS_ASSERT(stack.back().isInteger());
            BSLS_ASSERT(code.data().isInteger());

            const int argCount = stack.back().theInteger();
            frame->pop(&stack);
            BSLS_ASSERT(stack.size() >= argCount);
            BSLS_ASSERT(stack.size() - argCount >= frame->bottom());
            frames.emplace_back(stack.size() - argCount,
                                frame->firstCode(),
                                frame->firstCode() + code.data().theInteger());
            frame = &frames.back();
            frame->reserve(&stack, sjtt::Bytecode::s_MinInitialStackSize);
            continue;                             // skip past normal increment
          } break;

          case sjtt::Bytecode::e_Execute: {

            BSLS_ASSERT(!stack.empty());
            BSLS_ASSERT(sjtt::DatumUdtUtil::isExternalFunction(stack.back()));

            const sjtt::DatumUdtUtil::ExternalFunction f =
                         sjtt::DatumUdtUtil::getExternalFunction(stack.back());
            frame->pop(&stack);
            BSLS_ASSERT(stack.back().isInteger());
            const int numArgs = stack.back().theInteger();
            frame->pop(&stack);
            const Datum *end = stack.end();
            const Datum *firstArg = end - numArgs;
            const Datum result =
                       f(sjtt::ExecutionContext(allocator, firstArg, numArgs));
            frame->popMany(&stack, numArgs);
            stack.push_back(result);
          } break;

          case sjtt::Bytecode::e_Exit: {

            BSLS_ASSERT(!stack.empty());

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

          case sjtt::Bytecode::e_Reserve: {
            BSLS_ASSERT(code.data().isInteger());

            frame->reserve(&stack, code.data().theInteger());
          } break;
        }
        frame->incrementPc();
    }
}
}
