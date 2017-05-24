#include <bdld_datum.h>

#include <bslma_default.h>

#include <sjtm_engine.h>

#include <iostream>

int main(int argc, char* argv[]) {
    using namespace BloombergLP;
    bslma::Allocator *alloc = bslma::Default::defaultAllocator();
    sjtm::Engine engine(alloc);
    engine.setGlobal("foo", bdld::Datum::createNull());
    std::cout << "Hello world!\n";
    return 0;
}
