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
    void testExecute(const sjtt::ExecutionContext& context) {
        ASSERT(1 == context.stack()->size());
        ASSERT(bdld::Datum::createDouble(1.0) == (*context.stack())[0]);
        context.stack()->push_back(bdld::Datum::createInteger(2));
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
      case 3: {
        if (verbose) cout << endl
                          << "execute" << endl
                          << "=========" << endl;

        bdlma::SequentialAllocator alloc;
        bsl::vector<sjtt::Bytecode> code;
        code.push_back(sjtt::Bytecode::createPush(
                                              bdld::Datum::createDouble(1.0)));
        code.push_back(sjtt::Bytecode::createPush(
                           DatumUtil::datumFromExternalFunction(testExecute)));
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Execute));
        code.push_back(sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Return));
        const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                  &alloc,
                                                                  &code[0],
                                                                  code.size());
        ASSERT(bdld::Datum::createInteger(2) == result);
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "add doubles" << endl
                          << "=========" << endl;

        bdlma::SequentialAllocator alloc;
        bsl::vector<sjtt::Bytecode> code;
        code.push_back(sjtt::Bytecode::createPush(
                                              bdld::Datum::createDouble(3.0)));
        code.push_back(sjtt::Bytecode::createPush(
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
        code.push_back(sjtt::Bytecode::createPush(value));
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
