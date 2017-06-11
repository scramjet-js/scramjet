// sjtt_datumudtutil.t.cpp                                     -*-C++-*-

#include <sjtt_datumudtutil.h>

#include <bdls_testutil.h>
#include <bdlma_localsequentialallocator.h>

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


namespace {
    bdld::Datum testExternalFunction(const sjtt::ExecutionContext&) {
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
      case 8: {
        if (verbose) cout << endl
                          << "datumFromCode" << endl
                          << "=============" << endl;

        const bdld::Datum d = DatumUdtUtil::datumFromCode(0);
        ASSERT(DatumUdtUtil::isCode(d));
      } break;
      case 7: {
        if (verbose) cout << endl
                          << "getCode" << endl
                          << "=======" << endl;

        const bdld::Datum d = bdld::Datum::createUdt(0, DatumUdtUtil::e_Code);
        ASSERT(0 == DatumUdtUtil::getCode(d));
      } break;
      case 6: {
        if (verbose) cout << endl
                          << "isCode" << endl
                          << "======" << endl;

        const bdld::Datum d = bdld::Datum::createUdt(0, DatumUdtUtil::e_Code);

        // Code

        ASSERT(DatumUdtUtil::isCode(d));

        // Udt but not code

        ASSERT(!DatumUdtUtil::isCode(DatumUdtUtil::s_Undefined));

        // not udt

        ASSERT(!DatumUdtUtil::isCode(bdld::Datum::createInteger(3)));
      } break;
      case 5: {
        if (verbose) cout << endl
                          << "getExternalFunction" << endl
                          << "==================" << endl;
        const bdld::Datum d = DatumUdtUtil::datumFromExternalFunction(
                                                         testExternalFunction);
        ASSERT(testExternalFunction == DatumUdtUtil::getExternalFunction(d));
      } break;
      case 4: {
        if (verbose) cout << endl
                          << "isExternalFunction" << endl
                          << "==================" << endl;
        ASSERT(false ==
               DatumUdtUtil::isExternalFunction(bdld::Datum::createNull()));
        ASSERT(true ==
               DatumUdtUtil::isExternalFunction(
               DatumUdtUtil::datumFromExternalFunction(testExternalFunction)));
      } break;
      case 3: {
        if (verbose) cout << endl
                          << "datumFromExternalFunction" << endl
                          << "===========" << endl;
        const bdld::Datum result = DatumUdtUtil::datumFromExternalFunction(
                                                         testExternalFunction);
        ASSERT(result.isUdt());
        const bdld::DatumUdt udt = result.theUdt();
        ASSERT(testExternalFunction ==
                 reinterpret_cast<DatumUdtUtil::ExternalFunction>(udt.data()));
      } break;
      case 2: {
        if (verbose) cout << endl
                          << "s_Undefined" << endl
                          << "===========" << endl;
        const bdld::Datum undefined = bdld::Datum::createUdt(
                                                    0,
                                                    DatumUdtUtil::e_Undefined);
        ASSERT(undefined  == DatumUdtUtil::s_Undefined);
      } break;
      case 1: {
        if (verbose) cout << endl
                          << "s_Null" << endl
                          << "======" << endl;
        const bdld::Datum null = bdld::Datum::createNull();
        ASSERT(null == DatumUdtUtil::s_Null);
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
