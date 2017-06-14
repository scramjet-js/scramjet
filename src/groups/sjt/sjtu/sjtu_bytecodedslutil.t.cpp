// sjtu_bytecodedslutil.t.cpp                                     -*-C++-*-

#include <sjtu_bytecodedslutil.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <bsl_vector.h>

#include <sjtm_datumfactory.h>
#include <sjtt_bytecode.h>

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
bdld::Datum testFun(const sjtt::ExecutionContext& context) {
    return bdld::Datum::createNull();
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
      case 2: {
        if (verbose) cout << endl
                          << "readDSL" << endl
                          << "=======" << endl;

        bdlma::SequentialAllocator alloc;
        const sjtm::DatumFactory f(&alloc);

        BytecodeDSLUtil::FunctionNameToAddressMap functions;
        functions["foo"] = testFun;

        typedef sjtt::Bytecode BC;

        const struct Case {
            const char                  *name;
            const char                  *dsl;
            bool                         fails;
            bsl::vector<sjtt::Bytecode>  expected;
            const char                  *errorMessage;
        } cases [] = {
            { "empty", "", false, {}, "" },
            { "bad code", "'", true, {}, "invalid opcode ''' at position: 0" },
            {
                "push",
                "Pi2",
                false,
                { BC::createOpcode(BC::e_Push, f(2)) },
                "",
            },
            {
                "push foo",
                "Pefoo",
                false,
                { BC::createOpcode(BC::e_Push, f(testFun)) },
                "",
            },
            { "bad push", "P", true, {},
              "invalid datum for push at position: 0 -- empty datum",
            },
            { "load", "L3", false, { BC::createOpcode(BC::e_Load, f(3)) } },
            { "bad load", "Lx", true, {},
              "invalid index for load at position: 0",
            },
            { "store", "S8", false, { BC::createOpcode(BC::e_Store, f(8)) } },
            { "bad store", "Si", true, {},
              "invalid index for store at position: 0",
            },
            { "jump", "J8", false, { BC::createOpcode(BC::e_Jump, f(8)) } },
            { "bad jump", "Ji", true, {},
              "invalid index for jump at position: 0"
            },
            { "bad add", "+", true, {}, "invalid add in position: 0", },
            { "bad add type", "+x", true, {}, "invalid add in position: 0", },
            { "call", "C8", false, { BC::createOpcode(BC::e_Call, f(8)) } },
            { "bad call", "Ci", true, {},
              "invalid index for call at position: 0"
            },
            { "exec", "E", false, { BC::createOpcode(BC::e_Execute) } },
            { "bad exec", "E8", true, {}, "invalid execute at position: 0" },
            { "exit", "X", false, { BC::createOpcode(BC::e_Exit) } },
            { "bad exit", "X8", true, {}, "invalid exit at position: 0" },
            {
                "reserve",
                "V8",
                false,
                { BC::createOpcode(BC::e_Reserve, f(8)) },
            },
            {
                "bad reserve",
                "Vi",
                true,
                {},
                "invalid index for reserve at position: 0"
            },

            // combinations
            { "sequence term", "X|", false, { BC::createOpcode(BC::e_Exit) } },
            { "empty second", "X||", true, {},
              "empty bytecode beginning at position: 2" },
            { "bad second", "X|]", true, {},
              "invalid opcode ']' at position: 2" },
            {
                "sequence",
                "Pd2|Pd4|+d|X",
                false,
                {
                    BC::createOpcode(BC::e_Push, f(2.)),
                    BC::createOpcode(BC::e_Push, f(4.)),
                    BC::createOpcode(BC::e_AddDoubles),
                    BC::createOpcode(BC::e_Exit)
                },
                "",
            },
        };
        for (int i = 0; i < (sizeof(cases) / sizeof(cases[0])); ++i) {
            const Case& c = cases[i];
            bsl::vector<sjtt::Bytecode> result(&alloc);
            bsl::string errorMessage;
            const int ret = BytecodeDSLUtil::readDSL(&result,
                                                     &errorMessage,
                                                     c.dsl,
                                                     functions);
            if (c.fails) {
                LOOP_ASSERT(c.name, ret != 0);
                LOOP2_ASSERT(c.name,
                             errorMessage,
                             errorMessage == c.errorMessage);
            }
            else {
                LOOP_ASSERT(c.name, ret == 0);
                LOOP_ASSERT(c.name, result.size() == c.expected.size());
                for (int j = 0; j < result.size(); ++j) {
                    LOOP2_ASSERT(c.name, j, result[j] == c.expected[j]);
                }
            }
        }
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "readDatum" << endl
                          << "=========" << endl;
        bdlma::SequentialAllocator alloc;
        const sjtm::DatumFactory f(&alloc);

        BytecodeDSLUtil::FunctionNameToAddressMap functions;
        functions["foo"] = testFun;

        const struct Case {
            const char  *name;
            const char  *input;
            bool         fails;
            bdld::Datum  expectedResult;
            const char  *errorMessage;
        } cases [] = {
            { "simple failure", "", true, f(), "empty datum" },
            { "bad type", "q2", true, f(), "unknown datum type 'q'" },
            { "double", "d3", false, f(3.0), 0 },
            { "bad double", "dx", true, f(),
                "unable to parse double from 'x'" },
            { "int", "i6", false, f(6), 0 },
            { "bad int", "iq", true, f(),
                "unable to parse integer from 'q'" },
            { "bad fun name", "ebar", true, f(),
                "unknown function name 'bar'" },
            { "good fun name", "efoo", false, f(testFun), 0 },
        };
        for (int i = 0; i < (sizeof(cases) / sizeof(cases[0])); ++i) {
            const Case& c = cases[i];
            bsl::string errorMessage;
            bdld::Datum result = f();
            const int ret = BytecodeDSLUtil::readDatum(&result,
                                                       &errorMessage,
                                                       &alloc,
                                                       c.input,
                                                       functions);
            if (c.fails) {
                LOOP_ASSERT(c.name, ret != 0);
                LOOP2_ASSERT(c.name,
                             errorMessage,
                             errorMessage == c.errorMessage);
            }
            else {
                LOOP_ASSERT(c.name, ret == 0);
                LOOP2_ASSERT(c.name, result, result == c.expectedResult);
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
