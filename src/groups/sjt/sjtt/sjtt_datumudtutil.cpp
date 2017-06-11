// sjtu_datum.cpp
#include <sjtt_datumudtutil.h>

namespace sjtt {
using BloombergLP::bdld::Datum;

// CLASS DATA
const Datum DatumUdtUtil::s_Null = Datum::createNull();

const Datum DatumUdtUtil::s_Undefined =
                                Datum::createUdt(0, DatumUdtUtil::e_Undefined);
}
