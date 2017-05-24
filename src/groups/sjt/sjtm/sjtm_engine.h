#ifndef INCLUDED_SJTM_ENGINE
#define INCLUDED_SJTM_ENGINE

#ifndef INCLUDED_BDLD_DATUM
#include <bdld_datum.h>
#endif

#ifndef INCLUDED_BDLD_MANAGEDDATUM
#include <bdld_manageddatum.h>
#endif

#ifndef INCLUDED_BSL_STRING
#include <bsl_string.h>
#endif

#ifndef INCLUDED_BSL_UNORDERED_MAP
#include <bsl_unordered_map.h>
#endif

namespace BloombergLP {
namespace bslma { class Allocator; }
}

namespace sjtm {
class Engine {

    // ACCESSORS

    bsl::unordered_map<bsl::string, BloombergLP::bdld::ManagedDatum> d_globals;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

  public:
    // CREATORS

    explicit Engine(BloombergLP::bslma::Allocator *allocator);

    ~Engine();

    // MANIPULATORS

    void setGlobal(const BloombergLP::bslstl::StringRef& name,
                   const BloombergLP::bdld::Datum& value);
};
}

#endif /* INCLUDED_SJTM_ENGINE */
