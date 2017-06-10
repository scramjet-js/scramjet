// sjtt_frame.t.cpp                                     -*-C++-*-

#include <sjtt_frame.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

#include <sjtt_bytecode.h>

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
      case 11: {
        if (verbose) cout << endl
                          << "jump" << endl
                          << "====" << endl;
        bdlma::SequentialAllocator alloc;
        Bytecode code[3];
        Frame f(&alloc, 0, 0, 0, 0, code);
        f.jump(2);
        BSLS_ASSERT((code + 2) == f.pc());
      } break;
      case 10: {
        if (verbose) cout << endl
                          << "incrementPc" << endl
                          << "===========" << endl;
        bdlma::SequentialAllocator alloc;
        Bytecode code[3];
        Frame f(&alloc, 0, 0, 0, 0, code);
        f.incrementPc();
        BSLS_ASSERT((code + 1) == f.pc());
      } break;
      case 9 :{
        if (verbose) cout << endl
                          << "popMany" << endl
                          << "=======" << endl;
        bdlma::SequentialAllocator alloc;
        bdld::Datum l0 = bdld::Datum::createInteger(2);
        bdld::Datum l1 = bdld::Datum::createInteger(5);
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame f(&alloc, 0, 0, &l0, 1, &code);
        f.setLocal(0, l1);
        BSLS_ASSERT(bsl::vector<bdld::Datum>(&l1, &l1 + 1) == f.locals());
      } break;
      case 8 :{
        if (verbose) cout << endl
                          << "popMany" << endl
                          << "=======" << endl;
        // base
        {
            bdlma::SequentialAllocator alloc;
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Frame f(&alloc, 0, 0, 0, 0, &code);
            f.popMany(0);
            BSLS_ASSERT(bsl::vector<bdld::Datum>() == f.stack());
        }

        // one and all
        {
            bdlma::SequentialAllocator alloc;
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Frame f(&alloc, 0, 0, 0, 0, &code);
            const bdld::Datum d = bdld::Datum::createInteger(9);
            f.push(d);
            f.popMany(1);
            BSLS_ASSERT(bsl::vector<bdld::Datum>() == f.stack());
        }

        // one of some
        {
            bdlma::SequentialAllocator alloc;
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Frame f(&alloc, 0, 0, 0, 0, &code);
            const bdld::Datum d0 = bdld::Datum::createInteger(9);
            const bdld::Datum d1 = bdld::Datum::createInteger(22);
            f.push(d0);
            f.push(d1);
            f.popMany(1);
            BSLS_ASSERT(bsl::vector<bdld::Datum>(&d0, &d0 + 1) == f.stack());
        }

        // many all
        {
            bdlma::SequentialAllocator alloc;
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Frame f(&alloc, 0, 0, 0, 0, &code);
            const bdld::Datum d0 = bdld::Datum::createInteger(9);
            const bdld::Datum d1 = bdld::Datum::createInteger(22);
            f.push(d0);
            f.push(d1);
            f.popMany(2);
            BSLS_ASSERT(bsl::vector<bdld::Datum>() == f.stack());
        }
      } break;
      case 7 :{
        if (verbose) cout << endl
                          << "pop" << endl
                          << "===" << endl;
        bdlma::SequentialAllocator alloc;
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame f(&alloc, 0, 0, 0, 0, &code);
        const bdld::Datum d = bdld::Datum::createInteger(9);
        f.push(d);
        f.pop();
        BSLS_ASSERT(bsl::vector<bdld::Datum>() == f.stack());
      } break;
      case 6 :{
        if (verbose) cout << endl
                          << "push" << endl
                          << "====" << endl;
        bdlma::SequentialAllocator alloc;
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame f(&alloc, 0, 0, 0, 0, &code);
        const bdld::Datum d = bdld::Datum::createInteger(9);
        f.push(d);
        BSLS_ASSERT(bsl::vector<bdld::Datum>(&d, &d + 1) == f.stack());
      } break;
      case 5: {
        if (verbose) cout << endl
                          << "operator=" << endl
                          << "=========" << endl;

        bdlma::SequentialAllocator alloc;
        bdld::Datum arg = bdld::Datum::createNull();
        bdld::Datum arg2 = bdld::Datum::createInteger(3);
        bdld::Datum loc = bdld::Datum::createInteger(2);
        bdld::Datum loc2 = bdld::Datum::createInteger(8);
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Bytecode code2 = Bytecode::createOpcode(Bytecode::e_Return);
        Frame a(&alloc, &arg, 1, &loc, 1, &code);
        Frame b(&alloc, &arg2, 1, &loc2, 1, &code2);
        b = a;
        BSLS_ASSERT(a == b);
      } break;
      case 4: {
        if (verbose) cout << endl
                          << "swap" << endl
                          << "====" << endl;

        // member
        {
            bdlma::SequentialAllocator alloc;
            bdld::Datum arg = bdld::Datum::createNull();
            bdld::Datum arg2 = bdld::Datum::createInteger(3);
            bdld::Datum loc = bdld::Datum::createInteger(2);
            bdld::Datum loc2 = bdld::Datum::createInteger(8);
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Bytecode code2 = Bytecode::createOpcode(Bytecode::e_Return);
            Frame a(&alloc, &arg, 1, &loc, 1, &code);
            Frame b(&alloc, &arg2, 1, &loc2, 1, &code2);
            Frame aOriginal(a, &alloc);
            Frame bOriginal(b, &alloc);

            a.swap(b);
            BSLS_ASSERT(bOriginal == a);
            BSLS_ASSERT(aOriginal == b);
        }

        // free -- same allocator
        {
            bdlma::SequentialAllocator alloc;
            bdld::Datum arg = bdld::Datum::createNull();
            bdld::Datum arg2 = bdld::Datum::createInteger(3);
            bdld::Datum loc = bdld::Datum::createInteger(2);
            bdld::Datum loc2 = bdld::Datum::createInteger(8);
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Bytecode code2 = Bytecode::createOpcode(Bytecode::e_Return);
            Frame a(&alloc, &arg, 1, &loc, 1, &code);
            Frame b(&alloc, &arg2, 1, &loc2, 1, &code2);
            Frame aOriginal(a, &alloc);
            Frame bOriginal(b, &alloc);

            swap(a, b);
            BSLS_ASSERT(bOriginal == a);
            BSLS_ASSERT(aOriginal == b);
        }

        // free -- different allocator
        {
            bdlma::SequentialAllocator alloc;
            bdlma::SequentialAllocator otherAlloc;
            bdld::Datum arg = bdld::Datum::createNull();
            bdld::Datum arg2 = bdld::Datum::createInteger(3);
            bdld::Datum loc = bdld::Datum::createInteger(2);
            bdld::Datum loc2 = bdld::Datum::createInteger(8);
            Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
            Bytecode code2 = Bytecode::createOpcode(Bytecode::e_Return);
            Frame a(&alloc, &arg, 1, &loc, 1, &code);
            Frame b(&otherAlloc, &arg2, 1, &loc2, 1, &code2);
            Frame aOriginal(a, &alloc);
            Frame bOriginal(b, &alloc);

            swap(a, b);
            BSLS_ASSERT(bOriginal == a);
            BSLS_ASSERT(aOriginal == b);
            BSLS_ASSERT(&alloc == a.allocator());
            BSLS_ASSERT(&otherAlloc == b.allocator());
        }      } break;
      case 3: {
        if (verbose) cout << endl
                          << "copy ctr" << endl
                          << "========" << endl;
        bdlma::SequentialAllocator alloc;
        bdlma::SequentialAllocator otherAlloc;
        bdld::Datum arg = bdld::Datum::createNull();
        bdld::Datum loc = bdld::Datum::createInteger(2);
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame a(&alloc, &arg, 1, &loc, 1, &code);
        Frame b(a, &otherAlloc);
        BSLS_ASSERT(a == b);
        BSLS_ASSERT(&otherAlloc == b.allocator());
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "operator== and operator!=" << endl
                          << "=========================" << endl;
        bdlma::SequentialAllocator alloc1;
        bdlma::SequentialAllocator alloc2;
        bdld::Datum arg = bdld::Datum::createNull();
        bdld::Datum loc = bdld::Datum::createInteger(2);
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);

        // same
        {
            Frame a(&alloc1, &arg, 1, &loc, 1, &code);
            Frame b(&alloc1, &arg, 1, &loc, 1, &code);
            BSLS_ASSERT(a == b);
            BSLS_ASSERT(!(a != b));
        }

        // diff alloc
        {
            Frame a(&alloc1, &arg, 1, &loc, 1, &code);
            Frame b(&alloc2, &arg, 1, &loc, 1, &code);
            BSLS_ASSERT(a == b);
            BSLS_ASSERT(!(a != b));
        }

        // diff arg
        {
            bdld::Datum arg2 = bdld::Datum::createInteger(5);
            Frame a(&alloc1, &arg, 1, &loc, 1, &code);
            Frame b(&alloc1, &arg2, 1, &loc, 1, &code);
            BSLS_ASSERT(a != b);
        }

        // diff num args
        {
            Frame a(&alloc1, &arg, 1, &loc, 1, &code);
            Frame b(&alloc1, &arg, 2, &loc, 1, &code);
            BSLS_ASSERT(a != b);
        }

        // diff locals
        {
            bdld::Datum loc2 = bdld::Datum::createInteger(7);
            Frame a(&alloc1, &arg, 1, &loc2, 1, &code);
            Frame b(&alloc1, &arg, 1, &loc, 1, &code);
            BSLS_ASSERT(a != b);
        }

        // diff num locals
        {
            Frame a(&alloc1, &arg, 1, &loc, 3, &code);
            Frame b(&alloc1, &arg, 1, &loc, 1, &code);
            BSLS_ASSERT(a != b);
        }

        // diff code
        {
            sjtt::Bytecode code2 =
                        sjtt::Bytecode::createOpcode(sjtt::Bytecode::e_Return);
            Frame a(&alloc1, &arg, 1, &loc, 1, &code);
            Frame b(&alloc1, &arg, 1, &loc, 1, &code2);
            BSLS_ASSERT(a != b);
        }

      } break;
      case 1: {
        if (verbose) cout << endl
                          << "constructor" << endl
                          << "===========" << endl;
        bdlma::SequentialAllocator alloc;
        bdld::Datum arg = bdld::Datum::createNull();
        bdld::Datum loc = bdld::Datum::createInteger(2);
        Bytecode code = Bytecode::createOpcode(Bytecode::e_Exit);
        Frame frame(&alloc,
                    &arg,
                    1,
                    &loc,
                    1,
                    &code);
        BSLS_ASSERT(&alloc == frame.allocator());
        BSLS_ASSERT(&arg == frame.arguments());
        BSLS_ASSERT(bsl::vector<bdld::Datum>(&loc, &loc + 1) ==
                    frame.locals());
        BSLS_ASSERT(&alloc == frame.locals().get_allocator().mechanism());
        BSLS_ASSERT(1 == frame.numArguments());
        BSLS_ASSERT(&code == frame.pc());
        BSLS_ASSERT(frame.stack().empty());
        BSLS_ASSERT(&alloc == frame.stack().get_allocator().mechanism());
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
