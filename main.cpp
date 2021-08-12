#include <thread>
#include <time.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <stack>
#include <fstream>

#include "mapfunctions.h"
#include "input.h"
#include "phrases.h"
#include "generator.h"

using namespace std;

int input, viewDirection;
int spaceshipDist;

void clearScreen(){
    system(" printf \"\033c\" ");
}

void printPlayerSting(Player* player, Map* map){
    cout << player->getName() << ": HP = " << player->getHP() << ", Armor = " << player->getArmor() << ", Damage = " << player->getDamage();
    if(spaceshipDist == 0) cout << " You're on spaceship.\n";
    else cout << " Distance from spaceship: " << spaceshipDist << ".\n";
    cout << "\x1b[1;35mInventory: " << player->getInventory() << "\x1b[0m" << endl;
    map->printBiome();
    cout << "." << endl;
}

void printTitle(){
    ifstream title("/home/lucretiaxd/QT Projects/spacegamev3dec/title.txt");
    string titleLine;
    while(getline(title,titleLine)){
        cout << titleLine << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    title.close();
}

void printPlanet(){
    ifstream title("/home/lucretiaxd/QT Projects/spacegamev3dec/saturn.txt");
    string titleLine;
    while(getline(title,titleLine)){
        cout << titleLine << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    title.close();
}

void printCowboy(){
    ifstream title("/home/lucretiaxd/QT Projects/spacegamev3dec/cowboy.txt");
    string titleLine;
    while(getline(title,titleLine)){
        cout << titleLine << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    title.close();
}

std::mutex moving;

void mapCycle(stack<Map*>* mapStack, Player* player, MapGenerator* G){
    int help_counter = 0;
    Map* map = nullptr;
    while (input != 'e'){
        if(player->getHP() <= 0){
             cout << "You've lost, Cowboy. Press \"e\" to exit." << endl;
             return;
        }

        moving.lock();
        map = mapStack->top();
        map->refresh();
        moving.unlock();

        clearScreen();
        printPlayerSting(player, map);

        moving.lock();
        map->printMap();
        moving.unlock();

        if (help_counter == 1) Print(HELP);

        if(input == 'h'){
            help_counter += 1;
            help_counter %= 2;
            input = ZERO;
        }

        if(input == 't'){
            input = ZERO;
            player->throwItem();
        }

        if(map->reachedDoor(viewDirection)){
            spaceshipDist++;
            moving.lock();

            printPlanet();
            cout << endl;

            input = ZERO;

            G->setSize(19, rand() % 20 + 60);
            Map* temp_map = G->getRandMap();
            temp_map->setLevel(spaceshipDist);
            if(player->y > temp_map->getWidth()) player->y = rand()%temp_map->getWidth();
            temp_map->pushEntity(player);
            for(int k = 0; k < spaceshipDist + (rand() % 6) - 3; k++){
                if(rand()%2) temp_map->spawnRogue();
                else temp_map->spawnCultist();
            }

            mapStack->push(temp_map);
            map->popEntity(player);
            map = mapStack->top();
            moving.unlock();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        if(input == 'p' && mapStack->size() > 1){
            spaceshipDist--;
            moving.lock();
            input = ZERO;
            mapStack->pop();
            map = mapStack->top();
            if(player->y < map->getWidth()){
                player->y = rand()%map->getWidth();
            }
            if(mapStack->size() == 1){
                player->x = 7;
                player->y = 31;
            }
            map->pushEntity(player);
            moving.unlock();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        bool inputIsDirection = (input == UP || input == DOWN || input == LEFT || input == RIGHT);
        if (inputIsDirection && map->canWalk(input, player)){
            player->moveto(input);
            input = ZERO;
        }

        this_thread::sleep_for(chrono::milliseconds(45));
    }
}

void attackCycle(stack<Map*>* mapStack, Player* player){
    int dx = 0, dy = 0, tmp = 0, intInput = 0;
     Map* map = nullptr;
    while(input != 'e'){
        moving.lock();
        map = mapStack->top();
        moving.unlock();

        intInput = input - '0';
        if(intInput >= 0 && intInput <= 9) player->pickItem(intInput);

        if (input == ' ' && spaceshipDist > 0){
            input = ZERO;

            if(viewDirection == UP) dx = -1, dy = 0;
            if(viewDirection == DOWN) dx = 1, dy = 0;
            if(viewDirection == LEFT) dx = 0, dy = -1;
            if(viewDirection == RIGHT) dx = 0, dy = 1;

            tmp = viewDirection;

            moving.lock();
            Particle* ammo = nullptr;
            ammo = new Particle(player->getWeaponParticle(), player->getDamage());
            ammo->setAnger(0);
            ammo->x = player->x;
            ammo->y = player->y;
            map->pushEntity(ammo);
            moving.unlock();
            Entity* attackedEntity = nullptr;

            if(player->getWeapontype() == RANGED){
                while(map->canWalk(tmp, ammo)){
                    moving.lock();
                    ammo->moveto(tmp);
                    moving.unlock();
                    this_thread::sleep_for(chrono::milliseconds(40));
                }
            }
            else if(player->getWeapontype() == MELEE){
                if(tmp == LEFT || tmp == RIGHT) ammo->setTexture('-');
                for(int i = 0; i < 2 && map->canWalk(tmp, ammo); i++){
                    moving.lock();
                    ammo->moveto(tmp);
                    moving.unlock();
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            }
            else if(player->getWeapontype() == BOOMERANG){
                int inverse, t = rand()%3 + 5;
                if((tmp == LEFT || tmp == RIGHT) && ammo->getTexture() == '|') ammo->setTexture('-');
                if(tmp == RIGHT) inverse = LEFT, t = rand()%4 + 10;
                if(tmp == LEFT) inverse = RIGHT, t = rand()%4 + 10;
                if(tmp == UP) inverse = DOWN;
                if(tmp == DOWN) inverse = UP;
                for(int i = 0; i < t && map->canWalk(tmp, ammo); i++){
                    moving.lock();
                    ammo->moveto(tmp);
                    moving.unlock();
                    this_thread::sleep_for(chrono::milliseconds(40));
                }

                attackedEntity = map->findOnCoord(ammo->x + dx, ammo->y + dy);
                if(attackedEntity != map->returnEnd() && attackedEntity != player) player->attack(attackedEntity);

                for(int i = 0; i < t && map->canWalk(inverse, ammo); i++){
                    moving.lock();
                    ammo->moveto(inverse);
                    moving.unlock();
                    this_thread::sleep_for(chrono::milliseconds(40));
                }
            }

            moving.lock();
            attackedEntity = map->findOnCoord(ammo->x + dx, ammo->y + dy);
            if(player->getWeapontype() == MELEE && map->canWalk(tmp, attackedEntity)) attackedEntity->moveto(tmp);
            if(attackedEntity != player && attackedEntity != map->returnEnd()) player->attack(attackedEntity);
            map->popEntity(ammo);
            moving.unlock();
        }
    }
}

void monstersCycle(stack<Map*>* mapStack){
    Map* map = nullptr;
    while(input != 'e'){
        if(input == 'n') this_thread::sleep_for(chrono::milliseconds(2000));
        moving.lock();
        map = mapStack->top();
        map->moveMonsters(); // тут может появиться дедлок
        moving.unlock();
        this_thread::sleep_for(chrono::milliseconds(700));
    }
}

void spaceshipParticleCycle(stack<Map*>* mapStack){
    Map* map = nullptr;
    Particle* asteroid = nullptr;
    while(input != 'e'){
        if(spaceshipDist == 0){
            moving.lock();
            map = mapStack->top();
            asteroid = new Particle('*', 5);
            asteroid->setAnger(0);
            asteroid->x = rand()%14;
            if(rand()%2) asteroid->y = 1;

            map->pushEntity(asteroid);
            moving.unlock();
            while(map->returnBackground(asteroid->x, asteroid->y + 1) == GROUND && asteroid->y < 71){
                moving.lock();
                asteroid->moveto(RIGHT);
                moving.unlock();
                this_thread::sleep_for(chrono::milliseconds(30));
            }

            moving.lock();
            map->popEntity(asteroid);
            moving.unlock();
        }
    }
}

void inputCycle(){
    while(input != 'e'){
        input = getchar();
        if(input == UP || input == DOWN || input == LEFT || input == RIGHT) viewDirection = input;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main()
{
    printTitle();

    time_t t = time(nullptr);
    srand(t);
    string tmp;
    viewDirection = DOWN;

    stack<Map*> mapStack;

    Player* player = nullptr;
    player = new Player();    

    Print(START);
    cin >> tmp;
    player->setName(tmp);
    clearScreen();
    cout << "So, space cowboy " << player->getName() << "? That's a nice name." << endl;

    printCowboy();
    //Print(BEGIN);
    cout << "Game loading..." << endl;

    printPlanet();
    set_keypress();
    cout << "Press Space to start." << endl;

    int tmp1 = 0;
    while(tmp1 != ' ') tmp1 = getchar();

    clearScreen();

    int length = 15, width = 72;

    MapGenerator * G = nullptr;
    G = new MapGenerator();
    G->setSize(length, width);

    Map* map = G->getSpaceship();
    player->x = length / 2;
    player->y = width / 2 - 5;
    map->pushEntity(player);

    mapStack.push(map);
    spaceshipDist = 0;

    Weapon * firstPistol = new Weapon("Rusty Pistol", '*', 5, RANGED);
    Weapon * firstCrossbow = new Weapon("Rusty Crossbow", '^', 1000, RANGED);
    map->spawnContainer(firstPistol, 7, 40);
    map->spawnContainer(firstCrossbow, 9, 40);

    vector<thread> particles;
    for(int i = 0; i < 5; i++) particles.push_back(thread(spaceshipParticleCycle, &mapStack));

    thread thread_input(inputCycle);
    thread move_monsters(monstersCycle, &mapStack);
    thread move_ammo(attackCycle, &mapStack, player);
    thread map_refresh(mapCycle, &mapStack, player, G);

    for(int i = 0; i < 5; i++) particles[i].join();
    move_monsters.join();
    move_ammo.join();
    map_refresh.join();
    thread_input.join();

    reset_keypress();
    return 0;
}
