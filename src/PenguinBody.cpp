#define MAX_SPEED 200
#define SPEED_STEP 10
#define STOP_ACC SPEED_STEP*5
#define ROT_SPEED 1.5
#include "Game.h"
#include "PenguinBody.h"

PenguinBody* PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject &associated) : Component(associated), speed(0, 0){
	linearSpeed = 0;
	angle = 0;
	hp = 100;

	Sprite* spr = new Sprite(associated, "assets/img/penguin.png");

	associated.AddComponent(spr);
	associated.box.h = spr->GetHeight();
	associated.box.w = spr->GetWidth();

	associated.SetCenter();

	PenguinBody::player = this;

	associated.AddComponent(new Collider(associated));
}

void PenguinBody::Update(float dt) {
	InputManager IM = InputManager::GetInstance();
	//shared_ptr<GameObject> cannon = nullptr;



	if (IM.IsKeyDown('w') || IM.IsKeyDown('W')) {

		cout << "Body W." << endl;

		if (linearSpeed < MAX_SPEED) {
			linearSpeed += (linearSpeed + SPEED_STEP * dt > MAX_SPEED ? MAX_SPEED - linearSpeed : SPEED_STEP * dt);
		}

	}
	else if (IM.IsKeyDown('s') || IM.IsKeyDown('S')) {

		cout << "Body S." << endl;

		if (linearSpeed > -MAX_SPEED) {
			linearSpeed -= (linearSpeed - SPEED_STEP * dt < -MAX_SPEED ? MAX_SPEED + linearSpeed :
				SPEED_STEP * dt);
		}

	}
	else {
		if (linearSpeed > 0) {
			linearSpeed -= (linearSpeed - STOP_ACC * dt < 0 ? linearSpeed : STOP_ACC * dt);
		}
		else {
			linearSpeed += (linearSpeed + STOP_ACC * dt > 0 ? -linearSpeed : STOP_ACC * dt);
		}
	}

	if (IM.IsKeyDown('a') || IM.IsKeyDown('A')) {
		cout << "Body A." << endl;

		angle -= ROT_SPEED * dt;
	}
	else if (IM.IsKeyDown('d') || IM.IsKeyDown('D')) {
		cout << "Body D." << endl;
		angle += ROT_SPEED * dt;
	}

	associated.angleDeg = angle * 180 / PI;

	speed.x = linearSpeed * cos(angle);
	associated.box.x += speed.x;

	speed.y = linearSpeed * sin(angle);
	associated.box.y += speed.y;

	if (hp <= 0) {
		pcannon.lock()->RequestDelete();
		associated.RequestDelete();
	}



}

void PenguinBody::Render() {

}

bool PenguinBody::Is(string type) {
	return (type == "PenguinBody" || Being::Is(type));
}

void PenguinBody::Start() {
	shared_ptr<GameObject> go(new GameObject());
	Vec2 center = associated.box.GetCenter();
	weak_ptr<GameObject> body = Game::GetInstance().GetState().GetObjectPtr(&associated);

	go->box.x = center.x;
	go->box.y = center.y;
	go->AddComponent(new PenguinCannon(*go, body));
	//go->AddComponent(new CameraFollower(*go, true));

	Game::GetInstance().GetState().AddObject(go);

	pcannon = Game::GetInstance().GetState().GetObjectPtr(go.get());
}

void PenguinBody::NotifyCollision(GameObject &other) {
	auto bullet = (Bullet*)other.GetComponent("Bullet");

	if (bullet != nullptr && bullet->targetsPlayer) {
		hp -= bullet->GetDamage();

		if (hp <= 0) {
			Camera::Unfollow();

			Vec2 center = associated.box.GetCenter();
			shared_ptr<GameObject> go(new GameObject());

			Sprite* spr = new Sprite(*go, "assets/img/penguindeath.png", 5, 0.4, 2);
			go->box.x = center.x;
			go->box.y = center.y;
			go->SetCenter();
			go->AddComponent(spr);

			Sound* boom = new Sound(*go, "assets/audio/boom.wav");
			go->AddComponent(boom);
			boom->Play(1);

			//cout << "Shoot" << endl;
			Game::GetInstance().GetState().AddObject(go);

			pcannon.lock()->RequestDelete();
			associated.RequestDelete();
		}
	}
}

PenguinBody::~PenguinBody() {
	PenguinBody::player = nullptr;
}
