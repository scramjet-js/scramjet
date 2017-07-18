// sjtt_bytecode.cpp
#include <sjtt_bytecode.h>

namespace sjtt {
BSLMF_ASSERT(bsl::is_trivially_copyable<Bytecode>::value);
BSLMF_ASSERT(bsl::is_trivially_default_constructible<Bytecode>::value);
BSLMF_ASSERT(BloombergLP::bslmf::IsBitwiseMoveable<Bytecode>::value);

// FREE OPERATORS
bool operator==(const Bytecode& lhs, const Bytecode& rhs) {
    if (lhs.d_opcode != rhs.d_opcode) {
        return false;                                                 // RETURN
    }
    typedef Bytecode BC;
    switch (lhs.d_opcode) {
      case BC::e_Allocate:
        return lhs.d_dest == rhs.d_dest;
      case BC::e_AllocateI32:
        return lhs.d_dest == rhs.d_dest;
      case BC::e_AllocateDouble:
        return lhs.d_dest == rhs.d_dest;
      case BC::e_Store:
        return lhs.d_x == rhs.d_x && lhs.d_y == rhs.d_y;
      case BC::e_StoreI32:
        return lhs.d_x == rhs.d_x && lhs.d_y == rhs.d_y;
      case BC::e_StoreDouble:
        return lhs.d_x == rhs.d_x && lhs.d_y == rhs.d_y;
      case BC::e_Load:
        return lhs.d_dest == rhs.d_dest && lhs.d_x == rhs.d_x;
      case BC::e_LoadI32:
        return lhs.d_dest == rhs.d_dest && lhs.d_x == rhs.d_x;
      case BC::e_LoadDouble:
        return lhs.d_dest == rhs.d_dest && lhs.d_x == rhs.d_x;
      case BC::e_EqI32:
        return lhs.d_dest == rhs.d_dest &&
            lhs.d_x == rhs.d_x &&
            lhs.d_y == rhs.d_y;
      case BC::e_Const:
        return lhs.d_dest == rhs.d_dest && lhs.d_value == rhs.d_value;
      case BC::e_ConstI32:
        return lhs.d_dest == rhs.d_dest && lhs.d_value == rhs.d_value;
      case BC::e_ConstDouble:
        return lhs.d_dest == rhs.d_dest && lhs.d_value == rhs.d_value;
      case BC::e_ExtractI32:
        return lhs.d_dest == rhs.d_dest && lhs.d_x == rhs.d_x;
      case BC::e_ExtractDouble:
        return lhs.d_dest == rhs.d_dest && lhs.d_x == rhs.d_x;
      case BC::e_Jump:
        return lhs.d_offset == rhs.d_offset;
      case BC::e_IfI32:
        return lhs.d_x == rhs.d_x && lhs.d_offset == rhs.d_offset;
      case BC::e_AddI32:
        return lhs.d_dest == rhs.d_dest &&
            lhs.d_x == rhs.d_x &&
            lhs.d_y == rhs.d_y;
      case BC::e_Call: {
        // Note that for 'e_Call' we test that the argument indices are
        // deeply-equal.

        if (lhs.d_dest != rhs.d_dest || lhs.d_function != rhs.d_function) {
            return false;
        }
        return 0 == ::memcmp(
                            lhs.d_args_p,
                            rhs.d_args_p,
                            sizeof(*lhs.d_args_p) * lhs.d_function.argCount());
      } break;
      case BC::e_Return:
        return lhs.d_x == rhs.d_x;
    }
}
}

