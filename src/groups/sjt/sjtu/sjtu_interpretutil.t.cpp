// sjtu_interpretutil.t.cpp                                     -*-C++-*-

#include <sjtu_interpretutil.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <bsl_vector.h>

#include <sjtd_datumfactory.h>
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

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int         test = argc > 1 ? atoi(argv[1]) : 0;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    switch (test) { case 0:
      case 1: {
        bdlma::SequentialAllocator alloc;
        const sjtd::DatumFactory f(&alloc);

        BytecodeDSLUtil::FunctionMap functions;

        const struct Case {
            const char         *name;
            const char         *input;
            int                 argCount;
            int                 numLocals;
            const bdld::Datum  *args;
            bdld::Datum   expected;
        } cases[] = {
            {
                "const and return",
                "C0,T|R0",
                0,
                1,
                0,
                f(true),
            },
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
            const sjtt::Function fun = sjtt::Function::createFunction(
                                                                  code.data(),
                                                                  code.size(),
                                                                  c.argCount,
                                                                  c.numLocals);
            const bdld::Datum result = InterpretUtil::interpret(&alloc,
                                                                fun,
                                                                c.args);
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
