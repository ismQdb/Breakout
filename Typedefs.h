#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "IncludeHeader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FRAME_RATE 60.0

#define KEY_SEEN     1
#define KEY_RELEASED 2

#define BALL_START_X 400
#define BALL_START_Y 530

#define BLOCK_COUNT 20
#define BLOCK_ROWS 4

typedef struct Rectangle {
    int x1, x2;
    int y1, y2;
    int dx;
} Rectangle;

typedef struct Ball {
    int cx;
    int cy;
    int radius;
    int dx;
    int dy;
    unsigned int lives;
    int isDead;
    int livesAsChar;
    ALLEGRO_COLOR* color;
} Ball;

typedef struct Block {
    int x1;
    int y1;
    int x2;
    int y2;
    int active;
    ALLEGRO_COLOR color;
} Block;

typedef struct BlockCollection {
    Block* blocks[BLOCK_ROWS][BLOCK_COUNT];
} BlockCollection;

typedef struct GameObject {
    Rectangle* rect;
    Ball* ball;
    BlockCollection* blockCollection;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT* event;
    ALLEGRO_KEYBOARD_STATE* keyboardState;
    unsigned char key[ALLEGRO_KEY_MAX];
    int done;
    int redraw;
} GameObject;

#endif // !Typedefs.H
