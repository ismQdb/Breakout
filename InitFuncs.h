#ifndef InitFuncs
#define InitFuncs

#include "Typedefs.h"
#include "IncludeHeader.h"

void mustInit(bool test, const char* description);
void InitFunctions(GameObject*);
void initRect(GameObject*);
void initBall(GameObject*);
void initBlocks(GameObject* gameObject);
void initGameObject(GameObject* gameObject);

#endif // !InitFuncs
