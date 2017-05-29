// sjtu_datum.cpp
#include <sjtu_datumutil.h>

namespace sjtu {
using BloombergLP::bdld::Datum;

// CLASS DATA
const Datum DatumUtil::s_Null = Datum::createNull();

const Datum DatumUtil::s_Undefined =
                                   Datum::createUdt(0, DatumUtil::e_Undefined);
}
