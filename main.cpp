#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <iomanip>

using namespace std;

const int WIDTH = 115;
const int HEIGHT = 25;
const int AMMO = 100;
const int NUMBER_TARGET = 5;
const int RANGE_TARGET = 20;

enum class Direct { NONE, UP, DOWN, RIGHT, LEFT};

int random(int);
void gotoxy(int, int);

class Object {
public:
	
	int getPosX();
	int getPosY();
	int getSpeedX();
	
	int getSpeedY();
	Direct getDir();
	
	void setPosX(int);
	void setPosY(int);
	void setSpeedX(int);
	void setSpeedY(int);
	void setDir(Direct);

protected:
	
	int posX, posY;
	int speedX, speedY;
	Direct dir;
};

class Tank : public Object {
public:
	
	Tank();
	Tank(int, int);

	int barrelX, barrelY;
	int health;

};

class Bullets : public Object {
public:
	
	Bullets();
	void Reset();
	
};

class Target : public Object {
public:
	
	Target();
	void Respawn();
	void Explode();
	
	bool lockPlayer;
	
};

class Gameplay {
public:
	
	Gameplay();
	
	void Draw();
	void Keypress();
	void Logic();
	void Update();
	void Run();
	void Pause();
	
	void IfDie();
	
private:
	
	Tank tank;
	Bullets bullet[AMMO];
	Target target[NUMBER_TARGET];
	bool gameOver;
	int gameTimer;
	int score;
	int index;
	int currentAmmo;
	bool tankBoost;
	int boostTime;
	bool pause;

};


int main() {
	
	srand(time(NULL));
	
	Gameplay gameplay;
	gameplay.Run();
	
	return 0;
}

int random(int number) {
	return rand() % number + 1;
}

void gotoxy(int x, int y) {
	cout<< "\x1B[" << y << ";" << x << "H";
}

int Object::getPosX() {
	return posX;
}

int Object::getPosY() {
	return posY;
}

int Object::getSpeedX() {
	return speedX;
}

int Object::getSpeedY() {
	return speedY;
}

Direct Object::getDir() {
	return dir;
}

void Object::setPosX(int newPosX) {
	posX = newPosX;
}

void Object::setPosY(int newPosY) {
	posY = newPosY;
}

void Object::setSpeedX(int newSpeedX) {
	speedX = newSpeedX;
}

void Object::setSpeedY(int newSpeedY) {
	speedY = newSpeedY;
}

void Object::setDir(Direct newDir) {
	dir = newDir;
}

Tank::Tank() {
	posX = 0;
	posY = 0;
	health = 100;
	dir = Direct::RIGHT;
}

Tank::Tank(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;
}

Bullets::Bullets() {
	posX = -1;
	posY = -1;
	dir = Direct::NONE;
}

void Bullets::Reset() {
	posX = -1;
	posY = -1;
	dir = Direct::NONE;
}

Target::Target() {
	posX = random(WIDTH - 1) + 1;
	posY = random(HEIGHT - 1) + 1;
	dir = Direct::RIGHT;
	lockPlayer = false;
}

void Target::Respawn() {
	posX = random(WIDTH - 1) + 1;
	posY = random(HEIGHT - 1) + 1;
	
	int randDir = random(4);
		switch (randDir) {
			case 1 : dir = Direct::UP;
				break;
			case 2 : dir = Direct::DOWN;
				break;
			case 3 : dir = Direct::RIGHT;
				break;
			case 4 : dir = Direct::LEFT;
				break;
		}
}

void Target::Explode() {

	for (int i = -1; i <= 1; i++) {
		gotoxy(posX - 1, posY + i);
		cout << "ooo";
	}
	
	Sleep(60);
	
	for (int i = -2; i < 3; i++) {
		if (i == -2 || i == 2) {
			gotoxy(posX - 2, posY + i); cout << " *** ";
		} else {
			gotoxy(posX - 2, posY + i); cout << "*   *";
		}
	}
	
	Sleep(80);
	
}

Gameplay::Gameplay() {
	score = 0;
	index = 0;
	currentAmmo = 20;
	gameTimer = 0;
	gameOver = false;
	tankBoost = false;
	boostTime = 20;
	pause = false;
	tank.setPosX(2);
	tank.setPosY(2);
}

void Gameplay::Draw() {
	
	// Hide the cursor
	cout << "\033[?25l";
	
	// Clear screen
	cout << "\033[2J";
	
	// Draw the bullet
	for (int i = 0; i < AMMO; i++) {
		if (bullet[i].getPosX() < 0 || bullet[i].getPosY() < 0) continue;
		gotoxy(bullet[i].getPosX(), bullet[i].getPosY());
		
		if (bullet[i].getDir() == Direct::UP || bullet[i].getDir() == Direct::DOWN) {
			cout << "|";
		} else {
			cout << "-";
		}
	}
	
	gotoxy(tank.getPosX(), tank.getPosY()); 
	cout << "O";
	
	gotoxy(tank.barrelX, tank.barrelY);
	cout << "o";
	
	for (int i = 0; i < NUMBER_TARGET; i++) {
		gotoxy(target[i].getPosX(), target[i].getPosY());
		if (target[i].lockPlayer) {
			cout << "\033[31m";
    		cout << "x";
    		cout << "\033[0m";
		} else cout << "x"; 
	} 
	
	// Draw the score
	gotoxy(1, HEIGHT + 1); 
	cout << "Score : " << score; 
	
	gotoxy(1, HEIGHT + 2); 
	cout << "Health: " << tank.health; 
	
	gotoxy(1, HEIGHT + 3);
	cout << "Ammo  : " << currentAmmo;
	
	gotoxy(1, HEIGHT + 4);
	cout << "Boost : " << boostTime;
}

void Gameplay::Keypress() {
	if (kbhit()) {
		switch (getch()) {
			case 'a' : tank.setDir(Direct::LEFT);
				break;
			case 'd' : tank.setDir(Direct::RIGHT);
				break;
			case 'w' : tank.setDir(Direct::UP);
				break;
			case 's' : tank.setDir(Direct::DOWN);
				break;
			case 'j' : 
				if (currentAmmo > 0) {
					bullet[index].setPosX(tank.getPosX());
					bullet[index].setPosY(tank.getPosY());
					bullet[index].setDir(tank.getDir());
					index++;
					currentAmmo--;
				}
			 	break;
			case 'k' : if (!tankBoost) tankBoost = true;
				break;
			case 'p' : pause = !pause;
				break;
			case 'q' : gameOver = true;
				break;
		}
	}
}

int Diff(int a, int b){
	return a-b > 0 ? a-b : b-a;
}

int Distance(Object &A, Object &B) {
	return ((A.getPosX() - B.getPosX()) * (A.getPosX() - B.getPosX()) 
		  + (A.getPosY() - B.getPosY()) * (A.getPosY() - B.getPosY()));
}

void Gameplay::Logic() {
	
	if (index >= AMMO) index = 0;
	
	// Bullets reset when get out of border
	for (int i = 0; i < AMMO; i++) {
		if (bullet[i].getPosX() >= WIDTH || bullet[i].getPosX() <= 0 
		 || bullet[i].getPosY() >= HEIGHT || bullet[i].getPosY() <= 0) bullet[i].Reset(); 
	}
	
	// A bullet hits a target
	for (int i = 0; i < AMMO; i++) {
		for (int j = 0; j < NUMBER_TARGET; j++) {
			if (bullet[i].getPosX() == target[j].getPosX() && bullet[i].getPosY() == target[j].getPosY()) {
				target[j].Explode();
				target[j].Respawn();
				bullet[i].Reset();
				score+= 10;
				break;
		    }
		}
	}
	
	// Target random movement
	if (gameTimer % 50 == 0) {
		for (int i = 0; i < NUMBER_TARGET; i++) {
			int randDir = random(4);
			if (target[i].lockPlayer == false)
				switch (randDir) {
					case 1 : target[i].setDir(Direct::UP);
						break;
					case 2 : target[i].setDir(Direct::DOWN);
						break;
					case 3 : target[i].setDir(Direct::LEFT);
						break;
					case 4 : target[i].setDir(Direct::RIGHT);
						break;
				}
		}
	}
	
	// Target change direction when hit the border
	for (int i = 0; i < NUMBER_TARGET; i++) {
		if (target[i].getPosX() >= WIDTH-1)   target[i].setDir(Direct::LEFT);
		if (target[i].getPosX() <= 1) 	      target[i].setDir(Direct::RIGHT);
		if (target[i].getPosY() >= HEIGHT-1)  target[i].setDir(Direct::UP);
		if (target[i].getPosY() <= 1) 		  target[i].setDir(Direct::DOWN);
	}
	
	// Tank change direction when hit the border
	if (tank.getPosX() >= WIDTH-1)  	tank.setDir(Direct::LEFT);
	if (tank.getPosX() <= 1) 	        tank.setDir(Direct::RIGHT);
	if (tank.getPosY() >= HEIGHT-1) 	tank.setDir(Direct::UP);
	if (tank.getPosY() <= 0) 		    tank.setDir(Direct::DOWN);
	
	// Target hit the tank and explode
	for (int i = 0; i < NUMBER_TARGET; i++) {
		if (tank.getPosX() == target[i].getPosX() && tank.getPosY() == target[i].getPosY()
		||  tank.barrelX == target[i].getPosX() && tank.barrelY == target[i].getPosY()) {
			target[i].Explode();
			target[i].Respawn();
			tank.health-= 10;
		}
	}
	
	// Target try to come close to the tank
	int attackDistance = RANGE_TARGET * RANGE_TARGET;
	for (int i = 0; i < NUMBER_TARGET; i++) {
		if (Distance(tank, target[i]) <= attackDistance)
		{
			target[i].lockPlayer = true;
			
			int DiffX = Diff(tank.getPosX(), target[i].getPosX());
			int DiffY = Diff(tank.getPosY(), target[i].getPosY());
			
			if (DiffX > DiffY) 
			{
				target[i].setDir(tank.getPosX() - target[i].getPosX() > 0 ? Direct::RIGHT : Direct::LEFT); 
			} 
			else if (DiffX < DiffY)	
			{
				target[i].setDir(tank.getPosY() - target[i].getPosY() > 0 ? Direct::DOWN : Direct::UP); 
			}	 
		} else target[i].lockPlayer = false;
	}
	
	if (tank.health <= 0) gameOver = true;
}

void Move(Object &obj) {
	if (obj.getDir() == Direct::NONE) return;
	
	if (obj.getDir() == Direct::LEFT) {
		obj.setSpeedX(-1);
		obj.setSpeedY(0);
	} else if (obj.getDir() == Direct::RIGHT) {
		obj.setSpeedX(1);
		obj.setSpeedY(0);
	} else if (obj.getDir() == Direct::UP) {
		obj.setSpeedX(0);
		obj.setSpeedY(-1);
	} else if (obj.getDir() == Direct::DOWN) {
		obj.setSpeedX(0);
		obj.setSpeedY(1);
	} 
	
	obj.setPosX(obj.getPosX() + obj.getSpeedX());
	obj.setPosY(obj.getPosY() + obj.getSpeedY());
}

void Gameplay::Update() {
	
	if (gameTimer % 30 == 0)
		if (boostTime < 20) boostTime++;
		
	if (tankBoost) boostTime--;
	
	if (boostTime <= 0) tankBoost = false;
	
	if (gameTimer % 75 == 0)
		if (currentAmmo < 20) currentAmmo++;
	
	if (gameTimer % 12 == 0 || tankBoost)
		Move(tank);
	
	for (int i = 0; i < AMMO; i++)
		Move(bullet[i]);
	
	if (gameTimer % 10 == 0)
		for (int i = 0; i < NUMBER_TARGET; i++)	
			Move(target[i]);
		
	if (tank.getDir() == Direct::LEFT) {
		tank.barrelX = tank.getPosX() - 1;
		tank.barrelY = tank.getPosY();
	} else if (tank.getDir() == Direct::RIGHT) {
		tank.barrelX = tank.getPosX() + 1;
		tank.barrelY = tank.getPosY();
    } else if (tank.getDir() == Direct::UP) {
		tank.barrelX = tank.getPosX();
		tank.barrelY = tank.getPosY() - 1;
	} else if (tank.getDir() == Direct::DOWN) {
		tank.barrelX = tank.getPosX();
		tank.barrelY = tank.getPosY() + 1;
    }
	
	gameTimer++;
	if (gameTimer >= 10000) gameTimer = 0;
}

void Gameplay::IfDie() {
	cout << "\033[2J";
	
	gotoxy(54,14); cout << "YOU DIE!!!";
	gotoxy(54,15); cout << "Score: " << setw(3) << score;
	
	gotoxy(1,25);
	
	Sleep(500);
}

void Gameplay::Pause() {
	gotoxy(54,HEIGHT+2);
	cout << "PAUSE...";
}

void Gameplay::Run() {
	while (!gameOver) {
		Gameplay::Keypress();
		
		if (!pause) {
			Gameplay::Logic();
			Gameplay::Update();
			Gameplay::Draw();		
		} else {
			Gameplay::Pause();
		}
		
		Sleep(1);
	}
	
	if (gameOver) Gameplay::IfDie();
}




