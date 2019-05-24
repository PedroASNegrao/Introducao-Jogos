#pragma once
/* - Como usar esse arquivo:
 *
 * Onde quiser adicionar, por exemplo, SDL_image e SDL_mixer em um arquivo, fa�a o seguinte e
 * ele incluir� elas automaticamente e de forma multiplataforma (se usar o makefile tbm fornecido).
 */
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL
#include "SDL_include.h"
#include "State.h"
#include "Resources.h"
#include "InputManager.h"
#include <string>
#include <iostream>
#include <ctime>

#define STD_WIDTH 1024
#define STD_HEIGHT 600

using namespace std;

 /************************************************
 *					Game.h						*
 *************************************************/
class Game {
public:
	~Game();

	void Run();

	SDL_Renderer *GetRenderer();

	State& GetState();

	static Game&GetInstance();

	float GetDeltaTime();

private:

	State* state;

	static Game*instance;

	SDL_Window* window;

	SDL_Renderer* renderer;

	Game(string title, int width, int height);

	int frameStart;

	float dt;

	void CalculateDeltaTime();
};


