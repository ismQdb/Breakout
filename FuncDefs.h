#ifndef FUNCDEFS_H
#define FUNCDEFS_H

void GameLoop(GameObject*);
void GameLoop(GameObject* gameObject);
void drawBlocks(GameObject*);
int randBetween(int lo, int hi);
int ballBlockCollision(Ball* ball, Block* block);
void blocksRemoval(GameObject* gameObject);
int ballRectCollision(GameObject*);
void moveTheBall(GameObject*);
void respawnBall(GameObject* gameObject);

#endif // !FuncDefs
