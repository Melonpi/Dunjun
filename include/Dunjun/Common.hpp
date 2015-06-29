#ifndef DUNJUN_COMMON_HPP
#define DUNJUN_COMMON_HPP

#include <Dunjun/Config.hpp>
#include <Dunjun/Types.hpp>
#include <Dunjun/System/FileSystem.hpp>
#include <Dunjun/System/NonCopyable.hpp>

#include <cassert>
#include <cstdarg>
#include <functional>
#include <iostream>
#include <memory>

namespace Dunjun
{
namespace
{
template <typename T, typename... Args>
INTERNAL std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args)
{
	return std::unique_ptr<T>{new T{std::forward<Args>(args)...}};
}

template <typename T, typename... Args>
INTERNAL std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args)
{
	static_assert(std::extent<T>::value == 0,
				  "Dunjun::make_unique<T[N]>() is forbidden, "
				  "please use make_unique<T[]>().");

	typedef typename std::remove_extent<T>::type U;
	return std::unique_ptr<T>{
		new U[sizeof...(Args)]{std::forward<Args>(args)...}};
}
} // namespace (anonymous)

// NOTE(bill): Used as std::make_unique is not available in C++11, only C++14
//             MSVC does support std::make_unique but use this function instead
template <typename T, typename... Args>
inline std::unique_ptr<T> make_unique(Args&&... args)
{
	return make_unique_helper<T>(std::is_array<T>(),
								 std::forward<Args>(args)...);
}

// NOTE(bill): Very similar to doing
//             `*(T*)(&x)`
// NOTE(bill): This also uses snake_case rather than camelCase to be consistent
// with the other casting types (e.g. static_cast, dynamic_cast, etc.)
// NOTE(bill) IMPORTANT(bill): Only use for small types as it requires `memcpy`
template <typename T, typename U>
inline T pseudo_cast(const U& x)
{
	T to{0};
	std::memcpy(&to, &x, (sizeof(T) < sizeof(U)) ? sizeof(T) : sizeof(U));
	return to;
}

namespace Impl
{
template <typename Fn>
struct Defer
{
	Defer(Fn&& fn)
	: fn{std::forward<Fn>(fn)}
	{
	}
	~Defer() { fn(); };
	Fn fn;
};

template <typename Fn>
Defer<Fn> deferFn(Fn&& fn) { return Defer<Fn>(std::forward<Fn>(fn)); }
} // namespace Impl

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x) DEFER_2(x, __COUNTER__)
#define defer(code) auto DEFER_3(_defer_) = Impl::deferFn([&](){code;});

/* Example for defer
 * FILE* f{fopen("test.txt", "r"))};
 * if (f == nullptr)
 * 	return;
 * defer(fclose(f));
 */

////////////////////////////////////////////////////////////////////////////////

// Cross-platform version of sprintf that uses a local persist buffer
// If more than 1024 characters are needed, a std::stringstream may be needed
// instead.
inline std::string stringFormat(const char* fmt, ...)
{
	LOCAL_PERSIST char s_buf[1024];
	va_list v;
	va_start(v, fmt);
#if defined(DUNJUN_COMPILER_MSVC) // "Fix" MSVC's idea of "standards"
	_vsnprintf(s_buf, 1024, fmt, v);
#else
	vsnprintf(s_buf, 1024, fmt, v);
#endif
	va_end(v);
	s_buf[1023] = '\0';

	return {s_buf, strlen(s_buf)};
}

////////////////////////////////////////////////////////////////////////////////
// NOTE(bill): Helper functions for determining the length of a type and if that
// type is empty or not

template <typename T>
inline usize len(const T& t)
{
	return t.size();
}

template <typename T, usize N>
inline usize len(const T(&array)[N])
{
	return N;
}

template <typename T>
inline bool empty(const T& t)
{
	return t.empty();
}

template <typename T, usize N>
inline bool empty(const T(&array)[N])
{
	return false;
}


////////////////////////////////////////////////////////////////////////////////

// TODO(bill): Remove panic(...) from code eventually and use a log
// This is similar to an assert but not exactly. At the moment, it exit's the
// application but later, it should not.
inline void panic(const std::string& str)
{
	std::cerr << str.c_str() << std::endl;
	std::exit(EXIT_FAILURE);
}

enum class MessageBoxType
{
	Error,
	Warning,
	Information,
};

// Use this function to display a simple modal message box
bool showSimpleMessageBox(MessageBoxType type,
                          const std::string& title,
                          const std::string& message);

////////////////////////////////////////////////////////////////////////////////

namespace BaseDirectory
{
extern const std::string Textures;
extern const std::string Shaders;
} // namespace BaseDirectory

} // namespace Dunjun
#endif
