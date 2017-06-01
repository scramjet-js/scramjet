// sjtt_executioncontext.h

#ifndef INCLUDED_SJTT_EXECUTIONCONTEXT
#define INCLUDED_SJTT_EXECUTIONCONTEXT

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

namespace BloombergLP {
namespace bdld  { class Datum;     }
namespace bslma { class Allocator; }
}

namespace sjtt {

                          // ======================
                           // class ExecutionContext
                           // ======================
class ExecutionContext {
    // This class is an in-core, value-semantic type representing the context
    // of an external function execution.

  public:
    // TYPES
    typedef BloombergLP::bdld::Datum Datum;
    typedef BloombergLP::bslma::Allocator Allocator;

  private:
    // DATA
    Allocator    *d_allocator_p;
    const Datum  *d_args_p;
    int           d_numArgs;

  public:
    // CREATORS
    ExecutionContext(Allocator *allocator, const Datum *args, int numArgs);
        // Create a new 'ExecutionContext' having the specified 'allocator',
        // 'args', and 'numArgs'.  Note that 'args' may be 0 if '0 == numArgs'.

    ExecutionContext(const ExecutionContext&) = default;
    ExecutionContext& operator=(const ExecutionContext&) = default;

    // ACCESSORS
    Allocator *allocator() const;
        // Return the allocator associated with this object.

    const Datum *args() const;
        // Return the address of the first argument.

    int numArgs() const;
        // Return the number of argumetns.
};

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                           // ----------------------
                           // class ExecutionContext
                           // ----------------------

// CREATORS
inline
ExecutionContext::ExecutionContext(Allocator   *allocator,
                                   const Datum *args,
                                   int          numArgs)
: d_allocator_p(allocator)
, d_args_p(args)
, d_numArgs(numArgs) {
    BSLS_ASSERT(0 != allocator);
    BSLS_ASSERT(0 != args || 0 == numArgs);
}

// ACCESSORS
inline
BloombergLP::bslma::Allocator *ExecutionContext::allocator() const {
    return d_allocator_p;
}

inline
const BloombergLP::bdld::Datum *ExecutionContext::args() const {
    return d_args_p;
}

inline
int ExecutionContext::numArgs() const {
    return d_numArgs;
}
}
#endif
