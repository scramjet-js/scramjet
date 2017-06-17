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

#ifndef INCLUDED_SJTT_BYTECODE
#include <sjtt_bytecode.h>
#endif

#ifndef INCLUDED_SJTT_DATUMUDTUTIL
#include <sjtt_datumudtutil.h>
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
    const sjtt::Bytecode *d_firstCode_p;       // held, not owned
    const sjtt::Bytecode *d_pc_p;              // held, not owned
    int                   d_bottom;

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Frame,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    Frame(int                   bottom,
          const sjtt::Bytecode *firstCode,
          const sjtt::Bytecode *pc);
        // Create a new 'Frame' object whose stack begins at the specified
        // 'bottom' index.  The bytecode from this frame starts at the
        // specified 'firstCode', beginning with the specified 'pc' program
        // counter.  The behavior is undefined unless '0 <= bottom'.

    Frame(const Frame& rhs) = default;
        // Create a new 'Frame' object copied from the specified 'rhs' using
        // the specified 'allocator' to allocate memory.

    // MANIPULATORS
    Frame& operator=(const Frame& rhs) = default;
        // Assign to this object the value of the specified 'rhs' object and
        // return a reference to this object.

    void incrementPc();
        // Move the program counter to the next instruction.

    void jump(int index);
        // Move the program counter to the position of the specified 'index'.
        // The behavior is undefined unless '0 <= index'.

    // ACCESSORS
    int bottom() const;
        // Return the bottom of the stack for this frame.

    Datum& getValue(bsl::vector<Datum> *stack,
                    int                 index) const;
        // Return the value at the specified 'index' in this frame from the
        // specified 'stack'.  The behavior is undefined unless
        // '0 <= index && bottom() + index < stack.size()'.

    const sjtt::Bytecode *firstCode() const;
        // Return the address of the first byte code in this frame.

    const sjtt::Bytecode *pc() const;
        // Return the program counter for this frame.
};

// FREE OPERATORS
bool operator==(const Frame& lhs, const Frame& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' 'Frame' objects have the
    // same value, and 'false' otherwise.  Two 'Frame' objects have the same
    // value if their corresponding contained 'Datum' objects have the same
    // value.  For a detailed explanation about equality of 'Datum' objects,
    // refer to the documentation of operator '==' defined for 'Datum'.

bool operator!=(const Frame& lhs, const Frame& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' 'Frame' objects do not
    // have the same value, and 'false' otherwise.  Two 'Frame' objects do not
    // have the same value if their corresponding contained 'Datum' objects
    // have the same value.  For a detailed explanation about inequality of
    // 'Datum' objects, refer to the documentation of operator '==' defined for
    // 'Datum'.

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                                // -----------
                                // class Frame
                                // -----------
// CREATORS
inline
Frame::Frame(int                   bottom,
             const sjtt::Bytecode *firstCode,
             const sjtt::Bytecode *pc)
: d_firstCode_p(firstCode)
, d_pc_p(pc)
, d_bottom(bottom)
{
    BSLS_ASSERT(0 <= bottom);
    BSLS_ASSERT(0 != firstCode);
    BSLS_ASSERT(0 != pc);
}

inline
void Frame::incrementPc()
{
    ++d_pc_p;
}

inline
void Frame::jump(int index)
{
    BSLS_ASSERT(0 <= index);
    d_pc_p = d_firstCode_p + index;
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
const sjtt::Bytecode *Frame::firstCode() const
{
    return d_firstCode_p;
}

inline
const sjtt::Bytecode *Frame::pc() const
{
    return d_pc_p;
}
}  // close package namespace

// FREE OPERATORS
inline
bool sjtt::operator==(const Frame& lhs, const Frame& rhs)
{
    return lhs.firstCode() == rhs.firstCode() &&
        lhs.pc() == rhs.pc() &&
        lhs.bottom() == rhs.bottom();
}

inline
bool sjtt::operator!=(const Frame& lhs, const Frame& rhs)
{
    return !(lhs == rhs);
}

#endif
