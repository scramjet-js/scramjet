// sjtu_datumutil.h

#ifndef INCLUDED_SJTU_DATUMUTIL
#define INCLUDED_SJTU_DATUMUTIL

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

namespace sjtt { class ExecutionContext; }

namespace sjtu {

struct DatumUtil {
    // This is class provides a namespace for utilities to use `bdld::Datum`
    // objects.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef void (* ExternalFunction)(sjtt::ExecutionContext *context);

    enum UdtCode {
        // Enumeration used to describe the types of values used in the UDT
        // section of `Datum`.

        e_Undefined,
            // the datum represents an undefined value, the data is unused

        e_ExternalFunction,
            // the data of the datum will be of type 'ExternalFunction'

        e_User,
            // Values >= 'e_User' are available for use by clients of Scramjet
    };

    // CLASS DATA
    static const Datum s_Null;         // 'Datum::creaetNull'
    static const Datum s_Undefined;    // 'Datum::createUdt(0, d_Undefined)`
};
}

#endif
