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
typedef BytecodeDSLUtil::FunctionMap FunctionMap;


short parseShort(const StringRef& data)
    // Return the integer stored in specified 'data', or a value less than zero
    // if no valid address can be found.
{

    short result;
    bsl::istringstream is(data);
    if (is >> result) {
        return result;
    }
    return -1;
}


int parseShorts(bsl::vector<short> *dest, const StringRef& data)
    // Parse, into the specified 'dest', the array of comma-separated shorts in
    // the specified 'data' and return 0 if successful.  Otherwise, return a
    // non-zero value.
{
    bsl::string next;
    bsl::istringstream is(data);
    while(bsl::getline(is, next, ',')) {
        const short nextShort = parseShort(next);
        if (0 > nextShort) {
            return nextShort;
        }
        dest->push_back(nextShort);
    }
    return 0;
}

int parseAllocate(Bytecode           *result,
                  bsl::string        *errorMessage,
                  bslma::Allocator   *alloc,
                  const StringRef&    data,
                  const FunctionMap&  functions)
{
    bsl::string datumError;
    const int dest = parseShort(data);
    if (0 > dest) {
        *errorMessage = "invalid dest";
        return -1;
    }
    *result = Bytecode::createAllocate(dest);
    return 0;
}

int parseAllocateI32(Bytecode           *result,
                     bsl::string        *errorMessage,
                     bslma::Allocator   *alloc,
                     const StringRef&    data,
                     const FunctionMap&  functions)
{
    bsl::string datumError;
    const int dest = parseShort(data);
    if (0 > dest) {
        *errorMessage = "invalid dest";
        return -1;
    }
    *result = Bytecode::createAllocateI32(dest);
    return 0;
}

int parseAllocateDouble(Bytecode           *result,
                        bsl::string        *errorMessage,
                        bslma::Allocator   *alloc,
                        const StringRef&    data,
                        const FunctionMap&  functions)
{
    const int dest = parseShort(data);
    if (0 > dest) {
        *errorMessage = "invalid dest";
        return -1;
    }
    *result = Bytecode::createAllocateDouble(dest);
    return 0;
}

int parseStore(Bytecode           *result,
               bsl::string        *errorMessage,
               bslma::Allocator   *alloc,
               const StringRef&    data,
               const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createStore(shorts[0], shorts[1]);
    return 0;
}

int parseStoreI32(Bytecode           *result,
                  bsl::string        *errorMessage,
                  bslma::Allocator   *alloc,
                  const StringRef&    data,
                  const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createStoreI32(shorts[0], shorts[1]);
    return 0;
}

int parseStoreDbl(Bytecode           *result,
                  bsl::string        *errorMessage,
                  bslma::Allocator   *alloc,
                  const StringRef&    data,
                  const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createStoreDouble(shorts[0], shorts[1]);
    return 0;
}

int parseLoad(Bytecode           *result,
              bsl::string        *errorMessage,
              bslma::Allocator   *alloc,
              const StringRef&    data,
              const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createLoad(shorts[0], shorts[1]);
    return 0;
}

int parseLoadI32(Bytecode           *result,
                 bsl::string        *errorMessage,
                 bslma::Allocator   *alloc,
                 const StringRef&    data,
                 const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createLoadI32(shorts[0], shorts[1]);
    return 0;
}

int parseLoadDbl(Bytecode           *result,
                 bsl::string        *errorMessage,
                 bslma::Allocator   *alloc,
                 const StringRef&    data,
                 const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 2 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createLoadDouble(shorts[0], shorts[1]);
    return 0;
}

int parseEqI32(Bytecode           *result,
               bsl::string        *errorMessage,
               bslma::Allocator   *alloc,
               const StringRef&    data,
               const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 3 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createEqI32(shorts[0], shorts[1], shorts[2]);
    return 0;
}

int parseConst(Bytecode           *result,
               bsl::string        *errorMessage,
               bslma::Allocator   *alloc,
               const StringRef&    data,
               const FunctionMap&  functions)
{
    bsl::vector<short> shorts;
    if (0 != parseShorts(&shorts, data) || 3 != shorts.size()) {
        *errorMessage = "invalid arguments";
        return -1;
    }
    *result = Bytecode::createEqI32(shorts[0], shorts[1], shorts[2]);
    return 0;
}

typedef int (*ParserFunction)(Bytecode *,
                              bsl::string *,
                              bslma::Allocator *,
                              const StringRef&,
                              const FunctionMap&);


// Note that the entries in 's_Parsers' must be ordered s.t. most exact matches
// come first.  For example, if 'Ai' and 'A' are both valid opcode names, 'Ai'
// must come first.  Otherwise, 'A' will match 'Ai'.
const struct ParserEntry {
    const char     *code;
    ParserFunction  parser;
} s_Parsers[] = {
    { "Ad", parseAllocateDouble },
    { "Ai", parseAllocateI32 },
    { "A", parseAllocate },
    { "Si", parseStoreI32 },
    { "Sd", parseStoreDbl },
    { "S", parseStore },
    { "Li", parseLoadI32 },
    { "Ld", parseLoadDbl },
    { "L", parseLoad },
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
                               const StringRef&                 source) {
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
      default: {
        *errorMessage = "unknown datum type '";
        *errorMessage += source[0];
        *errorMessage += '\'';
        return -1;                                                    // RETURN
      } break;
    }
    return 0;
}

int BytecodeDSLUtil::readDSL(bsl::vector<sjtt::Bytecode> *result,
                             bsl::string                 *errorMessage,
                             const StringRef&             dsl,
                             const FunctionMap&           functions) {
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
