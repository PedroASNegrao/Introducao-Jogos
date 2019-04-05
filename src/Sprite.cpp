#include "Sprite.h"
#include "Game.h"
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL
#include "SDL_include.h"
#include <string>
#include <iostream>

//Pro meu pc:---------	

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h" 
//-------------------- 

using namespace std;
using std::cerr;


Sprite::Sprite(GameObject &associated) : Component(associated), texture(nullptr), width(0), height(0) {

}

Sprite::Sprite(GameObject &associated, string file) : Component(associated), texture(nullptr), width(0), height(0) {
	Open(file);
}

Sprite::~Sprite() {
	if (IsOpen() == false) {
		SDL_DestroyTexture(texture);
	}
}

int Sprite::GetHeight() {
	return height;
}

int Sprite::GetWidth() {
	return width;
}

void Sprite::SetClip(int x, int y, int w, int h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
	//dimens�es da imagem
}

void Sprite::Open(string file) {

	if (texture) {
		SDL_DestroyTexture(texture);
	}

	texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());

	if (!texture) {
		cerr << "IMG_LoadTexture retornou erro: " << SDL_GetError();
		exit(1);
	}

	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	SetClip(0, 0, width, height);

}


void Sprite::Render() {

	SDL_Rect dstRect;
	dstRect.x = this->associated.box.x;
	dstRect.y = this->associated.box.y;
	dstRect.w = clipRect.w;
	dstRect.h = clipRect.h;

	if (SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect) != 0) {
		cerr << "SDL_RenderCopy retornou erro " << SDL_GetError();
		exit(1);
	}

}

bool Sprite::IsOpen() {
	if (texture) {
		return true;
	}
	else {
		return false;
	}

}

void Sprite::Update(float dt) {

}

bool Sprite::Is(string type) {
	return (type == "Sprite");
}
