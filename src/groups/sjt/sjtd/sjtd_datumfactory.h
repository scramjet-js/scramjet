#ifndef INCLUDED_SJTD_DATUMFACTORY
#define INCLUDED_SJTD_DATUMFACTORY

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

#ifndef INCLUDED_SJTD_DATUMUDTUTIL
#include <sjtd_datumudtutil.h>
#endif

namespace BloombergLP {
namespace bslma { class Allocator; }
}

namespace sjtd {
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
        // Return 'sjtd::DatumUdtUtil::s_Undefined'.

    const Datum& operator()() const;
        // Return 'sjtd::DatumUdtUtil::s_Null'.

    Datum operator()(bool value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(int value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(double value) const;
        // Return a 'Datum' object having the specified 'value'.

    Datum operator()(sjtd::DatumUdtUtil::ExternalFunction value) const;
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
    return sjtd::DatumUdtUtil::s_Undefined;
}

inline
const BloombergLP::bdld::Datum& DatumFactory::operator()() const {
    return sjtd::DatumUdtUtil::s_Null;
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
DatumFactory::operator()(sjtd::DatumUdtUtil::ExternalFunction value) const {
    return sjtd::DatumUdtUtil::datumFromExternalFunction(value);
}
}

#endif /* INCLUDED_SJTD_DATUMFACTORY */
