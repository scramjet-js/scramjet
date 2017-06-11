// sjtu_interpretutil.t.cpp                                     -*-C++-*-

#include <sjtu_interpretutil.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <bsl_vector.h>

#include <sjtm_datumfactory.h>
#include <sjtt_bytecode.h>
#include <sjtt_executioncontext.h>
#include <sjtu_bytecodedslutil.h>
#include <sjtu_interpretutil.h>

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

    switch (test) { case 0:
      case 1: {
        bdlma::SequentialAllocator alloc;
        const sjtm::DatumFactory f(&alloc);

        BytecodeDSLUtil::FunctionNameToAddressMap functions;
        functions["foo"] = testExecute;

        const struct Case {
            const char   *name;
            const char   *input;
            bdld::Datum   expected;
        } cases[] = {
            { "push and return", "Pi3|X", f(3) },
            { "add doubles", "Pd3|Pd1|+d|X", f(4.) },
            { "exec 0 args", "Pi0|Pefoo|E|X", f(0.) },
            { "exec 1 args", "Pd3|Pi1|Pefoo|E|X", f(3.) },
            { "exec 2 args", "Pd4|Pd3|Pi2|Pefoo|E|X", f(7.) },
            { "exec preserving stack", "Pd3|Pi1|Pefoo|E|Pd2|+d|X", f(5.) },
            { "load & store", "Pi1|S3|Pi2|L3|X", f(1) },
            { "jump", "J3|Pi1|X|Pi3|X", f(3) },
            { "gosub", "Pi8|G3|Pi7|X", f(2) },
            { "return", "Pd2|Pi4|R|X|Pd3|+d|X", f(5.) },
        };
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
            const Case& c = cases[i];
            bsl::vector<sjtt::Bytecode> code(&alloc);
            bsl::string errorMessage;
            const int ret = BytecodeDSLUtil::readDSL(&code,
                                                     &errorMessage,
                                                     c.input,
                                                     functions);
            LOOP2_ASSERT(c.name, errorMessage, 0 == ret);
            const bdld::Datum result = InterpretUtil::interpretBytecode(
                                                                     &alloc,
                                                                     &code[0]);
            LOOP2_ASSERT(c.name, result, result == c.expected);
        }
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
