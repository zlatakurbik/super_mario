#include <iostream>
#include <cmath>
#include <windows.h>
#include <cstring>

#define MAP_WIDTH 80
#define MAP_HEIGHT 25

//базовый класс для всех объектов 
class BaseObject {
protected:
    float x, y;
    float width, height;
    float vertical_speed;
    float horizontal_speed;
    int is_flying;
    char symbol;
    
public:
    BaseObject(float x = 0, float y = 0, float w = 1, float h = 1, char sym = ' ') {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->symbol = sym;
        vertical_speed = 0;
        horizontal_speed = 0.2f;
        is_flying = 0;
    }
    
    virtual ~BaseObject() {}
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    char getSymbol() const { return symbol; }
    float getVerticalSpeed() const { return vertical_speed; }
    float getHorizontalSpeed() const { return horizontal_speed; }
    int isFlying() const { return is_flying; }
    
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setVerticalSpeed(float speed) { vertical_speed = speed; }
    void setHorizontalSpeed(float speed) { horizontal_speed = speed; }
    void setFlying(int flying) { is_flying = flying; }
    void setSymbol(char sym) { symbol = sym; }
    
    //проверка коллизии
    bool checkCollision(const BaseObject& other) const {
        return ((x + width) > other.x) && 
               (x < (other.x + other.width)) &&
               ((y + height) > other.y) && 
               (y < (other.y + other.height));
    }
    
    //отрисовка на карте
    virtual void drawOnMap(char map[MAP_HEIGHT][MAP_WIDTH + 1]) const {
        int ix = (int)round(x);
        int iy = (int)round(y);
        int iw = (int)round(width);
        int ih = (int)round(height);
        
        for (int i = ix; i < ix + iw && i < MAP_WIDTH; i++) {
            for (int j = iy; j < iy + ih && j < MAP_HEIGHT; j++) {
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                    map[j][i] = symbol;
                }
            }
        }
    }
    
    //виртуальные методы для обновления
    virtual void update(float gravity) {
        if (is_flying) {
            vertical_speed += gravity;
            y += vertical_speed;
        }
    }
    
    virtual void handleCollisionWith(BaseObject* other) {}
};

//класс игрока
class Player : public BaseObject {
public:
    Player() : BaseObject(39, 10, 3, 3, '@') {}
    
    void jump() {
        if (!is_flying) {
            vertical_speed = -1.2f;
            is_flying = 1;
        }
    }
    
    void moveHorizontal(float dx) {
        x += dx;
    }
    
    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }
};

//класс блока
class Brick : public BaseObject {
public:
    Brick(float x, float y, float w, float h, char sym) 
        : BaseObject(x, y, w, h, sym) {
        horizontal_speed = 0;
    }
};

//класс врага
class Enemy : public BaseObject {
private:
    float direction;
    
public:
    Enemy(float x, float y, float w, float h, char sym) 
        : BaseObject(x, y, w, h, sym) {
        direction = 1;
        horizontal_speed = 0.25f;
    }
    
    void update(float gravity) override {
        x += horizontal_speed * direction;
        if (is_flying) {
            vertical_speed += gravity;
            y += vertical_speed;
        }
    }
    
    void reverseDirection() {
        direction = -direction;
    }
    
    float getDirection() const { return direction; }
};

//класс предмета
class Item : public BaseObject {
private:
    int value;
    
public:
    Item(float x, float y, float w, float h, char sym, int val = 100) 
        : BaseObject(x, y, w, h, sym) {
        value = val;
        vertical_speed = -0.8f;
        is_flying = 1;
    }
    
    void update(float gravity) override {
        vertical_speed += gravity;
        y += vertical_speed;
    }
    
    int getValue() const { return value; }
};

//основной класс игры
class Game {
private:
    Player player;
    BaseObject** bricks;
    BaseObject** moving_objects;
    int bricks_count;
    int moving_objects_count;
    int level;
    int score;
    int max_level;
    char map[MAP_HEIGHT][MAP_WIDTH + 1];
    const float GRAVITY = 0.08f;
    float camera_x;  //камера для следования за игроком
    
public:
    Game() {
        bricks = nullptr;
        moving_objects = nullptr;
        bricks_count = 0;
        moving_objects_count = 0;
        level = 1;
        score = 0;
        max_level = 3;
        camera_x = 0;
    }
    
    ~Game() {
        clearMemory();
    }
    
    void clearMemory() {
        if (bricks != nullptr) {
            for (int i = 0; i < bricks_count; i++) {
                delete bricks[i];
            }
            delete[] bricks;
            bricks = nullptr;
        }
        
        if (moving_objects != nullptr) {
            for (int i = 0; i < moving_objects_count; i++) {
                delete moving_objects[i];
            }
            delete[] moving_objects;
            moving_objects = nullptr;
        }
        
        bricks_count = 0;
        moving_objects_count = 0;
    }
    
    void clearMap() {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                map[i][j] = ' ';
            }
            map[i][MAP_WIDTH] = '\0';
        }
    }
    
    void showMap() {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            printf("%s\n", map[i]);
        }
    }
    
    void putScoreOnMap() {
        char buffer[30];
        sprintf(buffer, "Score: %d  Level: %d", score, level);
        int len = strlen(buffer);
        for (int i = 0; i < len && i < MAP_WIDTH; i++) {
            map[0][i + 2] = buffer[i];
        }
    }
    
    void addBrick(BaseObject* brick) {
        BaseObject** new_array = new BaseObject*[bricks_count + 1];
        
        for (int i = 0; i < bricks_count; i++) {
            new_array[i] = bricks[i];
        }
        
        new_array[bricks_count] = brick;
        
        if (bricks != nullptr) {
            delete[] bricks;
        }
        
        bricks = new_array;
        bricks_count++;
    }
    
    void addMovingObject(BaseObject* obj) {
        BaseObject** new_array = new BaseObject*[moving_objects_count + 1];
        
        for (int i = 0; i < moving_objects_count; i++) {
            new_array[i] = moving_objects[i];
        }
        
        new_array[moving_objects_count] = obj;
        
        if (moving_objects != nullptr) {
            delete[] moving_objects;
        }
        
        moving_objects = new_array;
        moving_objects_count++;
    }
    
    void removeMovingObject(int index) {
        delete moving_objects[index];
        
        for (int i = index; i < moving_objects_count - 1; i++) {
            moving_objects[i] = moving_objects[i + 1];
        }
        
        moving_objects_count--;
        
        if (moving_objects_count > 0) {
            BaseObject** new_array = new BaseObject*[moving_objects_count];
            for (int i = 0; i < moving_objects_count; i++) {
                new_array[i] = moving_objects[i];
            }
            delete[] moving_objects;
            moving_objects = new_array;
        } else {
            delete[] moving_objects;
            moving_objects = nullptr;
        }
    }
    
    //движение игрока влево
    void movePlayerLeft() {
        float newX = player.getX() - 3.0f;
        player.setX(newX);
        
        //проверка коллизии
        for (int i = 0; i < bricks_count; i++) {
            if (player.checkCollision(*bricks[i])) {
                player.setX(newX + 3.0f);
                break;
            }
        }
    }
    
    //движение игрока вправо
    void movePlayerRight() {
        float newX = player.getX() + 3.0f;
        player.setX(newX);
        
        //проверка коллизии
        for (int i = 0; i < bricks_count; i++) {
            if (player.checkCollision(*bricks[i])) {
                player.setX(newX - 3.0f);
                break;
            }
        }
    }
    
    void verticalMoveObject(BaseObject* obj) {
        obj->setFlying(1);
        obj->setVerticalSpeed(obj->getVerticalSpeed() + GRAVITY);
        obj->setY(obj->getY() + obj->getVerticalSpeed());
        
        for (int i = 0; i < bricks_count; i++) {
            if (obj->checkCollision(*bricks[i])) {
                if (obj->getVerticalSpeed() > 0) {
                    obj->setFlying(0);
                }
                
                //специальные блоки
                if (bricks[i]->getSymbol() == '?' && 
                    obj->getVerticalSpeed() < 0 && 
                    obj == &player) {
                    bricks[i]->setSymbol('-');
                    Item* item = new Item(
                        bricks[i]->getX(), 
                        bricks[i]->getY() - 3, 
                        3, 2, '$', 100
                    );
                    addMovingObject(item);
                }
                
                if (bricks[i]->getSymbol() == '+') {
                    level++;
                    if (level > max_level) {
                        system("color 2F");
                        system("pause");
                        exit(0);
                    }
                    system("color 2F");
                    Sleep(300);
                    createLevel(level);
                    return;
                }
                
                obj->setY(obj->getY() - obj->getVerticalSpeed());
                obj->setVerticalSpeed(0);
                break;
            }
        }
    }
    
    void horizontalMoveMovingObject(BaseObject* obj) {
        obj->setX(obj->getX() + obj->getHorizontalSpeed());
        
        for (int i = 0; i < bricks_count; i++) {
            if (obj->checkCollision(*bricks[i])) {
                obj->setX(obj->getX() - obj->getHorizontalSpeed());
                obj->setHorizontalSpeed(-obj->getHorizontalSpeed());
                return;
            }
        }
        
        //проверка для врагов, не падают ли они в пустоту
        if (obj->getSymbol() == 'o') {
            BaseObject temp = *obj;
            verticalMoveObject(&temp);
            if (temp.isFlying() == 1) {
                obj->setX(obj->getX() - obj->getHorizontalSpeed());
                obj->setHorizontalSpeed(-obj->getHorizontalSpeed());
            }
        }
    }
    
    void checkPlayerCollisions() {
        for (int i = 0; i < moving_objects_count; i++) {
            if (player.checkCollision(*moving_objects[i])) {
                if (moving_objects[i]->getSymbol() == 'o') {
                    if (player.isFlying() == 1 && 
                        player.getVerticalSpeed() > 0 &&
                        player.getY() + player.getHeight() < 
                        moving_objects[i]->getY() + moving_objects[i]->getHeight() * 0.5) {
                        score += 50;
                        Beep(1500, 30);
                        removeMovingObject(i);
                        i--;
                        continue;
                    } else {
                        playerDead();
                        return;
                    }
                }
                
                if (moving_objects[i]->getSymbol() == '$') {
                    score += 100;
                    Beep(1000, 30);
                    removeMovingObject(i);
                    i--;
                    continue;
                }
            }
        }
    }
    
    void playerDead() {
        system("color 4F");
        system("pause");
        createLevel(level);
    }
    
    void createLevel(int level_num) {
        clearMemory();
        
        player = Player();
        score = 0;
        level = level_num;
        camera_x = 0;
        
        if (level_num == 1) {
            addBrick(new Brick(20, 20, 40, 5, '#'));
            addBrick(new Brick(30, 10, 5, 3, '?'));
            addBrick(new Brick(50, 10, 5, 3, '?'));
            addBrick(new Brick(60, 15, 40, 10, '#'));
            addBrick(new Brick(60, 5, 10, 3, '-'));
            addBrick(new Brick(70, 5, 5, 3, '?'));
            addBrick(new Brick(75, 5, 5, 3, '-'));
            addBrick(new Brick(80, 5, 5, 3, '?'));
            addBrick(new Brick(85, 5, 10, 3, '-'));
            addBrick(new Brick(100, 20, 20, 5, '#'));
            addBrick(new Brick(120, 15, 10, 10, '#'));
            addBrick(new Brick(150, 20, 40, 5, '#'));
            addBrick(new Brick(210, 15, 10, 10, '+'));
            
            addMovingObject(new Enemy(25, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(80, 10, 3, 2, 'o'));
        }
        
        if (level_num == 2) {
            addBrick(new Brick(20, 20, 40, 5, '#'));
            addBrick(new Brick(60, 15, 10, 10, '#'));
            addBrick(new Brick(80, 20, 20, 5, '#'));
            addBrick(new Brick(120, 15, 10, 10, '#'));
            addBrick(new Brick(150, 20, 40, 5, '#'));
            addBrick(new Brick(210, 15, 10, 10, '+'));
            
            addMovingObject(new Enemy(25, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(80, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(65, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(120, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(160, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(175, 10, 3, 2, 'o'));
        }
        
        if (level_num == 3) {
            addBrick(new Brick(20, 20, 40, 5, '#'));
            addBrick(new Brick(80, 20, 15, 5, '#'));
            addBrick(new Brick(120, 15, 15, 10, '#'));
            addBrick(new Brick(160, 10, 15, 15, '+'));
            
            addMovingObject(new Enemy(25, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(50, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(80, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(90, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(120, 10, 3, 2, 'o'));
            addMovingObject(new Enemy(130, 10, 3, 2, 'o'));
        }
        
        system("color 0F");
        system("cls");
    }
    
    void update() {
        clearMap();
        
        if (player.getY() > MAP_HEIGHT) {
            playerDead();
            return;
        }
        
        //прыжок
        if (player.isFlying() == 0 && GetKeyState(VK_SPACE) < 0) {
            player.jump();
        }
        
        //движение Марио 
        if (GetKeyState('A') < 0) {
            movePlayerLeft();   //движение влево
        }
        if (GetKeyState('D') < 0) {
            movePlayerRight();  //движение вправо
        }
        
        verticalMoveObject(&player);
        checkPlayerCollisions();
        
        //обновление движущихся объектов
        for (int i = 0; i < moving_objects_count; i++) {
            verticalMoveObject(moving_objects[i]);
            horizontalMoveMovingObject(moving_objects[i]);
            
            if (moving_objects[i]->getY() > MAP_HEIGHT) {
                removeMovingObject(i);
                i--;
                continue;
            }
        }
        
        camera_x = player.getX() - MAP_WIDTH / 2;
        if (camera_x < 0) camera_x = 0;
        
        //отрисовка блоков 
        for (int i = 0; i < bricks_count; i++) {
            int screenX = (int)(bricks[i]->getX() - camera_x);
            if (screenX + bricks[i]->getWidth() > 0 && screenX < MAP_WIDTH) {
                int ix = screenX;
                int iy = (int)round(bricks[i]->getY());
                int iw = (int)round(bricks[i]->getWidth());
                int ih = (int)round(bricks[i]->getHeight());
                
                for (int x = ix; x < ix + iw && x < MAP_WIDTH; x++) {
                    for (int y = iy; y < iy + ih && y < MAP_HEIGHT; y++) {
                        if (x >= 0 && y >= 0 && y < MAP_HEIGHT) {
                            map[y][x] = bricks[i]->getSymbol();
                        }
                    }
                }
            }
        }
        
        //отрисовка движущихся объектов с учетом камеры
        for (int i = 0; i < moving_objects_count; i++) {
            int screenX = (int)(moving_objects[i]->getX() - camera_x);
            if (screenX + moving_objects[i]->getWidth() > 0 && screenX < MAP_WIDTH) {
                int ix = screenX;
                int iy = (int)round(moving_objects[i]->getY());
                int iw = (int)round(moving_objects[i]->getWidth());
                int ih = (int)round(moving_objects[i]->getHeight());
                
                for (int x = ix; x < ix + iw && x < MAP_WIDTH; x++) {
                    for (int y = iy; y < iy + ih && y < MAP_HEIGHT; y++) {
                        if (x >= 0 && y >= 0 && y < MAP_HEIGHT) {
                            map[y][x] = moving_objects[i]->getSymbol();
                        }
                    }
                }
            }
        }
        
        //отрисовка игрока с учетом камеры
        int playerScreenX = (int)(player.getX() - camera_x);
        int playerScreenY = (int)round(player.getY());
        int playerWidth = (int)round(player.getWidth());
        int playerHeight = (int)round(player.getHeight());
        
        for (int x = playerScreenX; x < playerScreenX + playerWidth && x < MAP_WIDTH; x++) {
            for (int y = playerScreenY; y < playerScreenY + playerHeight && y < MAP_HEIGHT; y++) {
                if (x >= 0 && y >= 0 && y < MAP_HEIGHT) {
                    map[y][x] = player.getSymbol();
                }
            }
        }
        
        putScoreOnMap();
    }
    
    void run() {
        createLevel(1);
        
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
        
        do {
            update();
            
            setCursor(0, 0);
            showMap();
            
            Sleep(8);
        } while (GetKeyState(VK_ESCAPE) >= 0);
    }
    
    void setCursor(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
};

int main() {
    system("title MARIO GAME - REAL MOVEMENT");
    system("mode con cols=80 lines=26");
    
    Game game;
    game.run();
    
    return 0;
}