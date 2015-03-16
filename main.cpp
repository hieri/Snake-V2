#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };

SDL_Window *gameWindow{ NULL };
SDL_Renderer *gameRenderer{ NULL };

LTexture headTexture;

// load texture class
class LTexture
{
public:

	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void render(int x, int y, SDL_Rect *clip = NULL);
private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};

class SnakeHead
{

public:
	static const int SNAKEHEAD_WIDTH = 8;
	static const int SNAKEHEAD_HEIGHT = 8;

	SnakeHead();
	~SnakeHead();
	void handleEvent(SDL_Event& e);
	void move();
	void render();

private:
	int mPosX, mPosY;
};

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

SnakeHead::SnakeHead()
{
	mPosX = 0;
	mPosX = 0;
}

void SnakeHead::handleEvent(SDL_Event& e)
{
	int x, y;
	//If mouse move
	if (e.type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&x, &y);
	}
}

void SnakeHead::move()
{
	//Move the dot left or right
	mPosX += mVelX;

	//If the dot went too far to the left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH))
	{
		//Move back
		mPosX -= mVelX;
	}

	//Move the dot up or down
	mPosY += mVelY;

	//If the dot went too far up or down
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT))
	{
		//Move back
		mPosY -= mVelY;
	}
}

bool LTexture::loadFromFile(std::string path)
{
	using std::cout;
	//destroy any previous texture
	free();

	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load: " << path.c_str() << ", SDL_imgage Error: " << IMG_GetError();
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from: " << path.c_str() << ", SDL_imgage Error: " << IMG_GetError();
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;

}

void LTexture::render(int x, int y, SDL_Rect *clip)
{
	SDL_Rect renderQuad{ x, y, mWidth, mHeight };

	//NULL gets entire texture
	SDL_RenderCopy(gameRenderer, mTexture, NULL, &renderQuad);

}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}

}

//starts SDL and creates window/renderer
bool init();

//loads the media
bool loadMedia();

//closes SDL and destroys window/renderer
void close();

bool init()
{
	bool success{ true };
	using std::cout;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Failed! SDL Error: " << SDL_GetError() << "/n";
		success = false;
	}
	else
	{
		gameWindow = SDL_CreateWindow(
			"Snake",					// window title
			SDL_WINDOWPOS_UNDEFINED,	// initial x position
			SDL_WINDOWPOS_UNDEFINED,    // initial y position
			SCREEN_WIDTH,               // width, in pixels
			SCREEN_HEIGHT,              // height, in pixels
			SDL_WINDOW_SHOWN
			);
		if (gameWindow == NULL)
		{
			cout << "Window not be created! SDL Error: " << SDL_GetError() << "/n";
			success = false;
		}
		else
		{
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gameRenderer == NULL)
			{
				cout << "Renderer not be created! SDL Error: " << SDL_GetError() << "/n";
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
			}
		}
		return success;

	}
}

bool loadMedia()
{
	bool success{ true };
	if (!headTexture.loadFromFile("snakehead.bmp"))
	{
		std::cout << "Failed to load texture! SDL Error: " << SDL_GetError() << "/n";
		success = false;
	}
}

void close()
{
	headTexture.free();

	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	SDL_Quit();
}

int main(int argc, char ** argv)
{

	init();
	SDL_ShowCursor(SDL_DISABLE);
	bool quit{ false };
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			//user requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//set the color of the renderer
		SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		//clear the renderer to the color
		SDL_RenderClear(gameRenderer);

		//show the render
		SDL_RenderPresent(gameRenderer);
	}

	close();
	return 0;
}