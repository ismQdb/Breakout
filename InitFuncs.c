#include "IncludeHeader.h"

void mustInit(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void InitFunctions(GameObject* gameObject) {
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");

    gameObject->timer = al_create_timer(1.0 / FRAME_RATE);
    mustInit(gameObject->timer, "timer");

    gameObject->eventQueue = al_create_event_queue();
    mustInit(gameObject->eventQueue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    gameObject->display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    mustInit(gameObject->display, "display");

    gameObject->font = al_create_builtin_font();
    mustInit(gameObject->font, "font");

    mustInit(al_init_primitives_addon(), "primitives");

    mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "audio codecs");
    mustInit(al_reserve_samples(16), "reserve samples");

    al_register_event_source(gameObject->eventQueue, al_get_keyboard_event_source());
    al_register_event_source(gameObject->eventQueue, al_get_display_event_source(gameObject->display));
    al_register_event_source(gameObject->eventQueue, al_get_timer_event_source(gameObject->timer));
}

void initGameObject(GameObject* gameObject) {
    gameObject->rect = (Rectangle*)malloc(sizeof(Rectangle));
    gameObject->ball = (Ball*)malloc(sizeof(Ball));
    gameObject->blockCollection = (BlockCollection*)malloc(sizeof(BlockCollection));

    for (int i = 0; i < BLOCK_COUNT * BLOCK_ROWS; i++)
        gameObject->blockCollection->blocks[i] = (Block*)malloc(sizeof(Block));

    gameObject->timer = (ALLEGRO_TIMER*)malloc(sizeof(ALLEGRO_TIMER*));
    gameObject->eventQueue = (ALLEGRO_EVENT_QUEUE*)malloc(sizeof(ALLEGRO_EVENT_QUEUE*));
    gameObject->display = (ALLEGRO_DISPLAY*)malloc(sizeof(ALLEGRO_DISPLAY*));
    gameObject->font = (ALLEGRO_FONT*)malloc(sizeof(ALLEGRO_FONT*));
    gameObject->event = (ALLEGRO_EVENT*)malloc(sizeof(ALLEGRO_EVENT*));
    gameObject->keyboardState = (ALLEGRO_KEYBOARD_STATE*)malloc(sizeof(ALLEGRO_KEYBOARD_STATE*));
}

void initRect(GameObject* gameObject) {
    Rectangle* rect = gameObject->rect;

    gameObject->rect->x1 = 350;
    rect->y1 = 540;
    rect->x2 = 450;
    rect->y2 = 550;
    rect->dx = 10;
}

void initBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;

    ball->isDead = 1;
    ball->cx = BALL_START_X;
    ball->cy = BALL_START_Y;
    ball->radius = 10;
    ball->dx = 5 * randMultiplier();
    ball->dy = -5;
    ball->lives = 3;
    ball->livesAsChar = 51;
}

void initBlock1s(GameObject* gameObject) {
    BlockCollection* blockCollection = gameObject->blockCollection;

    double blockWidth = 25;
    double blockHeight = 15;

    double blankWidth = (800 - BLOCK_COUNT * 25) / (BLOCK_COUNT + 1);
    double blankHeight = blockWidth / 2;
    double currentXPos = 0;

    for (int i = 0; i < BLOCK_COUNT; i++) {
        currentXPos += blankWidth;
        blockCollection->blocks[i]->x1 = currentXPos;
        blockCollection->blocks[i]->x2 = currentXPos + blockWidth;
        blockCollection->blocks[i]->y1 = 100;
        blockCollection->blocks[i]->y2 = 100 + blockHeight;
        blockCollection->blocks[i]->color = al_map_rgb(255, 255, 255);
        blockCollection->blocks[i]->active = 1;

        currentXPos += blockWidth;
    }
}

void initBlocks(GameObject* gameObject) {
    BlockCollection* blockCollection = gameObject->blockCollection;

    double blockWidth = 25;
    double blockHeight = 15;

    double blankWidth = (800 - BLOCK_COUNT * 25) / (BLOCK_COUNT + 1);
    double blankHeight = blockHeight / 2;

    double currentXPos = 0;
    double currentYPos = 100;

    for (int row = 0; row < BLOCK_ROWS; row++) {
        currentXPos = 0;

        for (int i = 0; i < BLOCK_COUNT; i++) {
            currentXPos += blankWidth;

            blockCollection->blocks[20 * row + i]->x1 = currentXPos;
            blockCollection->blocks[20 * row + i]->x2 = currentXPos + blockWidth;
            blockCollection->blocks[20 * row + i]->y1 = currentYPos;
            blockCollection->blocks[20 * row + i]->y2 = currentYPos + blockHeight;
            blockCollection->blocks[20 * row + i]->color = al_map_rgb(255, 255, 255);
            blockCollection->blocks[20 * row + i]->active = 1;

            currentXPos += blockWidth;
        }

        currentYPos += blankHeight += blockHeight;
    }
}