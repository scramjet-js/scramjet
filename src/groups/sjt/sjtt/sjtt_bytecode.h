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
    //
    // # Function invocation
    //
    // When the bytecode for a function begins executing, its stack will
    // initially contain the arguments that were passed to it, along with
    // enough `DatumUdtUtil::s_Undefined` values so that at least
    // the stack has at least `s_MinInitialStackSize` elements.
    //
    // # Function return
    //
    // When an `e_Exit` opcode is encountered, the top of the stack is saved,
    // all values pertaining to the existing frame are popped off, then the
    // saved value is pushed back onto the stack as the return value of the
    // function.

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
            // Push the value from the value stored in the stack in the index
            // specified by the integer value stored in this object onto the
            // top of the stack.

        e_Store,
            // Pop the value from the top of the stack and store it in the
            // location in the stack indicated by the index specified by the
            // integer value stored in this object.

        e_Jump,
            // Jump to the bytecode at the index specified in the integer of
            // the data stored in this object.

        e_If,
            // If the boolean value on the top of the stack is true, jump to
            // the offset stored in this code; othewise, proceed to the next
            // code.

        e_IfEqInts,
            // Pop the two integers on the top of the stack. If they are equal
            // jump to the offset stored in this code; otherwise, proceed to
            // the next code.

        e_EqInts,
            // Pop the two integers on the top of the stack.  Push 'true' on
            // the stack if they are the same and 'false' otherwise.

        e_IncInt,
            // Increment the local variable stored in the index indicated by
            // the integer in the data for this code.

        e_AddDoubles,
            // Pop the top two arguments on the stack, add them, and push
            // the result.

        e_AddInts,
            // Pop the top two arguments on the stack, add them, and push
            // the result.

        e_Call,
            // The top value on the stack is an integer with the number of
            // arguments for the call. Create, and begin evaluating, a new
            // frame with a program counter set to the index indicated by the
            // data associated with this opcode, passing a pointer to the
            // arguments that are on the stack.

        e_Execute,
             // Pop the external function from the top of the stack, pop
             // the integer value containing the number of arguments for the
             // function, pop that number of values off the stack and invoke
             // the function with those valuse as arguments.  Push the return
             // value of the function on the stack.

        e_Exit,
            // Stop executing the current frame.  If the current frame is the
            // last one, return the value at the top of the current stack;
            // otherwise, store this value, return to the previous frame, pop,
            // from its stack, the argument count and arguments from its stack,
            // push onto its stack the return value, and resume executing.

        e_Reserve,
            // Ensure that the stack for the current frame has at least the
            // number of values stored in the integer of this opcode.
    };

    static const int s_MinInitialStackSize = 8;
        // The minimum number of values on the stack when a function starts.

  private:
    // FRIENDS
    friend bool operator==(const Bytecode& lhs, const Bytecode& rhs);

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

// FREE OPERATORS
bool operator==(const Bytecode& lhs, const Bytecode& rhs);
    // Return true if the specified 'lhs' and 'rhs' represent the same value.
    // Two 'Bytecode' objects represnet the same value if they have the same
    // 'data' and 'opcode'.


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

// FREE OPERATORS
inline bool operator==(const Bytecode& lhs, const Bytecode& rhs) {
    return lhs.d_data == rhs.d_data && lhs.d_opcode == rhs.d_opcode;
}
}

#endif
