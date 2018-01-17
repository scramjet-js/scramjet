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

// helper

struct Failure {};
static void assertionHandler(const char *text, const char *file, int line)
{
    throw Failure();
}

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int  test = argc > 1 ? atoi(argv[1]) : 0;
    const bool verbose = argc > 2;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    switch (test) { case 0:
      case 2: {
        if (verbose) cout << endl
                          << "interpret" << endl
                          << "============" << endl;

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
            {
                "allocate, store, load, return",
                "C0,T|A1|S1,0|L2,1|R2",
                0,
                3,
                0,
                f(true),
            },
            {
                "allocateI32, storeI32, loadI32, return",
                "C0,i9|Ai1|Si1,0|Li2,1|R2",
                0,
                3,
                0,
                f(9),
            },
            {
                "allocateD32, storeD32, loadD32, return",
                "C0,d7|Ad1|Sd1,0|Ld2,1|R2",
                0,
                3,
                0,
                f(7.0),
            },
            {
                "eqI32 (yes)",
                "C0,i8|C1,i8|=i2,0,1|R2",
                0,
                3,
                0,
                f(1),
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
      case 1: {
        if (verbose) cout << endl
                          << "checkAssignments" << endl
                          << "================" << endl;

        bdlma::SequentialAllocator alloc;
        const sjtd::DatumFactory f(&alloc);

        BytecodeDSLUtil::FunctionMap functions;

        const struct Case {
            const char *name;
            const char *input;
            int         argCount;
            bool        expectedToFail;
        } cases[] = {
            {
                "single allocate",
                "A0",
                0,
                false,
            },
            {
                "attempt to assign over first arg",
                "A0",
                1,
                true,
            },
            {
                "missed first arg",
                "A1",
                1,
                false,
            },
            {
                "hit last arg",
                "A1",
                2,
                true,
            },
            {
                "double assign",
                "A0|A0",
                0,
                true,
            },
            {
                "different assigns",
                "A0|A1",
                0,
                false,
            },
            {
                "second assign is OOB",
                "A1|A0",
                1,
                true,
            },
            {
                "good two different types of assigns",
                "A0|C1,T",
                0,
                false,
            },
            {
                "bad two different types of assigns",
                "A1|C1,T",
                0,
                true,
            },
            {
                "bad allocateI32",
                "Ai0",
                0,
                false,
            },
            {
                "bad allocateDbl",
                "Ad0",
                1,
                true,
            },
            {
                "bad load",
                "L0,0",
                1,
                true,
            },
            {
                "bad loadI32",
                "Li0,0",
                1,
                true,
            },
            {
                "bad loadDbl",
                "Ld0,0",
                1,
                true,
            },
            {
                "bad eqI32",
                "=i0,0,1",
                1,
                true,
            },
            {
                "bad const",
                "C0,T",
                1,
                true,
            },
            {
                "bad extractI32",
                "Ei0,1",
                1,
                true,
            },
            {
                "bad extractDbl",
                "Ed0,2",
                1,
                true,
            },
            {
                "bad allocate",
                "A0",
                1,
                true,
            },
            {
                "bad allocate",
                "A0",
                1,
                true,
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
                                                                  0);
            if (c.expectedToFail) {
                try {
                    {
                        bsls::AssertFailureHandlerGuard guard(
                                                             assertionHandler);
                        InterpretUtil::checkAssignments(fun);
                    }
                    LOOP2_ASSERT(c.name, "didn't fail", false);
                }
                catch(const Failure&) {
                }
            }
            else {
                InterpretUtil::checkAssignments(fun);
            }
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
