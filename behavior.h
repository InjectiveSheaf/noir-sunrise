#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <queue>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

enum DIRECTION{
    ZERO = 0,
    UP = 119,
    DOWN = 115,
    LEFT = 97,
    RIGHT = 100
};

enum TEXTURE{
    VACUUM = -1,
    GROUND = 0,
    PLAYER = 1,
    WALL = 2,
    RIVER = 3,
    FLAME = 4,
    MONSTER = 5,
    DOOR = 6,
    ROAD = 7,
    TREE = 8
};

enum Biome{
    GRASSLAND,
    CAVE,
    VELVET,
    SPACE,
    ARCTIC,
    CITY
};

/* Модели поведения (в данный момент используется Zombie и Dalek для роги и технокультиста,
 *соответственно, случайное блуждание и ходьба по линии)
 */

class Behavior{
private:
    int current_step;
    int step_number;
    vector<int> step_sequence;
public:
    vector<int> tmp_directions;
    Behavior();
    Behavior(int _step_number);
    Behavior(vector<int>* _stepSequence) : step_sequence(*_stepSequence){}
    virtual int next_step();
};

class Zombie_behavior : public Behavior{
private:
    vector<int> step_sequence;
public:
    Zombie_behavior();
    int next_step();
};

class Vampire_behavior : public Behavior{
private:
    vector<int> step_sequence;
public:
    Vampire_behavior();
};

class Dalek_behavior : public Behavior{
private:
    int current_step;
    vector<int> step_sequence;
public:
    Dalek_behavior();
    int next_step();
};

class Bullet_behavior : public Behavior{
private:
    int direction;
public:
    Bullet_behavior(int _direction){direction = _direction;}
    int next_step(){return direction;}
};

/*____________________________________________________*/

bool isWalkable(int i);

// Нахождение пути. Изначально планировался волновой алгоритм Ли, но постепенно всё свелось к этому.
// Проверка видимости игрока происходит при помощи алгоритма Брезенхема построения отрезка.

class PathFinder{
    vector<vector<int>> background;
    vector<int> directions;
    int x1, y1, x2, y2, counter;

    bool isSide(unsigned int i, unsigned int j);
public:
    PathFinder();
    PathFinder(vector<vector<int>> _background) : background(_background){
        mapUpdate();
    }
    void mapUpdate();

    void updateCoords(int _x0, int _y0, int _x1, int _y1);

    // int returnDir(int dx, int dy);

    bool ifSeeBresenham();

    bool ifSeeDDA();

    // void DDAgotoCoords();

    // void eraseDirestion();

    // int returnNextDirection();

    // Behavior* LineWalk();

};

#endif // BEHAVIOR_H
