#ifndef DUNJUN_CORE_STRING_HPP
#define DUNJUN_CORE_STRING_HPP

#include <Dunjun/Core/ContainerTypes.hpp>
#include <Dunjun/Core/Memory.hpp>

#include <iostream>

namespace Dunjun
{
struct String
{
	Array<char> data;

	String(Allocator& a = defaultAllocator());
	String(char c, Allocator& a = defaultAllocator());
	String(const char* s, Allocator& a = defaultAllocator());
	String(const char* s, usize length, Allocator& a = defaultAllocator());
	String(const String& s);

	String& operator=(const String& s);
	String& operator=(const char c);
	String& operator=(const char* str);

	~String() = default;

	inline char& operator[](usize index) { return data[index]; }
	inline const char& operator[](usize index) const { return data[index]; }
};

////////////////////////////////////////////////////////////////////////////////

usize len(const String& s);
usize capacity(const String& s);

usize append(String& s, char c);
usize append(String& s, const char* cStr);
usize append(String& s, const String& other);

char* begin(String& s);
const char* begin(const String& s);
char* end(String& s);
const char* end(const String& s);

char& front(String& s);
const char& front(const String& s);
char& back(String& s);
const char& back(const String& s);

const char* cString(const String& s);

void clear(String& s);

void shrinkToFit(String& s);

void resize(String& s, usize length);

void reserve(String& s, usize capacity);

// `substring` returns a copy of a part of the string in the range [begin, end)
String substring(const String& s, usize begin, usize end);

String operator+(const String& lhs, const String& rhs);

String operator+(const String& lhs, const char* rhs);
String operator+(const char* lhs, const String& rhs);

String operator+(const String& lhs, char rhs);
String operator+(char lhs, const String& rhs);

bool operator==(const String& lhs, const String& rhs);
bool operator==(const char* lhs, const String& rhs);
bool operator==(const String& lhs, const char* rhs);

bool operator!=(const String& lhs, const String& rhs);
bool operator!=(const char* lhs, const String& rhs);
bool operator!=(const String& lhs, const char* rhs);

bool operator<(const String& lhs, const String& rhs);
bool operator<(const char* lhs, const String& rhs);
bool operator<(const String& lhs, const char* rhs);

bool operator<=(const String& lhs, const String& rhs);
bool operator<=(const char* lhs, const String& rhs);
bool operator<=(const String& lhs, const char* rhs);

bool operator>(const String& lhs, const String& rhs);
bool operator>(const char* lhs, const String& rhs);
bool operator>(const String& lhs, const char* rhs);

bool operator>=(const String& lhs, const String& rhs);
bool operator>=(const char* lhs, const String& rhs);
bool operator>=(const String& lhs, const char* rhs);

std::ostream& operator<<(std::ostream& os, const String& s);
std::istream& operator>>(std::istream& is, String& s);

std::istream& getline(std::istream& is, String& s);
} // namespace Dunjun

#endif
