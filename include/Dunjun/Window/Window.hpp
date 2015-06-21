#ifndef DUNJUN_WINDOW_WINDOW_HPP
#define DUNJUN_WINDOW_WINDOW_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/System/Clock.hpp>
#include <Dunjun/Window/VideoMode.hpp>
#include <Dunjun/Window/GLContextSettings.hpp>

#include <SDL/SDL.h>

#include <string>

namespace Dunjun
{
namespace Style
{
	enum : u32
	{
		Borderless = 1,
		Windowed   = 2,
		Fullscreen = 4,
		Visible    = 8,
		Hidden     = 16,
		Minimized  = 32,
		Maximized  = 64,
		Resizable  = 128,

		Default = Windowed | Visible | Resizable,
	};
} // namespace Style

struct Event;

class Window
{
public:
	struct Dimensions
	{
		int width;
		int height;

		inline f32 aspectRatio() const
		{
			if (width == height)
				return 1.0f;

			return static_cast<f32>(width) / static_cast<f32>(height);
		}
	};

	Window() = default;

	explicit Window(VideoMode mode,
					const std::string& title,
					u32 style = Style::Default,
					const GLContextSettings& settings = GLContextSettings{});

	virtual ~Window();

	void create(VideoMode mode,
				const std::string& title,
				u32 style = Style::Default,
				const GLContextSettings& settings = GLContextSettings{});

	void close();
	bool isOpen() const;

	Vector2 getPosition() const;
	Window& setPosition(const Vector2& position);

	Dimensions getSize() const;
	Window& setSize(const Dimensions& size);

	/// UTF-8 Strings
	const char* getTitle() const;
	Window& setTitle(const char* title);

	Window& setVisible(bool visible);
	Window& setVerticalSyncEnabled(bool enabled);
	Window& setFramerateLimit(u32 limit);

	Window& setFullscreen(bool fullscreen);
	bool isFullscreen() const;

	bool pollEvent(Event& event);
	bool waitEvent(Event& event);

	void display();

	void makeGLContextCurrent() const;

	SDL_Window* getSDLHandle() const { return m_impl; }

private:
	SDL_Window* m_impl{nullptr};
	SDL_GLContext m_glContext{};
	Clock m_clock{};
	Time m_frameTimeLimit{Time::Zero};
	// Dimensions size;
};
} // namespace Dunjun
#endif
