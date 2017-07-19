// sjtu_bytecodedslutil.h

#ifndef INCLUDED_SJTU_BYTECODEDSLUTIL
#define INCLUDED_SJTU_BYTECODEDSLUTIL

#ifndef INCLUDED_BSL_UNORDERED_MAP
#include <bsl_unordered_map.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

#ifndef INCLUDED_SJTT_BYTECODE
#include <sjtt_bytecode.h>
#endif

#ifndef INCLUDED_SJTT_FUNCTION
#include <sjtt_function.h>
#endif

namespace sjtu {

struct BytecodeDSLUtil {
    // This is class provides a namespace for utilities to load
    // 'sjtt::Bytecode' from and write 'sjtt::Bytecode' to the following DSL:
    //
    // DSL           = [<bytecode> ('|' <bytecode>)*]
    // bytecode      = <allocate> |
    //                 <allocateI32> |
    //                 <allocateDbl> |
    //                 <store> |
    //                 <storeI32> |
    //                 <storeDbl> |
    //                 <load> |
    //                 <loadI32> |
    //                 <loadDouble> |
    //                 <eqI32> |
    //                 <const> |
    //                 <extractI32> |
    //                 <extractDouble> |
    //                 <jump> |
    //                 <ifI32> |
    //                 <addI32> |
    //                 <call> |
    //                 <return>
    // allocate      = 'A'<int>
    // allocateI32   = 'Ai'<int>
    // allocateDbl   = 'Ad'<int>
    // store         = 'S'<int>','<int>
    // storeI32      = 'Si'<int>','<int>
    // storeDbl      = 'Si'<int>','<int>
    // load          = 'L'<int>','<int>
    // loadI32       = 'Li'<int>','<int>
    // loadDbl       = 'Ld'<int>','<int>
    // eqI32         = '=i'<int>','<int>,<int>
    // const         = 'C'<int>','<datum>
    // extractI32    = 'Ei'<int>','<int>
    // extractDbl    = 'Ed'<int>','<int>
    // jump          = 'J'<int>
    // ifI32         = 'Ii'<int>','<int>
    // addI32        = '+i'<int>','<int>','<int>
    // call          = '()'<int>','<name>(','<int>)*'
    // return        = 'R'<int>
    // datum         = 'T' | 'F' | 'd'<double> | 'i'<int>
    //
    // Example:
    //     "C8,d3|()9,foo,1,2,8|R9"
    // Means:
    // 1. Assign the datum 3.0 to variable 8.
    // 2. Call the function 'foo' with the values in 1, 2, and 8; and assign
    //    the result to variable 9.
    // 3. Return the value in variable 9.
    //
    // Note that more capabilities will be added as needed.
    //
    // Note also that these utilities are intended for testing purposes; if
    // they perform well it is purely by accident.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;            // for convenience
    typedef BloombergLP::bslma::Allocator Allocator;   // for convenience
    typedef BloombergLP::bslstl::StringRef StringRef;  // for convenience

    typedef bsl::unordered_map<bsl::string, sjtt::Function> FunctionMap;
        // Describes a map used to associated the names of functions in the DSL
        // with the addresses of actual functions.

    // CLASS METHODS
    static int readDatum(Datum                           *result,
                         bsl::string                     *errorMessage,
                         Allocator                       *allocator,
                         const StringRef&                 source);
        // Read, into the specified 'result', the datum described by the
        // specified 'source', using the specified 'allocator' to allocate
        // memory.  If 'source' does not encode a valid datum value (as
        // described above), load, into the specified 'errorMessage', a string
        // describing the problem and return a non-zero value.

    static int readDSL(bsl::vector<sjtt::Bytecode> *result,
                       bsl::string                 *errorMessage,
                       const StringRef&             dsl,
                       const FunctionMap&           functions);
        // Load, into the specified 'result', the bytecodes described in the
        // specified 'dsl' and return 0 if 'dsl' is valid; otherwise return a
        // non-zero value and load a description into the specified
        // 'errorMessage'.  Translate function names using the specified
        // 'functions' map.  Note that the state of 'result' after a failed
        // parse is undefined.  Note also that the allocator associated with
        // 'result' is used to allocate memory for any returned 'Datum'
        // objects or argument arrays.
};
}

#endif
