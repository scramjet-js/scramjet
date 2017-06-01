// sjtu_interpretutil.h

#ifndef INCLUDED_SJTU_INTERPRETUTIL
#define INCLUDED_SJTU_INTERPRETUTIL

namespace BloombergLP {
namespace bdld  { class Datum; }
namespace bslma { class Allocator; }
}

namespace sjtt { class Bytecode; }

namespace sjtu {

struct InterpretUtil {
    // This is class provides a namespace for functions to interpret Scramjet
    // bytecode.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef BloombergLP::bslma::Allocator Allocator;

    // CLASS METHODS
    static Datum interpretBytecode(Allocator            *allocator,
                                   const sjtt::Bytecode *firstCode,
                                   int                   numCodes);
        // Evaluate the specified 'numCodes' byte 'codes' and return the result
        // after evaluating an 'e_Return' code, using the specified 'allocator'
        // to allocate memory.  The behavior is undefined if the codes cannot
        // be evaluated e.g., if the interpreter is directed to execute a
        // non-function, or the interpreter would be directed to execute a code
        // at an index not within the range of valid codes.
};
}

#endif
