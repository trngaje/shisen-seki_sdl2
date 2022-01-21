#include "states.h"

#include <SDL.h>
#include "game.h"
#include "hiscore.h"
#include "main.h"
#include "scaler.h"
#include "title.h"
#include "video.h"

State programStateActive = STATE_NONE;
State programStateNew = STATE_TITLE;
#ifdef OGS_SDL2
extern SDL_Window* sdlWindow;
extern SDL_Surface* sdlSurface;
#endif

void checkState()
{
	if(programStateActive != programStateNew)
	{
		// Unload current state.
		switch(programStateActive)
		{
			case STATE_TITLE:
				titleUnload();
			break;
			case STATE_GAME:
				gameUnload();
			break;
			case STATE_HISCORE:
				hiscoreUnload();
			break;

			default:
			break;
		}
		// Load new state.
		switch(programStateNew)
		{
			case STATE_TITLE:
				titleLoad();
			break;
			case STATE_GAME:
				gameLoad();
			break;
			case STATE_HISCORE:
				hiscoreLoad();
			break;

			default:
			break;
		}

		programStateActive = programStateNew;
	}
}

void logic()
{
	checkState();

	switch(programStateActive)
	{
		case STATE_TITLE:
			titleLogic();
		break;
		case STATE_GAME:
			gameLogic();
		break;
		case STATE_HISCORE:
			hiscoreLogic();
		break;

		default:
		break;
	}
}

void draw()
{
	switch(programStateActive)
	{
		case STATE_TITLE:
			titleDraw();
		break;
		case STATE_GAME:
			gameDraw();
		break;
		case STATE_HISCORE:
			hiscoreDraw();
		break;

		default:
		break;
	}

	switch (scale)
	{
		case 1:
		break;
		case 2:
			upscale2((uint32_t *)screenScaled->pixels, (uint32_t *)screen->pixels);
		break;
	}

#ifdef OGS_SDL2
	// for test
	//SDL_FillRect(screenScaled, NULL, SDL_MapRGB(screenScaled->format, 255, 0, 0));
	SDL_BlitScaled(screenScaled, NULL, sdlSurface, NULL);
	SDL_UpdateWindowSurface(sdlWindow);
#else
	SDL_Flip(screenScaled);
#endif
}
