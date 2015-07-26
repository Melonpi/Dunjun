#ifndef DUNJUN_CORE_STRCONV_HPP
#define DUNJUN_CORE_STRCONV_HPP

#include <Dunjun/Core/String.hpp>

namespace Dunjun
{
namespace Strconv
{
// TODO(bill): Documentation for Strconv

String formatBool(bool b);
// `formatFloat` converts the floating-point number f to a String
// The format `format` is on of these:
// 'b' == (-ddddp±ddd) binary exponent
// 'B' == ` ` `
// 'e' == (-d.dddde±dd) decimal exponent
// 'E' == ` ` `
// 'f' == (-ddd.ddd) no exponent
// 'F' == ` ` `
// 'g' == 'e' for large exponents
// 'G' == ` ` `
String formatFloat(f64 f, char format, int prec, int bitSize);

String formatInt(s64 i, int base);
String formatUint(u64 u, int base);

bool parseBool(String s, bool& b);

bool parseFloat(String s, f32& f);
bool parseFloat(String s, f64& f);

bool parseInt(String s, int base, s8& i);
bool parseInt(String s, int base, s16& i);
bool parseInt(String s, int base, s32& i);
bool parseInt(String s, int base, s64& i);

bool parseUint(String s, int base, u8& u);
bool parseUint(String s, int base, u16& u);
bool parseUint(String s, int base, u32& u);
bool parseUint(String s, int base, u64& u);

// TODO(bill): Add more string conversion functions


} // namespace Strconv
} // namespace Dunjun


#endif
