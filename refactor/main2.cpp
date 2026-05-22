#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <windows.h>

#define mapWidth 80
#define mapHeight 25

//хранение координатов
typedef struct {
    float x, y;
    float width, height;
    float vertical_speed;
    int is_flying;
    char symbol;
    float horizontal_speed;
} GameObject;


char map[mapHeight][mapWidth+1];

typedef struct {
    GameObject player;
    GameObject* bricks;
    int bricks_count;
    GameObject* moving_objects;
    int moving_objects_count;
    int level;
    int score;
    int max_level;
} GameState;

void ClearMap()
{
	for (int i = 0; i < mapWidth; i++)
		map [0][i] = ' ';
	map[0][mapWidth] = '\0';
	for (int j = 0; j < mapHeight; j++)
		sprintf( map[j], map[0]);
}

void ShowMap()
{
	map[mapHeight - 1][mapWidth - 1] = '\0';
	for (int j = 0; j < mapHeight; j++)
		printf("%s\n", map[j]);
}

//позиция персонажа
void SetObjectPos(TObject *obj, float xPos, float yPos)
{
	(*obj).x = xPos;
	(*obj).y = yPos;
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType)
{
	SetObjectPos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
	(*obj).vertSpeed = 0;
	(*obj).cType = inType;
	(*obj).horizSpeed = 0.2;
}

void PlayerDead()
{
	system("color 4F");
	Sleep(500);
	CreateLevel(level);
}


BOOL IsCollision(TObject o1, TObject o2);
void CreateLevel(int lvl);
TObject *GetNewMoving();

void vertical_move_object(GameObject* obj, GameState* state) {
    obj->is_flying = 1;               
    obj->vertical_speed += 0.05;      
    obj->y += obj->vertical_speed;    
    //Проверяем столкновения
    for (int i = 0; i < state->bricks_count; i++) {
        if (check_collision(*obj, state->bricks[i])) {
            if (obj->vertical_speed > 0) {
                obj->is_flying = 0;   
            }
            
            if (state->bricks[i].symbol == '?' && obj->vertical_speed < 0 && obj == &state->player) {
                state->bricks[i].symbol = '-';  
                
                GameObject* new_obj = add_moving_object(state);
                init_object(new_obj, state->bricks[i].x, state->bricks[i].y - 3, 3, 2, '$');
                new_obj->vertical_speed = -0.7;  
            }
            obj->y -= obj->vertical_speed;
            obj->vertical_speed = 0;
            if (state->bricks[i].symbol == '+') {
                state->level++;
                if (state->level > state->max_level) {
                    state->level = 1;  
                }
                system("color 2F");
                Sleep(500);
                create_level(state, state->level);  
            }
            break;
        }
    }
}

void DeleteMoving(int i)
{
	movingLength--;
	moving[i] = moving[movingLength];
	moving = (TObject*)realloc( moving, sizeof(*moving) * movingLength );
}

void MarioCollision()
{
	for (int i = 0; i < movingLength; i++)
		if (IsCollision(mario, moving[i]))
		{
			if (moving[i].cType == 'o')
			{
				if ((mario.IsFly == TRUE)
					&& (mario.vertSpeed > 0)
					&& (mario.y + mario.height < moving[i].y + moving[i].height * 0.5)
					)
				{
					score += 50;
					DeleteMoving(i);
					i--;
					continue;
				}
				else 
					PlayerDead();
			}
			
			if (moving[i].cType == '$')
			{
				score += 100;
				DeleteMoving(i);
				i--;
				continue;
			}
		}
}

void HorizonMoveObject(TObject *obj)
{
	obj[0].x += obj[0].horizSpeed;
	
	for (int i = 0; i < brickLength; i++)
		if (IsCollision(obj[0], brick[i]))
		{
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
			return;
		}
	
	if (obj[0].cType == 'o')
	{
		TObject tmp = *obj;
		VertMoveObject(&tmp);
		if (tmp.IsFly == TRUE)
		{
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
		}
	}
}

BOOL IsPosInMap(int x, int y)
{
	return ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
}

void put_object_on_map(GameObject obj, char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iw = (int)round(obj.width);
    int ih = (int)round(obj.height);
    
    for (int i = ix; i < ix + iw; i++) {
        for (int j = iy; j < iy + ih; j++) {
            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                map[j][i] = obj.symbol;
            }
        }
    }
}	

void setCur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord );
}

void HorizonMoveMap(float dx)
{
	mario.x -= dx;
	for (int i = 0; i < brickLength; i++)
		if (IsCollision(mario, brick[i]))
		{
			mario.x += dx;
			return;
		}
	mario.x += dx;
	
	for (int i = 0; i < brickLength; i++)
		brick[i].x +=dx;
	for (int i = 0; i < movingLength; i++)
		moving[i].x +=dx;
}

BOOL IsCollision(TObject o1, TObject o2)
{
	return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
	((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
	
}

GameObject* add_brick(GameState* state) {
    state->bricks_count++;
    state->bricks = (GameObject*)realloc(state->bricks, sizeof(GameObject) * state->bricks_count);
    return state->bricks + state->bricks_count - 1;
}

void delete_moving_object(GameState* state, int index) {
    state->moving_objects_count--;
    state->moving_objects[index] = state->moving_objects[state->moving_objects_count];
    state->moving_objects = (GameObject*)realloc(state->moving_objects,sizeof(GameObject) * state->moving_objects_count);
}

TObject *GetNewMoving()
{
	movingLength++;
	moving = (TObject*)realloc( moving, sizeof(*moving) * movingLength);
	return moving + movingLength - 1;
}

void PutScoreOnMap()
{
	char c[30];
	sprintf(c, "Score: %d", score);
	int len = strlen(c);
	for (int i = 0; i < len; i++)
	{
		map[1][i+5] = c[i];
	}
}

//создание всего уровня
void create_level(GameState* state, int level_num) {
    system("color 9F");      
    
	//очистка памяти
    free(state->bricks);
    free(state->moving_objects);
    state->bricks = NULL;
    state->moving_objects = NULL;
    state->bricks_count = 0;
    state->moving_objects_count = 0;
    
	//инициализация
    init_object(&state->player, 39, 10, 3, 3, '@');
    state->score = 0;          
    state->level = level_num;  
    state->max_level = 3;      
    
    if (level_num == 1) {
        init_object(add_brick(state), 20, 20, 40, 5, '#');
        init_object(add_brick(state), 30, 10, 5, 3, '?');
        init_object(add_brick(state), 50, 10, 5, 3, '?');
        init_object(add_brick(state), 60, 15, 40, 10, '#');
        init_object(add_brick(state), 60, 5, 10, 3, '-');
        init_object(add_brick(state), 70, 5, 5, 3, '?');
        init_object(add_brick(state), 75, 5, 5, 3, '-');
        init_object(add_brick(state), 80, 5, 5, 3, '?');
        init_object(add_brick(state), 85, 5, 10, 3, '-');
        init_object(add_brick(state), 100, 20, 20, 5, '#');
        init_object(add_brick(state), 120, 15, 10, 10, '#');
        init_object(add_brick(state), 150, 20, 40, 5, '#');
        init_object(add_brick(state), 210, 15, 10, 10, '+');         
        init_object(add_moving_object(state), 25, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 80, 10, 3, 2, 'o');
    }
    
    if (level_num == 2) {
        init_object(add_brick(state), 20, 20, 40, 5, '#');
        init_object(add_brick(state), 60, 15, 10, 10, '#');
        init_object(add_brick(state), 80, 20, 20, 5, '#');
        init_object(add_brick(state), 120, 15, 10, 10, '#');
        init_object(add_brick(state), 150, 20, 40, 5, '#');
        init_object(add_brick(state), 210, 15, 10, 10, '+');
        
        init_object(add_moving_object(state), 25, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 80, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 65, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 120, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 160, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 175, 10, 3, 2, 'o');
    }
    
    if (level_num == 3) {
        init_object(add_brick(state), 20, 20, 40, 5, '#');
        init_object(add_brick(state), 80, 20, 15, 5, '#');
        init_object(add_brick(state), 120, 15, 15, 10, '#');
        init_object(add_brick(state), 160, 10, 15, 15, '+');
        
        init_object(add_moving_object(state), 25, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 50, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 80, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 90, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 120, 10, 3, 2, 'o');
        init_object(add_moving_object(state), 130, 10, 3, 2, 'o');
    }
}

int main() {
    GameState game;                     
    char map[MAP_HEIGHT][MAP_WIDTH + 1];  
	
	//инициализация
    game.bricks = NULL;
    game.moving_objects = NULL;
    game.bricks_count = 0;
    game.moving_objects_count = 0;
    game.level = 1;
    game.score = 0;
    game.max_level = 3;
    
    create_level(&game, game.level);
    do {
        clear_map(map); 
        
        if (game.player.is_flying == 0 && GetKeyState(VK_SPACE) < 0) {
            game.player.vertical_speed = -1;
        }
        if (GetKeyState('A') < 0) {
            horizontal_move_map(&game, 1);
        }
        if (GetKeyState('D') < 0) {
            horizontal_move_map(&game, -1);
        }
        if (game.player.y > MAP_HEIGHT) {
            player_dead(&game);
        }
        vertical_move_object(&game.player, &game);
        check_player_collision(&game);
        for (int i = 0; i < game.bricks_count; i++) {
            put_object_on_map(game.bricks[i], map);
        }
        for (int i = 0; i < game.moving_objects_count; i++) {
            vertical_move_object(&game.moving_objects[i], &game);
            horizontal_move_moving_object(&game.moving_objects[i], &game);
            if (game.moving_objects[i].y > MAP_HEIGHT) {
                delete_moving_object(&game, i);
                i--;
                continue;
            }
            put_object_on_map(game.moving_objects[i], map);
        }
        put_object_on_map(game.player, map);
        put_score_on_map(game.score, map);
        
        set_cursor(0, 0);
        show_map(map);
        
        Sleep(10);  
    } while (GetKeyState(VK_ESCAPE) >= 0);  
    
    return 0;
}