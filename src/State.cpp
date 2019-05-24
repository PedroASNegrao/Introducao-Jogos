#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL
#include "SDL_include.h"
#include "State.h"
#include <string>

#define PI 3.1415926535

using namespace std;
using std::cerr;

State::State() : started(false) {

	shared_ptr<GameObject> go(new GameObject());
	Sprite *spr = new Sprite(*go, "assets/img/ocean.jpg");

	go->AddComponent(new CameraFollower(*go));
	go->AddComponent(spr);
	go->box.x = 0;
	go->box.y = 0;
	go->box.h = spr->GetHeight();
	go->box.w = spr->GetWidth();

	objectArray.emplace_back(go);


	shared_ptr<GameObject> go2(new GameObject());
	TileSet *ts = new TileSet(TILE_WIDTH, TILE_HEIGHT, "assets/img/tileset.png");
	TileMap *map = new TileMap(*go2, "assets/map/tileMap.txt", ts);


	go2->AddComponent(map);
	go2->box.x = 0;
	go2->box.y = 0;

	objectArray.emplace_back(go2);

	shared_ptr<GameObject> go3(new GameObject());
	go3->box.x += 512;
	go3->box.y += 300;
	Alien *a = new Alien(*go3, 6);
	go3->AddComponent(a);
	//go3->AddComponent(new CameraFollower(*go3, true));

	objectArray.emplace_back(go3);


	shared_ptr<GameObject> go4(new GameObject());
	go4->box.x += 704;
	go4->box.y += 640;
	PenguinBody *body = new PenguinBody(*go4);
	go4->AddComponent(body);
	//go3->AddComponent(new CameraFollower(*go3, true));

	Camera::Follow(go4.get());
	objectArray.emplace_back(go4);


	LoadAssets();
	quitRequested = false;
	music.Play();

	/*
	shared_ptr<GameObject> go(new GameObject());
	Sprite* spr = new Sprite(*go, "assets/img/ocean.jpg");

	go->AddComponent(new CameraFollower(*go));
	go->AddComponent(spr);
	go->box.x = 0;
	go->box.y = 0;
	go->box.h = spr->GetHeight();
	go->box.w = spr->GetWidth();

	objectArray.emplace_back(go);


	shared_ptr<GameObject> go2(new GameObject());
	TileSet* ts = new TileSet(TILE_WIDTH, TILE_HEIGHT, "assets/img/tileset.png");
	TileMap* map = new TileMap(*go2, "assets/map/tileMap.txt", ts);


	go2->AddComponent(map);
	go2->box.x = 0;
	go2->box.y = 0;

	objectArray.emplace_back(go2);


	shared_ptr<GameObject> go3(new GameObject());
	Alien* a = new Alien(*go3, 4);

	go3->AddComponent(a);
	go3->box.x = 512;
	go3->box.y = 300;

	objectArray.emplace_back(go3);


	LoadAssets();
	quitRequested = false;
	music.Play();
	*/
}

void State::Render() {
	int i;

	for (i = 0; i < (int)objectArray.size(); ++i) {
		objectArray[i]->Render();
	}
}

void State::LoadAssets() {
	music.Open("assets/audio/stageState.ogg");
}

bool State::QuitRequested() {
	return quitRequested;
}

void State::Update(float dt) {
	InputManager IM = InputManager::GetInstance();
	vector<int> colliderIndexes;

	if (IM.IsKeyDown(ESCAPE_KEY) || IM.QuitRequested()) {
		quitRequested = true;
	}

	Camera::Update(dt);

	if (IM.KeyPress(SDLK_SPACE)) {
		Vec2 objPos =
			Vec2(200, 0).GetRotated(-M_PI + M_PI * (rand() % 1001) / 500.0) + Vec2(IM.GetMouseX(), IM.GetMouseY());
		AddObject(objPos.x + Camera::pos.x, objPos.y + Camera::pos.y);
	}

	for (int i = 0; i < (int)objectArray.size(); ++i) {
		objectArray[i]->Update(dt);

		Collider *col = (Collider *)objectArray[i]->GetComponent("Collider");

		if (col != nullptr) {
			colliderIndexes.emplace_back(i);
		}
	}


	for (int i = 0; i < (colliderIndexes.size() - 1); ++i) {
		Collider *col = (Collider *)objectArray[colliderIndexes[i]]->GetComponent("Collider");
		if (col == nullptr) {
			cout << "Wrong collider index." << endl;
			exit(1);
		}

		for (int j = i + 1; j < colliderIndexes.size(); ++j) {
			Collider *intCol = (Collider *)objectArray[colliderIndexes[j]]->GetComponent("Collider");

			if (intCol == nullptr) {
				cout << "Wrong internal loop collider index." << endl;
				exit(1);
			}

			bool colision = Collision::IsColliding(col->box, intCol->box,
				(float)(objectArray[colliderIndexes[i]]->angleDeg * PI / 180),
				(float)(objectArray[colliderIndexes[j]]->angleDeg * PI / 180));

			if (colision) {
				//cout << "Colides. i: " << i << " j: " << j <<  endl;
				objectArray[colliderIndexes[i]]->NotifyCollision(*(objectArray[colliderIndexes[j]].get()));
				objectArray[colliderIndexes[j]]->NotifyCollision(*(objectArray[colliderIndexes[i]].get()));
			}
		}
	}

	colliderIndexes.clear();

	for (int i = 0; i < (int)objectArray.size(); ++i) {
		if (objectArray[i]->IsDead()) {

			objectArray.erase(objectArray.begin() + i);

		}
	}

}

weak_ptr<GameObject> State::GetObjectPtr(GameObject *go) {

	for (int i = 0; i < (int)objectArray.size(); ++i) {
		if (objectArray[i].get() == go) {
			return weak_ptr<GameObject>(objectArray[i]);
		}
	}

	return weak_ptr<GameObject>();
}

weak_ptr<GameObject> State::AddObject(shared_ptr<GameObject> go) {

	objectArray.push_back(go);
	if (started) {
		go->Start();
	}

	return weak_ptr<GameObject>(go);
}

void State::Start() {
	LoadAssets();

	for (int i = 0; i < (int)objectArray.size(); ++i) {
		objectArray[i]->Start();
	}

	started = true;
}

State::~State() {
	objectArray.clear();
}

void State::AddObject(float mouseX, float mouseY) {

}