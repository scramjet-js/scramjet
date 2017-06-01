// sjtu_interpretutil.t.cpp                                     -*-C++-*-

#include <sjtu_interpretutil.h>


#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <bsl_vector.h>

#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtu_datumutil.h>

using namespace BloombergLP;
using namespace bsl;
using namespace sjtu;

// ============================================================================
//                     STANDARD BDE ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        cout << "Error " __FILE__ "(" << line << "): " << message
             << "    (failed)" << endl;

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BDLS_TESTUTIL_ASSERT
#define ASSERTV      BDLS_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BDLS_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BDLS_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BDLS_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BDLS_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BDLS_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BDLS_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BDLS_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BDLS_TESTUTIL_LOOP6_ASSERT

#define Q            BDLS_TESTUTIL_Q   // Quote identifier literally.
#define P            BDLS_TESTUTIL_P   // Print identifier and value.
#define P_           BDLS_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BDLS_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BDLS_TESTUTIL_L_  // current Line number


namespace {
    bdld::Datum testExecute(const sjtt::ExecutionContext& context) {
        double result = 0;
        for (int i = 0; i < context.numArgs(); ++i) {
            const bdld::Datum& arg = context.args()[i];
            result += arg.theDouble();
        }
        return bdld::Datum::createDouble(result);
    }
}

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int         test = argc > 1 ? atoi(argv[1]) : 0;
    const bool     verbose = argc > 2;
    const bool veryVerbose = argc > 3;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    // TODO: need to set up a DSEL for this.

    switch (test) { case 0:
      case 4: {
        if (verbose) cout << endl
                          << "load and store" << endl
                          << "==============" << endl;

        // We're going to:
        // 1. store a 1 in location 3
        // 2. push a 2 on the stack
        // 3. load the value from location 3
        // 4. return it and verify it's a 1

        bdlma::SequentialAllocator alloc;
        bsl::vector<sjtt::Bytecode> code;
        code.push_back(sjtt::Bytecode::createOpcode(
                                               sjtt::Bytecode::e_Push,
                                               bdld::Datum::createInteger(1)));
        code.push_back(sjtt::Bytecode::createOpcode(
                                               sjtt::Bytecode::e_Store,
                                               bdld::Datum::createInteger(3)));
        code.push_back(sjtt::Bytecode::createOpcode(
                                               sjtt::Bytecode::e_Push,
                                               bdld::Datum::createInteger(2)));
        code.push_back(sjtt::Bytecode::createOpcode(
                                               sjtt::Bytecode::e_Load,
                                               bdld::Datum::createInteger(3)));
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Return));
        const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                  &alloc,
                                                                  &code[0],
                                                                  code.size());
        ASSERT(bdld::Datum::createInteger(1) == result);
      } break;

      case 3: {
        if (verbose) cout << endl
                          << "execute" << endl
                          << "=========" << endl;

        // This loop is meant to test that the execute logic:
        // 1. works for 0 args
        // 2. works for 1 arg
        // 3. works for > 1 args
        // 4. leaves the stack in the right place after evaluation

        bdlma::SequentialAllocator alloc;

        for (int i = 0; i < 3; ++i) {
            bsl::vector<sjtt::Bytecode> code;

            // Base value, so we always add at least one

            code.push_back(sjtt::Bytecode::createOpcode(
                                                sjtt::Bytecode::e_Push,
                                                bdld::Datum::createDouble(1)));

            // The arguments for the function to execute

            for (int j = 0; j < i; ++j) {
                code.push_back(sjtt::Bytecode::createOpcode(
                                                sjtt::Bytecode::e_Push,
                                                bdld::Datum::createDouble(2)));
            }

            // push the number of arguments

            code.push_back(sjtt::Bytecode::createOpcode(
                                               sjtt::Bytecode::e_Push,
                                               bdld::Datum::createInteger(i)));

            // the function to execute

            code.push_back(sjtt::Bytecode::createOpcode(
                     sjtt::Bytecode::e_Push,
                     sjtu::DatumUtil::datumFromExternalFunction(testExecute)));

            // execute it

            code.push_back(sjtt::Bytecode::createOpcode(
                                                   sjtt::Bytecode::e_Execute));

            // add the base 1 to the result

            code.push_back(sjtt::Bytecode::createOpcode(
                                                sjtt::Bytecode::e_AddDoubles));

            // return it

            code.push_back(sjtt::Bytecode::createOpcode(
                                                    sjtt::Bytecode::e_Return));
            const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                  &alloc,
                                                                  &code[0],
                                                                  code.size());
            const bdld::Datum expected = bdld::Datum::createDouble(1 + i * 2);
            LOOP_ASSERT(i, expected == result);
        }
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "add doubles" << endl
                          << "=========" << endl;

        bdlma::SequentialAllocator alloc;
        bsl::vector<sjtt::Bytecode> code;
        code.push_back(sjtt::Bytecode::createOpcode(
                                              sjtt::Bytecode::e_Push,
                                              bdld::Datum::createDouble(3.0)));
        code.push_back(sjtt::Bytecode::createOpcode(
                                              sjtt::Bytecode::e_Push,
                                              bdld::Datum::createDouble(1.0)));
        code.push_back(sjtt::Bytecode::createOpcode(
                                                sjtt::Bytecode::e_AddDoubles));
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Return));
        const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                  &alloc,
                                                                  &code[0],
                                                                  code.size());
        ASSERT(bdld::Datum::createDouble(4.0) == result);
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "push and return" << endl
                          << "=========" << endl;

        bdlma::SequentialAllocator alloc;
        bsl::vector<sjtt::Bytecode> code;
        const bdld::Datum value = bdld::Datum::createInteger(3);
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Push,
                                                    value));
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Return));
        const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                  &alloc,
                                                                  &code[0],
                                                                  code.size());
        ASSERT(value == result);
      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "." << endl;
    }
    return testStatus;
}
