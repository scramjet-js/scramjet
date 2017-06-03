// sjtt_bytecode.h

#ifndef INCLUDED_SJTT_BYTECODE
#define INCLUDED_SJTT_BYTECODE

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

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
class ExecutionContext;


                               // ==============
                               // class Bytecode
                               // ==============


class Bytecode {
    // This class is an in-core, value-semantic type describing operations to
    // execution in the Scramjet interpreter.

  public:
        // Signature for functions provided by the user.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;

    enum Opcode {
        // Enumeration used to discriminate between different operations
        // supported by the interpreter.

        e_Push,
            // Push the data in this opcode on the stack.

        e_Load,
            // Push the value from the local variable stored in the index
            // specified by the integer value stored in this object  onto the
            // top of the stack.

        e_Store,
            // Pop the value from the top of the stack and store it into the
            // index specified by the integer value stored in this object.

        e_Jump,
            // Jump to the bytecode at the index specified in the integer of
            // the data stored in this object.

        e_Gosub,
            // Jump to the bytecode at the index specified in the integer of
            // the data stored in this object and push the offset of the next
            // instruction onto the stack.

        e_Return,
            // Pop an integer address off the stack and jump to it.

        e_AddDoubles,
            // Pop the top two arguments on the stack, add them, and push
            // the result.

        e_Execute,
             // pop and evaluate the item at the top of the stack

        e_Exit
            // stop evaluation and return the value on the top of the stack
    };

    static const int s_NumLocals = 32;
        // Maximum number of addressable local variables.

  private:
    // DATA
    Datum  d_data;
    Opcode d_opcode;    // operation for this bytecode
  public:
    // CLASS METHODS
    static Bytecode createOpcode(Opcode opcode);
        // Return a new 'Bytecode' object having the specified 'opcode' and
        // 'Datum::createNull()' for data.

    static Bytecode createOpcode(Opcode opcode, const Datum& data);
        // Return a new 'Bytecode' object having the specified 'opcode' and the
        // specified 'data'.

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode, bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode,
                                   bsl::is_trivially_default_constructible);
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    //! Bytecode() = default;
        // Create a bytecode having an uninitialized value.  The behavior for
        // every accessor method is undefined until this object is assigned a
        // value.

    //! Bytecode(const Bytecode& original) = default;
        // Create a bytecode having the value of the specified 'original'.

    //! ~Bytecode() = default;
        // Destroy this object. Note that this method does not deallocate any
        // dynamically allocated memory used by this object (see 'destroy').

    // MANIPULATORS
    //! Bytecode& operator=(const Bytecode& rhs) = default;
        // Assign to this object the value of the specified 'rhs' object. Note
        // that this method's definition is compiler generated.

    // ACCESSORS
    const Datum& data() const;
        // Return the 'data' for this object.

    Opcode opcode() const;
        // Return the opcode associated with this object.
};

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                               // --------------
                               // class Bytecode
                               // --------------
// CLASS METHODS
inline
Bytecode Bytecode::createOpcode(Opcode opcode) {
    Bytecode result;
    result.d_opcode = opcode;
    result.d_data = Datum::createNull();
    return result;
}

inline
Bytecode Bytecode::createOpcode(Opcode opcode, const Datum& data) {
    Bytecode result;
    result.d_opcode = opcode;
    result.d_data = data;
    return result;
}

// ACCESSORS
inline
const BloombergLP::bdld::Datum& Bytecode::data() const {
    return d_data;
}

inline
Bytecode::Opcode Bytecode::opcode() const {
    return d_opcode;
}
}

#endif
