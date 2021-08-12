#include "entity.h"

// ENTITY

void Entity::attack(Entity *E){
    if(E == nullptr) return;
    E->HP -= this->getDamage();
    if(E->HP <= 0){
        if(rand()%100 > 50) this->HP += 5;
    }
}

void Entity::moveto(int dir){
    int dx = 0, dy = 0;
    x_prev = x, y_prev = y;
    if(dir == UP) dx = -1, dy = 0;
    else if(dir == DOWN) dx = 1, dy = 0;
    else if(dir == LEFT) dx = 0, dy = -1;
    else if(dir == RIGHT) dx = 0, dy = 1;
    x += dx;
    y += dy;
}

Monster::Monster(){
    HP=50;
    type = "Monster";
}

Monster::Monster(string _name, int _HP, int _damage){
    HP = _HP;
    name = _name;
    damage = _damage;
    type = "Monster";
    if(_name == "Rogue"){
        texture = 'r';
        Zombie_behavior * zombie = nullptr;
        zombie = new Zombie_behavior();
        behavior = zombie;
    }
    else if(_name == "Cultist"){
        texture = 'c';
        Dalek_behavior * dalek = nullptr;
        dalek = new Dalek_behavior();
        behavior = dalek;
    }
    else if(_name == "vampire"){
        texture = 'V';
        Vampire_behavior * vampire = nullptr;
        vampire = new Vampire_behavior();
        behavior = vampire;
    }
}

void Entity::setBehavior(Behavior *_behavior){
    behavior = _behavior;
}

int Monster::getBehavior(){
    return behavior->next_step();
}

// Функции игрока - установка имени, взаимодействие с предметами, получение текстуры и типа оружия, и так далее.

Player::Player(){
    HP = 100;
    armor = 10;
    damage = 3;
    type = "Player";
    texture = '@';
    pickedItem = 0;
    balance = 0;
}

void Player::setName(string _name){
    name = _name;
    angry = 0;
}

void Entity::takeItem(Item *I){
    inventory.push_back(I);
}

void Player::pickItem(int i){
    if((unsigned int) i < inventory.size()+1 && i > 0){
        pickedItem = i-1;
        damage = inventory[pickedItem]->getDamage();
    }
}

void Player::throwItem(){
    if(inventory.size() == 1) return;
    inventory.erase(inventory.begin()+pickedItem);
    pickItem(pickedItem-1);
}

string Player::getWeaponName(){
    return inventory[pickedItem]->getName();
}

string Player::getInventory(){
    string s;
    for(size_t i = 0; i < inventory.size(); i++){
        if(i == (size_t)pickedItem) s += "\x1b[4m";
        s += inventory[i]->getName();
        s += "\x1b[24m ";
    }
    return s;
}

char Player::getWeaponParticle(){
    return inventory[pickedItem]->getTexture();
}

Particle::Particle(char _texture, int _damage){
    HP = 1;
    type = "Particle";
    texture = _texture;
    damage = _damage;
    angry = 0;
}

void Container::setCoords(int _x, int _y){
    x = _x; y = _y;
}

void Container::tryGivingItem(Player *P){
    int px = P->x, py = P->y;
    if( (px-x)*(px-x) + (py-y)*(py-y) <= 1 && !open){
        open = true;
        P->takeItem(item);
    }
}
