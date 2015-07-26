#ifndef DUNJUN_CORE_STRINGFUNCTIONS_HPP
#define DUNJUN_CORE_STRINGFUNCTIONS_HPP

#include <Dunjun/Core/String.hpp>

namespace Dunjun
{
namespace Strings
{
// TODO(bill): Fully support UTF-8 and unicode code points (e.g. u32)
// This will most likely _require_ a 3rd-party library

// `contains` returns true if substr is within s
bool contains(const String& s, const String& substr);
// `containsAny` returns true if any character in chars aw within s
bool containsAny(const String& s, const String& chars);

// `hasPrefix` tests whether the string s begins with prefix
bool hasPrefix(const String& s, const String& prefix);
// `hasSuffix` tests whether the string s ends with suffix
bool hasSuffix(const String& s, const String& suffix);

// `join` concatenates the elements of a to create a single string
// The separator string sep is placed between elements in the resulting string
String join(const Array<String>& array, const String& sep);

// `split` splits s into all substrings separated by sep and sets the `out`
// array to the substrings between those separators
void split(const String& s, const String& sep, Array<String>& out);

// `toLower` returns a lower case version of s
String toLower(const String& s);
// `toLower` returns a lower case version of c
char toLower(const char c);

// `toTitle` returns a titlized version of s
String toTitle(const String& s);

// `toUpper` returns an upper case version of s
String toUpper(const String& s);
// `toUpper` returns an upper case version of c
char toUpper(const char c);

// `isSpace` returns true if the character is whitespace
bool isSpace(const char c);
// `isAlpha` returns true if the character is [a-zA-Z]
bool isAlpha(const char c);
// `isDigit` returns true if the character is [0-9]
bool isDigit(const char c);
// `isAlphaNumeric` returns true if the character is [a-zA-Z0-9]
bool isAlphaNumeric(const char c);

// `trimLeft`
String trimLeft(const String& s, const String& cutset);
// `trimRight`
String trimRight(const String& s, const String& cutset);
// `trim(s, cutset)` is equivalent to `trimLeft(trimRight(s, cutset), cutset)`
String trim(const String& s, const String& cutset);
// `trimSpace`
String trimSpace(const String& s);

// `count` coutns the number of non-overlapping instances of sep in s
usize count(const String& s, const String& sep);

// `index` returns the index of the first instance of sep in s, or -1 if sep is
// not present in s
ssize index(const String& s, const String& sep);
// `indexAny` returns the index of the first instnace of any character from
// chars in s, or -1 if no character from chars is present in s
ssize indexAny(const String& s, const String& chars);
// `lastIndex` returns the index of the last instance of sep in s, or -1 if sep
// is
// not present in s
ssize lastIndex(const String& s, const String& sep);
// `lastIndexAny` returns the index of the last instnace of any character from
// chars in s, or -1 if no character from chars is present in s
ssize lastIndexAny(const String& s, const String& chars);

// `repeat` returns a new string consisting of count copies of the string s
String repeat(const String& s, usize count);
// `replace` returns a copy of the string s with the frist n non-overlapping
// instances of oldStr replaced by newStr.
// If oldStr is empty, it matches at the beginning of the string and after each
// character, yielding up to k+1 replacements for k-char string
// If n < 0, there is no limit on the number of replacements.
// String replace(const String& s, const String& oldStr, const String& newStr,
// ssize n); // TODO(bill)
} // namespace Strings
} // namespace Dunjun

#endif
