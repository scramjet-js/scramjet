// sjtu_bytecodedslutil.t.cpp                                     -*-C++-*-

#include <sjtu_bytecodedslutil.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <bsl_vector.h>

#include <sjtd_datumfactory.h>
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
                          << "readDSL" << endl
                          << "=======" << endl;

        bdlma::SequentialAllocator alloc;
        const sjtd::DatumFactory f(&alloc);

        const sjtt::Bytecode *code =
            reinterpret_cast<const sjtt::Bytecode *>("hello");
        const sjtt::Function testFun = sjtt::Function::createFunction(code,
                                                                      2,
                                                                      2);
        BytecodeDSLUtil::FunctionMap functions;
        functions["foo"] = testFun;
        functions["bar"] = testFun1;
        functions["baz"] = testFun;

        bsl::vector<short> args = { 4, 5 };

        typedef sjtt::Bytecode BC;

        const struct Case {
            const char                  *name;
            const char                  *dsl;
            bool                         fails;
            bsl::vector<sjtt::Bytecode>  expected;
            const char                  *errorMessage;
        } cases [] = {
            { "empty", "", false, {}, "" },
            {
                "bad code",
                "'",
                true,
                {},
                "invalid opcode at position: 0 -- '''",
            },
            {
                "allocate",
                "A2",
                false,
                { BC::createAllocate(2) },
                "",
            },
            {
                "allocateI32",
                "Ai2",
                false,
                { BC::createAllocateI32(2) },
                "",
            },
            {
                "allocateDbl",
                "Ad2",
                false,
                { BC::createAllocateDouble(2) },
                "",
            },
            {
                "store",
                "S3,4",
                false,
                { BC::createStore(3, 4) },
                "",
            },
            {
                "storeI32",
                "Si3,4",
                false,
                { BC::createStoreI32(3, 4) },
                "",
            },
            {
                "storeDbl",
                "Sd3,4",
                false,
                { BC::createStoreDouble(3, 4) },
                "",
            },
            {
                "load",
                "L6,7",
                false,
                { BC::createLoad(6,7) },
                "",
            },
            {
                "loadI32",
                "Li6,7",
                false,
                { BC::createLoadI32(6,7) },
                "",
            },
            {
                "loadDbl",
                "Ld6,7",
                false,
                { BC::createLoadDouble(6,7) },
                "",
            },
            {
                "eqI32",
                "=i4,5,6",
                false,
                { BC::createEqI32(4, 5, 6) },
                "",
            },
            {
                "const",
                "C2,T",
                false,
                { BC::createConst(2, f(true)) },
                "",
            },
            {
                "extractI32",
                "Ei4,5",
                false,
                { BC::createExtractI32(4, 5) },
                "",
            },
            {
                "extractDbl",
                "Ed8,2",
                false,
                { BC::createExtractDouble(8, 2) },
                "",
            },
            {
                "jump",
                "J89",
                false,
                { BC::createJump(89) },
                "",
            },
            {
                "ifI32",
                "Ii7,5",
                false,
                { BC::createIfI32(7, 5) },
                ""
            },
            {
                "addI32",
                "+i6,3,3",
                false,
                { BC::createAddI32(6, 3, 3) },
                "",
            },
            {
                "call, no args",
                "()2,foo",
                false,
                { BC::createCall(2, testFun, 0) },
                "",
            },
            {
                "call, one arg",
                "()2,foo,4",
                false,
                { BC::createCall(2, testFun, args.data()) },
                "",
            },
            {
                "call, two args",
                "()2,foo,4,5",
                false,
                { BC::createCall(2, testFun, args.data()) },
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
                LOOP2_ASSERT(c.name, errorMessage, ret == 0);
                LOOP2_ASSERT(c.name,
                             result.size(),
                             result.size() == c.expected.size());
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
        const sjtd::DatumFactory f(&alloc);

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
            { "true", "T", false, f(true) },
            { "false", "F", false, f(false) },
        };
        for (int i = 0; i < (sizeof(cases) / sizeof(cases[0])); ++i) {
            const Case& c = cases[i];
            bsl::string errorMessage;
            bdld::Datum result = f();
            const int ret = BytecodeDSLUtil::readDatum(&result,
                                                       &errorMessage,
                                                       &alloc,
                                                       c.input);
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
