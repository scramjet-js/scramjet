// sjtt_frame.t.cpp                                     -*-C++-*-

#include <sjtt_frame.h>

#include <bdls_testutil.h>

#include <sjtt_bytecode.h>
#include <sjtt_datumudtutil.h>

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
    const bool veryVerbose = argc > 3;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    switch (test) { case 0:
      case 6: {
        if (verbose) cout << endl
                          << "reserve" << endl
                          << "=======" << endl;
        const bdld::Datum N = bdld::Datum::createNull();
        const bdld::Datum U = sjtt::DatumUdtUtil::s_Undefined;
        const struct Case {
            const char               *name;
            bsl::vector<bdld::Datum>  initial;
            int                       offset;
            int                       count;
            bsl::vector<bdld::Datum>  expected;
        } cases[] = {
            { "base", {}, 0, 0, {} },
            { "base add some", {}, 0, 1, { U } },
            { "base no add 0", { N }, 0, 0, { N } },
            { "base no add 1", { N }, 0, 1, { N } },
            { "base add one, not all", { N }, 0, 2, { N, U } },
            { "offset base", { N }, 1, 0, { N } },
            { "offset base add some", { N }, 1, 1, { N, U } },
            { "offset base no add 0", { N, N }, 1, 0, { N, N } },
            { "offset base no add 1", { N, N }, 1, 1, { N, N } },
            { "offset base add one, not all", { N, N }, 1, 2, { N, N, U } },
        };
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
            const Case& c = cases[i];
            Bytecode code[1];
            Frame f(c.offset, code, code);
            bsl::vector<bdld::Datum> stack = c.initial;
            f.reserve(&stack, c.count);
            LOOP_ASSERT(c.name, c.expected == stack);
        }
      } break;
      case 5: {
        if (verbose) cout << endl
                          << "getValue" << endl
                          << "========" << endl;
        bsl::vector<bdld::Datum> stack = {
            bdld::Datum::createInteger(2),
            bdld::Datum::createInteger(3),
            bdld::Datum::createInteger(4),
        };
        Bytecode code[1];

        ASSERT(&stack[0] == &Frame(0, code, code).getValue(&stack, 0));
        ASSERT(&stack[1] == &Frame(0, code, code).getValue(&stack, 1));
        ASSERT(&stack[1] == &Frame(1, code, code).getValue(&stack, 0));
        ASSERT(&stack[2] == &Frame(2, code, code).getValue(&stack, 0));
        ASSERT(&stack[2] == &Frame(1, code, code).getValue(&stack, 1));
        ASSERT(&stack[2] == &Frame(0, code, code).getValue(&stack, 2));
      } break;
      case 4: {
        if (verbose) cout << endl
                          << "jump" << endl
                          << "====" << endl;
        Bytecode code[3];
        Frame f(0, code, code);
        f.jump(2);
        ASSERT(code == f.firstCode());
        ASSERT((code + 2) == f.pc());
      } break;
      case 3: {
        if (verbose) cout << endl
                          << "incrementPc" << endl
                          << "===========" << endl;
        Bytecode code[3];
        Frame f(0, code, code + 1);
        f.incrementPc();
        ASSERT((code + 2) == f.pc());
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "operator== and operator!=" << endl
                          << "=========================" << endl;
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Bytecode code1 = Bytecode::createOpcode(Bytecode::e_Push);

        // same
        {
            Frame a(1, &code, &code1);
            Frame b(1, &code, &code1);
            ASSERT(a == b);
            ASSERT(!(a != b));
        }

        // diff bottom
        {
            Frame a(8, &code, &code1);
            Frame b(4, &code, &code1);
            ASSERT(a != b);
        }

        // diff first code
        {
            sjtt::Bytecode code2 =
                        sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Push);
            Frame a(0, &code, &code1);
            Frame b(0, &code1, &code1);
            ASSERT(a != b);
        }

        // diff pc
        {
            sjtt::Bytecode code2 =
                        sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Push);
            Frame a(0, &code, &code);
            Frame b(0, &code, &code1);
            ASSERT(a != b);
        }
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "constructor and basic acccessors" << endl
                          << "================================" << endl;
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame frame(1, &code, &code + 1);
        ASSERT(1 == frame.bottom());
        ASSERT(&code == frame.firstCode());
        ASSERT(&code + 1 == frame.pc());
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
