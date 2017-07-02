// sjtd_datum.cpp
#include <sjtd_datumudtutil.h>

namespace sjtd {
using BloombergLP::bdld::Datum;

// CLASS DATA
const Datum DatumUdtUtil::s_Null = Datum::createNull();

const Datum DatumUdtUtil::s_Undefined =
                                Datum::createUdt(0, DatumUdtUtil::e_Undefined);
}
