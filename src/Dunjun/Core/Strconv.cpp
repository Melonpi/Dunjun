#include <Dunjun/Core/Strconv.hpp>

#include <cstdlib>

namespace Dunjun
{
namespace Strconv
{
String formatBool(bool b)
{
	if (b)
		return "true";
	return "false";
}

String formatFloat(f64 f, char format, int prec, int bitSize)
{
	// TODO(bill): finish formatFloat
	return "";
}

String formatInt(s64 i, int base)
{
	// TODO(bill): finish formatInt
	return "";
}

String formatUint(u64 u, int base)
{
	// TODO(bill): finish formatUint
	return "";
}

bool parseBool(String s, bool& b)
{
	if (s == "true" || s == "TRUE" || s == "1")
	{
		b = true;
		return true;
	}
	if (s == "false" || s == "FALSE" || s == "0")
	{
		b = false;
		return true;
	}

	return false;
}

bool parseFloat(String s, f32& f)
{
	f32 out;
	if (sscanf(cString(s), "%f", &out) != 1)
		return false;

	f = out;
	return true;
}

bool parseFloat(String s, f64& f)
{
	f64 out;
	if (sscanf(cString(s), "%g", &out) != 1)
		return false;

	f = out;
	return true;
}

bool parseInt(String s, int base, s8& i)
{
	// TODO(bill): handle base
	s8 out;
	if (sscanf(cString(s), "%d", &out) != 1)
		return false;

	i = out;
	return true;
}

bool parseInt(String s, int base, s16& i)
{
	// TODO(bill): handle base
	s16 out;
	if (sscanf(cString(s), "%d", &out) != 1)
		return false;

	i = out;
	return true;
}

bool parseInt(String s, int base, s32& i)
{
	// TODO(bill): handle base
	s32 out;
	if (sscanf(cString(s), "%d", &out) != 1)
		return false;

	i = out;
	return true;
}

bool parseInt(String s, int base, s64& i)
{
	// TODO(bill): handle base
	s64 out;
	if (sscanf(cString(s), "%lld", &out) != 1)
		return false;

	i = out;
	return true;
}

bool parseUint(String s, int base, u8& u)
{
	// TODO(bill): handle base
	u8 out;
	if (sscanf(cString(s), "%u", &out) != 1)
		return false;

	u = out;
	return true;
}

bool parseUint(String s, int base, u16& u)
{
	// TODO(bill): handle base
	u8 out;
	if (sscanf(cString(s), "%u", &out) != 1)
		return false;

	u = out;
	return true;
}

bool parseUint(String s, int base, u32& u)
{
	// TODO(bill): handle base
	u8 out;
	if (sscanf(cString(s), "%u", &out) != 1)
		return false;

	u = out;
	return true;
}

bool parseUint(String s, int base, u64& u)
{
	// TODO(bill): handle base
	u8 out;
	if (sscanf(cString(s), "%llu", &out) != 1)
		return false;

	u = out;
	return true;
}

} // namespace Strconv
} // namespace Dunjun
