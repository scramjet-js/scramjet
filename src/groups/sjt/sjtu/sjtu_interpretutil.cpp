// sjtu_interpretutil.cpp
#include <sjtu_interpretutil.h>

#include <bdlma_localsequentialallocator.h>

#include <bsl_iostream.h>
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

    bsl::vector<sjtt::Frame> frames;
    frames.emplace_back(static_cast<const bdld::Datum *>(0), 0, codes);
    sjtt::Frame *frame = &frames.back();
    while (true) {
        const sjtt::Bytecode& code = *frame->pc();
        switch (code.opcode()) {

          case sjtt::Bytecode::e_Push: {

            frame->push(code.data());
          } break;

          case sjtt::Bytecode::e_Load: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(sjtt::Bytecode::s_NumLocals >
                        code.data().theInteger());

            frame->push(frame->locals()[code.data().theInteger()]);
          } break;

          case sjtt::Bytecode::e_Store: {

            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(sjtt::Bytecode::s_NumLocals >
                        code.data().theInteger());
            frame->setLocal(code.data().theInteger(), frame->stack().back());
            frame->pop();
          } break;

          case sjtt::Bytecode::e_Jump: {

            BSLS_ASSERT(code.data().isInteger());
            frame->jump(code.data().theInteger());
            continue;
          } break;

          case sjtt::Bytecode::e_Gosub: {

            BSLS_ASSERT(code.data().isInteger());

            const int offset = (frame->pc() + 1) - frame->firstCode();
            frame->push(bdld::Datum::createInteger(offset));
            frame->jump(code.data().theInteger());
            continue;
          } break;

          case sjtt::Bytecode::e_Return: {

            BSLS_ASSERT(!frame->stack().empty());
            BSLS_ASSERT(frame->stack().back().isInteger());

            frame->jump(frame->stack().back().theInteger());
            frame->pop();
            continue;
          } break;

          case sjtt::Bytecode::e_AddDoubles: {

            BSLS_ASSERT(1 < frame->stack().size());
            BSLS_ASSERT(frame->stack().back().isDouble());
            BSLS_ASSERT(frame->stack()[frame->stack().size() - 2].isDouble());

            const double l = frame->stack().back().theDouble();
            frame->pop();
            const double result = l + frame->stack().back().theDouble();
            frame->pop();
            frame->push(bdld::Datum::createDouble(result));
          } break;

          case sjtt::Bytecode::e_Call: {
            BSLS_ASSERT(!frame->stack().empty());
            BSLS_ASSERT(frame->stack().back().isInteger());
            BSLS_ASSERT(frame->stack().size() >
                        frame->stack().back().theInteger() + 1);
            BSLS_ASSERT(code.data().isInteger());

            const int argCount = frame->stack().back().theInteger();
            const Datum *args = (&frame->stack().back()) - 1;
            frames.emplace_back(args,
                                argCount,
                                frame->firstCode() + code.data().theInteger());
            frame = &frames.back();
            continue;                             // skip past normal increment
          } break;

          case sjtt::Bytecode::e_Execute: {

            BSLS_ASSERT(!frame->stack().empty());
            BSLS_ASSERT(sjtt::DatumUdtUtil::isExternalFunction(
                                                       frame->stack().back()));

            const sjtt::DatumUdtUtil::ExternalFunction f =
                sjtt::DatumUdtUtil::getExternalFunction(frame->stack().back());
            frame->pop();
            BSLS_ASSERT(frame->stack().back().isInteger());
            const int numArgs = frame->stack().back().theInteger();
            frame->pop();
            const Datum *end = frame->stack().end();
            const Datum *firstArg = end - numArgs;
            const Datum result =
                       f(sjtt::ExecutionContext(allocator, firstArg, numArgs));
            frame->popMany(numArgs);
            frame->push(result);
          } break;

          case sjtt::Bytecode::e_Exit: {

            BSLS_ASSERT(!frame->stack().empty());

            const Datum value = frame->stack().back();
            if (1 == frames.size()) {
                // If last frame, return the value.

                return value.clone(allocator);                // RETURN
            }
            else {
                // Otherwise, pop the frame and push the last value

                frames.pop_back();
                frame = &frames.back();
                BSLS_ASSERT(1 <= frame->stack().size());
                BSLS_ASSERT(frame->stack().back().isInteger());
                BSLS_ASSERT((1 + frame->stack().back().theInteger()) <=
                            frame->stack().size());

                // Pop off the arg count and arguments that were used in the
                // previous call.

                frame->popMany(1 + frame->stack().back().theInteger());
            }
          } break;
        }
        frame->incrementPc();
    }
}
}
