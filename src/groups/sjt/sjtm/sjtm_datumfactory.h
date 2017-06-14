#ifndef INCLUDED_SJTM_DATUMFACTORY
#define INCLUDED_SJTM_DATUMFACTORY

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

#ifndef INCLUDED_SJTT_DATUMUDTUTIL
#include <sjtt_datumudtutil.h>
#endif

namespace BloombergLP {
namespace bslma { class Allocator; }
}

namespace sjtm {
class DatumFactory {
    // This class is a mechanism that provides a syntactically-minimal way to
    // create 'Datum' objects.

  public:
    // TYPES
    typedef BloombergLP::bslma::Allocator Allocator;
    typedef BloombergLP::bdld::Datum Datum;

  private:
    // DATA
    Allocator *d_allocator_p;

    // CREATORS
    DatumFactory(const DatumFactory&) = delete;
    DatumFactory& operator=(const DatumFactory&) = delete;

  public:
    // CREATORS
    explicit DatumFactory(BloombergLP::bslma::Allocator *allocator);

    // ACCESSORS
    Allocator *allocator() const;
        // Return the allocator associated with this object.

    const Datum& u() const;
        // Return 'sjtt::DatumUdtUtil::s_Undefined'.

    const Datum& operator()() const;
        // Return 'sjtt::DatumUdtUtil::s_Null'.

    Datum operator()(bool value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(int value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(double value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(sjtt::DatumUdtUtil::ExternalFunction value) const;
        // Return a 'Datum' object having the specified 'value'.
};

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                             // ------------------
                             // class DatumFactory
                             // ------------------
// CREATORS
inline
DatumFactory::DatumFactory(BloombergLP::bslma::Allocator *allocator)
: d_allocator_p(allocator) {
    BSLS_ASSERT(0 != allocator);
}

// ACCESSORS
inline
BloombergLP::bslma::Allocator *DatumFactory::allocator() const {
    return d_allocator_p;
}

inline
const BloombergLP::bdld::Datum& DatumFactory::u() const {
    return sjtt::DatumUdtUtil::s_Undefined;
}

inline
const BloombergLP::bdld::Datum& DatumFactory::operator()() const {
    return sjtt::DatumUdtUtil::s_Null;
}

inline
BloombergLP::bdld::Datum DatumFactory::operator()(bool value) const
{
    return Datum::createBoolean(value);
}

inline
BloombergLP::bdld::Datum DatumFactory::operator()(int value) const {
    return Datum::createInteger(value);
}

inline
BloombergLP::bdld::Datum DatumFactory::operator()(double value) const {
    return Datum::createDouble(value);
}

inline
BloombergLP::bdld::Datum
DatumFactory::operator()(sjtt::DatumUdtUtil::ExternalFunction value) const {
    return sjtt::DatumUdtUtil::datumFromExternalFunction(value);
}
}

#endif /* INCLUDED_SJTM_DATUMFACTORY */
