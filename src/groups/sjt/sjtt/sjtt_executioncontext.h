// sjtt_executioncontext.h

#ifndef INCLUDED_SJTT_EXECUTIONCONTEXT
#define INCLUDED_SJTT_EXECUTIONCONTEXT

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

namespace BloombergLP {
namespace bslma { class Allocator; }
}

namespace sjtt {

                          // ======================
                           // class ExecutionContext
                           // ======================
class ExecutionContext {
    // This class is an in-core, value-semantic type representing the context
    // of bytecode interpretation.  Note that 'ExecutionContext' objects do not
    // clean up the 'Datum' object (i.e., with 'Datum::destroy') in their
    // stacks.

  public:
    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef BloombergLP::bslma::Allocator Allocator;

  private:
    // DATA
    Allocator          *d_allocator_p;
    bsl::vector<Datum> *d_stack_p;

  public:
    // CREATORS
    explicit ExecutionContext(Allocator *allocator);
        // Create a new 'ExecutionContext' object that allocates memory from
        // the specified 'allocator'.

    ExecutionContext(Allocator *allocator, bsl::vector<Datum> *stack);

    ExecutionContext& operator=(const ExecutionContext& rhs) = default;
        // Assign to this object the value of the specified 'rhs' object and
        // return a reference to this object.

    // ACCESSORS
    Allocator *allocator() const;
        // Return the allocator associated with this object.

    bsl::vector<Datum>* stack() const;
        // Return the value stack for this context.
};

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                           // ----------------------
                           // class ExecutionContext
                           // ----------------------

// CREATORS
inline
ExecutionContext::ExecutionContext(Allocator          *allocator,
                                   bsl::vector<Datum> *stack)
: d_allocator_p(allocator)
, d_stack_p(stack) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != stack);
}

// ACCESSORS
inline
BloombergLP::bslma::Allocator *ExecutionContext::allocator() const {
    return d_allocator_p;
}

inline
bsl::vector<BloombergLP::bdld::Datum>* ExecutionContext::stack() const {
    return d_stack_p;
}
}
#endif
