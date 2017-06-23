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
    // This class is an in-core, value-semantic type a function.

  private:
    // FRIENDS
    friend bool operator==(const Function& lhs, const Function& rhs);

    // DATA
    const Bytecode  *d_code_p;
    int              d_argCount;   // number of named arguments to the function
    int              d_numLocals;  // 

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Function, bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(Function,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    Function(const Bytecode *code,
             int             argCount,
             int             numLocals);
        // Create a new 'Fnction' object having the specified 'code',
        // 'argCount', and 'numLocals'.  The behavior is undefined unless
        // '0 <= argCount' and '0 <= numLocals'.

    //! Function(const Function& original) = default;
        // Create a function having the value of the specified 'original'.

    //! ~Function() = default;
        // Destroy this object. Note that this method does not deallocate any
        // dynamically allocated memory used by this object (see 'destroy').

    // MANIPULATORS
    //! Function& operator=(const Function& rhs) = default;
        // Assign to this object the value of the specified 'rhs' object. Note
        // that this method's definition is compiler generated.

    // ACCESSORS
    int argCount() const;
        // Return the argument count for this function.  The first 'argCount'
        // addreses in a function will contain either the supplied arguments,
        // or 'undefined' for values that are omitted; these values are
        // read-only.

    const Bytecode *code() const;
        // Return the code for this function.

    int numLocals() const;
        // Return the number of locals for this funciton.  The function will
        // have this many values to assign to, with identifiers beginning at
        // 'argCount() + 1'.
};

// FREE OPERATORS
bool operator==(const Function& lhs, const Function& rhs);
    // Return true if the specified 'lhs' and 'rhs' represent the same value.
    // Two 'Function' objects represnet the same value if they have the same
    // 'data' and 'opcode'.


// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                               // --------------
                               // class Function
                               // --------------
// CLASS METHODS
inline
Function Function::createOpcode(Opcode opcode) {
    Function result;
    result.d_opcode = opcode;
    result.d_data = Datum::createNull();
    return result;
}

inline
Function Function::createOpcode(Opcode opcode, const Datum& data) {
    Function result;
    result.d_opcode = opcode;
    result.d_data = data;
    return result;
}

// ACCESSORS
inline
const BloombergLP::bdld::Datum& Function::data() const {
    return d_data;
}

inline
Function::Opcode Function::opcode() const {
    return d_opcode;
}

// FREE OPERATORS
inline bool operator==(const Function& lhs, const Function& rhs) {
    return lhs.d_data == rhs.d_data && lhs.d_opcode == rhs.d_opcode;
}
}

#endif
