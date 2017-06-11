#include <sjtu_bytecodedslutil.h>
#include <sjtu_interpretutil.h>

#include <bdlma_sequentialallocator.h>

#include <bsl_iostream.h>

using namespace BloombergLP;

namespace {
void printUsage() {
    bsl::cerr << "Usage:\n"
        << "evalbytecode <bytecode DSL>\n\n"
        << "Where <bytecode> is described in 'sjtu_bytedslutil.h', print the "
        << "result.\n";
}
}

int main(int argc, char* argv[]) {
    if (2 != argc) {
        printUsage();
        return 1;
    }
    bdlma::SequentialAllocator alloc;
    bsl::vector<sjtt::Bytecode> codes(&alloc);
    bsl::string errorMessage;
    sjtu::BytecodeDSLUtil::FunctionNameToAddressMap functions;
    const int ret = sjtu::BytecodeDSLUtil::readDSL(&codes,
                                                   &errorMessage,
                                                   argv[1],
                                                   functions);
    if (0 != ret) {
        bsl::cerr << errorMessage << "\n\n";
        printUsage();
        return 1;
    }
    const bdld::Datum value = sjtu::InterpretUtil::interpretBytecode(
                                                                    &alloc,
                                                                    &codes[0]);
    bsl::cout << value << '\n';
    return 0;
}
