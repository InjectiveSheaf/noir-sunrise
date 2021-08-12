#ifndef MAPFUNCTIONS_H
#define MAPFUNCTIONS_H

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#include "entity.h"

using namespace std;

/* Два больших класса - WeaponGenerator и Map - первый генерирует оружие, второй - сама карта,
 * в которой при смерти монстров генерируются контейнеры с различным лутом (пока реализовано оружие,
 * но в ближайшее время добавятся лекарства и улучшения для брони).
 * Каждая карта содержит дверь, которая является ключом для перехода на другой уровень - приближение игрока к двери
 * отслеживается в main, и, соответственно при приближении генерирует и кладёт в стек уровней ещё одну карту.
 */

class WeaponGenerator{
private:
    int level;
    vector<string> Names;
    vector<string> MeleeFamilies;
    vector<string> RangedFamilies;
    vector<string> BoomerangFamilies;
    string RangedTextures;
    string BoomerangTextures;
public:

    WeaponGenerator(){
        Names = {"Basic", "Mega", "Small", "Big", "Omega", "Alpha", "Chaotic",
                 "Laser", "Anti-Matter", "Thermal", "Heroic", "Dead men's",
                 "Eternal", "Faceless", "Phase", "Deputy's", "Bandit's", "Outlaw",
                "Bandito's", "Bonesaw's", "Gambler's", "Clockwork", "Cowboy", "Iron",
                 "Tungsten", "Titanium", "Anarchic", "Objective"};
        BoomerangFamilies = {"orange", "boomerang", "worm", "boomerang", "chakram"};
        MeleeFamilies = {"sword", "lightsaber", "whip", "katana", "void"};
        RangedFamilies = {"bow", "assault rifle", "rocket launcher", "shotgun",
                          "sniper rifle", "magic wand", "Lebovski", "javelin", "kunai", "snowball"};
        RangedTextures = "*+-~‹•×÷";
        BoomerangTextures = "|+•×";
        // RangedTextures = {'*', '+', '-', '~', '‹', '•', '¬', '×', '÷'};
        // BoomerangTextures = {'^', '|', '+', '•', '×'};
    }
    Weapon* generateWeapon(int level){
        int type = rand()%3;
        string name;
        unsigned char texture;
        int k;
        if(type == MELEE){
            name = Names[rand()%Names.size()];
            name += " ";
            name += MeleeFamilies[rand()%MeleeFamilies.size()];
            texture = '|';
            k = 7;
        }
        else if(type == RANGED){
            name = Names[rand()%Names.size()];
            name += " ";
            name += RangedFamilies[rand()%RangedFamilies.size()];
            int t = rand()%8;
            switch (t) {
            case 0:
                texture = '-';
            case 1:
                texture = '*';
            case 2:
                texture = 207;
            case 3:
                texture = 167;
            case 4:
                texture = '+';
            case 5:
                texture = 'o';
            case 6:
                texture = 248;
            case 7:
                texture = '~';
            case 8:
                texture = 250;
            case 9 :
                texture = 176;
            default:
                break;
            }
            k = 5;
        }
        else if(type == BOOMERANG){
            name = Names[rand()%Names.size()];
            name += " ";
            name += BoomerangFamilies[rand()%BoomerangFamilies.size()];
            int t = rand()%7;
            switch (t) {
            case 0:
                texture = '^';
            case 1:
                texture = 'o';
            case 2:
                texture = '+'; // работает
            case 3:
                texture = 254;
            case 4:
                texture = '|'; // работает
            case 5:
                texture = 207;
            case 6:
                texture = 158;
            default:
                break;
            }
            texture = BoomerangTextures[rand()%BoomerangTextures.size()];
            k = 4;
        }
        Weapon *newWeapon = new Weapon(name, texture, level*k ,(TYPE) type);
        return newWeapon;
    }
};

/* Карта содержит сведения о своём размере, биоме, уровне, переднем и заднем плане.
 * Помимо этого, в ней находятся векторы существ, контейнеров и два экземпляра обслуживающих карту
 * классов Pathfinder и WeaponGenerator, отвечающих, соответственно, за искусственное зрение и генерацию лута
 * после убийства монстров игроком.
 * Собственно говоря, раньше карта всегда содержала ссылку на игрока, но так как игрок всегда будет находиться по ссылке
 * из entities[0] (так как на карте его создают первого), ссылка оказалась не нужна.
 */

class Map{
private:
    int length, width, biome, level;
    vector<Entity*> entities;
    vector<Container*> containers;

    vector<int> directions;
    vector<vector<int>> background;
    vector<vector<char>> foreground;

    PathFinder* AI;
    WeaponGenerator* weaponGenerator;

    bool isSide(unsigned int i, unsigned int j);
public:
    Map();
    Map(int _length, int _width);
    Map(vector<vector<int>> &_background);

    // Функции-интерфейс, отвечающие за отправление-принятие инфрмации
    virtual int getWidth(){return width;}
    virtual int getEntityNumber(){return entities.size();}
    virtual int returnBackground(int x, int y){return background[x][y];}
    virtual void setBiome(int _biome);
    virtual void setLevel(int _level);
    virtual void printBiome();
    // Функции добавления-удаления врагов
    virtual void pushEntity(Entity* E);
    virtual void popEntity(Entity* E);
    /* Важные функции движения монстров и отдельных сущностей (разделить их важно было для механики патронов,
     * которые являются дочерним классом Entity)
     *  Помимо этого, функция refresh создаёт на месте монстра контейнер.
     */
    virtual void moveEntity(int direction, Entity* E);
    virtual void moveMonsters();
    virtual void moveParticles();
    virtual void refresh();
    // Булевы функции, необходимые для внешнего отслеживания движения и взаимодействия с картой.
    virtual bool canWalk(int direction, Entity* E);
    virtual bool reachedDoor(int direction);
    /* Эти две необходимы для нанесения удара по клетке, в которой находится враг, из-за того, что пуля не обладает информации
     * о местонахождении сущности, по которой придётся удар
     */
    virtual Entity* findOnCoord(int x, int y);
    virtual Entity* returnEnd(){return *entities.end();}
    // Спавн вора и технокультиста
    virtual void spawnRogue();
    virtual void spawnCultist();
    // А также функции взаимодействия карты с контейнером - собственно, спавн и удаление
    virtual void spawnContainer(Item* I, Entity* E){
        Container* C = new Container(I);
        C->setCoords(E->x, E->y);
        containers.push_back(C);
    }
    virtual void spawnContainer(Item *I, int x, int y){
        Container* C = new Container(I);
        C->setCoords(x, y);
        containers.push_back(C);
    }
    virtual void popContainer(Container* C);
    /* И, последнее, вывод карты на экран - сначала выводится background, если поверх него есть непустой foreground,
     * то выводится он - ничего сложного.
     */
    virtual void printBackground(int element);
    virtual void printMap();
};

#endif // MAPFUNCTIONS_H
