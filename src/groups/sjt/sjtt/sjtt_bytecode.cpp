// sjtt_bytecode.cpp
#include <sjtt_bytecode.h>

namespace sjtt {
BSLMF_ASSERT(bsl::is_trivially_copyable<Bytecode>::value);
BSLMF_ASSERT(bsl::is_trivially_default_constructible<Bytecode>::value);
BSLMF_ASSERT(BloombergLP::bslmf::IsBitwiseMoveable<Bytecode>::value);
}

