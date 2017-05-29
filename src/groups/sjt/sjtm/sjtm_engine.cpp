#include <sjtm_engine.h>

namespace sjtm {

Engine::Engine(BloombergLP::bslma::Allocator *allocator)
    : d_globals(allocator) {
}

Engine::~Engine() {
}

void Engine::setGlobal(const BloombergLP::bslstl::StringRef& name,
                       const BloombergLP::bdld::Datum&       value) {
    d_globals[name].clone(value);
}

const BloombergLP::bdld::Datum&
Engine::getGlobal(const BloombergLP::bslstl::StringRef& name) {
    return d_globals[name].datum();
}
}
