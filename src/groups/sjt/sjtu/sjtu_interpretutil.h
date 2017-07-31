// sjtu_interpretutil.h

#ifndef INCLUDED_SJTU_INTERPRETUTIL
#define INCLUDED_SJTU_INTERPRETUTIL

namespace BloombergLP {
namespace bdld  { class Datum; }
namespace bslma { class Allocator; }
}

namespace sjtt { class Function; }

namespace sjtu {

struct InterpretUtil {
    // This is class provides a namespace for functions to interpret Scramjet
    // bytecode.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef BloombergLP::bslma::Allocator Allocator;

    // CLASS METHODS
    static Datum interpret(Allocator             *allocator,
                           const sjtt::Function&  function,
                           const Datum           *args);
        // Return the result of interpreting the specified 'function' over the
        // specified 'args', using the specified 'allocator' to allocate memory
        // return in the result.  The behavior undefined unless
        // '0 != args || 0 == function.argCount()'.
        // Furthermore, the behavior is undefined unless the code in 'function'
        // is well-defined.

    static void checkSingleAssignment(const sjtt::Function& function);
        // Do nothing.  The behavior is undefined unless the specified
        // 'function' assigns to each of its local variables at most once.
        // Note that indices associated with arguments (i.e.,
        // 'index < function.numArgs()'), are already assigned.
};
}

#endif
