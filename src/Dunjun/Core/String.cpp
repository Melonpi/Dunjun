#include <Dunjun/Core/String.hpp>

#include <Dunjun/Core/Array.hpp>

#include <cstring> // memcpy

namespace Dunjun
{
String::String(Allocator& a)
: data(a)
{
}

String::String(char c, Allocator& a)
: data(a)
{
	append(data, c);
}

String::String(const char* s, Allocator& a)
: data(a)
{
	append(data, s, len(s));
}

String::String(const char* s, usize length, Allocator& a)
: data(a)
{
	append(data, s, length);
}


String::String(const String& s)
: data(s.data)
{
}

String& String::operator=(const String& s)
{
	data = s.data;
	return *this;
}

String& String::operator=(const char c)
{
	clear(data);
	append(data, c);
	return *this;
}

String& String::operator=(const char* str)
{
	clear(data);
	append(data, str, len(str));
	return *this;
}

usize len(const String& str) { return len(str.data); }

usize cap(const String& str) { return cap(str.data); }

usize append(String& str, char c) { return append(str.data, c); }

usize append(String& str, const char* cStr)
{
	return append(str.data, cStr, len(cStr));
}

usize append(String& str, const String& other)
{
	return append(str.data, &other.data[0], len(other.data));
}

char* begin(String& str) { return begin(str.data); }

const char* begin(const String& str) { return begin(str.data); }

char* end(String& str) { return end(str.data); }

const char* end(const String& str) { return end(str.data); }

char& front(String& str) { return front(str.data); }

const char& front(const String& str) { return front(str.data); }

char& back(String& str) { return back(str.data); }

const char& back(const String& str) { return back(str.data); }

const char* cString(const String& str)
{
	// NOTE(bill): use const correct cast here
	// NOTE(bill): C style string must be zero terminated
	append(const_cast<Array<char>&>(str.data), '\0');
	popBack(const_cast<Array<char>&>(str.data));
	return begin(str);
}

void clear(String& str) { clear(str.data); }

void shrinkToFit(String& str) { shrinkToFit(str.data); }

void resize(String& str, usize length) { resize(str.data, length); }

void reserve(String& str, usize capacity) { reserve(str.data, capacity); }

String substring(const String& s, usize begin, usize end)
{
	if (end > len(s))
		end = len(s);

	String out{s.data.allocator};
	usize l = end - begin;
	reserve(out, l);

	for (usize i = begin; i < end; i++)
		append(out, s[i]);

	return out;
}

// iostreams

String operator+(const String& lhs, const String& rhs)
{
	String out = lhs;
	append(out, rhs);
	return out;
}

String operator+(const String& lhs, const char* rhs)
{
	String out = lhs;
	append(out, rhs);
	return out;
}

String operator+(const char* lhs, const String& rhs)
{
	String out = lhs;
	append(out, rhs);
	return out;
}

String operator+(const String& lhs, char rhs)
{
	String out = lhs;
	append(out, rhs);
	return out;
}

String operator+(char lhs, const String& rhs)
{
	String out = lhs;
	append(out, rhs);
	return out;
}

INTERNAL inline s32 stringCompare(const String& lhs, const String& rhs)
{
	const char* s1 = cString(lhs);
	const char* s2 = cString(rhs);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(const u8*)(s1) - *(const u8*)(s2);
}

bool operator==(const String& lhs, const String& rhs)
{
	return stringCompare(lhs, rhs) == 0;
}

bool operator==(const char* lhs, const String& rhs)
{
	return stringCompare(lhs, cString(rhs)) == 0;
}

bool operator==(const String& lhs, const char* rhs)
{
	return stringCompare(cString(lhs), rhs) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
	return !operator==(lhs, rhs);
}

bool operator!=(const char* lhs, const String& rhs)
{
	return !operator==(lhs, rhs);
}

bool operator!=(const String& lhs, const char* rhs)
{
	return !operator==(lhs, rhs);
}

bool operator<(const String& lhs, const String& rhs)
{
	return stringCompare(lhs, rhs) < 0;
}

bool operator<(const char* lhs, const String& rhs)
{
	return stringCompare(lhs, cString(rhs)) < 0;
}

bool operator<(const String& lhs, const char* rhs)
{
	return stringCompare(cString(lhs), rhs) < 0;
}

bool operator<=(const String& lhs, const String& rhs)
{
	return stringCompare(lhs, rhs) <= 0;
}

bool operator<=(const char* lhs, const String& rhs)
{
	return stringCompare(lhs, cString(rhs)) <= 0;
}

bool operator<=(const String& lhs, const char* rhs)
{
	return stringCompare(cString(lhs), rhs) <= 0;
}

bool operator>(const String& lhs, const String& rhs)
{
	return stringCompare(lhs, rhs) > 0;
}

bool operator>(const char* lhs, const String& rhs)
{
	return stringCompare(lhs, cString(rhs)) > 0;
}

bool operator>(const String& lhs, const char* rhs)
{
	return stringCompare(cString(lhs), rhs) > 0;
}

bool operator>=(const String& lhs, const String& rhs)
{
	return stringCompare(lhs, rhs) >= 0;
}

bool operator>=(const char* lhs, const String& rhs)
{
	return stringCompare(lhs, cString(rhs)) >= 0;
}

bool operator>=(const String& lhs, const char* rhs)
{
	return stringCompare(cString(lhs), rhs) >= 0;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
	os << cString(str);
	return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
	char c = '\0';
	while (true)
	{
		if (is.eof())
			break;
		is >> c;
		if (c != '\0')
			break;

		append(str, c);
	}
	return is;
}

std::istream& getline(std::istream& is, String& s)
{
	char c;
	clear(s);
	while (is.get(c) && c != '\n')
		append(s, c);
	return is;
}
} // namespace Dunjun
