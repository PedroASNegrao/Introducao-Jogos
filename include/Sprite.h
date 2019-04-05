#pragma once
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL
#include "SDL_include.h"
#include "Component.h"
#include <iostream>
#include <string>

using namespace std;

/************************************************
*					Sprite.h						*
**************************************************/

class Sprite : public Component {
public:

	Sprite(GameObject &associated);

	Sprite(GameObject &associated, string file);


	~Sprite();


	void Open(string file);

	void SetClip(int x, int y, int w, int h);

	int GetWidth();

	int GetHeight();

	bool IsOpen();


	void Update(float dt) override;

	void Render() override;

	bool Is(string type) override;

private:

	SDL_Texture *texture;

	int width;

	int height;

	SDL_Rect clipRect;
};
