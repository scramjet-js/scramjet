// sjtd_datumfactory.t.cpp                                     -*-C++-*-

#include <sjtd_datumfactory.h>

#include <bdlma_sequentialallocator.h>
#include <bdls_testutil.h>

using namespace BloombergLP;
using namespace bsl;
using namespace sjtd;

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
        return sjtd::DatumUdtUtil::s_Null;
    }
}

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int         test = argc > 1 ? atoi(argv[1]) : 0;
    const bool     verbose = argc > 2;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    // TODO: need to set up a DSEL for this.

    switch (test) { case 0:
      case 7: {
        if (verbose) cout << endl
                          << "()(bool)" << endl
                          << "========" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(bdld::Datum::createBoolean(true) == f(true));
        ASSERT(bdld::Datum::createBoolean(false) == f(false));
      } break;
      case 6: {
        if (verbose) cout << endl
                          << "()(ExternalFunction)" << endl
                          << "====================" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(sjtd::DatumUdtUtil::datumFromExternalFunction(testFun) ==
               f(testFun));
      } break;
      case 5: {
        if (verbose) cout << endl
                          << "()(double)" << endl
                          << "==========" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(bdld::Datum::createDouble(3) == f(3.));
      } break;
      case 4: {
        if (verbose) cout << endl
                          << "()(int)" << endl
                          << "=======" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(bdld::Datum::createInteger(3) == f(3));
      } break;
      case 3: {
        if (verbose) cout << endl
                          << "()()" << endl
                          << "====" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(&sjtd::DatumUdtUtil::s_Null == &f());
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "u()" << endl
                          << "===" << endl;

        bdlma::SequentialAllocator alloc;
        const DatumFactory f(&alloc);
        ASSERT(&sjtd::DatumUdtUtil::s_Undefined == &f.u());
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "constructor" << endl
                          << "===========" << endl;

        bdlma::SequentialAllocator alloc;
        DatumFactory f(&alloc);
        ASSERT(&alloc == f.allocator());
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
