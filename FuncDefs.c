#include "IncludeHeader.h"

void GameLoop(GameObject* gameObject) {
    al_start_timer(gameObject->timer);

    while (1)
    {
        al_wait_for_event(gameObject->eventQueue, gameObject->event);

        al_get_keyboard_state(gameObject->keyboardState);

        switch (gameObject->event->type)
        {
            case ALLEGRO_EVENT_TIMER:
                if (gameObject->key[ALLEGRO_KEY_LEFT])
                    if ((gameObject->rect->x1 - gameObject->rect->dx) > 0) {
                        gameObject->rect->x1 -= gameObject->rect->dx;
                        gameObject->rect->x2 -= gameObject->rect->dx;
                    }
                if (gameObject->key[ALLEGRO_KEY_RIGHT])
                    if ((gameObject->rect->x2 + gameObject->rect->dx) < 800) {
                        gameObject->rect->x1 += gameObject->rect->dx;
                        gameObject->rect->x2 += gameObject->rect->dx;
                    }

                if(!gameObject->ball->isDead)
                    moveTheBall(gameObject);

                if (gameObject->key[ALLEGRO_KEY_ESCAPE])
                    gameObject->done = true;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    gameObject->key[i] &= KEY_SEEN;

                gameObject->redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                gameObject->key[gameObject->event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                gameObject->key[gameObject->event->keyboard.keycode] &= KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                gameObject->done = true;
                break;
        }

        if (gameObject->done)
            break;

        if (gameObject->redraw && al_is_event_queue_empty(gameObject->eventQueue))
        {
            char liveString[10] = "Lives: ";
            liveString[7] = gameObject->ball->livesAsChar;
            liveString[8] = '\0';

            gameObject->ball->isDead = 0;

            if (gameObject->ball->lives == 0) {
                al_rest(2);

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(gameObject->font, al_map_rgb(255, 255, 255),
                    SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2, 0, "Game over.");
                al_flip_display();
                al_rest(5);
                gameObject->done = 1;
            }
            else {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                drawBlocks(gameObject);
                al_draw_filled_rectangle(gameObject->rect->x1, gameObject->rect->y1, gameObject->rect->x2, gameObject->rect->y2, al_map_rgba_f(1, 1, 1, 0.5));
                al_draw_filled_circle(gameObject->ball->cx, gameObject->ball->cy, gameObject->ball->radius, al_map_rgba_f(1, 1, 1, 0));
                al_draw_text(gameObject->font, al_map_rgb(255, 255, 255), 0, 0, 0, liveString);
                al_flip_display();
            }

            gameObject->redraw = false;

        }
    }
}

void drawBlocks(GameObject* gameObject) {
    BlockCollection* blockCollection = gameObject->blockCollection;
    int numOfBlocks = BLOCK_COUNT;

    for(unsigned row = 0; row < BLOCK_ROWS; row++)
        for (unsigned column = 0; column < BLOCK_COUNT; column)
            if (blockCollection->blocks[row][column]->active)
                al_draw_filled_rectangle(
                    blockCollection->blocks[row][column]->x1,
                    blockCollection->blocks[row][column]->y1,
                    blockCollection->blocks[row][column]->x2,
                    blockCollection->blocks[row][column]->y2,
                    blockCollection->blocks[row][column]->color
                );
}

int randBetween(int lo, int hi) {
    return lo + (rand() % (hi - lo));
}

int randMultiplier() {
    int arr[2];
    arr[0] = 1;
    arr[1] = -1;

    srand(time(0));
    return arr[rand() % 2];
}

int ballBlockCollision(Ball* ball, Block* block) {
    int returnResult = 0;

    if (((ball->cx - ball->radius) >= block->x1) && (ball->cx - ball->radius) <= block->x2)
        if ((ball->cy >= block->y1) && ball->cy <= block->y2)
            if (block->active) {
                ball->dx *= -1;
                returnResult = 1;
            }

    if (((ball->cx + ball->radius) >= block->x1) && (ball->cx + ball->radius) <= block->x2)
        if ((ball->cy >= block->y1) && ball->cy <= block->y2)
            if (block->active) {
                ball->dx *= -1;
                returnResult = 1;
            }

    if(((ball->cy - ball->radius) >= block->y1) && (ball->cy - ball->radius) <= block->y2)
        if (ball->cx >= block->x1 && ball->cx <= block->x2) 
            if(block->active){
                ball->dy *= -1;
                returnResult = 1;
            }

    if (((ball->cy + ball->radius) >= block->y1) && (ball->cy + ball->radius) <= block->y2)
        if (ball->cx >= block->x1 && ball->cx <= block->x2) 
            if(block->active){
                ball->dy *= -1;
                returnResult = 1;
            }

    return returnResult;
}

void blocksRemoval(GameObject* gameObject) {
    Ball* ball = gameObject->ball;
    BlockCollection* blockCollection = gameObject->blockCollection;

    for (unsigned row = 0; row < BLOCK_ROWS; row++)
        for (unsigned column = 0; column < BLOCK_COUNT; column)
            if (ballBlockCollision(gameObject->ball, gameObject->blockCollection->blocks[row][column]))
                blockCollection->blocks[row][column]->active = 0;

}

int ballRectCollision(GameObject* gameObject) {
    Ball* ball = gameObject->ball;
    Rectangle* rect = gameObject->rect;

    if (((ball->cy + ball->radius) > rect->y1) && (
        ((ball->cx + ball->radius) < rect->x1)
        || ((ball->cx - ball->radius) > rect->x2)))
        return 0;
    else
        return 1;
}

void moveTheBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;
    Rectangle* rect = gameObject->rect;

    ball->cx += ball->dx;
    ball->cy += ball->dy;

    if ((ball->cx - ball->radius) < 0)
        ball->dx *= -1;

    if ((ball->cx + ball->radius) > SCREEN_WIDTH) {
        ball->cx -= ((ball->cx - SCREEN_WIDTH + ball->radius));
        ball->dx *= -1;
    }

    if ((ball->cy - ball->radius) < 0)
        ball->dy *= -1;

    blocksRemoval(gameObject);

    // No collision, spawn the ball at the middle
    if ((ball->cy + ball->radius) > rect->y1) {
        if (ballRectCollision(gameObject))
            ball->dy *= -1;
        else
            respawnBall(gameObject);
    }
}

void respawnBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;

    al_rest(1);
    ball->isDead = 1;
    ball->lives--;
    ball->livesAsChar--;
    ball->cx = BALL_START_X;
    ball->cy = BALL_START_Y;
    ball->dx = 5 * randMultiplier();
    ball->dy = -5;
    initRect(gameObject);
}