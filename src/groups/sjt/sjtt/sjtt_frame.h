// sjtt_frame.h

#ifndef INCLUDED_SJTT_FRAME
#define INCLUDED_SJTT_FRAME

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

#ifndef INCLUDED_BSL_TYPE_TRAITS
#include <bsl_type_traits.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

#ifndef INCLUDED_BSLMF_ISBITWISEMOVEABLE
#include <bslmf_isbitwisemoveable.h>
#endif

#ifndef INCLUDED_BSLMF_NESTEDTRAITDECLARATION
#include <bslmf_nestedtraitdeclaration.h>
#endif

#ifndef INCLUDED_SJTT_BYTEDCODE
#include <sjtt_bytecode.h>
#endif

#ifndef INCLUDED_SJTT_FUNCTION
#include <sjtt_function.h>
#endif

namespace sjtt {

class Bytecode;

                                // ===========
                                // class Frame
                                // ===========


class Frame {
    // This class is an in-core, value-semantic type describing a frame of
    // evaluation.

  public:
    // TYPES
    typedef BloombergLP::bdld::Datum Datum;

  private:
    // DATA
    Function        d_function;
    const Bytecode *d_pc_p;              // held, not owned
    int                   d_bottom;

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Frame,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    Frame(int                    bottom,
          const Function&  function,
          const Bytecode  *pc);
        // Create a new 'Frame' object whose stack begins at the specified
        // 'bottom' index and whose next code is the specified 'pc'.  The
        // behavior is undefined unless '0 != pc || 0 == function.numCodes())'
        // and, if '0 != pc',
        // 'pc >= function.code()' and
        // 'pc < (function.code() + function.numCodes())'.
        // The behavior is also undefined unless '0 >= bottom'.

    Frame(const Frame& rhs) = default;
        // Create a new 'Frame' object copied from the specified 'rhs' using
        // the specified 'allocator' to allocate memory.

    // MANIPULATORS
    Frame& operator=(const Frame& rhs) = default;
        // Assign to this object the value of the specified 'rhs' object and
        // return a reference to this object.

    void incrementPc();
        // Move the program counter to the next instruction.  The behavior is
        // undefined unless '0 != pc()' and
        // 'function.codes() + function.numCodes() > pc() + 1'.

    void jump(int index);
        // Move the program counter to the position of the specified 'index'.
        // The behavior is undefined unless
        // '0 >= index' and
        // '0 != function.code()' and
        // '0 <= index && function().numCodes() > index'.

    // ACCESSORS
    int bottom() const;
        // Return the bottom of the stack for this frame.

    Datum& getValue(bsl::vector<Datum> *stack,
                    int                 index) const;
        // Return the value at the specified 'index' in this frame from the
        // specified 'stack'.  The behavior is undefined unless
        // '0 <= index && bottom() + index < stack.size()'.

    const Function& function() const;
        // Return the 'Function' object associated with this frame.

    const Bytecode *pc() const;
        // Return the program counter for this frame.
};

// FREE OPERATORS
bool operator==(const Frame& lhs, const Frame& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' 'Frame' objects have the
    // same value, and 'false' otherwise.  Two 'Frame' objects have the same
    // value if their corresponding 'bottom', 'function', and 'pc' attributes
    // are the same.

bool operator!=(const Frame& lhs, const Frame& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' 'Frame' objects do not
    // have the same value, and 'false' otherwise.  Two 'Frame' objects do not
    // have the same value if any of their corresponding 'bottom', 'function',
    // or 'pc' attributes are different.

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                                // -----------
                                // class Frame
                                // -----------
// CREATORS
inline
Frame::Frame(int                    bottom,
             const Function&  function,
             const Bytecode  *pc)
: d_function(function)
, d_pc_p(pc)
, d_bottom(bottom)
{
    BSLS_ASSERT(0 != pc || 0 == function.numCodes());
    BSLS_ASSERT(0 == pc ||
      (pc >= function.code() && pc < (function.code() + function.numCodes())));
    BSLS_ASSERT(0 <= bottom);
}

inline
void Frame::incrementPc()
{
    BSLS_ASSERT(0 != d_function.code() &&
                d_function.code() + d_function.numCodes() > pc() + 1);
    ++d_pc_p;
}

inline
void Frame::jump(int index)
{
    BSLS_ASSERT(0 <= index);
    BSLS_ASSERT(0 != d_function.code() &&
                0 <= index && d_function.numCodes() > index);
    d_pc_p = d_function.code() + index;
}

// ACCESSORS
inline
int Frame::bottom() const {
    return d_bottom;
}

inline
BloombergLP::bdld::Datum& Frame::getValue(bsl::vector<Datum> *stack,
                                          int                 index) const
{
    BSLS_ASSERT(0 <= index);
    BSLS_ASSERT(d_bottom + index < stack->size());
    return (*stack)[d_bottom + index];
}

inline
const Function& Frame::function() const
{
    return d_function;
}

inline
const Bytecode *Frame::pc() const
{
    return d_pc_p;
}

// FREE OPERATORS
inline
bool operator==(const Frame& lhs, const Frame& rhs)
{
    return lhs.function () == rhs.function() &&
        lhs.pc() == rhs.pc() &&
        lhs.bottom() == rhs.bottom();
}

inline
bool operator!=(const Frame& lhs, const Frame& rhs)
{
    return !(lhs == rhs);
}
}  // close package namespace

#endif
