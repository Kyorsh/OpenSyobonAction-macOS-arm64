#include <locale.h>
#include "DxLib.h"

SDL_Joystick* joystick;
SDL_Window *window;
SDL_Renderer *renderer;

bool keysHeld[512];
bool sound = true;
void deinit();
int DxLib_Init()
{
    atexit(deinit);
    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	return -1;
    }

    window = SDL_CreateWindow("Syobon Action (しょぼんのアクション)",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             480, 420, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    screen = SDL_CreateRGBSurface(0, 480, 420, 32, 0, 0, 0, 0);
    if (!screen) {
        fprintf(stderr, "Unable to create surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        fprintf(stderr, "Unable to init SDL_img: %s\n", IMG_GetError());
        return -1;
    }

    //Initialize font
    if (TTF_Init() == -1) {
	fprintf(stderr, "Unable to init SDL_ttf: %s\n", TTF_GetError());
	return -1;
    }

    //Audio Rate, Audio Format, Audio Channels, Audio Buffers
#define AUDIO_CHANNELS 2
    if (sound && Mix_OpenAudio(22050, AUDIO_S16SYS, AUDIO_CHANNELS, 1024)) {
        fprintf(stderr, "Unable to init SDL_mixer: %s\n", Mix_GetError());
        sound = false;
        }
    //Try to get a joystick
    joystick = SDL_JoystickOpen(0);

    for (int i = 0; i < 512; i++)
	keysHeld[i] = false;
    for (int i = 0; i < FONT_MAX; i++)
	font[i] = NULL;
    srand(time(NULL));

    return 0;
}

//Main screen
SDL_Surface *screen;

//Fonts
byte fontsize = 0;
TTF_Font *font[FONT_MAX];

//Strings
void SetFontSize(byte size)
{
    fontsize = size;
    if (font[size] == NULL) {
	font[size] = TTF_OpenFont("res/sazanami-gothic.ttf", size);
	if (font[size] == NULL) {
	    printf("Unable to load font: %s\n", TTF_GetError());
	    exit(1);
	}
    }
}

byte fontType = DX_FONTTYPE_NORMAL;
void ChangeFontType(byte type)
{
    fontType = type;
}

void DrawString(int a, int b, const char *x, Uint32 c)
{
    SDL_Color color = { c >> 16, c >> 8, c };
    SDL_Surface *rendered = TTF_RenderUTF8_Solid(font[fontsize], x, color);
    if (fontType == DX_FONTTYPE_EDGE) {
	SDL_Color blk = { 0, 0, 0 };
	SDL_Surface *shadow = TTF_RenderUTF8_Solid(font[fontsize], x, blk);
	DrawGraphZ(a - 1, b - 1, shadow);
	DrawGraphZ(a, b - 1, shadow);
	DrawGraphZ(a + 1, b - 1, shadow);
	DrawGraphZ(a - 1, b, shadow);
	DrawGraphZ(a + 1, b, shadow);
	DrawGraphZ(a - 1, b + 1, shadow);
	DrawGraphZ(a, b + 1, shadow);
	DrawGraphZ(a + 1, b + 1, shadow);
	SDL_FreeSurface(shadow);
    }
    DrawGraphZ(a, b, rendered);
    SDL_FreeSurface(rendered);
}

void DrawFormatString(int a, int b, Uint32 color, const char *str, ...)
{
    va_list args;
    char *newstr = new char[strlen(str) + 16];
    va_start(args, str);
    vsprintf(newstr, str, args);
    va_end(args);
    DrawString(a, b, newstr, color);
    delete newstr;
}

//void DrawFormatString(int a, int b, int c

//Key Aliases
#define KEY_INPUT_ESCAPE SDLK_ESCAPE

bool ex = false;

void UpdateKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_KEYDOWN:
	    keysHeld[event.key.keysym.sym] = true;
	    break;
	case SDL_KEYUP:
	    keysHeld[event.key.keysym.sym] = false;
	    break;
	case SDL_CONTROLLERAXISMOTION:
	    if(event.caxis.which == 0)
	    {
		if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
		    if(event.caxis.value < -16384) keysHeld[SDLK_LEFT] = true;
		    else if(event.caxis.value > 16384) keysHeld[SDLK_RIGHT] = true;
		    else {
			keysHeld[SDLK_LEFT] = false;
			keysHeld[SDLK_RIGHT] = false;
		    }
		}
		else if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		{
		    if(event.caxis.value < -16384) keysHeld[SDLK_UP] = true;
		    else if(event.caxis.value > 16384) keysHeld[SDLK_DOWN] = true;
		    else {
			keysHeld[SDLK_UP] = false;
			keysHeld[SDLK_DOWN] = false;
		    }
		}
	    }
	    break;
	case SDL_QUIT:
	    ex = true;
	    break;
	}
    }
}

byte ProcessMessage()
{
    return ex;
}

byte CheckHitKey(int key)
{
    if(key == SDLK_z && keysHeld[SDLK_SEMICOLON]) return true;
    return keysHeld[key];
}

byte WaitKey()
{
    SDL_Event event;
    while (true) {
	while (SDL_PollEvent(&event))
	    if (event.type == SDL_KEYDOWN)
		return event.key.keysym.sym;
    }
}

/*Uint32 GetColor(byte r, byte g, byte b)
{
    return r << 8 * 3 | g << 8 * 2 | b << 8 | 0xFF;
}*/

void DrawGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;
        SDL_BlitSurface(mx, NULL, screen, &offset);
    }
}

void DrawTurnGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;

        SDL_Surface *flipped = zoomSurface(mx, -1, 1, 0);
        SDL_SetColorKey(flipped, SDL_TRUE,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

void DrawVertTurnGraph(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a - mx->w / 2;
        offset.y = b - mx->h / 2;

        SDL_Surface *flipped = rotozoomSurface(mx, 180, 1, 0);
        SDL_SetColorKey(flipped, SDL_TRUE,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

SDL_Surface *DerivationGraph(int srcx, int srcy, int width, int height,
			     SDL_Surface * src)
{
    SDL_Surface *img =
	SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    SDL_Rect offset;
    offset.x = srcx;
    offset.y = srcy;
    offset.w = width;
    offset.h = height;

    SDL_BlitSurface(src, &offset, img, NULL);
    SDL_SetColorKey(img, SDL_TRUE,
		    SDL_MapRGB(img->format, 9 * 16 + 9, 255, 255));
    return img;
}

//Noticably different than the original
SDL_Surface *LoadGraph(const char *filename)
{
    SDL_Surface *image = IMG_Load(filename);

    if (image) return image;
	fprintf(stderr, "Error: Unable to load %s: %s\n", filename, IMG_GetError());
	exit(1);
}

void PlaySoundMem(Mix_Chunk* s, int l)
{
    if(sound) Mix_PlayChannel(-1, s, l);
}

Mix_Chunk* LoadSoundMem(const char* f)
{
    if(!sound) return NULL;

    Mix_Chunk* s = Mix_LoadWAV(f);
    if(s) return s;
    fprintf(stderr, "Error: Unable to load sound %s: %s\n", f, Mix_GetError());
    return NULL;
}

Mix_Music* LoadMusicMem(const char* f)
{
    if(!sound) return NULL;

    Mix_Music* m = Mix_LoadMUS(f);
    if(m) return m;
    fprintf(stderr, "Error: Unable to load music %s: %s\n", f, Mix_GetError());
    return NULL;
}

void ScreenFlip()
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

// SDL2_gfx wrapper functions for surface-based drawing
int pixelColor(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color)
{
    if (!dst || x < 0 || x >= dst->w || y < 0 || y >= dst->h) return -1;
    
    // Lock the surface if needed
    if (SDL_MUSTLOCK(dst)) {
        if (SDL_LockSurface(dst) < 0) return -1;
    }
    
    // Convert color to the surface's format
    Uint8 r, g, b, a;
    SDL_GetRGBA(color, dst->format, &r, &g, &b, &a);
    Uint32 mapped_color = SDL_MapRGBA(dst->format, r, g, b, a);
    
    // Calculate pixel position based on surface format
    int bpp = dst->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)dst->pixels + y * dst->pitch + x * bpp;
    
    switch (bpp) {
        case 1:
            *p = mapped_color;
            break;
        case 2:
            *(Uint16 *)p = mapped_color;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (mapped_color >> 16) & 0xff;
                p[1] = (mapped_color >> 8) & 0xff;
                p[2] = mapped_color & 0xff;
            } else {
                p[0] = mapped_color & 0xff;
                p[1] = (mapped_color >> 8) & 0xff;
                p[2] = (mapped_color >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = mapped_color;
            break;
    }
    
    if (SDL_MUSTLOCK(dst)) {
        SDL_UnlockSurface(dst);
    }
    return 0;
}

int lineColor(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    if (!dst) return -1;
    
    // Simple Bresenham line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        pixelColor(dst, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return 0;
}

int rectangleColor(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    if (!dst) return -1;
    
    lineColor(dst, x1, y1, x2, y1, color);
    lineColor(dst, x2, y1, x2, y2, color);
    lineColor(dst, x2, y2, x1, y2, color);
    lineColor(dst, x1, y2, x1, y1, color);
    return 0;
}

int boxColor(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    if (!dst) return -1;
    
    for (int y = y1; y <= y2; y++) {
        lineColor(dst, x1, y, x2, y, color);
    }
    return 0;
}

int ellipseColor(SDL_Surface *dst, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color)
{
    if (!dst || rx <= 0 || ry <= 0) return -1;
    
    // Simple ellipse drawing algorithm
    for (int angle = 0; angle < 360; angle++) {
        double rad = angle * M_PI / 180.0;
        int x = xc + rx * cos(rad);
        int y = yc + ry * sin(rad);
        pixelColor(dst, x, y, color);
    }
    return 0;
}

int filledEllipseColor(SDL_Surface *dst, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color)
{
    if (!dst || rx <= 0 || ry <= 0) return -1;
    
    // Simple filled ellipse drawing
    for (int y = yc - ry; y <= yc + ry; y++) {
        for (int x = xc - rx; x <= xc + rx; x++) {
            double dx = (double)(x - xc) / rx;
            double dy = (double)(y - yc) / ry;
            if (dx * dx + dy * dy <= 1.0) {
                pixelColor(dst, x, y, color);
            }
        }
    }
    return 0;
}

void deinit()
{
    for (int i = 0; i < FONT_MAX; i++)
	if (font[i] != NULL)
	    TTF_CloseFont(font[i]);
    if (joystick)
	SDL_JoystickClose(joystick);
    if (sound) {
	Mix_CloseAudio();
    }
    TTF_Quit();
    IMG_Quit();
    if (screen)
	SDL_FreeSurface(screen);
    if (renderer)
	SDL_DestroyRenderer(renderer);
    if (window)
	SDL_DestroyWindow(window);
    SDL_Quit();
}

