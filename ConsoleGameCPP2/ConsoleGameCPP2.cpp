
// I used this project to learn c++, i still need a lot more time with it but i feel like i know enough to do most things, although not in the cleanest most efficient ways.

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <windows.h>  
#include <conio.h>  
#include<chrono>
#include<time.h>
#include<thread>

using std::vector;
using std::string;
int main()
{

	enum HostileType
	{
		WeakNormal,
		MidNormal,
		StrongNormal
	};
	enum BulletType
	{
		zeroLevelPlayerBullet,
		oneLevelPlayerBullet,
		twoLevelPlayerBullet,

		WeakHostileBullet,
		MidHostileBullet,
	};
	static class Screen
	{
	public:
		const int ScreenWidth = 16;
		const int ScreenHeight = 16;
		const char whiteSpace = ' ';
	public:
		class Pixel
		{
		public:
			char letter;
			int xpos;
			int ypos;
			Pixel(int x, int y, char l)
			{
				xpos = x;
				ypos = y;
				letter = l;
			}
		};
		vector<Pixel> pixels;
		void PrintScreen(vector<Pixel> GamePixels)
		{
			pixels = vector<Pixel>();
			for (int y = ScreenHeight; y > 0; y--)
			{
				for (int x = 0; x < ScreenWidth; x++)
				{
					pixels.push_back(Screen::Pixel(x, y, whiteSpace));
				}
			}

			//replace empty with populated
			std::string screen = "";
			for (unsigned int i = 0; i < GamePixels.size(); i++)
			{
				const auto& gamePixel = GamePixels[i];
				for (unsigned int ii = 0; ii < pixels.size(); ii++)
				{
					auto& pixel = pixels[ii];
					if (gamePixel.ypos == pixel.ypos)
					{
						if (gamePixel.xpos == pixel.xpos)
						{
							pixels[ii].letter = gamePixel.letter;
							break;

						}
					}

				}
			}


			for (Pixel x : pixels)
			{

				screen += x.letter;

				if (x.xpos == ScreenWidth - 1)
				{
					screen += "\n";
				}
			}
			std::string whiteSpace = "";
			for (int i = 0; i < 50; i++)
			{
				whiteSpace += "\n";
			}
			std::cout << whiteSpace;
			std::cout << screen;
		}
	}screen;
	class Bullet
	{
	public:
		BulletType type;
		bool playerBullet;
		float x;
		float y;
		float damage;
		float peirce;
		float direction;
		Bullet(int xpos, int ypos, float dir, BulletType Type = zeroLevelPlayerBullet)
		{
			x = xpos;
			y = ypos;
			direction = dir;
			type = Type;
			switch (type)
			{
			case zeroLevelPlayerBullet:
				damage = 35;
				peirce = 0;
				playerBullet = true;
				return;
			case oneLevelPlayerBullet:
				damage = 50;
				peirce = 1;
				playerBullet = true;
				return;
			case twoLevelPlayerBullet:
				damage = 70;
				peirce = 2;
				playerBullet = true;
				return;

			case WeakHostileBullet:
				playerBullet = false;
				damage = 10;
				peirce = 0;
			case MidHostileBullet:
				playerBullet = false;
				damage = 25;
				peirce = 2;
			}
		}
		//     Bullet() {};
		vector< Screen::Pixel> getPixels()
		{
			switch (type)
			{
			case WeakHostileBullet:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), 'v')
				};
			case zeroLevelPlayerBullet:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), '*')
				};
			case oneLevelPlayerBullet:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), '^')
				};
			case twoLevelPlayerBullet:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), '|')
				};
			case MidHostileBullet:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), 'I')
				};
			default:
				return vector<Screen::Pixel>
				{
					Screen::Pixel((int)round(x), (int)round(y), '?')
				};
			}
		}
		void move()
		{
			if (direction == 1)
			{
				y++;
			}
			if (direction == -1)
			{
				y--;
			}
		}
	};
	class Player
	{

	public:
		int xPos = 10;
		int yPos = 10;

		int baseHealth = 100;
		int baseReload = 5;
		BulletType bulletType;

		int health;

		int reloadTime; //EVERYTHING MESURED IN FRAMES
		int timeTillShoot = reloadTime;

		vector<Bullet> bulletsSpawed;

		void inputs()
		{
			int xchange = 0;
			int ychange = 0;
			if (GetKeyState('D') < 0)
			{
				xchange++;
			}
			if (GetKeyState('A') < 0)
			{
				xchange--;
			}
			if (GetKeyState('W') < 0)
			{
				ychange++;
			}
			if (GetKeyState('S') < 0)
			{
				ychange--;
			}

			xPos += xchange;
			yPos += ychange;
		}
		vector <Screen::Pixel> getPixels()
		{
			return vector<Screen::Pixel>
			{
				Screen::Pixel(xPos - 1, yPos, '<'), Screen::Pixel(xPos, yPos, 'H'), Screen::Pixel(xPos + 1, yPos, '>')
			};
		}
		void shoot()
		{
			if (timeTillShoot != 0)
			{
				timeTillShoot--;
				bulletsSpawed = vector < Bullet >();
				return;
			}
			else
			{
				//SHOTTTTTTTTTTTT
				timeTillShoot = reloadTime;
				const Bullet& bullet = Bullet(xPos, yPos, 1.0, bulletType);
				bulletsSpawed = vector<Bullet>{ bullet };
			}
		}
		void playerLoop()
		{
			inputs();
			shoot();
		}
	};
	enum HostileAnimation
	{
		Spawn,
		Idle,
		Hit,
		Death
	};
	class Hostile
	{
	public:
		float x;
		float y;
		int health;
		int reloadTime;
		int timeTillShoot;
		int animRemaining;
		int timeTillMove;
		int moveTickTime;
		int bulletType;
		HostileAnimation anim;
		HostileType type;
		vector<Bullet> bulletsSpawed;
		Hostile(int X, int Y, HostileType Type)
		{
			x = X;
			y = Y;
			type = Type;
			anim = Spawn;
			animRemaining = 10;
			switch (type)
			{
			case WeakNormal:
				health = 100;
				reloadTime = 20;
				timeTillShoot = reloadTime;
				moveTickTime = 30;
				timeTillMove = moveTickTime;
				bulletType = WeakHostileBullet;
				return;
			case MidNormal:
				health = 200;
				reloadTime = 20;
				timeTillShoot = reloadTime;
				moveTickTime = 50;
				timeTillMove = moveTickTime;
				bulletType = MidHostileBullet;
				return;
			case StrongNormal:
				health = 200;
				reloadTime = 5;
				timeTillShoot = reloadTime;
				moveTickTime = 60;
				timeTillMove = moveTickTime;
				bulletType = MidHostileBullet;
				return;
			}
		}
		Hostile() {};
		vector<Screen::Pixel> getPixels()
		{
			switch (type)
			{
			case WeakNormal:
				switch (anim)
				{
				case Spawn:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x, y, 'v'),
					};
				case Idle:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '^'),
							Screen::Pixel(x, y, 'Y'),
							Screen::Pixel(x + 1, y, '^')
					};
				case Hit:
				{
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '^'),
							Screen::Pixel(x, y, '#'),
							Screen::Pixel(x + 1, y, '^')
					};
				}
				}
			case MidNormal:
				switch (anim)
				{
				case Spawn:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x, y, 'X'),
					};
				case Idle:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '<'),
							Screen::Pixel(x, y, 'I'),
							Screen::Pixel(x + 1, y, '>')
					};
				case Hit:
				{
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '<'),
							Screen::Pixel(x, y, '#'),
							Screen::Pixel(x + 1, y, '>')
					};
				}

				}
			case StrongNormal:
				switch (anim)
				{
				case Spawn:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x, y, 'X'),
					};
				case Idle:
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '('),
							Screen::Pixel(x, y, 'X'),
							Screen::Pixel(x + 1, y, ')')
					};
				case Hit:
				{
					return
						vector<Screen::Pixel>
					{
						Screen::Pixel(x - 1, y, '('),
							Screen::Pixel(x, y, '#'),
							Screen::Pixel(x + 1, y, ')')
					};
				}

				}
			}
		}
		void Tick()
		{
			if (animRemaining != 0)
				animRemaining--;
			else
				anim = Idle;




			shoot();
			move();
		}
		void shoot()
		{
			if (timeTillShoot != 0)
			{
				bulletsSpawed = vector<Bullet>();
				timeTillShoot--;
				return;
			}
			timeTillShoot = reloadTime;
			switch (type)
			{
			case WeakNormal:
				bulletsSpawed = { Bullet(x, y, -1, BulletType(bulletType)) };
				break;
			case MidNormal:
				bulletsSpawed = { Bullet(x, y, -1, BulletType(bulletType)) };
				break;
			case StrongNormal:
				bulletsSpawed = { Bullet(x, y, -1, BulletType(bulletType)) };
				break;
			}

		}
		void move()
		{
			if (timeTillMove != 0)
				timeTillMove--;
			else
			{
				timeTillMove = moveTickTime;
				y--;
			}
		}
		//especials
	};
	static class Upgrades
	{ //costs
	public:

		int healthLevel = 0;
		int damageLevel = 0;
		int fireRateLevel = 0;

		vector<int> healthCost{ 1000,2000,3000,4500,10000,15000,30000,40000,-1 };
		vector<int> healthIncrease{ 50,100,150,200,300,500,750 ,1000 }; // set, not additive, eg. lvl 2 is not 150, IS 100

		vector<int> damageCost{ 1500,3000,5000,10000,-1 };
		vector<int> bulletTypes{ 1,2,3 ,4 };

		vector<int> fireRateCost{ 1000 , 1500 ,4000,10000,50000,-1 };
		vector<float> fireRateReduction{ 1,2,3 ,4,5 };
	}upgrades;
	static class GameManager
	{
#pragma region menuVars
		const int BlinkDuration = 5;
		const int MenuBlinkRate = 5;
		const char BlinkChar = '~';

		int TimeTillBlink; //for button select

		int BlinkLeft = -1;
		int SelectedButton = -1;
		bool buttonHeld = false;
	public:
		string postOutput = "";
#pragma endregion
#pragma region gameVars


		bool inMenu = true;
		int level = 1; // gamelevel
		int igc = 0; //MONEY
		int igcBeforePlay;

		bool running = true;
		int playtime = 0;
		Player player;
		vector<Bullet> bullets;
		vector<Screen::Pixel> pixels;
		vector<Hostile> hostiles;

#pragma endregion
#pragma region gameVoids

		void bulletTick()
		{
			for (int i = 0; i < player.bulletsSpawed.size(); i++)
			{
				bullets.push_back(player.bulletsSpawed[i]);
			}
			player.bulletsSpawed = vector<Bullet>();

			for (Hostile hostile : hostiles)
			{
				for (Bullet bullet : hostile.bulletsSpawed)
				{
					bullets.push_back(bullet);
				}
			}


			for (int i = 0; i < bullets.size(); i++)
			{
				bullets[i].move();
			}
		}
		void getAllPixels()
		{
			pixels = vector<Screen::Pixel>();
			addToPixels(player.getPixels());
			for (Bullet bullet : bullets)
			{
				addToPixels(bullet.getPixels());
			}
			for (Hostile hostile : hostiles)
			{
				addToPixels(hostile.getPixels());
			}
		}
		void spawnHostile(int x, int y, HostileType type)
		{
			hostiles.push_back(Hostile(x, y, type));
		}
		void hostileTicks()
		{
			for (Hostile& hostile : hostiles)
			{
				hostile.Tick();
			}
		}
		void hitDetection()
		{
			vector<int> BulletDestroyed;
			vector <int> HostileDestroyed; // indexes
			for (int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i].playerBullet == true) //hostile hit detection =============
				{
					for (int ii = 0; ii < hostiles.size(); ii++)
					{

						if (bullets[i].x == (int)round(hostiles[ii].x))
							if (bullets[i].y == (int)round(hostiles[ii].y))
							{
								hostiles[ii].health -= bullets[i].damage;
								hostiles[ii].anim = Hit;
								hostiles[ii].animRemaining = 1;
								bullets[i].peirce--;

								igc += bullets[i].damage; //money calc
								if (hostiles[ii].health > 0)
									igc += hostiles[ii].health;

								if (hostiles[ii].health <= 0)
								{
									HostileDestroyed.push_back(ii);
								}
								if (bullets[i].peirce <= 0)
								{
									BulletDestroyed.push_back(i);
								}
							}
					}
				}
				else // player hit detection =================
				{
					if (bullets[i].x == player.xPos)
						if (bullets[i].y == player.yPos)
						{
							player.health -= bullets[i].damage;
							bullets[i].peirce--;
							if (player.health <= 0)
							{
								PlayerDeath();
							}
							if (bullets[i].peirce <= 0)
							{
								BulletDestroyed.push_back(i);
							}
						}
				}
			}


			// kill objects
			for (int x : BulletDestroyed)
			{
				bullets.erase(bullets.begin() + x);
			}
			for (int x : HostileDestroyed)
			{
				hostiles.erase(hostiles.begin() + x);
			}
		}
		void PlayerDeath()
		{
			youDiedScreen();
			startMainMenu();
		}
		void SpawnHostiles()
		{
		check:
			switch (level)
			{
			case 1:
			{
				if (playtime == 0)
					spawnHostile(8, 15, WeakNormal);
				if (playtime == 80)
				{
					spawnHostile(12, 15, WeakNormal);
					spawnHostile(4, 15, WeakNormal);

				}
				if (playtime == 220)
				{
					spawnHostile(15, 15, WeakNormal);
					spawnHostile(1, 15, WeakNormal);



				}
				if (playtime == 280)
				{

					spawnHostile(12, 15, WeakNormal);
					spawnHostile(4, 15, WeakNormal);
				}
				if (playtime == 340)
				{

					spawnHostile(8, 15, WeakNormal);
				}
				if (playtime == 390)
				{
					spawnHostile(8, 15, MidNormal);
				}
				break;
			}
			case 2:
			{
				if (playtime == 0)
				{
					spawnHostile(8, 14, WeakNormal);
				}
				if (playtime == 2)
				{
					spawnHostile(10, 15, WeakNormal);
					spawnHostile(6, 15, WeakNormal);
				}
				if (playtime == 3)
				{
					spawnHostile(12, 16, WeakNormal);
					spawnHostile(4, 16, WeakNormal);
				}
				if (playtime == 160)
				{
					spawnHostile(10, 15, MidNormal);
					spawnHostile(6, 15, MidNormal);
				}
				if (playtime == 300)
				{
					spawnHostile(8, 12, StrongNormal);
				}
				if (playtime == 302)
				{
					spawnHostile(10, 15, WeakNormal);
					spawnHostile(6, 15, WeakNormal);
				}
				if (playtime == 304)
				{
					spawnHostile(12, 16, WeakNormal);
					spawnHostile(4, 16, WeakNormal);
				}
				break;
			}
			}

			playtime++;
			if (hostiles.size() == 0)
			{
				if (playtime < 360)
					goto check;
				else
				{
					inMenu = true;
					level++;
					levelCompleteScreen();
				}
			}

		} // main thingy for running game
		void PrintHealthbar()
		{
			int maxHealth;
			if (upgrades.healthLevel != 0)
				maxHealth = upgrades.healthIncrease[upgrades.healthLevel - 1] + player.baseHealth;
			else
			{
				maxHealth = player.baseHealth;
			}
			string OutHealth = "----------";
			if (player.health <= 0)
				return;
			int health = player.health;
			int maxI = (int)(((float)health / (float)maxHealth) * 10.0);
			std::cout << maxI << " max :" << maxHealth;
			for (int i = 0; i < maxI; i++)
			{
				//if ()
				OutHealth[i] = '=';
			}
			std::cout << "\n{" << OutHealth << "}";
		}
		void startGame()
		{
			igcBeforePlay = igc;
			playtime = 0;
			inMenu = 0;
			bullets = vector<Bullet>();
			hostiles = vector<Hostile>();
			applyUpgrades();

		}
		void startMainMenu()
		{
			inMenu = true;
			BlinkLeft = 0;
			TimeTillBlink = MenuBlinkRate;
			SelectedButton = -1;
		}
		void youDiedScreen()
		{
			pixels.clear();
			string gain = std::to_string(igc - igcBeforePlay);
			vector<Screen::Pixel> animPixels
			{
				Screen::Pixel(4,6,'Y'),
				 Screen::Pixel(5,6,'O'),
				 Screen::Pixel(6,6,'U'),
				 Screen::Pixel(8,6,'D'),
				 Screen::Pixel(9,6,'I'),
				 Screen::Pixel(10,6,'E'),
				 Screen::Pixel(11,6,'D'),
				 Screen::Pixel(3,4,'+'),
				 Screen::Pixel(4 + gain.size(),4,'$'),
			};

			for (int i = 0; i < gain.size(); i++)
			{
				animPixels.push_back(Screen::Pixel(4 + i, 4, gain[i]));
			}
			screen.PrintScreen(animPixels);
			Sleep(4000);
		}
		void applyUpgrades()
		{
			player.health = player.baseHealth;
			player.reloadTime = player.baseReload;
			if (upgrades.healthLevel != 0)
			{
				player.health += upgrades.healthIncrease[upgrades.healthLevel - 1];
			}

			player.bulletType = BulletType(upgrades.damageLevel);

			if (upgrades.fireRateLevel != 0)
			{
				player.reloadTime -= upgrades.fireRateReduction[upgrades.fireRateLevel - 1];
			}
		}
		void levelCompleteScreen()
		{
			pixels.clear();
			string gain = std::to_string(igc - igcBeforePlay);
			string gameLevel = std::to_string(level);
			vector<Screen::Pixel> animPixels
			{
				Screen::Pixel(4,6,'Y'),
				 Screen::Pixel(5,6,'O'),
				 Screen::Pixel(6,6,'U'),
				 Screen::Pixel(8,6,'W'),
				 Screen::Pixel(9,6,'I'),
				 Screen::Pixel(10,6,'N'),

								 Screen::Pixel(4,5,'L'),
									 Screen::Pixel(5,5,'V'),
										 Screen::Pixel(6,5,'L'),
											 Screen::Pixel(7,5,':'),

				 Screen::Pixel(3,4,'+'),
				 Screen::Pixel(4 + gain.size(),4,'$'),
			};

			for (int i = 0; i < gain.size(); i++)
			{
				animPixels.push_back(Screen::Pixel(4 + i, 4, gain[i]));
			}
			for (int i = 0; i < gameLevel.size(); i++)
			{
				animPixels.push_back(Screen::Pixel(8 + i, 5, gameLevel[i]));
			}
			screen.PrintScreen(animPixels);
			Sleep(4000);
		}
		void ingameHotkeys()
		{
			if (GetKeyState(VK_ESCAPE) < 0)
			{
				startMainMenu();
			}

		}
#pragma endregion
#pragma region Menu stuff
		class variableText
		{
		public:
			string data;
			int Startx; //scrolls right
			int y;
			variableText(string d, int x, int Y)
			{
				data = d;
				Startx = x;
				y = Y;
			}
		};
		class buttonText
		{
		public:
			string text;
			int sortIndex;
			int x;
			int y;
			int up;
			int down;
			int left;
			int right;
			buttonText(int X, int Y, string t, int UP = 0, int DOWN = 0, int LEFT = 0, int RIGHT = 0)
			{
				text = t;
				x = X;
				y = Y;

				up = UP;
				down = DOWN;
				left = LEFT;
				right = RIGHT;

			}
			buttonText() {};
			int newSelected(int xChange, int yChange)
			{
				if (xChange == 1)
					return right;
				if (xChange == -1)
					return left;
				if (yChange == 1)
					return up;
				if (yChange == -1)
					return down;
			}
		};
		void MainMenuLoop()
		{
			int midLineX = 10;

			vector<variableText> variableTexts;
			vector<buttonText> buttons;
			vector<Screen::Pixel> screen
			{
				Screen::Pixel(0,16,'U'),
				 Screen::Pixel(1,16,'P'),
				 Screen::Pixel(2,16,'G'),
				 Screen::Pixel(3,16,'R'),
				 Screen::Pixel(4,16,'A'),
				 Screen::Pixel(5,16,'D'),
				 Screen::Pixel(6,16,'E'),
				 Screen::Pixel(7,16,'S'),


				Screen::Pixel(midLineX,16,'|'),
				Screen::Pixel(midLineX,15,'|'),
				Screen::Pixel(midLineX,14,'|'),
				Screen::Pixel(midLineX,13,'|'),
				Screen::Pixel(midLineX,12,'|'),
				Screen::Pixel(midLineX,11,'|'),
				Screen::Pixel(midLineX,10,'|'),
				Screen::Pixel(midLineX,9,'|'),
				Screen::Pixel(midLineX,8,'|'),
				Screen::Pixel(midLineX,7,'|'),
				Screen::Pixel(midLineX,6,'|'),
				Screen::Pixel(midLineX,5,'|'),
				Screen::Pixel(midLineX,4,'|'),
				Screen::Pixel(midLineX,3,'|'),
				Screen::Pixel(midLineX,2,'|'),
				Screen::Pixel(midLineX,1,'|'),
				Screen::Pixel(midLineX,0,'|'),

				Screen::Pixel(0,15,'h'),
				Screen::Pixel(1,15,'e'),
				Screen::Pixel(2,15,'a'),
				Screen::Pixel(3,15,'l'),
				Screen::Pixel(4,15,'t'),
				Screen::Pixel(5,15,'h'),

				// var text 0 14
				// button 6;7 , 14 (+-)

				Screen::Pixel(0,13,'d'),
				Screen::Pixel(1,13,'a'),
				Screen::Pixel(2,13,'m'),
				Screen::Pixel(3,13,'a'),
				Screen::Pixel(4,13,'g'),
				Screen::Pixel(5,13,'e'),

				// var text 0 12  
				// button 6;7,12 (+-)

				Screen::Pixel(0,11,'f'),
				Screen::Pixel(1,11,'i'),
				Screen::Pixel(2,11,'r'),
				Screen::Pixel(3,11,'e'),
				Screen::Pixel(4,11,'R'),
				Screen::Pixel(5,11,'a'),
				Screen::Pixel(6,11,'t'),
				Screen::Pixel(7,11,'e'),

				// var text 0 10
				// button 6;7,10 (+-)

				// button 0,0 (exit)
				// button 16,0 (play)

				Screen::Pixel(13,14,'^'),
				Screen::Pixel(14,14,'>'),
				Screen::Pixel(12,14,'<'),

				Screen::Pixel(12,11,'c'),
				Screen::Pixel(13,11,'a'),
				Screen::Pixel(14,11,'s'),
				Screen::Pixel(15,11,'h'),
				// var text 12,10
			};
#pragma region Dynamic elements



			variableTexts.push_back(variableText(std::to_string(igc), 12, 10)); //cash
			variableTexts.push_back(variableText(std::to_string(player.baseHealth), 0, 14)); // health bar
			variableTexts.push_back(variableText(std::to_string(player.baseHealth), 0, 12)); // damage bar
			variableTexts.push_back(variableText(std::to_string((int)(((double)player.reloadTime / (double)20) * (double)600)), 0, 10)); // damage bar

			buttons.push_back(buttonText(6, 14, "+", 6, 2, 6, 1)); // 0
			buttons.push_back(buttonText(7, 14, "-", 6, 3, 0, 7)); // 1

			buttons.push_back(buttonText(6, 12, "+", 0, 4, 6, 3)); // 2
			buttons.push_back(buttonText(7, 12, "-", 1, 5, 2, 7)); // 3

			buttons.push_back(buttonText(6, 10, "+", 2, 6, 6, 5)); // 4
			buttons.push_back(buttonText(7, 10, "-", 3, 6, 4, 7)); // 5



			buttons.push_back(buttonText(0, 1, "Exit", 4, 0, 7, 7));  // 6
			buttons.push_back(buttonText(12, 1, "Play", 5, 1, 6, 6));  // 7

			if (SelectedButton == -1)
			{
				SelectedButton = buttons.size() - 1;
			}
			if (BlinkLeft == -1)
			{
				BlinkLeft = BlinkDuration;
			}
#pragma endregion
			SelectedButton = menuNav(buttons, SelectedButton);

			for (int i = 0; i < variableTexts.size(); i++)
			{
				auto text = variableTexts[i];
				auto x = text.Startx;
				auto y = text.y;
				for (int ii = 0; ii < variableTexts[i].data.length(); ii++)
				{

					auto Char = text.data.at(ii);
					screen.push_back(Screen::Pixel(x + ii, y, Char));
				}
			}
			for (int i = 0; i < buttons.size(); i++)
			{
				auto button = buttons[i];
				if (SelectedButton == i)
				{
					if (TimeTillBlink <= 0)
					{
						if (BlinkLeft <= 0)//reset
						{
							TimeTillBlink = MenuBlinkRate;
							BlinkLeft = BlinkDuration;
							for (int ii = 0; ii < button.text.length(); ii++)
							{
								screen.push_back(Screen::Pixel(button.x + ii, button.y, button.text[ii]));
							}
							TimeTillBlink--;
						}
						else
						{
							for (int ii = 0; ii < button.text.length(); ii++)
							{
								screen.push_back(Screen::Pixel(button.x + ii, button.y, BlinkChar));
							}
							BlinkLeft--;
						}
					}
					else
					{
						for (int ii = 0; ii < button.text.length(); ii++)
						{
							screen.push_back(Screen::Pixel(button.x + ii, button.y, button.text[ii]));
						}
						TimeTillBlink--;
					}

				}
				else
				{
					for (int ii = 0; ii < button.text.length(); ii++)
					{
						screen.push_back(Screen::Pixel(button.x + ii, button.y, button.text[ii]));
					}
				}
			}//blinks & prints
			vector<Screen::Pixel> costs = showCosts();
			for (int i = 0; i < costs.size(); i++)
			{
				screen.push_back(costs[i]);
			}
			std::cout << "\n guh: " << costs.size();
			pixels = screen;
		}
		vector<Screen::Pixel> showCosts()
		{
			string output = "COST: ";
			int cost = -1;
			switch (SelectedButton)
			{
			case 0:
				cost = upgrades.healthCost[upgrades.healthLevel];
				break;
			case 2:
				cost = upgrades.damageCost[upgrades.damageLevel];
				break;
			case 4:
				cost = upgrades.fireRateCost[upgrades.fireRateLevel];
				break;
			case 1:
				if (upgrades.healthLevel != 0)
				{
					cost = -upgrades.healthCost[upgrades.healthLevel - 1];
				}
				else
				{
					cost = 0;
				}
				break;
			case 3:
				if (upgrades.healthLevel != 0)
				{
					cost = -upgrades.damageCost[upgrades.damageLevel - 1];
				}
				else
				{
					cost = 0;
				}
				break;
			case 5:
				if (upgrades.healthLevel != 0)
				{
					cost = -upgrades.fireRateCost[upgrades.fireRateLevel - 1];
				}
				else
				{
					cost = 0;
				}
				break;
			}
			if (cost == -1)
				return vector<Screen::Pixel>();

			string strCost = std::to_string(cost);
			for (int i = 0; i < strCost.size(); i++)
			{
				output.push_back(strCost[i]);
			}

			vector<Screen::Pixel> pixelOutput;
			for (int i = 0; i < output.size(); i++)
			{
				pixelOutput.push_back(Screen::Pixel(0 + i, 8, output[i]));
			}
			return pixelOutput;
		}
		void buttonFunctionality(int pressed, int menu = 0)
		{
			if (menu == 0) // main menu
			{
				switch (pressed)
				{
				case 0:
					if (igc > upgrades.healthCost[upgrades.healthLevel])
					{
						if (upgrades.healthCost[upgrades.healthLevel == -1])
							return;
						igc -= upgrades.healthCost[upgrades.healthLevel];
						upgrades.healthLevel++;
					}
					break;
				case 2:
					if (igc > upgrades.damageCost[upgrades.damageLevel])
					{
						if (upgrades.damageCost[upgrades.damageLevel] == -1)
							return;
						igc -= upgrades.damageCost[upgrades.damageLevel];
						upgrades.damageLevel++;
					}
					break;
				case 4:
					if (igc > upgrades.fireRateCost[upgrades.fireRateLevel])
					{
						if (upgrades.fireRateCost[upgrades.fireRateLevel] == -1)
							return;
						igc -= upgrades.fireRateCost[upgrades.fireRateLevel];
						upgrades.fireRateLevel++;
					}
					break;
				case 7: // play button
					startGame();
					break;
				case 6: // exit
					running = false;
					break;

				}

			}
		}
		int menuNav(vector<buttonText> buttons, int current, int Menu = 0)
		{
			int xchange = 0;
			int ychange = 0;
			if (!buttonHeld)
			{
				if (GetKeyState('D') < 0)
				{
					xchange++;
					buttonHeld = true;
				}
				if (GetKeyState('A') < 0)
				{
					xchange--;
					buttonHeld = true;
				}
				if (GetKeyState('W') < 0)
				{
					ychange++;
					buttonHeld = true;
				}
				if (GetKeyState('S') < 0)
				{
					ychange--;
					buttonHeld = true;
				}
				if (GetKeyState(VK_SPACE) < 0)
				{
					buttonHeld = true;
					buttonFunctionality(current, Menu);

				}
			}
			else if (!(GetKeyState('D') < 0 || GetKeyState('A') < 0 || GetKeyState('S') < 0 || GetKeyState('W') < 0))
			{
				buttonHeld = false;
				std::cout << "\n nothing pressed";
				return current;
			}
			else
			{
				return current;
			}

			if (ychange == 0 && xchange == 0)
			{
				return current;
			}

			return	buttons[current].newSelected(xchange, ychange);
		}
#pragma endregion

	private:
		void addToPixels(vector<Screen::Pixel> toAdd)
		{
			for (int i = 0; i < toAdd.size(); i++)
			{
				pixels.push_back(toAdd[i]);
			}
		}
	}gameManager;
	static class Program
	{
	public:

		//============================================================================================== GAMELOOP ===========================================================
		static  void Gameloop()
		{
			if (gameManager.inMenu == false)
			{
				vector<Screen::Pixel> screenPixels = vector<Screen::Pixel>();
				gameManager.SpawnHostiles();
				gameManager.player.playerLoop();
				gameManager.hostileTicks();
				gameManager.bulletTick();
				gameManager.hitDetection();
				gameManager.getAllPixels();
				screen.PrintScreen(gameManager.pixels);
				gameManager.PrintHealthbar();
				gameManager.ingameHotkeys();
			}
			else
			{
				gameManager.MainMenuLoop();
				screen.PrintScreen(gameManager.pixels);
			}
		}
		//============================================================================================== GAMELOOP ===========================================================
		static void start()
		{

		}


	}program;


	std::cout << "wasd to move, space to select, esc to go back to the main menu\n\ntype anything then enter to continue\n";
	string throwaway;
	std::cin >> throwaway;

	gameManager.bullets = vector<Bullet>();
	const auto targetFrameTime = std::chrono::milliseconds(50); //50 = 1 seccond / 20fps 
	program.start();
	do
	{
		//FIXED TIME LOOP

		auto  startTime = std::chrono::high_resolution_clock::now();



		program.Gameloop();

		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		auto timeToSleep = std::chrono::duration_cast<std::chrono::milliseconds>(targetFrameTime - duration);
		std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
		std::cout << "working frame time: " << duration.count() << "\n"
			<< " sleeping frame time: " << timeToSleep.count();
	} while (gameManager.running);
	std::cout << "\n \n================== end of program ===============================";
}
