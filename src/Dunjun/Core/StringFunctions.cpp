#include <Dunjun/Core/StringFunctions.hpp>

#include <cassert>
#include <cstring>

namespace Dunjun
{
namespace Strings
{
namespace
{
const u32 PrimeRK = 16777619;
} // namespace (anonymous)

// `hashStr` returns the hash and the appropriate multiplicative factor
// for use in the Rabin-Karp algorithm
INTERNAL void hashStr(const String& sep, u32& hash, u32& power)
{
	hash = 0;

	const usize lsep = len(sep);
	for (usize i = 0; i < lsep; i++)
		hash = hash * PrimeRK + (u32)sep[i];

	power = 1;

	u32 sq = PrimeRK;
	for (usize i = lsep; i > 0; i >>= 1)
	{
		if ((i & 1) != 0)
			power *= sq;
		sq *= sq;
	}
}

// `hashStrRev` returns the hash of the reverse of sep and the appropriate
// multiplicative factor for use in Rabin-Karp algorithm.
INTERNAL void hashStrRev(const String& sep, u32& hash, u32& power)
{
	hash = 0;

	const usize lsep = len(sep);
	for (usize i = lsep - 1; i >= 0; i--)
		hash = hash * PrimeRK + (u32)sep[i];

	power = 1;

	u32 sq = PrimeRK;
	for (usize i = lsep; i > 0; i >>= 1)
	{
		if ((i & 1) != 0)
			power *= sq;
		sq *= sq;
	}
}

bool contains(const String& s, const String& substr)
{
	return index(s, substr) >= 0;
}

bool containsAny(const String& s, const String& chars)
{
	return indexAny(s, chars) >= 0;
}

bool hasPrefix(const String& s, const String& prefix)
{
	return len(s) >= len(prefix) && substring(s, 0, len(prefix)) == prefix;
}

bool hasSuffix(const String& s, const String& suffix)
{
	return len(s) >= len(suffix) &&
	       substring(s, len(s) - len(suffix), len(s)) == suffix;
}

INTERNAL char* copyStringToCString(const String& s, Allocator& a)
{
	const char* cStr = cString(s);
	const usize l    = len(s) + 1;
	char* out = (char*)a.allocate(l);
	memcpy(out, cStr, l);
	return out;
}

String join(const Array<char*>& a, const String& sep)
{
	const ssize aLen = len(a);
	if (aLen == 0)
		return "";

	if (aLen == 1)
		return a[0];

	ssize n = len(sep) * (aLen - 1);
	for (ssize i = 0; i < aLen; i++)
		n += len(a[i]);

	String out = a[0];

	for (ssize i = 1; i < aLen; i++)
	{
		append(out, sep);
		append(out, a[i]);
	}

	return out;
}

void split(const String& s, const String& sep, Array<char*>& out)
{
	Allocator& a = out.allocator;

	if (sep == "")
	{
		const usize n = len(s);

		resize(out, n);

		for (usize i = 0; i < n; i++)
			out[i] = copyStringToCString(s[i], a);

		return;
	}

	const usize n = count(s, sep) + 1;
	const char c  = sep[0];

	usize start = 0;
	usize na = 0;
	resize(out, n);

	const usize lsep = len(sep);
	const usize ls   = len(s);

	for (usize i = 0; i + lsep <= ls && na + 1 < n; i++)
	{
		if (s[i] == c && (lsep == 1 || substring(s, i, i + lsep) == sep))
		{
			out[na] = copyStringToCString(substring(s, start, i), a);
			na++;
			start = i + lsep;
			i += lsep - 1;
		}
	}

	out[na] = copyStringToCString(substring(s, start, ls), a);

	resize(out, na + 1);
}

void deallocateCStringArray(Array<char*>& array)
{
	Allocator& a = array.allocator;
	for (char* s : array)
		a.deallocate(s);
}

String toLower(const String& s)
{
	String out = s;

	const usize ls = len(s);
	for (usize i = 0; i < ls; i++)
		out[i] = toLower(s[i]);

	return out;
}

char toLower(const char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';
	return c;
}

String toTitle(const String& s)
{

	const usize ls = len(s);
	if (ls == 0)
		return s;

	String out = s;

	usize i = 0;
	while (isSpace(out[i]))
		i++;

	out[i] = toUpper(out[i]);
	i++;

	// TODO(bill): Only upper the first of words that are a certain length (3-4)
	// TODO(bill): Always captialize the first and last words
	for (; i < ls; i++)
	{
		// NOTE(bill): If the previous character is a space, then capitalize the
		// current one
		if (isSpace(out[i - 1]))
			out[i] = toUpper(out[i]);
	}

	return out;
}

String toUpper(const String& s)
{
	String out = s;

	const usize ls = len(s);
	for (usize i = 0; i < ls; i++)
		out[i] = toUpper(s[i]);

	return out;
}

char toUpper(const char c)
{
	if (c >= 'a' && c <= 'z')
		return c + 'A' - 'a';

	return c;
}

bool isSpace(const char c)
{
	const char* whiteSpace = " \t\n\v\f\r";
	for (usize i = 0; i < 6; i++)
	{
		if (c == whiteSpace[i])
			return true;
	}
	return false;
}

bool isAlpha(const char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(const char c) { return (c >= '0' && c <= '9'); }

bool isAlphaNumeric(const char c) { return isAlpha(c) || isDigit(c); }

String trimLeft(const String& s, const String& cutset)
{
	// TODO(bill): Make more efficient
	const ssize slen = len(s);
	const usize clen = len(cutset);
	if (len(s) == 0)
		return s;
	ssize pos = -1;
	bool t    = false;

	for (ssize i = 0; i < slen; i++)
	{
		for (usize j = 0; j < clen; j++)
		{
			if (s[i] == cutset[j])
			{
				t   = true;
				pos = i;
			}
		}
		if (!t)
			break;
		t = false;
	}

	if (pos != -1)
		return substring(s, pos + 1, slen);
	return s;
}

String trimRight(const String& s, const String& cutset)
{
	// TODO(bill): Make more efficient
	const usize slen = len(s);
	const usize clen = len(cutset);
	if (slen == 0)
		return s;
	ssize pos = -1;
	bool t    = false;

	for (usize i = slen - 1; i >= 0; i--)
	{
		for (usize j = 0; j < clen; j++)
		{
			if (s[i] == cutset[j])
			{
				t   = true;
				pos = i;
			}
		}
		if (!t)
			break;
		t = false;
	}

	if (pos != -1)
		return substring(s, 0, pos);
	return s;
}

String trim(const String& s, const String& cutset)
{
	return trimLeft(trimRight(s, cutset), cutset);
}

String trimSpace(const String& s) { return trim(s, " \t\n\v\f\r"); }

usize count(const String& s, const String& sep)
{
	usize n = 0;

	const usize ls   = len(s);
	const usize lsep = len(sep);

	if (lsep == 0)
		return ls + 1;
	if (lsep == 1)
	{
		char c = sep[0];
		for (usize i = 0; i < ls; i++)
		{
			if (s[i] == c)
				n++;
		}

		return n;
	}
	if (lsep > ls)
		return 0;
	if (lsep == ls)
	{
		if (sep == s)
			return 1;
		return 0;
	}

	// Rabin-Karp search
	u32 hashsep, power;
	hashStr(sep, hashsep, power);

	u32 h = 0;
	for (usize i = 0; i < lsep; i++)
		h = h * PrimeRK + (u32)s[i];

	usize lastMatch = 0;
	if (h == hashsep && substring(sep, 0, lsep) == sep)
	{
		n++;
		lastMatch = lsep;
	}

	for (usize i = lsep; i < ls; /**/)
	{
		h *= PrimeRK;
		h += (u32)s[i];
		h -= power * (u32)s[i - lsep];
		i++;
		if (h == hashsep &&          //
		    lastMatch <= i - lsep && //
		    substring(s, i - lsep, i) == sep)
		{
			n++;
			lastMatch = i;
		}
	}

	return n;
}

ssize index(const String& s, const String& sep)
{
	const usize n  = len(sep);
	const usize sl = len(s);

	if (n == 0)
		return 0;
	if (n == 1)
	{
		const char c = sep[0];
		for (usize i = 0; i < sl; i++)
		{
			if (s[i] == c)
				return (ssize)i;
		}
		return -1;
	}
	if (n == sl)
	{
		if (sep == s)
			return 0;
		return -1;
	}
	if (n > sl)
		return -1;

	// Search using Rabin-Karp
	u32 hashsep, power;
	hashStr(sep, hashsep, power);
	u32 h = 0;
	for (usize i = 0; i < n; i++)
	{
		h = h * PrimeRK + (u32)s[i];
	}
	if (h == hashsep && substring(s, 0, n) == sep)
	{
		return 0;
	}
	for (usize i = n; i < sl; /**/)
	{
		h *= PrimeRK;
		h += (u32)s[i];
		h -= power * (u32)s[i - n];
		i++;
		if (h == hashsep && substring(s, i - n, i) == sep)
			return i - n;
	}

	return -1;
}

ssize indexAny(const String& s, const String& chars)
{
	const usize cLen = len(chars);
	const usize sLen = len(s);

	if (cLen > 0)
	{
		for (usize i = 0; i < sLen; i++)
		{
			for (usize j = 0; j < cLen; j++)
			{
				if (s[i] == chars[j])
					return (ssize)i;
			}
		}
	}

	return -1;
}

ssize lastIndex(const String& s, const String& sep)
{
	usize n = len(sep);

	if (n == 0)
		return len(s);
	if (n == 1)
	{
		char c = sep[0];
		for (usize i = len(s) - 1; i >= 0; i--)
		{
			if (s[i] == c)
				return i;
		}
		return -1;
	}
	if (n == len(s))
	{
		if (sep == s)
			return 0;
		return -1;
	}
	if (n > len(s))
		return -1;

	u32 hashsep, power;
	hashStrRev(sep, hashsep, power);
	usize last = len(s) - n;

	u32 h = 0;
	for (usize i = len(s) - 1; i >= last; i--)
		h = h * PrimeRK + (u32)s[i];

	if (h == hashsep && substring(s, last, len(s)) == sep)
		return last;

	for (usize i = last - 1; i >= 0; i--)
	{
		h *= PrimeRK;
		h += (u32)s[i];
		h -= power * (u32)s[i + n];
		if (h == hashsep && substring(s, i, i + n) == sep)
			return i;
	}

	return -1;
}

ssize lastIndexAny(const String& s, const String& chars)
{
	if (len(chars) > 0)
	{
		for (usize i = len(s) - 1; i >= 0; i--)
		{
			for (usize j = 0; j < len(chars); j++)
			{
				if (s[i] == chars[j])
					return i;
			}
		}
	}

	return -1;
}

String repeat(const String& s, usize count)
{
	String out;
	reserve(out, count * len(s));

	for (usize i = 0; i < count; i++)
		append(out, s);

	return out;
}

String
replace(const String& s, const String& oldStr, const String& newStr, ssize n)
{
	if (oldStr == newStr || n == 0)
		return s;

	// TODO(bill)

	return s;
}
} // namespace Strings
} // namespace Dunjun
