// sjtu_datumutil.h

#ifndef INCLUDED_SJTU_DATUMUTIL
#define INCLUDED_SJTU_DATUMUTIL

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

namespace sjtt { class ExecutionContext; }
namespace sjtt { class Bytecode; }

namespace sjtu {

struct DatumUtil {
    // This is class provides a namespace for utilities to use 'bdld::Datum'
    // objects.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef Datum (* ExternalFunction)(const sjtt::ExecutionContext& context);

    enum UdtCode {
        // Enumeration used to describe the types of values used in the UDT
        // section of 'Datum'.

        e_Undefined,
            // the datum represents an undefined value, the data is unused

        e_ExternalFunction,
            // the data of the datum will be of type 'ExternalFunction'

        e_Code,
            // the data of the datum will be of type 'const Byecode *'

        e_User,
            // Values >= 'e_User' are available for use by clients of Scramjet
    };

    // CLASS DATA
    static const Datum s_Null;         // 'Datum::creaetNull'
    static const Datum s_Undefined;    // 'Datum::createUdt(0, d_Undefined)'

    // CLASS METHODS
    static bool isExternalFunction(const Datum& value);
        // Return true if the specified 'value' contains an 'ExternalFunction'
        // and false otherwise.

    static bool isCode(const Datum& value);
        // Return true if the specified 'value' is code and false otherwise.

    static const sjtt::Bytecode *getCode(const Datum& value);
        // Return the code in the specified 'value'.  The behavior is undefined
        // unless 'true == isCode(value)'.

    static Datum datumFromCode(const sjtt::Bytecode *code);
        // Return a new 'Datum' object containing the specified 'code'.

    static ExternalFunction getExternalFunction(const Datum& value);
        // Return the 'ExternalFunction' in th specified 'value'.  The behavior
        // is undfined unless 'true == isExternalFunction(value)'.

    static Datum datumFromExternalFunction(ExternalFunction function);
        // Return a new 'Datum' object containing the specified 'function'.
};

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                              // ---------------
                              // class DatumUtil
                              // ---------------

inline
bool DatumUtil::isExternalFunction(const Datum& value) {
    return value.isUdt() && value.theUdt().type() == e_ExternalFunction;
}

inline
bool DatumUtil::isCode(const Datum& value) {
    return value.isUdt() && value.theUdt().type() == e_Code;
}

inline
const sjtt::Bytecode *DatumUtil::getCode(const Datum& value) {
    BSLS_ASSERT(isCode(value));

    return static_cast<sjtt::Bytecode *>(value.theUdt().data());
}

inline
BloombergLP::bdld::Datum DatumUtil::datumFromCode(const sjtt::Bytecode *code) {
    return Datum::createUdt(const_cast<sjtt::Bytecode *>(code), e_Code);
}

inline
DatumUtil::ExternalFunction
DatumUtil::getExternalFunction(const Datum& value) {
    BSLS_ASSERT(DatumUtil::isExternalFunction(value));
    // Note that casing from data to function pointer is theoretically
    // non-portable, but doees work on every (non-theoretical) modern platform.

    return reinterpret_cast<ExternalFunction>(value.theUdt().data());
}

inline
DatumUtil::Datum
DatumUtil::datumFromExternalFunction(ExternalFunction function) {
    BSLS_ASSERT(0 != function);
    return Datum::createUdt(reinterpret_cast<void *>(function),
                            e_ExternalFunction);
}
}

#endif
