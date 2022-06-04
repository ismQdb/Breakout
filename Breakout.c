#include "IncludeHeader.h"
#include "FuncDefs.h"
#include "InitFuncs.h"

int main()
{
    GameObject* gameObject = (GameObject*)malloc(sizeof(GameObject));
    initGameObject(gameObject);
    InitFunctions(gameObject);

    gameObject->done = 0;
    gameObject->redraw = 1;

    initRect(gameObject);
    initBall(gameObject);
    initBlocks(gameObject);

    memset(gameObject->key, 0, sizeof(gameObject->key));

    GameLoop(gameObject);

    al_destroy_font(gameObject->font);
    al_destroy_display(gameObject->display);
    al_destroy_timer(gameObject->timer);
    al_destroy_event_queue(gameObject->eventQueue);

    free(gameObject);

    return 0;
}