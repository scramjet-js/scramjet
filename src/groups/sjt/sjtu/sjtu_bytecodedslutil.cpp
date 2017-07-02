// sjtu_bytecodedslutil
#include <sjtu_bytecodedslutil.h>

#include <algorithm>

#include <bdlb_stringrefutil.h>

#include <bslstl_ostringstream.h>
#include <bslstl_istringstream.h>

using namespace BloombergLP;

namespace sjtu {
namespace {

using bdld::Datum;
using bslstl::StringRef;
using sjtt::Bytecode;
typedef BytecodeDSLUtil::FunctionNameToAddressMap FunctionNameToAddressMap;


int parseInt(const StringRef& data) {
    // Return the integer stored in specified 'data', or a value less than zero
    // if no valid address can be found.

    int result;
    bsl::istringstream is(data);
    if (is >> result) {
        return result;
    }
    return -1;
}

int parsePush(Bytecode                        *result,
              bsl::string                     *errorMessage,
              bslma::Allocator                *alloc,
              const StringRef&                 data,
              const FunctionNameToAddressMap&  functions)
{
    bsl::string datumError;
    Datum value;
    const int ret =
       BytecodeDSLUtil::readDatum(&value, &datumError, alloc, data, functions);
    if (0 > ret) {
        *errorMessage = "invalid datum";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Push, value);
    return 0;
}

int parseLoad(Bytecode                        *result,
              bsl::string                     *errorMessage,
              bslma::Allocator                *alloc,
              const StringRef&                 data,
              const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Load,
                                     Datum::createInteger(addr));
    return 0;
}

int parseStore(Bytecode                        *result,
               bsl::string                     *errorMessage,
               bslma::Allocator                *alloc,
               const StringRef&                 data,
               const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Store,
                                     Datum::createInteger(addr));
    return 0;
}

int parseJump(Bytecode                        *result,
              bsl::string                     *errorMessage,
              bslma::Allocator                *alloc,
              const StringRef&                 data,
              const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Jump,
                                     Datum::createInteger(addr));
    return 0;
}

int parseIf(Bytecode                        *result,
            bsl::string                     *errorMessage,
            bslma::Allocator                *alloc,
            const StringRef&                 data,
            const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_If,
                                     Datum::createInteger(addr));
    return 0;
}

int parseIfIntsEq(Bytecode                        *result,
                  bsl::string                     *errorMessage,
                  bslma::Allocator                *alloc,
                  const StringRef&                 data,
                  const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_IfEqInts,
                                     Datum::createInteger(addr));
    return 0;
}

int parseEqInts(Bytecode                        *result,
                bsl::string                     *errorMessage,
                bslma::Allocator                *alloc,
                const StringRef&                 data,
                const FunctionNameToAddressMap&  functions)
{
    if (!data.empty()) {
        *errorMessage = "trailing data";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_EqInts);
    return 0;
}

int parseIncInt(Bytecode                        *result,
                bsl::string                     *errorMessage,
                bslma::Allocator                *alloc,
                const StringRef&                 data,
                const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_IncInt,
                                     Datum::createInteger(addr));
    return 0;
}

int parseAddDoubles(Bytecode                        *result,
                    bsl::string                     *errorMessage,
                    bslma::Allocator                *alloc,
                    const StringRef&                 data,
                    const FunctionNameToAddressMap&  functions)
{
    if (!data.empty()) {
        *errorMessage = "trailing data";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_AddDoubles);
    return 0;
}

int parseAddInts(Bytecode                        *result,
                 bsl::string                     *errorMessage,
                 bslma::Allocator                *alloc,
                 const StringRef&                 data,
                 const FunctionNameToAddressMap&  functions)
{
    if (!data.empty()) {
        *errorMessage = "trailing data";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_AddInts);
    return 0;
}

int parseCall(Bytecode                        *result,
              bsl::string                     *errorMessage,
              bslma::Allocator                *alloc,
              const StringRef&                 data,
              const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Call,
                                     Datum::createInteger(addr));
    return 0;
}

int parseExecute(Bytecode                        *result,
                 bsl::string                     *errorMessage,
                 bslma::Allocator                *alloc,
                 const StringRef&                 data,
                 const FunctionNameToAddressMap&  functions)
{
    if (!data.empty()) {
        *errorMessage = "trailing data";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Execute);
    return 0;
}

int parseExit(Bytecode                        *result,
              bsl::string                     *errorMessage,
              bslma::Allocator                *alloc,
              const StringRef&                 data,
              const FunctionNameToAddressMap&  functions)
{
    if (!data.empty()) {
        *errorMessage = "trailing data";
        return -1;
    }
    *result = Bytecode::createOpcode(sjtt::Bytecode::e_Exit);
    return 0;
}

int parseResize(Bytecode                        *result,
                bsl::string                     *errorMessage,
                bslma::Allocator                *alloc,
                const StringRef&                 data,
                const FunctionNameToAddressMap&  functions)
{
    const int addr = parseInt(data);
    if (0 > addr) {
        *errorMessage = "invalid index";
        return -1;
    }
    *result = Bytecode::createOpcode(Bytecode::e_Resize,
                                     Datum::createInteger(addr));
    return 0;
}

typedef int (*ParserFunction)(Bytecode *,
                              bsl::string *,
                              bslma::Allocator *,
                              const StringRef&,
                              const FunctionNameToAddressMap&);


const struct ParserEntry {
    const char     *code;
    ParserFunction  parser;
} s_Parsers[] = {
    { "P", parsePush },
    { "L", parseLoad },
    { "S", parseStore },
    { "J", parseJump },
    { "I=i", parseIfIntsEq },
    { "=i", parseEqInts },
    { "I", parseIf },
    { "++i", parseIncInt },
    { "+d", parseAddDoubles },
    { "+i", parseAddInts },
    { "C", parseCall },
    { "E", parseExecute },
    { "X", parseExit },
    { "V", parseResize },
};

int findParser(StringRef* data)
    // Return the index of the parser that matches the specified 'data', or
    // a negative value if there is no such parser.  If a match is found, set
    // 'data' to be the remainder of the text.
{
    for (int i = 0; i != sizeof(s_Parsers) / sizeof(s_Parsers[0]); ++i) {
        const ParserEntry& e = s_Parsers[i];
        const StringRef match = bdlb::StringRefUtil::strstr(*data, e.code);
        if(match == e.code && match.data() == data->data()) {
            *data = StringRef(data->begin() + match.size(), data->end());
            return i;
        }
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
        *result = sjtd::DatumUdtUtil::datumFromExternalFunction(i->second);
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
            return -1;                                                // RETURN
        }
        const int pos = (next - dsl.begin());
        StringRef data = StringRef(next, end);
        const int parserIndex = findParser(&data);
        if (0 > parserIndex) {
            bsl::ostringstream txt;
            txt << "invalid opcode at position: " << pos << " -- '" << data
                << "'";
            *errorMessage = txt.str();
            return -1;                                                // RETURN
        }
        const ParserEntry& entry = s_Parsers[parserIndex];
        bsl::string parserError;
        Bytecode code;
        const int res =
                     entry.parser(&code, &parserError, alloc, data, functions);
        if (0 != res) {
            bsl::ostringstream txt;
            txt << "failed to parse code '" << entry.code << "' from '"
                << data << "' at position: " << pos << " -- "
                << parserError;
            *errorMessage = txt.str();
            return -1;                                                // RETURN
        }
        result->push_back(code);
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
