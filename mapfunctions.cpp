#include <utility>
#include "mapfunctions.h"

enum Color{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,

};

bool isWalkable(int i){
    if ((i == GROUND) || (i == ROAD)) return 1;
    else return 0;
}

pair <int,int> getNextCoord(Entity* E, int viewDirection){
    int x = E->x, y = E->y;
    int dx, dy;
    if(viewDirection == UP) dx = -1, dy = 0;
    if(viewDirection == DOWN) dx = 1, dy = 0;
    if(viewDirection == LEFT) dx = 0, dy = -1;
    if(viewDirection == RIGHT) dx = 0, dy = 1;
    return make_pair(x+dx, y+dy);
}

// Конструкторы и вспомогательные функции

Map::Map(int _length, int _width){
    directions = {UP, DOWN, LEFT, RIGHT};
    length = _length;
    width = _width;
    background = vector<vector<int>>(length, vector<int>(width, GROUND));
    foreground = vector<vector<char>>(length, vector<char>(width, VACUUM));
    for (int i = 0; i < length; i++){
        for (int j = 0; j < width; j++){
            if(isSide(i,j)) background[i][j] = WALL;
        }
    }
}

Map::Map(vector<vector<int> > &_background){
    directions = {UP, DOWN, LEFT, RIGHT};
    background = _background;
    length = background.size();
    width = background[0].size();
    foreground = vector<vector<char>>(length, vector<char>(width, VACUUM));
    AI = new PathFinder(background);
    weaponGenerator = new WeaponGenerator();
}

bool Map::isSide(unsigned int i, unsigned int j){
    if(i == 0 || j == 0 || i == background.size()-1 || j == background[i].size()-1)
    return 1;
    else return 0;
}

// Движение, добавление и удаление сущностей

void Map::moveEntity(int direction, Entity* E){
    if(canWalk(direction, E)){
        E->moveto(direction);
    }
}

void Map::moveMonsters(){
    int direction = 0;
    for(unsigned int i = 0; i < entities.size(); i++){
        if(entities[i]->getType() == "Monster"){
            AI->updateCoords(entities[i]->x, entities[i]->y, entities[0]->x, entities[0]->y);
            entities[i]->setAnger(AI->ifSeeDDA());
            if(entities[i]->isAngry()){
                if(entities[i]->x > entities[0]->x) direction = UP;
                else if(entities[i]->x < entities[0]->x) direction = DOWN;
                else if(entities[i]->y > entities[0]->y) direction = LEFT;
                else if(entities[i]->y < entities[0]->y) direction = RIGHT;
            }
            else if(!entities[i]->isAngry()){
                direction = entities[i]->getBehavior();
            }
            Entity* nextOnCoord = findOnCoord(getNextCoord(entities[i], direction).first, getNextCoord(entities[i], direction).second);
            if(canWalk(direction, entities[i])) entities[i]->moveto(direction);
            else if(!canWalk(direction, entities[i]) && nextOnCoord != *entities.end()) entities[i]->attack(nextOnCoord);
        }
    }
}

void Map::moveParticles(){
    int direction;
    for(unsigned int i = 0; i < entities.size(); i++){
        direction = entities[i]->getBehavior();
        if((entities[i]->getType() == "Particle")){
            if(canWalk(direction, entities[i])) entities[i]->moveto(direction);
                else popEntity(entities[i]);
        }
    }
}

void Map::pushEntity(Entity* E){
    if(E->getType() == "Player") entities.emplace(entities.begin(), E);
    else entities.push_back(E);

    while(!isWalkable(background[E->x][E->y])){
        E->x = rand()%(length-1) + 1;
        E->y = rand()%(width-1) + 1;
    }
}

void Map::popEntity(Entity* E){
    if(E != *entities.end() )entities.erase(find(entities.begin(), entities.end(), E));
}

void Map::popContainer(Container* C){
    if(C != *containers.end() ) containers.erase(find(containers.begin(), containers.end(), C));
}

Entity* Map::findOnCoord(int x, int y){
    auto it = [x,y](const Entity* E){
        return (E->x == x) && (E->y == y);};
    return *find_if(entities.begin(), entities.end(), it);
}

void Map::spawnRogue(){
    Monster* Rogue = nullptr;{
    Rogue = new Monster("Rogue", 50*level, 3*level);

    Weapon * oldSword = new Weapon("Old Sword", '|', 15*level, MELEE);
    Rogue->takeItem(oldSword);

    Rogue->x = rand()%(length-1);
    Rogue->y = rand()%(width-1) + 1;
    while(background[Rogue->x][Rogue->y] != GROUND){
        Rogue->x = rand()%(length-1) + 1;
        Rogue->y = rand()%(width-1) + 1;
    }
    pushEntity(Rogue);}
}

void Map::spawnCultist(){
    Monster* Cultist = nullptr;{
    Cultist = new Monster("Cultist", 75*level, 3*level);

    Weapon * oldBlaster = new Weapon("Old Blaster", '*', 10*level, RANGED);
    Cultist->takeItem(oldBlaster);

    Cultist->x = rand()%(length-1);
    Cultist->y = rand()%(width-1) + 1;
    while(background[Cultist->x][Cultist->y] != GROUND){
        Cultist->x = rand()%(length-1) + 1;
        Cultist->y = rand()%(width-1) + 1;
    }
    pushEntity(Cultist);}
}

bool Map::canWalk(int direction, Entity *E){
    if (E == *entities.end()) return 0;
    if ((direction == UP && E->x-1 < 0) || (direction == DOWN && E->x+1 > length)
            || (direction == LEFT && E->y - 1 < 0) || (direction == RIGHT && E->y + 1 >= width)) return 0;

    if( (direction == UP && (!isWalkable(background[E->x-1][E->y]) || foreground[E->x-1][E->y] != VACUUM)) ||
            (direction == DOWN && (!isWalkable(background[E->x+1][E->y])|| foreground[E->x+1][E->y] != VACUUM)) ||
                (direction == LEFT && (!isWalkable(background[E->x][E->y-1]) || foreground[E->x][E->y-1] != VACUUM)) ||
                    (direction == RIGHT && (!isWalkable(background[E->x][E->y+1]) || foreground[E->x][E->y+1] != VACUUM)) ) return 0;
    else return 1;
}

// Обновление карты, установка и вывод биома, установка сложности уровня

void Map::refresh(){
    foreground = vector<vector<char>>(length, vector<char>(width, VACUUM));
    for(unsigned int i = 0; i < entities.size(); i++){
        if(entities[i]->getHP() <= 0){
            if(entities[i]->getType() != "Particle"){
                 if(rand()%100 > 55){
                    Weapon* monsterSword = weaponGenerator->generateWeapon(level);
                    spawnContainer(monsterSword, entities[i]->x, entities[i]->y);
                }
            }
            popEntity(entities[i]);
        }
        foreground[entities[i]->x][entities[i]->y] = entities[i]->getTexture();
    }
    for(unsigned int i = 0; i < containers.size(); i++){
        if(!containers[i]->isOpen()) foreground[containers[i]->x][containers[i]->y] = 'I';
        containers[i]->tryGivingItem((Player*) entities[0]);
        if(containers[i]->isOpen()) popContainer(containers[i]);
    }
}

bool Map::reachedDoor(int direction){
    Entity *E = entities[0];
    if ((direction == UP && E->x-1 < 0) || (direction == DOWN && E->x+1 > length)
            || (direction == LEFT && E->y - 1 < 0) || (direction == RIGHT && E->y + 1 >= width)) return 0;

    if( (direction == UP && background[E->x-1][E->y] == DOOR) ||
            (direction == DOWN && background[E->x+1][E->y] == DOOR) ||
                (direction == LEFT && background[E->x][E->y-1] == DOOR) ||
                    (direction == RIGHT && background[E->x][E->y+1] == DOOR)) return 1;
    else return 0;
}

void Map::setBiome(int _biome){
    biome = _biome;
}

void Map::printBiome(){
    cout << "You are in ";
    if(biome == SPACE){
        cout << "space";
    }
    else if(biome == ARCTIC){
        cout << "an arctic world";
    }
    else if(biome == GRASSLAND){
        cout << "the grassland";
    }
    else if(biome == CAVE){
        cout << "caves";
    }
    else if(biome == VELVET){
        cout << "a velvet underground";
    }
    else if(biome == CITY){
        cout << "a town";
    }
}

void Map::setLevel(int _level){
    level = _level;
}

void Map::printBackground(int element){
    int flamecolor[2] = {RED, YELLOW};
    if (element == DOOR) cout << "|";
    if (element == FLAME) cout << "\033[1;" << flamecolor[rand()%2] << "m:\033[0m";
    if(biome == SPACE){
        if (element == WALL) cout << "\033[1;36;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;30;40" << "m \033[0m";
    }
    else if(biome == ARCTIC){
        if (element == WALL) cout << "\033[1;34;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;36;40" << "m \033[0m";
    }
    else if(biome == GRASSLAND){
        if (element == WALL) cout << "\033[1;32;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;32;40" << "m \033[0m";
    }
    else if(biome == CAVE){
        if (element == WALL) cout << "\033[1;30;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;30;40" << "m \033[0m";
    }
    else if(biome == VELVET){
        if (element == WALL) cout << "\033[1;35;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;31;40" << "m \033[0m";
    }
    else if(biome == CITY){
        if (element == WALL) cout << "\033[1;30;40m¤\033[0m";
        else if (element == GROUND) cout << "\033[1;31;47" << "m \033[0m";
        else if (element == ROAD) cout << "\033[1;31;40" << "m \033[0m";
    }
}

void Map::printMap(){
    bool isAngry = 0;
    for(unsigned int i = 0; i < background.size(); i++){
        for(unsigned int j = 0; j < background[0].size(); j++){
            if(foreground[i][j] == VACUUM) printBackground(background[i][j]);
            else{
                Entity* pointerAtIJ = findOnCoord(i,j);
                if(pointerAtIJ != *entities.end()) isAngry = pointerAtIJ->isAngry();
                else isAngry = 0;

                int encolor;
                if(isAngry) encolor = RED;
                else if(biome == CITY && background[i][j] == ROAD) encolor = 37;
                else if(biome == CITY && background[i][j] == GROUND) encolor = 30;
                else encolor = 36;

                int bgcolor;
                bgcolor = 40;
                if(biome == CITY && background[i][j] == ROAD) bgcolor = 40;
                if(biome == CITY && background[i][j] == GROUND) bgcolor = 47;

                cout << "\x1b[1;" << encolor << ";" << bgcolor << "m" << foreground[i][j] << "\x1b[0m";
            }
        }
        cout << endl;
    }
}

