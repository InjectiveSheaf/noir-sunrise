#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <string>
#include "behavior.h"
#include <ctime>

using namespace std;

enum TYPE{
    MELEE,
    RANGED,
    BOOMERANG,
    NOWEAPON
};
// Предметы и их свойства
class Item{
protected:
    string name;
    int damage, armor, regeneration;
    char texture;
    TYPE type;
public:
    Item() : name("null"), damage(0), armor(0), texture(' '){}
    Item(string _name, char _texture) : name(_name), damage(0), armor(0), regeneration(0), texture(_texture){}
    
    string getName(){return name;}
    TYPE getType(){return type;}
    char getTexture(){return texture;}
    int getDamage(){return damage;}
    int getArmor(){return armor;}
    int getRegeneration(){return regeneration;}
};

class Weapon : public Item{
public:
    Weapon(string _name, char _texture, int _damage, TYPE _type) : Item(_name, _texture){
        damage = _damage;
        type = _type;
    }
};

class Food : public Item{
public:
    Food(string _name, char _texture, int _regeneration) : Item(_name, _texture){
        regeneration = _regeneration;
    }
};

class Armor : public Item{
public:
        Armor(string _name, char _texture, int _armor) : Item(_name, _texture){
        armor = _armor;
    }
};

// Существа и их свойства

class Entity{
protected:
    string name, type;
    char texture;
    int HP, armor, damage, pickedItem, balance;
    Behavior* behavior;
    vector<Item*> inventory;
    bool angry;
public:
    int x, y, x_prev, y_prev;

    virtual string getName(){return name;}
    virtual string getType(){return type;}
    virtual char getTexture(){return texture;}

    virtual int getArmor(){return armor;}
    virtual int getDamage(){return damage;}
    virtual int getHP(){return HP;}
    virtual int getBehavior(){return 0;}

    virtual void moveto(int dir);
    virtual void takeDamage(int d){this->HP -= d;}
    virtual void setBehavior(Behavior* _behavior);
    virtual void takeItem(Item* I);
    virtual void attack(Entity* E);

    virtual void setAnger(bool _angry){angry = _angry;}
    virtual bool isAngry(){return angry;}
    virtual TYPE getWeapontype(){return inventory[pickedItem]->getType();}
};

class Monster : public Entity{
protected:
public:
    Monster();
    Monster(string _name, int HP, int _damage);
    int getBehavior();
};

class Player : public Entity{
public:
    Player();
    void pickItem(int i);
    void throwItem();
    void setName(string _name);
    string getInventory();
    string getWeaponName();
    char getWeaponParticle();
};

class Particle : public Entity{
private:
     Behavior* behavior;
public:
    Particle(char _texture, int _damage);
    void setBulletBehavior(int direction);
    void setTexture(char _T){texture = _T;}
};

// Контейнер как класс-посредник для взаимодействия между первыми двумя

class Container{
private:
    Item* item;
    int cash;
    bool open;
public:
    int x,y;
    Container(Item* _item) : item(_item), open(0) {}
    void setCoords(int _x, int _y);
    bool isOpen() {return open;}
    void tryGivingItem(Player* P);
};

#endif // ENTITY_H
