// sjtt_function.t.cpp                                     -*-C++-*-

#include <sjtt_function.h>

#include <bdls_testutil.h>

using namespace BloombergLP;
using namespace bsl;
using namespace sjtt;

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
    const bool     verbose = argc > 2;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    switch (test) { case 0:
      case 2: {
        if (verbose) cout << endl
                          << "operator== && operator!=" << endl
                          << "==========" << endl;
        const Bytecode *code1 = reinterpret_cast<const Bytecode *>("hello");
        const Bytecode *code2 = reinterpret_cast<const Bytecode *>("world");
        const Function f = Function::createFunction(code1, 1, 2);
        ASSERT(f == Function::createFunction(code1, 1, 2));
        ASSERT(!(f != Function::createFunction(code1, 1, 2)));
        ASSERT(f != Function::createFunction(code2, 1, 2));
        ASSERT(f != Function::createFunction(code1, 2, 2));
        ASSERT(f != Function::createFunction(code1, 1, 4));
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "createFunction and basic acccessors" << endl
                          << "================================" << endl;
        const Bytecode *code = reinterpret_cast<const Bytecode *>("hello");
        Function f = Function::createFunction(code, 1, 2);
        ASSERT(code == f.code());
        ASSERT(1 == f.argCount());
        ASSERT(2 == f.numLocals());
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
