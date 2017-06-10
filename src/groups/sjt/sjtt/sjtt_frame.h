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
    typedef BloombergLP::bslma::Allocator Allocator;

  private:
    // DATA
    bsl::vector<Datum>    d_stack;
    bsl::vector<Datum>    d_locals;
    const Datum          *d_arguments_p;   // held, not owned
    const sjtt::Bytecode *d_firstCode_p;
    const sjtt::Bytecode *d_pc_p;          // held, not owned
    int                   d_numArguments;

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Frame,
                                   BloombergLP::bslma::UsesBslmaAllocator);
    BSLMF_NESTED_TRAIT_DECLARATION(Frame,
                                   BloombergLP::bslmf::IsBitwiseMoveable);

    // CREATORS
    Frame(Allocator            *allocator,
          const Datum          *arguments,
          int                   numArguments,
          const Datum          *locals,
          int                   numLocals,
          const sjtt::Bytecode *code);
        // Create a new 'Frame' object using the specified 'allocator' to
        // allocate memory, having the specified 'argumentCount' 'arguments',
        // to execute the specified 'code'.  Note that if '0 == numArgments',
        // 'arguments' may be null, and if '0 == numLocals', 'locals' may be
        // null.

    Frame(const Frame& rhs, Allocator *allocator);
        // Create a new 'Frame' object copied from the specified 'rhs' using
        // the specified 'allocator' to allocate memory.

    // MANIPULATORS
    Frame& operator=(const Frame& rhs);
        // Assign to this object the value of the specified 'rhs' object and
        // return a reference to this object.

    void incrementPc();
        // Move the program counter to the next instruction.

    void jump(int index);
        // Move the program counter to the position of the specified 'index'.
        // The behavior is undefined unless '0 <= index'.

    void pop();
        // Pop the top value off of this stack.  The behavior is undefined
        // unless there is at least one value on the stack.

    void popMany(int count);
        // Pop the specified 'count' values from the top of the stack.  The
        // behavior is undefined unless '0 <= count && stackSize() >= count';

    void push(const Datum& value);
        // Push the specified 'value' on the stack for this frame.

    void setLocal(int index, const Datum& value);
        // Assign the specified 'value' to the local at the specified 'index'.
        // The behavior is undefined unless 'locals.size() > index'.

    void swap(Frame& other);
        // Swap contents of this 'Frame' object with the contents of the
        // specified 'other' 'Frame' object.  The behavior is undefined
        // unless both the 'Frame' objects are allocated using the same

    // ACCESSORS
    Allocator *allocator() const;
        // Return the allocator associated with this object.

    const Datum* arguments() const;
        // Return a pointer to the arguments for this frame.

    const bsl::vector<Datum>& locals() const;
        // Return the local variables for this frame.

    int numArguments() const;
        // Return the number of arguments for this frame.

    const sjtt::Bytecode *pc() const;
        // Return the program counter for this frame.

    const bsl::vector<Datum>& stack() const;
        // Return the stack for this frame.
};

// FREE OPERATORS
void swap(Frame& a, Frame& b);
    // Swap contents of the specified 'a' 'Frame' object with the contents of
    // the specified 'b' 'Frame' object.

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
Frame::Frame(Allocator            *allocator,
             const Datum          *arguments,
             int                   numArguments,
             const Datum          *locals,
             int                   numLocals,
             const sjtt::Bytecode *code)
: d_stack(allocator)
, d_locals(locals, locals + numLocals, allocator)
, d_arguments_p(arguments)
, d_firstCode_p(code)
, d_pc_p(code)
, d_numArguments(numArguments) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != arguments || 0 == numArguments);
    BSLS_ASSERT(0 <= numArguments);
    BSLS_ASSERT(0 != locals || 0 == numLocals);
    BSLS_ASSERT(0 <= numLocals);
    BSLS_ASSERT(0 != code);
}

inline
Frame::Frame(const Frame& rhs, Allocator *allocator)
: d_stack(rhs.d_stack, allocator)
, d_locals(rhs.d_locals, allocator)
, d_arguments_p(rhs.d_arguments_p)
, d_firstCode_p(rhs.d_firstCode_p)
, d_pc_p(rhs.d_pc_p)
, d_numArguments(rhs.d_numArguments) {
    BSLS_ASSERT(0 != allocator);
}

// MANIPULATORS
inline
Frame& Frame::operator=(const Frame& rhs)
{
    Frame copy(rhs, allocator());
    swap(copy);
    return *this;
}

inline
void Frame::incrementPc() {
    ++d_pc_p;
}

inline
void Frame::jump(int index) {
    BSLS_ASSERT(0 <= index);
    d_pc_p = d_firstCode_p + index;
}

inline
void Frame::pop() {
    BSLS_ASSERT(!d_stack.empty());
    d_stack.pop_back();
}

inline
void Frame::popMany(int count) {
    BSLS_ASSERT(0 <= count && d_stack.size() >= count);
    d_stack.erase(d_stack.end() - count, d_stack.end());
}

inline
void Frame::push(const Datum& value) {
    d_stack.push_back(value);
}

inline
void Frame::setLocal(int index, const Datum& value) {
    BSLS_ASSERT(d_locals.size() > index);
    d_locals[index] = value;
}

inline
void Frame::swap(Frame& other)
{
    BSLS_ASSERT(allocator() == other.allocator());
    using bsl::swap;
    swap(d_arguments_p, other.d_arguments_p);
    swap(d_stack, other.d_stack);
    swap(d_locals, other.d_locals);
    swap(d_firstCode_p, other.d_firstCode_p);
    swap(d_pc_p, other.d_pc_p);
    swap(d_numArguments, other.d_numArguments);
}

// ACCESSORS
inline
BloombergLP::bslma::Allocator *Frame::allocator() const {
    return d_stack.get_allocator().mechanism();
}

inline
const BloombergLP::bdld::Datum* Frame::arguments() const {
    return d_arguments_p;
}

inline
const bsl::vector<BloombergLP::bdld::Datum>& Frame::locals() const {
    return d_locals;
}

inline
int Frame::numArguments() const {
    return d_numArguments;
}

inline
const sjtt::Bytecode *Frame::pc() const {
    return d_pc_p;
}

inline
const bsl::vector<BloombergLP::bdld::Datum>& Frame::stack() const {
    return d_stack;
}
}  // close package namespace

// FREE OPERATORS
inline
void sjtt::swap(Frame& a, Frame& b)
{
    if (a.allocator() == b.allocator()) {
        a.swap(b);
    }
    else {
        Frame tempA(a, b.allocator());
        Frame tempB(b, a.allocator());

        a.swap(tempB);
        b.swap(tempA);
    }
}

inline
bool sjtt::operator==(const Frame& lhs, const Frame& rhs)
{
    return lhs.arguments() == rhs.arguments() &&
           lhs.locals() == rhs.locals() &&
           lhs.numArguments() == rhs.numArguments() &&
           lhs.pc() == rhs.pc() &&
           lhs.stack() == rhs.stack();
}

inline
bool sjtt::operator!=(const Frame& lhs, const Frame& rhs)
{
    return !(lhs == rhs);
}

#endif
