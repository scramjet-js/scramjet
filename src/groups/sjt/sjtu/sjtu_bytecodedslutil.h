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

#ifndef INCLUDED_SJTT_DATUMUDTUTIL
#include <sjtt_datumudtutil.h>
#endif

namespace sjtu {

struct BytecodeDSLUtil {
    // This is class provides a namespace for utilities to load
    // 'sjtt::Bytecode' from and write 'sjtt::Bytecode' to the following DSL:
    //
    // DSL           = [<bytecode> ('|' <bytecode>)*]
    // bytecode      = <push> |
    //                 <load> |
    //                 <store> |
    //                 <jump> |
    //                 <if> |
    //                 <if equal ints> |
    //                 <equal ints> |
    //                 <increment int> |
    //                 <add doubles> |
    //                 <call> |
    //                 <execute> |
    //                 <exit> |
    //                 <reserve>
    // push          = 'P'<datum>
    // load          = 'L'<int>
    // store         = 'S'<int>
    // jump          = 'J'<int>
    // if            = 'I'<int>
    // if equal ints = 'I=i'<int>
    // equal ints    = '=i'
    // increment int = '++i'<int>
    // add doubles   = '+d'
    // call          = 'C'<int>
    // execute       = 'E'
    // exit          = 'X'
    // datum         = 'd'<double> | 'i'<int> | 'e'<external function name>
    // reserve       = 'V'<int>
    //
    // Example:
    //     "Pd2|Pd3|+d|X"
    // Means to push 2.0, push 3.0, add the values, then return the result.
    //
    // Note that more capabilities will be added as needed.
    //
    // Note also that these utilities are intended for testing purposes; if
    // they perform well it is purely by accident.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;            // for convenience
    typedef BloombergLP::bslma::Allocator Allocator;   // for convenience
    typedef BloombergLP::bslstl::StringRef StringRef;  // for convenience

    typedef bsl::unordered_map<bsl::string,
                               sjtt::DatumUdtUtil::ExternalFunction>
            FunctionNameToAddressMap;
        // Describes a map used to associated the names of functions in the DSL
        // with the addresses of actual functions.

    // CLASS METHODS
    static int readDatum(Datum                           *result,
                         bsl::string                     *errorMessage,
                         Allocator                       *allocator,
                         const StringRef&                 source,
                         const FunctionNameToAddressMap&  functions);
        // Read, into the specified 'result', the datum described by the
        // specified 'source', using the specified 'allocator' to allocate
        // memory and the specified 'functions' to translate function names
        // into addresses and return 0.  If 'source' does not encode a valid
        // datum value (as described above), load, into the specified
        // 'errorMessage', a string describing the problem and return a
        // non-zero value.

    static int readDSL(bsl::vector<sjtt::Bytecode>     *result,
                       bsl::string                     *errorMessage,
                       const StringRef&                 dsl,
                       const FunctionNameToAddressMap&  functions);
        // Load, into the specified 'result', the bytecodes described in the
        // specified 'dsl' and return 0 if 'dsl' is valid; otherwise return a
        // non-zero value and load a description into the specified
        // 'errorMessage'.  Translate function names to addresses using the
        // specified 'functions' map.  Note that the state of 'result' after a
        // failed parse is undefined.  Note also that the allocator associated
        // with 'result' is used to allocate memory for any returned 'Datum'
        // objects.
};
}

#endif
