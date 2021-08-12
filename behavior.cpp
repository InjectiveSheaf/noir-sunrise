#include "behavior.h"

Behavior::Behavior(){
    step_sequence = vector<int>(1, 0);
    tmp_directions = {UP, DOWN, LEFT, RIGHT, ZERO};
    current_step = 1;
}

Behavior::Behavior(int _step_number){
    current_step = 1;
    step_number = _step_number;
}

int Behavior::next_step(){
    int tmp_step = current_step;
    current_step += 1;
    if(!step_sequence.empty()) return step_sequence[tmp_step % step_sequence.size()];
    return 0;
}

Zombie_behavior::Zombie_behavior(){
    step_sequence = {UP, UP, UP, LEFT, LEFT, LEFT, DOWN, DOWN, DOWN, RIGHT, RIGHT, RIGHT};
}

int Zombie_behavior::next_step(){
    int dir = tmp_directions[rand() % 4];
    return dir;
}

Vampire_behavior::Vampire_behavior(){
    step_sequence = {UP, UP, LEFT, LEFT, LEFT, DOWN, DOWN, DOWN, DOWN, UP, RIGHT, RIGHT};
}

Dalek_behavior::Dalek_behavior(){
    step_sequence = {UP, UP, UP, DOWN, DOWN, DOWN};
}

int Dalek_behavior::next_step(){
    current_step++;
    return step_sequence[current_step % step_sequence.size()];
}

bool PathFinder::isSide(unsigned int i, unsigned int j){
    if(i == 0 || j == 0 || i == background.size()-1 || j == background[i].size()-1)
    return 1;
    else return 0;
}

void PathFinder::mapUpdate(){
    for(unsigned int i = 0; i < background.size(); i++)
        for(unsigned int j = 0; j < background[0].size(); j++)
            if(!isWalkable(background[i][j])) background[i][j] = -1;
                else background[i][j] = 0;
}

void PathFinder::updateCoords(int _x0, int _y0, int _x1, int _y1){
    x1 = _x0, y1 = _y0, x2 = _x1, y2 = _y1;
}

/*int PathFinder::returnDir(int dx, int dy){
    if(dx == -1 && dy == 0) return UP;
    if(dx == 1 && dy == 0) return DOWN;
    if(dx == 0 && dy == -1) return LEFT;
    if(dx == 0 && dy == 1) return RIGHT;
    return 0;
}*/

bool PathFinder::ifSeeBresenham(){
    int A = y2 - y1;
    int B = x1 - x2;
    int x_temp = x1, y_temp = y1;
    int signA = 0, signB = 0;
    int f = 0;
    if(A > 0) signA = 1;
    else signA = -1;
    if(B > 0) signB = 1;
    else signB = -1;
    while(x_temp != x2 || y_temp != y2){
        if(abs(A)<abs(B)){
            f += A*signA;
            if(f > 0){
                f -= B*signB;
                y_temp += signA;
            }
            x_temp -= signB;

            for(int i = -1; i < 2; i++)
                for(int j = -1; j < 2; j++)
                    if(background[x_temp+i][y_temp+j] == -1 && !isSide(x_temp+i, y_temp+j)) return 0;
        }
        else{
            f += B*signB;
            if(f > 0){
                f -= A*signA;
                x_temp -= signB;
            }
            y_temp += signA;

            for(int i = -1; i < 2; i++)
                for(int j = -1; j < 2; j++)
                    if(background[x_temp+i][y_temp+j] == -1 && !isSide(x_temp+i, y_temp+j)) return 0;
        }
    }
    return 1;
}

bool PathFinder::ifSeeDDA(){
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    while(x1 != x2 || y1 != y2){
        if(background[x1][y1] == -1) return 0;
        int error2 = error * 2;

        if(error2 > -deltaY){
            error -= deltaY;
            x1 += signX;
        }
        else if(error2 < deltaX){
            error += deltaX;
            y1 += signY;
        }
    }
    return 1;
}

/*void PathFinder::DDAgotoCoords(){
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    int dx = 0, dy = 0;
    // background[x2][y2] = WALL;
    while(x1 != x2 || y1 != y2){
        // background[x1][y1] = WALL;
        int error2 = error * 2;
        if(error2 > -deltaY){
            error -= deltaY;
            x1 += signX;
            dx = signX, dy = 0;
        }
        else if(error2 < deltaX){
            error += deltaX;
            y1 += signY;
            dx = 0, dy = signY;
        }
        int dir = returnDir(dx, dy);
        if(dir != 0) directions.push_back(dir);
    }
}

void PathFinder::eraseDirestion(){
    counter = 0;
    directions.erase(directions.begin(), directions.end());
}

int PathFinder::returnNextDirection(){
    int prevCounter = counter;
    counter++;
    if(!directions.empty()) return directions[prevCounter];
    else return 0;
}

Behavior* PathFinder::LineWalk(){
    Behavior* tmp = new Behavior(&directions);
    return tmp;
}*/
