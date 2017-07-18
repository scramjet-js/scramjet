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
            // Assign, in the variable at 'dest', a non-zero value if the
            // variables at the indices 'x' and 'y' have the same value and 0
            // otherwise.  The behavior is undefined unless 'x' and 'y' were
            // assigned int32 values.

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
            // If the value at the integer store in the variable at 'x' is
            // non-zero, jump to 'offset'.  Otherwise, do nothing.  The
            // behavior is undefined unless the variableo at 'x' was assigned
            // an integer.

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
        // having the specified 'dest', 'function', and 'args'.  The behavior
        // is undefined unless '0 != args || 0 == function.argCount()'.

    static Bytecode createReturn(short x);
        // Return a 'Bytecode' object that represents an 'e_Return' operation
        // having the specified 'x'.

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode, bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode,
                                   bsl::is_trivially_default_constructible);
    BSLMF_NESTED_TRAIT_DECLARATION(Bytecode,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // ACCESSORS
    const Datum& value() const;
        // Return the 'value' of this object.  The behavior is undefined unless
        // 'value' was initialized.

    const Function& function() const;
        // Return the 'function' of this object.  The behavior is undefined
        // unless 'function' was initialized.

    short offset() const;
        // Return the 'offset' of this object.  The behavior is undefined
        // unless 'offset' was initialized.

    short x() const;
        // Return the 'x' of this object.  The behavior is undefined unless
        // 'x' was initialized.

    short y() const;
        // Return the 'y' of this object.  The behavior is undefined unless
        // 'y' was initialized.

    const short *args() const;
         // Return the 'args' of this object.  The behavior is undefined unless
         // 'args' was initialized.

    short dest() const;
        // Return the 'dest' of this object.  The behavior is undefined unless
        // 'dest' was initialized.

    Opcode opcode() const;
        // Return the opcode of this object.
};

// FREE OPERATORS
bool operator==(const Bytecode& lhs, const Bytecode& rhs);
    // Return true if the specified 'lhs' and 'rhs' represent the same value.
    // Two 'Bytecode' objects represnet the same value if they have the same
    // 'opcode' and associated arguments.


// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                               // --------------
                               // class Bytecode
                               // --------------
// CLASS METHODS
inline
Bytecode Bytecode::createAllocate(short dest)
{
    Bytecode result;
    result.d_opcode = e_Allocate;
    result.d_dest = dest;
    return result;
}

inline
Bytecode Bytecode::createAllocateI32(short dest)
{
    Bytecode result;
    result.d_opcode = e_AllocateI32;
    result.d_dest = dest;
    return result;
}

inline
Bytecode Bytecode::createAllocateDouble(short dest)
{
    Bytecode result;
    result.d_opcode = e_AllocateDouble;
    result.d_dest = dest;
    return result;
}

inline
Bytecode Bytecode::createStore(short x, short y)
{
    Bytecode result;
    result.d_opcode = e_Store;
    result.d_x = x;
    result.d_y = y;
    return result;
}

inline
Bytecode Bytecode::createStoreI32(short x, short y)
{
    Bytecode result;
    result.d_opcode = e_StoreI32;
    result.d_x = x;
    result.d_y = y;
    return result;
}

inline
Bytecode Bytecode::createStoreDouble(short x, short y)
{
    Bytecode result;
    result.d_opcode = e_StoreDouble;
    result.d_x = x;
    result.d_y = y;
    return result;
}

inline
Bytecode Bytecode::createLoad(short dest, short x)
{
    Bytecode result;
    result.d_opcode = e_Load;
    result.d_dest = dest;
    result.d_x = x;
    return result;
}

inline
Bytecode Bytecode::createLoadI32(short dest, short x)
{
    Bytecode result;
    result.d_opcode = e_LoadI32;
    result.d_dest = dest;
    result.d_x = x;
    return result;
}

inline
Bytecode Bytecode::createLoadDouble(short dest, short x)
{
    Bytecode result;
    result.d_opcode = e_LoadDouble;
    result.d_dest = dest;
    result.d_x = x;
    return result;
}

inline
Bytecode Bytecode::createEqI32(short dest, short x, short y)
{
    Bytecode result;
    result.d_opcode = e_EqI32;
    result.d_dest = dest;
    result.d_x = x;
    result.d_y = y;
    return result;
}

inline
Bytecode Bytecode::createConst(short dest, const Datum& value)
{
    Bytecode result;
    result.d_opcode =  e_Const;
    result.d_dest = dest;
    result.d_value = value;
    return result;
}

inline
Bytecode Bytecode::createExtractI32(short dest, short x)
{
    Bytecode result;
    result.d_opcode = e_ExtractI32;
    result.d_dest = dest;
    result.d_x = x;
    return result;
}

inline
Bytecode Bytecode::createExtractDouble(short dest, short x)
{
    Bytecode result;
    result.d_opcode = e_ExtractDouble;
    result.d_dest = dest;
    result.d_x = x;
    return result;
}

inline
Bytecode Bytecode::createJump(short offset)
{
    Bytecode result;
    result.d_opcode = e_Jump;
    result.d_offset = offset;
    return result;
}

inline
Bytecode Bytecode::createIfI32(short x, short offset)
{
    Bytecode result;
    result.d_opcode = e_IfI32;
    result.d_x = x;
    result.d_offset = offset;
    return result;
}

inline
Bytecode Bytecode::createAddI32(short dest, short x, short y)
{
    Bytecode result;
    result.d_opcode = e_AddI32;
    result.d_dest = dest;
    result.d_x = x;
    result.d_y = y;
    return result;
}

inline
Bytecode Bytecode::createCall(short            dest,
                              const Function&  function,
                              const short     *args)
{
    BSLS_ASSERT(0 != args || 0 == function.argCount());
    Bytecode result;
    result.d_opcode = e_Call;
    result.d_dest = dest;
    result.d_function = function;
    result.d_args_p = args;
    return result;
}

inline
Bytecode Bytecode::createReturn(short x)
{
    Bytecode result;
    result.d_opcode = e_Return;
    result.d_x = x;
    return result;
}

// ACCESSORS

inline
const Bytecode::Datum& Bytecode::value() const
{
    return d_value;
}

inline
const Function& Bytecode::function() const
{
    return d_function;
}

inline
short Bytecode::offset() const
{
    return d_offset;
}

inline
short Bytecode::x() const
{
    return d_x;
}

inline
short Bytecode::y() const
{
    return d_y;
}

inline
const short *Bytecode::args() const
{
    return d_args_p;
}

inline
short Bytecode::dest() const
{
    return d_dest;
}

inline
Bytecode::Opcode Bytecode::opcode() const {
    return d_opcode;
}
}

#endif
