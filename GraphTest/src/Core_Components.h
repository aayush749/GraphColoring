#pragma once
#define SDL_MAIN_HANDLED
#include "SDL.h"

#include <iostream>

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)		//time for which a single frame stays on the screen(in ms)

extern int game_is_running;		//to be defined in the main file (where the main function lies)

class Renderer
{
	const char* m_title;
public:
	Renderer(const char* title)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			fprintf(stderr, "Could not initialize engine... :-(\n[Error] : %s", SDL_GetError());
			return;
		}
		else
		{
			printf("Engine initialized!\n");
		}

		m_title = title;
		window = SDL_CreateWindow(
				m_title,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				WINDOW_WIDTH,
				WINDOW_HEIGHT,
				SDL_WINDOW_VULKAN
			);
		
		if (window == NULL)
		{
			printf("Could not create a game window :-(\n");
			return;
		}
		else
			printf("Window Initialized!\n");

		renderer = SDL_CreateRenderer(window, -1, 0);
		
		if (renderer == NULL)
		{
			printf("Could not initialize renderer :-(\n");
			return;
		}
		else
			printf("Renderer initialized\n");
		game_is_running = TRUE;

		printf("Game Started!\n");
	}


	~Renderer()
	{
		if(renderer)
			SDL_DestroyRenderer(renderer);
		if(window)
			SDL_DestroyWindow(window);
		SDL_Quit();
	}

	SDL_Window* GetWindowPtr()
	{
		return window;
	}

	SDL_Renderer* GetRendererPtr()
	{
		return renderer;
	}

	void FillScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderClear(renderer);
	}

	void SwapBuffers()
	{
		SDL_RenderPresent(renderer);
	}
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
};

template<typename T>
struct Vector2
{
	T x, y;

	void operator+=(Vector2<T> other)
	{
		x += other.x;
		y += other.y;
	}

	Vector2() = default;
	
	Vector2(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2<T>& other)
		:x(other.x), y(other.y)
	{}

	void operator=(const Vector2<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	bool operator==(const Vector2<T>& other)
	{
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const Vector2<T>& other)
	{
		return this->x != other.x || this->y != other.y;
	}
};

