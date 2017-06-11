// sjtt_frame.cpp
#include <sjtt_frame.h>

#include <sjtt_datumudtutil.h>

namespace sjtt {
const BloombergLP::bdld::Datum Frame::s_DefaultLocals[Bytecode::s_NumLocals]
    = {
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
    DatumUdtUtil::s_Undefined,
};
}
