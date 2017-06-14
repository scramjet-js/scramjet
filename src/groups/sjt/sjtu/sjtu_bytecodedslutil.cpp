// sjtu_bytecodedslutil
#include <sjtu_bytecodedslutil.h>

#include <algorithm>

#include <bslstl_ostringstream.h>
#include <bslstl_istringstream.h>

using namespace BloombergLP;

namespace sjtu {
namespace {

int parseInt(const char *begin, const char *end) {
    // Return the integer stored in specified range '[begin, end)', or a value
    // less than zero if no valid address can be found.

    int result;
    bsl::istringstream is(bslstl::StringRef(begin, end));
    if (is >> result) {
        return result;
    }
    return -1;
}

int parseTwoInts(int        *firstResult,
                 int        *secondResult,
                 const char *begin,
                 const char *end) {
    // Return the integer values stored in the string in the specified
    // '[begin, end)' range, load the first into the specified 'firstResult'
    // and the second into the specified 'secondResult', and return 0.  If the
    // string does not have the form '<int>,<int>', return a non-zero value.

    char sep;
    bsl::istringstream is(bslstl::StringRef(begin, end));
    if (is >> *firstResult && is >> sep && is >> *secondResult && ',' == sep) {
        return 0;
    }
    return -1;
}
}

int BytecodeDSLUtil::readDatum(Datum                           *result,
                               bsl::string                     *errorMessage,
                               Allocator                       *allocator,
                               const StringRef&                 source,
                               const FunctionNameToAddressMap&  functions) {
    if (0 == source.length()) {
        *errorMessage = "empty datum";
        return -1;                                                    // RETURN
    }
    if ("T" == source) {
        *result = bdld::Datum::createBoolean(true);
        return 0;                                                     // RETURN
    }
    if ("F" == source) {
        *result = bdld::Datum::createBoolean(false);
        return 0;                                                     // RETURN
    }
    switch (source[0]) {
      case 'd': {
        const StringRef input(source.begin() + 1, source.end());
        bsl::istringstream s(input);
        double d;
        if (s >> d) {
            *result = Datum::createDouble(d);
        }
        else {
            *errorMessage = "unable to parse double from '";
            *errorMessage += input;
            *errorMessage += '\'';
            return -1;                                                // RETURN
        }
      } break;
      case 'i': {
        const StringRef input(source.begin() + 1, source.end());
        bsl::istringstream s(input);
        int i;
        if (s >> i) {
            *result = Datum::createInteger(i);
        }
        else {
            *errorMessage = "unable to parse integer from '";
            *errorMessage += input;
            *errorMessage += '\'';
            return -1;                                                // RETURN
        }
      } break;
      case 'e': {
        const bsl::string name(source.begin() + 1, source.end());
        FunctionNameToAddressMap::const_iterator i = functions.find(name);
        if (functions.end() == i) {
            *errorMessage = "unknown function name '" + name + "'";
            return -1;                                                // RETURN
        }
        *result = sjtt::DatumUdtUtil::datumFromExternalFunction(i->second);
      } break;
      default: {
        *errorMessage = "unknown datum type '";
        *errorMessage += source[0];
        *errorMessage += '\'';
        return -1;                                                    // RETURN
      } break;
    }
    return 0;
}

int BytecodeDSLUtil::readDSL(bsl::vector<sjtt::Bytecode>     *result,
                             bsl::string                     *errorMessage,
                             const StringRef&                 dsl,
                             const FunctionNameToAddressMap&  functions) {
    Allocator *alloc = result->get_allocator().mechanism();
    const char *next = dsl.begin();
    while (next != dsl.end()) {
        const char *end = std::find(next, dsl.end(), '|');
        if (end == next) {
            bsl::ostringstream txt;
            txt << "empty bytecode beginning at position: "
                << (next - dsl.begin());
            *errorMessage = txt.str();
            return -1;
        }
        const int pos = (next - dsl.begin());
        const char code = *next++;
        switch (code) {
          case 'P': {
            bsl::string datumError;
            Datum value;
            const int ret = readDatum(&value,
                                      &datumError,
                                      alloc,
                                      StringRef(next, end),
                                      functions);
            if (0 == ret) {
                result->push_back(sjtt::Bytecode::createOpcode(
                                                        sjtt::Bytecode::e_Push,
                                                        value));
            }
            else {
                bsl::ostringstream txt;
                txt << "invalid datum for push at position: " << pos
                    << " -- " << datumError;
                *errorMessage = txt.str();
                return ret;                                           // RETURN
            }
          } break;
          case 'L': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for load at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_Load,
                                                  Datum::createInteger(addr)));
          } break;
          case 'S': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for store at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_Store,
                                                  Datum::createInteger(addr)));
          } break;
          case 'J': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for jump at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_Jump,
                                                  Datum::createInteger(addr)));
          } break;
          case 'I': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for if at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_If,
                                                  Datum::createInteger(addr)));
          } break;
          case '+': {
              bool good = true;
              sjtt::Bytecode::Opcode code;
              if ((end - next) == 1) {
                switch (*next) {
                  case 'd': {
                    code = sjtt::Bytecode::e_AddDoubles;
                  } break;
                  default: {
                    good = false;
                  } break;
                }
              }
              else {
                  good = false;
              }
              if (!good) {
                  bsl::ostringstream txt;
                  txt << "invalid add in position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              else {
                  result->push_back(sjtt::Bytecode::createOpcode(code));
              }
          } break;
          case 'C': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for call at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_Call,
                                                  Datum::createInteger(addr)));
          } break;
          case 'E': {
              if (next != end) {
                  bsl::ostringstream txt;
                  txt << "invalid execute at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                   sjtt::Bytecode::e_Execute));

          } break;
          case 'X': {
              if (next != end) {
                  bsl::ostringstream txt;
                  txt << "invalid exit at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                      sjtt::Bytecode::e_Exit));

          } break;
          case 'V': {
              const int addr = parseInt(next, dsl.end());
              if (0 > addr) {
                  bsl::ostringstream txt;
                  txt << "invalid index for reserve at position: " << pos;
                  *errorMessage = txt.str();
                  return -1;                                          // RETURN
              }
              result->push_back(sjtt::Bytecode::createOpcode(
                                                  sjtt::Bytecode::e_Reserve,
                                                  Datum::createInteger(addr)));
          } break;
          default: {
            bsl::ostringstream txt;
            txt << "invalid opcode '" << code << "' at position: " << pos;
            *errorMessage = txt.str();
            return -1;
          } break;
        }
        next = end;
        if (dsl.end() != next) {
            // If 'next' isn't the end, it points to '|' and we need to
            // increment it.

            ++next;
        }
    }
    return 0;
}
}
