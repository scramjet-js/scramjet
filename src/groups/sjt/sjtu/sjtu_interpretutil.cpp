// sjtu_interpretutil.cpp
#include <sjtu_interpretutil.h>

#include <bdlma_localsequentialallocator.h>

#include <bsl_iostream.h>
#include <bsl_vector.h>
#include <bsls_assert.h>

#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtu_datumutil.h>

using namespace BloombergLP;

namespace sjtu {
bdld::Datum
InterpretUtil::interpretBytecode(Allocator            *allocator,
                                 const sjtt::Bytecode *firstCode,
                                 int                   numCodes) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != firstCode || 0 == numCodes);

    bdlma::LocalSequentialAllocator<2046> tempAlloc(allocator);
    bsl::vector<bdld::Datum> stack(&tempAlloc);

    bdld::Datum locals[sjtt::Bytecode::s_NumLocals];

#ifdef BSLS_ASSERT_IS_ACTIVE
    int i = 0;
#endif

    while (true) {
        const sjtt::Bytecode& code = *firstCode;
        switch (code.opcode()) {

          case sjtt::Bytecode::e_Push: {

            stack.push_back(code.data());
          } break;

          case sjtt::Bytecode::e_Load: {
            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(sjtt::Bytecode::s_NumLocals >
                        code.data().theInteger());

            stack.push_back(locals[code.data().theInteger()]);
          } break;

          case sjtt::Bytecode::e_Store: {
            BSLS_ASSERT(code.data().isInteger());
            BSLS_ASSERT(sjtt::Bytecode::s_NumLocals >
                        code.data().theInteger());
            locals[code.data().theInteger()] = stack.back();
            stack.pop_back();
          } break;

          case sjtt::Bytecode::e_AddDoubles: {

            BSLS_ASSERT(1 < stack.size());
            BSLS_ASSERT(stack.back().isDouble());
            BSLS_ASSERT(stack[stack.size() - 2].isDouble());

            const double l = stack.back().theDouble();
            stack.pop_back();
            const double result = l + stack.back().theDouble();
            stack.pop_back();
            stack.push_back(bdld::Datum::createDouble(result));
          } break;

          case sjtt::Bytecode::e_Execute: {

            BSLS_ASSERT(!stack.empty());
            BSLS_ASSERT(DatumUtil::isExternalFunction(stack.back()));

            const DatumUtil::ExternalFunction f =
                                  DatumUtil::getExternalFunction(stack.back());
            stack.pop_back();
            BSLS_ASSERT(stack.back().isInteger());
            const int numArgs = stack.back().theInteger();
            stack.pop_back();
            const Datum *end = stack.end();
            const Datum *firstArg = end - numArgs;
            const Datum result =
                      f(sjtt::ExecutionContext(&tempAlloc, firstArg, numArgs));
            stack.erase(firstArg, end);
            stack.push_back(result);
          } break;

          case sjtt::Bytecode::e_Return: {

            BSLS_ASSERT(!stack.empty());

            return stack.back().clone(allocator);
          } break;
        }
        ++firstCode;
        BSLS_ASSERT(++i < numCodes);
    }
}
}
