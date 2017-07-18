// sjtt_bytecode.t.cpp                                     -*-C++-*-

#include <sjtt_bytecode.h>

#include <sjtd_datumfactory.h>

#include <bdlma_sequentialallocator.h>
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

    bdlma::SequentialAllocator alloc;
    sjtd::DatumFactory d(&alloc);

    switch (test) { case 0:
      case 19: {
        typedef Bytecode BC;
        const Bytecode code = BC::createAllocate(3);
        const Function f1 = Function::createFunction(&code, 3, 4);
        const Function f2 = Function::createFunction(&code, 3, 5);
        short someArgs[] = {1, 2, 3};
        short someSameArgs[] = {1, 2, 3};
        short moreArgs[] = {2, 2, 3};

        if (verbose) cout << endl
                          << "operator==" << endl
                          << "==========" << endl;
        struct Case {
            const char *name;
            Bytecode    lhs;
            Bytecode    rhs;
            bool        expected;
        } cases [] = {
            {
                "diff codes",
                BC::createReturn(5),
                BC::createAllocate(5),
                false
            },
            {
                "same e_Allocate",
                BC::createAllocate(2),
                BC::createAllocate(2),
                true
            },
            {
                "diff e_Allocate",
                BC::createAllocate(2),
                BC::createAllocate(4),
                false
            },
            {
                "same e_AllocateI32",
                BC::createAllocateI32(2),
                BC::createAllocateI32(2),
                true
            },
            {
                "diff e_AllocateI32",
                BC::createAllocateI32(2),
                BC::createAllocateI32(4),
                false
            },
            {
                "same e_AllocateDouble",
                BC::createAllocateDouble(2),
                BC::createAllocateDouble(2),
                true
            },
            {
                "diff e_AllocateDouble",
                BC::createAllocateDouble(2),
                BC::createAllocateDouble(4),
                false
            },
            {
                "same e_Store",
                BC::createStore(2, 4),
                BC::createStore(2, 4),
                true
            },
            {
                "diff e_Store x",
                BC::createStore(1, 4),
                BC::createStore(2, 4),
                false
            },
            {
                "diff e_Store y",
                BC::createStore(2, 1),
                BC::createStore(2, 4),
                false
            },
            {
                "same e_StoreI32",
                BC::createStoreI32(2, 4),
                BC::createStoreI32(2, 4),
                true
            },
            {
                "diff e_StoreI32 x",
                BC::createStoreI32(1, 4),
                BC::createStoreI32(2, 4),
                false
            },
            {
                "diff e_StoreI32 y",
                BC::createStoreI32(2, 1),
                BC::createStoreI32(2, 4),
                false
            },
            {
                "same e_StoreDouble",
                BC::createStoreDouble(2, 4),
                BC::createStoreDouble(2, 4),
                true
            },
            {
                "diff e_StoreDouble x",
                BC::createStoreDouble(1, 4),
                BC::createStoreDouble(2, 4),
                false
            },
            {
                "diff e_StoreDouble y",
                BC::createStoreDouble(2, 1),
                BC::createStoreDouble(2, 4),
                false
            },
            {
                "same e_Load",
                BC::createLoad(5, 6),
                BC::createLoad(5, 6),
                true
            },
            {
                "diff e_Load dest",
                BC::createLoad(1, 6),
                BC::createLoad(5, 6),
                false
            },
            {
                "diff e_Load x",
                BC::createLoad(5, 6),
                BC::createLoad(5, 1),
                false
            },
            {
                "same e_LoadI32",
                BC::createLoadI32(5, 6),
                BC::createLoadI32(5, 6),
                true
            },
            {
                "diff e_LoadI32 dest",
                BC::createLoadI32(1, 6),
                BC::createLoadI32(5, 6),
                false
            },
            {
                "diff e_LoadI32 x",
                BC::createLoadI32(5, 6),
                BC::createLoadI32(5, 1),
                false
            },
            {
                "same e_LoadDouble",
                BC::createLoadDouble(5, 6),
                BC::createLoadDouble(5, 6),
                true
            },
            {
                "diff e_LoadDouble dest",
                BC::createLoadDouble(1, 6),
                BC::createLoadDouble(5, 6),
                false
            },
            {
                "diff e_LoadDouble x",
                BC::createLoadDouble(5, 6),
                BC::createLoadDouble(5, 1),
                false
            },
            {
                "same e_EqI32",
                BC::createEqI32(1, 2, 3),
                BC::createEqI32(1, 2, 3),
                true
            },
            {
                "diff e_EqI32 dest",
                BC::createEqI32(2, 2, 3),
                BC::createEqI32(1, 2, 3),
                false
            },
            {
                "diff e_EqI32 x",
                BC::createEqI32(1, 3, 3),
                BC::createEqI32(1, 2, 3),
                false
            },
            {
                "diff e_EqI32 y",
                BC::createEqI32(1, 2, 4),
                BC::createEqI32(1, 2, 3),
                false
            },
            {
                "same e_Const",
                BC::createConst(1, d(8)),
                BC::createConst(1, d(8)),
                true,
            },
            {
                "diff e_Const dest",
                BC::createConst(1, d(8)),
                BC::createConst(2, d(8)),
                false,
            },
            {
                "diff e_Const value",
                BC::createConst(1, d(9)),
                BC::createConst(1, d(8)),
                false,
            },
            {
                "same e_ExtractI32",
                BC::createExtractI32(1, 2),
                BC::createExtractI32(1, 2),
                true
            },
            {
                "diff e_ExtractI32 dest",
                BC::createExtractI32(8, 2),
                BC::createExtractI32(1, 2),
                false
            },
            {
                "diff e_ExtractI32 x",
                BC::createExtractI32(1, 8),
                BC::createExtractI32(1, 2),
                false
            },
            {
                "same e_ExtractDouble",
                BC::createExtractDouble(1, 2),
                BC::createExtractDouble(1, 2),
                true
            },
            {
                "diff e_ExtractDouble dest",
                BC::createExtractDouble(8, 2),
                BC::createExtractDouble(1, 2),
                false
            },
            {
                "diff e_ExtractDouble x",
                BC::createExtractDouble(1, 8),
                BC::createExtractDouble(1, 2),
                false
            },
            {
                "same e_Jump",
                BC::createJump(2),
                BC::createJump(2),
                true
            },
            {
                "diff e_Jump",
                BC::createJump(1),
                BC::createJump(2),
                false
            },
            {
                "same e_IfI32",
                BC::createIfI32(4, 8),
                BC::createIfI32(4, 8),
                true
            },
            {
                "diff e_IfI32 x",
                BC::createIfI32(3, 8),
                BC::createIfI32(4, 8),
                false
            },
            {
                "diff e_IfI32 dest",
                BC::createIfI32(4, 9),
                BC::createIfI32(4, 8),
                false
            },
            {
                "same AddI32",
                BC::createAddI32(1, 2, 3),
                BC::createAddI32(1, 2, 3),
                true
            },
            {
                "diff AddI32 dest",
                BC::createAddI32(8, 2, 3),
                BC::createAddI32(1, 2, 3),
                false
            },
            {
                "diff AddI32 x",
                BC::createAddI32(1, 2, 3),
                BC::createAddI32(1, 7, 3),
                false
            },
            {
                "diff AddI32 y",
                BC::createAddI32(1, 2, 5),
                BC::createAddI32(1, 2, 3),
                false
            },
            {
                "same e_Call",
                BC::createCall(1, f1, someArgs),
                BC::createCall(1, f1, someSameArgs),
                true,
            },
            {
                "diff e_Call dest",
                BC::createCall(1, f1, someArgs),
                BC::createCall(3, f1, someSameArgs),
                false,
            },
            {
                "diff e_Call fun",
                BC::createCall(1, f1, someArgs),
                BC::createCall(1, f2, someSameArgs),
                false,
            },
            {
                "diff e_Call args",
                BC::createCall(1, f1, someArgs),
                BC::createCall(1, f1, moreArgs),
                false,
            },
            {
                "same e_Return",
                BC::createReturn(1),
                BC::createReturn(1),
                true,
            },
            {
                "diff e_Return",
                BC::createReturn(1),
                BC::createReturn(9),
                false,
            },
        };
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
            const Case& c = cases[i];
            const bool result = c.lhs == c.rhs;
            LOOP_ASSERT(c.name, result == c.expected);
        }
      } break;
      case 18: {
        if (verbose) cout << endl
                          << "createReturn" << endl
                          << "============" << endl;
        const Bytecode bc = Bytecode::createReturn(3);
        ASSERT(Bytecode::e_Return == bc.opcode());
        ASSERT(3 == bc.x());
      } break;
      case 17: {
        if (verbose) cout << endl
                          << "createCall" << endl
                          << "==========" << endl;
        const Bytecode code = Bytecode::createAllocate(3);
        const Function f = Function::createFunction(&code, 3, 4);
        short someArgs[] = {1, 2, 3};
        const Bytecode bc = Bytecode::createCall(3, f, someArgs);
        ASSERT(Bytecode::e_Call == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(f == bc.function());
        ASSERT(someArgs == bc.args());
      } break;
      case 16: {
        if (verbose) cout << endl
                          << "createAddI32" << endl
                          << "============" << endl;
        const Bytecode bc = Bytecode::createAddI32(3, 7, 5);
        ASSERT(Bytecode::e_AddI32 == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(7 == bc.x());
        ASSERT(5 == bc.y());
      } break;
      case 15: {
        if (verbose) cout << endl
                          << "createIfI32" << endl
                          << "===========" << endl;
        const Bytecode bc = Bytecode::createIfI32(3, 2);
        ASSERT(Bytecode::e_IfI32 == bc.opcode());
        ASSERT(3 == bc.x());
        ASSERT(2 == bc.offset());
      } break;
      case 14: {
        if (verbose) cout << endl
                          << "createJump" << endl
                          << "==========" << endl;
        const Bytecode bc = Bytecode::createJump(3);
        ASSERT(Bytecode::e_Jump == bc.opcode());
        ASSERT(3 == bc.offset());
      } break;
      case 13: {
        if (verbose) cout << endl
                          << "createExtractDouble" << endl
                          << "===================" << endl;
        const Bytecode bc = Bytecode::createExtractDouble(3, 9);
        ASSERT(Bytecode::e_ExtractDouble == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(9 == bc.x());
      } break;
      case 12: {
        if (verbose) cout << endl
                          << "createExtractI32" << endl
                          << "================" << endl;
        const Bytecode bc = Bytecode::createExtractI32(3, 9);
        ASSERT(Bytecode::e_ExtractI32 == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(9 == bc.x());
      } break;
      case 11: {
        if (verbose) cout << endl
                          << "createConst" << endl
                          << "===========" << endl;
        const Bytecode bc = Bytecode::createConst(3, d(4));
        ASSERT(Bytecode::e_Const == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(d(4) == bc.value());
      } break;
      case 10: {
        if (verbose) cout << endl
                          << "createEqI32" << endl
                          << "===========" << endl;
        const Bytecode bc = Bytecode::createEqI32(3, 8, 9);
        ASSERT(Bytecode::e_EqI32 == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(8 == bc.x());
        ASSERT(9 == bc.y());
      } break;
      case 9: {
        if (verbose) cout << endl
                          << "createLoadDouble" << endl
                          << "================" << endl;
        const Bytecode bc = Bytecode::createLoadDouble(3, 5);
        ASSERT(Bytecode::e_LoadDouble == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(5 == bc.x());
      } break;
      case 8: {
        if (verbose) cout << endl
                          << "createLoadI32" << endl
                          << "=============" << endl;
        const Bytecode bc = Bytecode::createLoadI32(3, 5);
        ASSERT(Bytecode::e_LoadI32 == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(5 == bc.x());
      } break;
      case 7: {
        if (verbose) cout << endl
                          << "createLoad" << endl
                          << "==========" << endl;
        const Bytecode bc = Bytecode::createLoad(3, 5);
        ASSERT(Bytecode::e_Load == bc.opcode());
        ASSERT(3 == bc.dest());
        ASSERT(5 == bc.x());
      } break;
      case 6: {
        if (verbose) cout << endl
                          << "createStoreDouble" << endl
                          << "=================" << endl;
        const Bytecode bc = Bytecode::createStoreDouble(3, 4);
        ASSERT(Bytecode::e_StoreDouble == bc.opcode());
        ASSERT(3 == bc.x());
        ASSERT(4 == bc.y());
      } break;
      case 5: {
        if (verbose) cout << endl
                          << "createStoreI32" << endl
                          << "==============" << endl;
        const Bytecode bc = Bytecode::createStoreI32(3, 4);
        ASSERT(Bytecode::e_StoreI32 == bc.opcode());
        ASSERT(3 == bc.x());
        ASSERT(4 == bc.y());
      } break;
      case 4: {
        if (verbose) cout << endl
                          << "createStore" << endl
                          << "===========" << endl;
        const Bytecode bc = Bytecode::createStore(3, 4);
        ASSERT(Bytecode::e_Store == bc.opcode());
        ASSERT(3 == bc.x());
        ASSERT(4 == bc.y());
      } break;
      case 3: {
        if (verbose) cout << endl
                          << "createAllocateDouble" << endl
                          << "====================" << endl;
        const Bytecode bc = Bytecode::createAllocateDouble(3);
        ASSERT(Bytecode::e_AllocateDouble == bc.opcode());
        ASSERT(3 == bc.dest());
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "createAllocateI32" << endl
                          << "=================" << endl;
        const Bytecode bc = Bytecode::createAllocateI32(3);
        ASSERT(Bytecode::e_AllocateI32 == bc.opcode());
        ASSERT(3 == bc.dest());
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "createAllocate" << endl
                          << "==============" << endl;
        const Bytecode bc = Bytecode::createAllocate(3);
        ASSERT(Bytecode::e_Allocate == bc.opcode());
        ASSERT(3 == bc.dest());
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
