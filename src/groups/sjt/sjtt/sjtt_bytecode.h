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
    // This class is an in-core, value-semantic type describing Scramjet
    // operations.  Scramjet uses a single-assignment representation -- a
    // variable index can be used as the 'dest' of an operation at most
    // once in a function.  Furthermore, the first N variables of a function
    // with N arguments are its parameters and cannot be used as destination at
    // all.

  public:
        // Signature for functions provided by the user.

    // TYPES
    typedef BloombergLP::bdld::Datum Datum;

    enum Opcode {
        // Enumeration used to discriminate between different operations
        // supported by the interpreter.

        e_Allocate,
            // Allocate space for a 'Datum' value and assign its address to
            // the variable at 'dest'.

        e_AllocateI32,
            // Allocate space for a 32-bit integer value and assign its address
            // to the variable at 'dest'.

        e_AllocateDouble,
            // Allocate space for a double value and assign its address to
            // the variable at 'dest'.

        e_Store,
            // Copy the value stored in the variable at 'y' into the address
            // stored in the variable at 'x'; the behavior is undefined unless
            // this address was allocated by the opcode 'e_Allocate' and 'y'
            // was assigned a 'Datum' value.

        e_StoreI32,
            // Copy the value stored in the variable at 'y' into the address
            // stored in the variable at 'x'; the behavior is undefined unless
            // this address was allocated by the opcode 'e_AllocateI32' and 'y'
            // was assigned an int32 value.

        e_StoreDouble,
            // Copy the value stored in the variable at 'y' into the address
            // stored in the variable at 'x'; the behavior is undefined unless
            // this address was allocated by the opcode 'e_AllocateDouble' and
            // 'y' was assigned a double value.

        e_Load,
            // Assign, to the variable at 'dest', the value stored in the
            // address at 'x'.  The behavior is undefined unless this
            // address was iniitialized at least once with the opcode
            // 'e_Store'.

        e_LoadI32,
            // Assign, to the variable at 'dest', the value stored in the
            // address at 'x'.  The behavior is undefined unless this
            // address was iniitialized at least once with the opcode
            // 'e_StoreI32'.

        e_LoadDouble,
            // Assign, to the variable at 'dest', the value stored in the
            // address at 'x'.  The behavior is undefined unless this
            // address was iniitialized at least once with the opcode
            // 'e_StoreDouble'.

        e_EqI32,
            // Assign , in the variable at 'dest', 1 if the variables at the
            // indices 'x' and 'y' have the same value and 0 otherwise.  The
            // behavior is undefined unless 'x' and 'y' were assigned int32
            // values.

        e_Const,
            // Assign 'value' to the variable at 'dest'.

        e_ConstI32,
            // Assign the integer in 'value' to the variable at 'dest'.

        e_ConstDouble,
            // Assignh the double in 'value' to the variable at 'dest'.

        e_Jump,
            // Jump to the bytecode at 'offset'.

        e_IfI32,
            // If the value at the integer store in the variable at 'x' is 1,
            // jump to 'offset'.  Otherwise, do nothing.  The behavior is
            // undefined unless the variableo at 'x' was assigned an integer.

        e_AddI32,
            // Assign, to the variable at 'dest', the result of adding the
            // values at 'x' and 'y'.  The behavior is undefined unless 'x' and
            // 'y' have been assigned int32 values.

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

        e_Resize,
            // Set the stack for the current frame to the size specified by the
            // integer stored with this opcode, popping excess values and
            // populating new values with 'DatumUdtUtil::e_Undefined'.
    };

  private:
    // FRIENDS
    friend bool operator==(const Bytecode& lhs, const Bytecode& rhs);

    // DATA
    Datum  d_value;     // constant value
    Opcode d_opcode;    // operation for this bytecode
    short  d_dest;      // index of a variable to assign to
    short  d_x;         // x argument variable index
    short  d_y;         // y argument variable index
    short  d_offset;    // opcode offset

  public:
    // CLASS METHODS

    static Bytecode createOpcode(Opcode       opcode,
                                 const Datum& x = Datum::createNull(),
                                 const Datum& y = Datum::createNull());
        // Return a new 'Bytecode' object having the specified 'opcode', 'x',
        // and 'y'.

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
    Opcode opcode() const;
        // Return the opcode associated with this object.

    const Datum& x() const;
        // Return the 'x' for this object.

    const Datum& y() const;
        // Return the 'y' for this object.
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
Bytecode Bytecode::createOpcode(Opcode opcode, const Datum& x, const Datum& y)
{
    Bytecode result;
    result.d_x = x;
    result.d_y = y;
    return result;
}

// ACCESSORS
inline
const BloombergLP::bdld::Datum& Bytecode::x() const {
    return d_x;
}

inline
const BloombergLP::bdld::Datum& Bytecode::y() const {
    return d_y;
}

inline
Bytecode::Opcode Bytecode::opcode() const {
    return d_opcode;
}

// FREE OPERATORS
inline bool operator==(const Bytecode& lhs, const Bytecode& rhs) {
    return lhs.d_opcode == rhs.d_opcode &&
        lhs.d_x == rhs.d_x &&
        lhs.d_y == rhs.d_y;
}
}

#endif
