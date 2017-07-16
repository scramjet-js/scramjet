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

#ifndef INCLUDED_sJTT_FUNCTION
#include <sjtt_function.h>
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
            // Assign, in the variable at 'dest', 1 if the variables at the
            // indices 'x' and 'y' have the same value and 0 otherwise.  The
            // behavior is undefined unless 'x' and 'y' were assigned int32
            // values.

        e_Const,
            // Assign 'value' to the variable at 'dest'.

        e_ConstI32,
            // Assign the integer in 'value' to the variable at 'dest'.

        e_ConstDouble,
            // Assign the double in 'value' to the variable at 'dest'.

        e_ExtractI32,
            // Assign the integer value in the variable at 'x' to the variable
            // at 'dest'.

        e_ExtractDouble,
            // Assign the double value in the variable at 'x' to the variable
            // at 'dest'.

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
            // Call 'function' with 'args' and store assign the result to
            // 'dest'.  The behavior is undefined unless 'args' contains the
            // number of arguments required by 'function'.

        e_Return,
            // Return the value stored in 'x' from this function.
    };

  private:
    // FRIENDS
    friend bool operator==(const Bytecode& lhs, const Bytecode& rhs);

    // DATA
    union {
        Datum    d_value;     // constant value
        Function d_function;  // function to call
    };
    union {
        struct {
            short  d_offset;    // opcode offset
            short  d_x;         // x argument variable index
            short  d_y;         // y argument variable index
        };
        struct {
            const short *d_args_p;  // indices of arguments; held, not owned
        };
    };
    short  d_dest;      // index of a variable to assign to
    Opcode d_opcode;    // operation for this bytecode

  public:
    // CLASS METHODS
    static Bytecode createAllocate(short dest);
        // Return a 'Bytecode' object that represents an 'e_Allocate'
        // operation having the specified 'dest'.

    static Bytecode createAllocateI32(short dest);
        // Return a 'Bytecode' object that represents an 'e_AllocateI32'
        // operation having the specified 'dest'.

    static Bytecode createAllocateDouble(short dest);
        // Return a 'Bytecode' object that reprents an 'e_AllocateDouble'
        // operation having the specified 'dest'.

    static Bytecode createStore(short x, short y);
        // Return a 'Bytecode' object that represents an 'e_Store' operation
        // having the specified 'x' and 'y'.

    static Bytecode createStoreI32(short x, short y);
        // Return a 'Bytecode' object that represents an 'e_StoreI32' operation
        // having the specified 'x' and 'y'.

    static Bytecode createStoreDouble(short x, short y);
        // Return a 'Bytecode' object that represents an 'e_StoreDouble'
        // operation having the specified 'x' and 'y'.

    static Bytecode createLoad(short dest, short x);
        // Return a 'Bytecode' object that represents an 'e_Load' operation
        // having the specified 'dest' and 'x'.

    static Bytecode createLoadI32(short dest, short x);
        // Return a 'Bytecode' object that represents an 'e_LoadI32' operation
        // having the specified 'dest' and 'x'.

    static Bytecode createLoadDouble(short dest, short x);
        // Return a 'Bytecode' object that represents an 'e_LoadDouble'
        // operation having the specified 'dest' and 'x'.

    static Bytecode createEqI32(short dest, short x, short y);
        // Return a 'Bytecode' object that represents an 'e_EqI32' operation
        // having the specified 'dest', 'x', and 'y'.

    static Bytecode createConst(short dest, const Datum& value);
        // Return a 'Bytecode' object that represents an 'e_Const' operation
        // having the specified 'dest' and 'value'.

    static Bytecode createExtractI32(short dest, short x);
        // Return a 'Bytecode' object that represents an 'e_ExtractI32'
        // operation having the specified 'dest' and 'x'.

    static Bytecode createExtractDouble(short dest, short x);
        // Return a 'Bytecode' object that represents an 'e_ExtractDouble'
        // operation having the specified 'dest' and 'x'.

    static Bytecode createJump(short offset);
        // Return a 'Bytecode' object that represents an 'e_Jump' operation
        // having the specified 'offset'.

    static Bytecode createIfI32(short x, short offset);
        // Return a 'Bytecode' object that represents an 'e_IfI32' operation
        // having the specified 'x' and 'offset'.

    static Bytecode createAddI32(short dest, short x, short y);
        // Return a 'Bytecode' object that represenbts an 'e_AddI32' operation
        // having the specified 'dest', x', and 'y'.

    static Bytecode createCall(short            dest,
                               const Function&  function,
                               const short     *args);
        // Return a 'Bytecode' object that represents an 'e_Call' operation
        // having the specified 'dest', 'function', and 'args'.

    static Bytecode createReturn(short x);
        // Return a 'Bytecode' object that represents an 'e_Return' operation
        // having the specified 'x'.

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
