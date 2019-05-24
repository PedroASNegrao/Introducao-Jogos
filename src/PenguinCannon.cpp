#include "Game.h"
#include "PenguinCannon.h"

#define SHOT_DELAY 1

void PenguinCannon::Update(float dt) {
	timer.Update(dt);

	if (pbody.lock() == nullptr) {
		associated.RequestDelete();
		return;
	}

	InputManager IM = InputManager::GetInstance();
	float distX = IM.GetMouseX() + Camera::pos.x, distY = IM.GetMouseY() + Camera::pos.y;
	Vec2 center = associated.box.GetCenter();
	Vec2 target(distX - center.x, distY - center.y);
	angle = target.IncX();
	associated.angleDeg = 90 - (angle * 180 / PI);


	Vec2 bodyCenter = pbody.lock().get()->box.GetCenter();
	associated.box.x = bodyCenter.x - associated.box.w / 2;
	associated.box.y = bodyCenter.y - associated.box.h / 2;

	if (IM.MousePress(LEFT_MOUSE_BUTTON) && timer.Get() >= SHOT_DELAY) {
		//cout << "Cannon shoot. Timer: " << timer.Get() << endl;
		Shoot();
		timer.Restart();
	}

	//timer.Update(dt);

}

void PenguinCannon::Render() {

}

bool PenguinCannon::Is(string type) {
	return type == "PenguinCannon";
}

void PenguinCannon::Shoot() {

	Vec2 center = associated.box.GetCenter();

	shared_ptr<GameObject> go(new GameObject());
	go->box.x = center.x + (associated.box.w / 2) * sin(angle);
	go->box.y = center.y + (associated.box.w / 2) * cos(angle);

	go->AddComponent(new Bullet(*go, angle, BULLET_SPEED, 100, 500, "assets/img/penguinbullet.png", 4,
		0.3, false));

	//cout << "Shoot" << endl;
	Game::GetInstance().GetState().AddObject(go);
}

PenguinCannon::PenguinCannon(GameObject &associated, weak_ptr<GameObject> penguinBody) : Component
(associated), angle(0), pbody(penguinBody), timer(*new Timer()) {

	Sprite *spr = new Sprite(associated, "assets/img/cubngun.png");

	associated.AddComponent(spr);
	associated.box.h = spr->GetHeight();
	associated.box.w = spr->GetWidth();

	associated.SetCenter();

	//associated.AddComponent(new Collider(associated));
}
