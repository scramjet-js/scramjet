// sjtt_function.h

#ifndef INCLUDED_SJTT_FUNCTION
#define INCLUDED_SJTT_FUNCTION

#ifndef INCLUDED_BSL_TYPE_TRAITS
#include <bsl_type_traits.h>
#endif

#ifndef INCLUDED_BSLMF_ISBITWISEMOVEABLE
#include <bslmf_isbitwisemoveable.h>
#endif

#ifndef INCLUDED_BSLMF_NESTEDTRAITDECLARATION
#include <bslmf_nestedtraitdeclaration.h>
#endif

namespace sjtt {

class Bytecode;

                               // ==============
                               // class Function
                               // ==============


class Function {
    // This class is an in-core, value-semantic type representing a function.

  private:
    // FRIENDS
    friend bool operator==(const Function& lhs, const Function& rhs);

    // DATA
    const Bytecode  *d_code_p;     // held, not owned
    int              d_numCodes;
    int              d_argCount;   // number of named arguments to the function
    int              d_numLocals;  // number of local variables

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Function, bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(Function,
                                   bsl::is_trivially_default_constructible);
    BSLMF_NESTED_TRAIT_DECLARATION(Function,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    static Function createFunction(const Bytecode *code,
                                   int             numCodes,
                                   int             argCount,
                                   int             numLocals);
        // Create a new 'Fnction' object having the specified 'code',
        // 'argCount', and 'numLocals'.  The behavior is undefined unless
        // '0 <= argCount' and '0 <= numLocals', and
        // '0 != code || 0 == numCodes'.

    // ACCESSORS
    int argCount() const;
        // Return the argument count for this function.  The first 'argCount'
        // addreses in a function will contain either the supplied arguments,
        // or 'undefined' for values that are omitted; these values are
        // read-only.

    const Bytecode *code() const;
        // Return the code for this function.

    int numCodes() const;
        // Return the number of byte codes in this function.

    int numLocals() const;
        // Return the number of locals for this funciton.  The function will
        // have this many values to assign to, with identifiers beginning at
        // 'argCount() + 1'.
};

// FREE OPERATORS
bool operator==(const Function& lhs, const Function& rhs);
    // Return true if the specified 'lhs' and 'rhs' represent the same value.
    // Two 'Function' objects represnet the same value if they have the same
    // 'argCount', 'code', and 'numLocals'.

bool operator!=(const Function& lhs, const Function& rhs);
    // Return false if the specified 'lhs' and 'rhs' represent the same value.
    // Two 'Function' objects represnet the same value if they have the same
    // 'argCount', 'code', and 'numLocals'.

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                               // --------------
                               // class Function
                               // --------------
// CLASS METHODS
inline
Function Function::createFunction(const Bytecode *code,
                                  int             numCodes,
                                  int             argCount,
                                  int             numLocals)
{
    BSLS_ASSERT(0 != code || 0 == numCodes);
    BSLS_ASSERT(0 <= argCount);
    BSLS_ASSERT(0 <= numLocals);
    Function result;
    result.d_code_p = code;
    result.d_argCount = argCount;
    result.d_numCodes = numCodes;
    result.d_numLocals = numLocals;
    return result;
}

// ACCESSORS
inline
int Function::argCount() const
{
    return d_argCount;
}

inline
const Bytecode *Function::code() const
{
    return d_code_p;
}

inline
int Function::numCodes() const
{
    return d_numCodes;
}

inline
int Function::numLocals() const
{
    return d_numLocals;
}

// FREE OPERATORS
inline bool operator==(const Function& lhs, const Function& rhs) {
    return lhs.d_argCount == rhs.d_argCount &&
        lhs.d_code_p == rhs.d_code_p &&
        lhs.d_numCodes == rhs.d_numCodes &&
        lhs.d_numLocals == rhs.d_numLocals;
}

inline bool operator!=(const Function& lhs, const Function& rhs) {
    return !(lhs == rhs);
}
}

#endif
