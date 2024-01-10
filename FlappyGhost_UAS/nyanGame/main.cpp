#include <sl.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include "properties.h"

bool isCollision(Nyan, Enemy);

int main()
{
	slWindow(1000, 500, "Flappy Ghost", false);
// load assets
	int imgghost = slLoadTexture("assets/ghost.png");
	int imgEnemy = slLoadTexture("assets/enemysweet.png");
	int imgBurger = slLoadTexture("assets/burger.png");
	int imgBuah = slLoadTexture("assets/3cherry.png");
	int imgNyawa = slLoadTexture("assets/heart.png");
	int imgNyawaR = slLoadTexture("assets/heartbreak.png");
	int imgBack = slLoadTexture("assets/bg.png");
	int fontM = slLoadFont("assets/sweetheart.ttf");
	int sndJump = slLoadWAV("assets/oink.wav");
	int sndFall		= slLoadWAV("assets/jatoh.wav");
	int sndStart = slLoadWAV("assets/menu.wav");
	int sndEnd = slLoadWAV("assets/gameover.wav");

//Property
	const double G = -0.2;
	std::random_device rand;
	std::vector<Enemy> troll;
	for (int i = 0; i < 3; ++i)
	{
		double x = 1000 + i * 250;
		double y = (rand() % 5 + 1) * 113;
		troll.push_back({ x, y, 25 });
	}
	/*
	std::vector<Boost> burger;
	for (int i = 0; i < 2; ++i)
	{
		double x = 1000 + i * 250;
		double y = (rand() % 7 + 1) * 113;
		burger.push_back({ x,y,25 });
	}
	*/
	bool end = false;
	int score = 0;
	int play = 0;

	Nyan nyan{ 180, 250, 0, 0 ,40 };
	while (!slShouldClose())
	{
	// Render Background 
		slSetForeColor(1,1, 1, 1);
		slSprite(imgBack, 500, 250, 1000, 500);

	// START MENU
		slSetForeColor(1, 0, 0, 1);
		slSetTextAlign(SL_ALIGN_CENTER);
		slSetFont(fontM, 50);
		slText(500, 300, "flappy ghost game");
		slSetFont(fontM, 20);
		slText(500, 240, "Press [enter] to continue");
		slText(500, 220, "press [esc] to exit");
		slSetForeColor(1, 1, 1, 1);
		slSprite(imgghost, 50, 420, 148, 198);

	// ENTER = START GAME
		if (slGetKey(SL_KEY_ENTER))
		{
			play = play += 1;
			slSoundLoop(sndStart);
			Sleep(300);
		}
		else if (slGetKey(SL_KEY_ESCAPE)) {
			slClose();
		}

	// IN GAME 
		if (play == 1 || play % 2 == 1) 
		{
		// JUMP MECHANIC
			if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT) || slGetKey(' '))
			{
				nyan.velY = 3;
				slSoundPlay(sndJump);
			};

		// Membatasi karakter agar tidak offside ke atas
			nyan.velY += G;
			nyan.posY += nyan.velY;
			if (nyan.posY >= 500)
			{
				nyan.posY = 500;
			}

			if (end == true)
			{
				nyan.health = nyan.health - 1;
			}

		// Background IN GAME
			slSetForeColor(1, 1, 1, 1);
			slSprite(imgBack, 500, 250, 1000, 500);

		//	PLAY
			if (nyan.health > 0)
			{
			// Render Cat
				slSetForeColor(1, 1, 1, 1);
				slSprite(imgghost, nyan.posX, nyan.posY, 148, 198);

			// Render Enemy
				slSetForeColor(1, 1, 1, 1);
				for (const Enemy& e : troll)
					slSprite(imgEnemy, e.posX, e.posY, 100, 80);
				for (Enemy& e : troll)
				{
					e.posX -= 5;		// bergerak ke kiri
					if (e.posX <= 0)	// jika di tepi kiri, reset posisi
					{
						e.posX = 1000;
						e.posY = (rand() % 5 + 1) * 113;
						score++;		// update score
					}
					if (isCollision(nyan, e))
					{
						slSoundPlay(sndFall);
						nyan.health = nyan.health - 1;
						nyan.posY = 500;
						nyan.velY = 0;
						Sleep(600);
						for (int i = 0; i < troll.size(); ++i)
						{
							double x = 800 + i * 200;
							double y = (rand() % 3 + 1) * 100;
							troll[i].posX = x;
							troll[i].posY = y;
						}
					}
				}
				
			
				//Boost
				/*
				slSetForeColor(1, 1, 1, 1);
				for (const Boost& b : burger)
					slSprite(imgBurger, b.posX, b.posY, 50, 30);
				for (Boost& b : burger)
				{
					b.posX -= 5;
					if (b.posX <= 0)
					{
						b.posX = 1000;
						b.posY = (rand() % 5 * 113);
					}
				}
				*/

			// Score
				slSetForeColor(1, 0, 0, 1);
				slSetFont(fontM, 25);
				slSetTextAlign(SL_ALIGN_LEFT);
				slText(20, 480, "score:");
				slText(150, 480, std::to_string(score).c_str());

			// Pause
				slSetFont(fontM, 24);
				slSetTextAlign(SL_ALIGN_CENTER);
				slText(520, 10, "press [enter] to pause");

			// FALL
				if (nyan.health > 0)
				{
					if (nyan.posY <= 0)
					{
						nyan.posY = 500;
						nyan.velY = 0;
						nyan.health = nyan.health - 1;
						Sleep(600);
						for (int i = 0; i < troll.size(); ++i)
						{
							double x = 800 + i * 200;
							double y = (rand() % 3 + 1) * 100;
							troll[i].posX = x;
							troll[i].posY = y;
						}
						slSoundPlay(sndFall);
					}
				}

			//Heart
				slSetForeColor(1, 1, 1, 1);
				if (nyan.health == 3)
				{
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawa, 905, 480, 30, 30);
					slSprite(imgNyawa, 930, 480, 30, 30);
				}
				else if (nyan.health == 2)
				{
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawa, 905, 480, 30, 30);
					slSprite(imgNyawaR, 930, 480, 52, 52);
				}
				else if (nyan.health == 1) {
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawaR, 905, 480, 52, 52);
					slSprite(imgNyawaR, 930, 480, 52, 52);
					
				}
				else {
					slSprite(imgNyawaR, 880, 480, 52, 52);
					slSprite(imgNyawaR, 905, 480, 52, 52);
					slSprite(imgNyawaR, 930, 480, 52, 52);
					slSoundPlay(sndEnd);
					Sleep(1000); 
				}
			}
		// GAME OVER
			else
			{
				slSetForeColor(1, 0, 0, 1);
				slSetTextAlign(SL_ALIGN_CENTER);
				slSetFont(fontM, 100);
				slText(500, 300, "Game Over");
				slSetFont(fontM, 30);
				slText(500, 240, "Press [enter] to continue");
				if (slGetKey(SL_KEY_ENTER))
					nyan.health = 3;
				nyan.velY = 0;
				nyan.posY = 250;
				for (int i = 0; i < troll.size(); ++i)
				{
					double x = 800 + i * 200;
					double y = (rand() % 3 + 1) * 100;
					troll[i].posX = x;
					troll[i].posY = y;
				}
				score = 0;
				//play = play +=2;
			}
		}
		slRender();
	}
	slClose();
	return 0;
}


bool isCollision(Nyan c, Enemy e)
{
	double dx = e.posX - c.posX;
	double dy = e.posY - c.posY;
	double hit = e.radius + c.radius;
	double distance = sqrt(pow(dx, 2) + pow(dy, 2));
	if (distance <= hit)
		return true;
	return false;
}