#include "Window.h"
#include "GameErrors.h"

namespace JCEngine {
	Window::Window() {

	}

	Window::~Window() {

	}

	int Window::create(std::string name, int screenWidth, int screenHeight, unsigned int windowFlags) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;
		//check our flags to add properties to window
		if (windowFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;

		if (windowFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		if (windowFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		//open sdl window
		_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_window == nullptr) {
			fatalError("SDL window could not be opened");
		}

		SDL_GLContext context = SDL_GL_CreateContext(_window);

		if (context == nullptr) {
			fatalError("Context could not be created");
		}

		GLenum code = glewInit();

		if (code != GLEW_OK) {
			fatalError("Could not initialize GLEW");
		}

		std::printf("***Open GL version %s***\n", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//set vsync
		SDL_GL_SetSwapInterval(0);

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::setColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_window);
	}

}
