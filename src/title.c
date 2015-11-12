#include "title.h"

#include <SDL.h>
#include "audio.h"
#include "board.h"
#include "fileio.h"
#include "font.h"
#include "game.h"
#include "input.h"
#include "main.h"
#include "menu.h"
#include "states.h"
#include "video.h"

menuContainer *curMenu;
int menuSel;
int displayHelp;
int displayCredits;
int helpPage;
int creditsPage;
int savePresent;

void titleUnload()
{
	SDL_FreeSurface(titleBackgroundIMG);
	titleBackgroundIMG = NULL;
}

void titleLoad()
{
	if (!titleBackgroundIMG)
	{
		titleBackgroundIMG = loadImage("data/gfx/background.bmp");
	}

	savePresent = getBoard(1);
	menuSel = savePresent ? 0 : 1;
	bgdMusic = loadMusic(bgdMusic, "data/music/bgd01.mp3");

	if(bgdMusic)
	{
		playMusic(bgdMusic);
	}

	menuLoad();
	curMenu = &menuMain;
}

void titleLogic()
{
	if(keys[KEY_BACK])
	{
		keys[KEY_BACK] = 0;

		if (displayHelp)
		{
			displayHelp = 0;
		}
		else if (displayCredits)
		{
			displayCredits = 0;
		}
		else
		{
			quit = 1;
		}
	}
	if(keys[KEY_CANCEL])
	{
		keys[KEY_CANCEL] = 0;

		if (displayHelp)
		{
			displayHelp = 0;
		}
		else if (displayCredits)
		{
			displayCredits = 0;
		}
		else
		{
			if (curMenu == &menuNewGame)
			{
				curMenu = &menuMain;
				menuSel = savePresent ? 0 : 1;
			}
			else
			{
				menuSel = curMenu->length - 1;
			}
		}
	}
	if(keys[KEY_OK])
	{
		keys[KEY_OK] = 0;

		if (displayHelp)
		{
			if(++helpPage > 1)
			{
				helpPage = 0;
				displayHelp = 0;
			}
		}
		else if (displayCredits)
		{
			if(++creditsPage > 1)
			{
				creditsPage = 0;
				displayCredits = 0;
			}
		}
		else
		{
			switch (curMenu->items[menuSel])
			{
				case MENU_BACK:
					if (curMenu == &menuNewGame)
					{
						curMenu = &menuMain;
						menuSel = savePresent ? 0 : 1;
					}
				break;
				case MENU_CONTINUE:
					if (savePresent)
					{
						programStateNew = STATE_GAME;
						continueGame = 1;
					}
				break;

				case MENU_START_GAME:
					programStateNew = STATE_GAME;
					continueGame = 0;
					practice = 0;
				break;

				case MENU_PRACTICE:
					programStateNew = STATE_GAME;
					continueGame = 0;
					practice = 1;
				break;

				case MENU_NEW_GAME:
					curMenu = &menuNewGame;
					menuSel = 0;
				break;

				case MENU_GAME_TYPE:
					if (++newGameMode >= GAME_MODE_COUNT)
					{
						newGameMode = GAME_MODE_UNSET+1;
					}
				break;

				case MENU_ALGORITHM:
					if (++currentAlgorithm >= ALGO_COUNT)
					{
						currentAlgorithm = ALGO_UNSET+1;
					}
				break;

				case MENU_HISCORE:
					programStateNew = STATE_HISCORE;
				break;

				case MENU_HELP:
					displayHelp = 1;
				break;

				case MENU_CREDITS:
					displayCredits = 1;
				break;

				case MENU_QUIT:
					quit = 1;
				break;

				default:
				break;
			}
		}
	}

	if(keys[KEY_LEFT])
	{
		keys[KEY_LEFT] = 0;

		if (displayHelp)
		{
			if (--helpPage < 0)
			{
				helpPage = 0;
			}
		}
		else if (displayCredits)
		{
			if (--creditsPage < 0)
			{
				creditsPage = 0;
			}
		}
		else if (curMenu->items[menuSel] == MENU_GAME_TYPE && --newGameMode == GAME_MODE_UNSET)
		{
			newGameMode = GAME_MODE_COUNT - 1;
		}
		else if (curMenu->items[menuSel] == MENU_ALGORITHM && --currentAlgorithm == ALGO_UNSET)
		{
			currentAlgorithm = ALGO_COUNT - 1;
		}
	}

	if(keys[KEY_RIGHT])
	{
		keys[KEY_RIGHT] = 0;

		if (displayHelp)
		{
			if (++helpPage > 1)
			{
				helpPage = 1;
			}
		}
		if (displayCredits)
		{
			if (++creditsPage > 1)
			{
				creditsPage = 1;
			}
		}
		else if (curMenu->items[menuSel] == MENU_GAME_TYPE && ++newGameMode >= GAME_MODE_COUNT)
		{
			newGameMode = GAME_MODE_UNSET + 1;
		}
		else if (curMenu->items[menuSel] == MENU_ALGORITHM && ++currentAlgorithm >= ALGO_COUNT)
		{
			currentAlgorithm = ALGO_UNSET + 1;
		}
	}

	if (!displayHelp)
	{
		if(keys[KEY_UP])
		{
			keys[KEY_UP] = 0;

			--menuSel;

			if (curMenu->items[menuSel] == MENU_SEPARATOR)
			{
				--menuSel;
			}

			if(menuSel < (savePresent ? 0 : 1))
			{
				menuSel = curMenu->length - 1;
			}
		}

		if(keys[KEY_DOWN])
		{
			keys[KEY_DOWN] = 0;

			++menuSel;

			if (curMenu->items[menuSel] == MENU_SEPARATOR)
			{
				++menuSel;
			}

			if(menuSel > curMenu->length - 1)
			{
				menuSel = savePresent ? 0 : 1;
			}
		}
	}
}

void titleDraw()
{
	int i;

	drawImage(titleBackgroundIMG, NULL, screen, 0, 0);

	if (displayHelp)
	{
		char page[10] = "";
		char helpHeader[2][10] = { "Rules", "Controls" };
		char helpText[2][500] = { "The goal of Shisen-Sho is to remove\nall stones from the board. Do so\nby matching pairs of stones with\nthe same face.\n\nYou can only match stones if they\ncan be joined by 3 or less lines.\nEach line must be either horizontal\nor vertical and can't cross other\nstones on the board.\n\nNot all games are solvable.", "D-PAD  - move selection\nA      - select a stone\nB      - remove selection\nSELECT - exit game" };

		strcpy(page, helpPage ? "[Page 2]" : "[Page 1]");

		dText(&gameFontBlack, page, 2, 2, SHADOW_NONE);
		dTextCentered(&gameFontRegular, helpHeader[helpPage], 50 - (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);
		dTextCentered(&gameFontBlack, helpText[helpPage], 50, SHADOW_NONE);
	}
	else if (displayCredits)
	{
		char page[10] = "";
		char creditsText[2][10][500] =
		{
			{ "Programming:",
			  "Artur Rojek",
			  "",
			  "Graphics:",
			  "Joe Vargas",
			  "",
			  "",
			  "",
			  "",
			  ""
			},
			{ "Music:",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  ""
			}
		};

		strcpy(page, creditsPage ? "[Page 2]" : "[Page 1]");

		dText(&gameFontBlack, page, 2, 2, SHADOW_NONE);
		dTextCentered(&gameFontRegular, "Credits", 50 - (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);

		for (i = 0; i < 10; ++i)
		{
			dTextCentered(&gameFontBlack, creditsText[creditsPage][i], 50 + (gameFontRegular.h + gameFontRegular.leading) * i, SHADOW_NONE);
		}
	}
	else
	{
		char version[15];
		sprintf(version, "v%d.%d.%d", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
		dText(&gameFontBlack, version, SCREEN_W - strlen(version) * (gameFontSelected.w + gameFontSelected.tracking), 1, SHADOW_NONE);
		dTextCentered(&gameFontRegular, "S h i s e n - S e k i", 40, SHADOW_OUTLINE);

		if (programStateNew == STATE_GAME)
		{
			dTextCentered(&gameFontRegular, "* Generating new board *", SCREEN_H/2, SHADOW_OUTLINE);
			dTextCentered(&gameFontRegular, "Please wait...", SCREEN_H/2 + (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);
		}
		else
		{
			if (curMenu == &menuNewGame)
			{
				switch (newGameMode)
				{
					case GAME_MODE_CLASSIC:
						strncpy(menuText[10], "Game type: classic", 20);
					break;
					case GAME_MODE_GRAVITY:
						strncpy(menuText[10], "Game type: gravity", 20);
					break;

					default:
					break;
				}

				switch (currentAlgorithm)
				{
					case ALGO_RANDOM:
						strncpy(menuText[11], "Algorithm: random", 20);
					break;
					case ALGO_REVERSE:
						strncpy(menuText[11], "Algorithm: reverse", 20);
					break;

					default:
					break;
				}
			}

			menuDraw(curMenu, &gameFontRegular, &gameFontSelected, menuSel, savePresent ? 0 : 1, 80);
		}

		dTextCentered(&gameFontSelected, "(c) 2015 Artur Rojek", SCREEN_H - (gameFontSelected.h + gameFontSelected.leading), SHADOW_DROP);

	}
}
