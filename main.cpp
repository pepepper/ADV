#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <tchar.h>

void SDL_UTF8Render(SDL_Renderer *render, TTF_Font *font, const char *text, int r, int g, int b, int x, int y) {
	SDL_Color color = { r, g, b };
	SDL_Surface *textsurface;
	SDL_Texture *texture;
	SDL_Rect rect;
	textsurface = TTF_RenderUTF8_Blended(font, text, color);
	texture = SDL_CreateTextureFromSurface(render, textsurface);
	SDL_FreeSurface(textsurface);
	TTF_SizeUTF8(font, text, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;
	SDL_RenderCopy(render, texture, NULL, &rect);
}

SDL_Texture* IMG_Load_Texture(SDL_Renderer *renderer, const char *imgname) {
	SDL_Surface *image;
	SDL_Texture *texture;
	image = IMG_Load(imgname);
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	return texture;
}

void deinit(SDL_Renderer *renderer, TTF_Font *font, SDL_Window *window) {
	if (renderer)SDL_DestroyRenderer(renderer);
	if (font)TTF_CloseFont(font);
	if (window)SDL_DestroyWindow(window);					  // ウィンドウを閉じて破棄する
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int init() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Can't init SDL!\nerror:%s\n", SDL_GetError());
		return -1;
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
		printf("Can't init SDL_img!\nerror:%s\n", IMG_GetError());
		return -1;
	}
	if (TTF_Init()) {
		printf("Can't init SDL_TTF!\nerror:%s\n", TTF_GetError());
		return -1;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	SDL_Window *window = 0;                    // ポインタを宣言する
	SDL_Texture *bg = 0, *text = 0;
	TTF_Font *font = 0;
	SDL_Renderer *renderer = 0;
	SDL_Rect rect = { 0, 336, 640, 160 };
	SDL_Event event;
	char errortext[2048];
	const SDL_MessageBoxButtonData buttons[] = { { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "はい" } };
	const SDL_MessageBoxColorScheme colorScheme = { {
		{ 255, 255, 255 },
	{ 0, 0, 0 },
	{ 255, 255, 0 },
	{ 0, 0, 255 },
	{ 255, 0, 255 }
		} };

	if (init()) {
		deinit(renderer, font, window);
		return -1;
	}
	SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
	if (window == NULL) {// ウィンドウの生成に成功したかチェックする
		sprintf(errortext, "Can't create window!\nerror:%s\n", SDL_GetError());		// ここを通ったならばウィンドウを生成できなかった...
		const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, NULL, "エラー", errortext, SDL_arraysize(buttons), buttons, &colorScheme };
		SDL_ShowMessageBox(&messageboxdata, NULL);
		return -1;
	}
	SDL_SetWindowTitle(window, "ADV System");
	// ウィンドウが開いた: ここでプログラムループに入る (SDL_PollEvent()を参照すること)
	font = TTF_OpenFont("uzura.ttf", 22);
	bg = IMG_Load_Texture(renderer, ".\\opening.png");
	if (!bg) {
		sprintf(errortext, "Can't load image!\nerror:%s\n", IMG_GetError());
		const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, NULL, "エラー", errortext, SDL_arraysize(buttons), buttons, &colorScheme };
		SDL_ShowMessageBox(&messageboxdata, NULL);
	}
	SDL_RenderCopy(renderer, bg, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);

	SDL_UTF8Render(renderer, font, u8"千春", 0, 0, 0, 10, 346);
	SDL_RenderPresent(renderer);

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
	}
	deinit(renderer, font, window);
	return 0;
}