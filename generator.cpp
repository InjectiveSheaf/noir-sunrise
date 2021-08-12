#include "generator.h"

// Технические функции - генератор рисует отрезок/комнату/корабль/пещеру

int MapGenerator::countNeighbours(int x, int y){
    int counter = 0;
    for(int i = -1; i < 2; i++)
        for(int j = -1; j < 2; j++)
            if (!isSide(x,y) && background[x+i][y+j] != GROUND) counter++;
    return counter;
}

void MapGenerator::drawContours(){
    for (int i = 0; i < length; i++){
        for (int j = 0; j < width; j++){
            if(isSide(i,j)) background[i][j] = WALL;
            else background[i][j] = GROUND;
        }
    }
}

void MapGenerator::drawRoom(int x, int y, int l, int w){ // x, y - левый верхний угол
    background[x][y] = WALL;
    if(x+w >= length) w = rand()%(length-x-1);
    if(y+l >= width) l = rand()%(width-y-1);
    drawLine(x, y, x, y+l);
    drawLine(x+w, y+l, x+w, y);
    drawLine(x, y, x+w , y);
    drawLine(x, y+l, x+w, y+l);
    if(w == 0 || l == 0){
        background[x][y] = GROUND;
        return;
    }
}

void MapGenerator::drawRoomwDoor(int x, int y, int l, int w){
    background[x][y] = WALL;
    if(x+w >= length) w = rand()%(length-x-1);
    if(y+l >= width) l = rand()%(width-y-1);
    drawLine(x, y, x, y+l);
    drawLine(x+w, y+l, x+w, y);
    drawLine(x, y, x+w , y);
    drawLine(x, y+l, x+w, y+l);
    if(w == 0 || l == 0){
        background[x][y] = GROUND;
        return;
    }
    int a = rand()%w + x, b= rand()%l + y;
    while(true){
        if(a > x && a < x+w && b > y && b < y+l && !isSide(a,b)) a = rand()%w + x, b= rand()%l + y;
        else{
            background[a][b] = GROUND;
            return;
        }
    }
}

void MapGenerator::drawLine(int x1, int y1, int x2, int y2){
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
            background[x_temp][y_temp] = WALL;
        }
        else{
            f += B*signB;
            if(f > 0){
                f -= A*signA;
                x_temp -= signB;
            }
            y_temp += signA;
            background[x_temp][y_temp] = WALL;
        }
    }
}

void MapGenerator::drawLine(int x1, int y1, int x2, int y2, int MATERIAL){
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
            background[x_temp][y_temp] = MATERIAL;
        }
        else{
            f += B*signB;
            if(f > 0){
                f -= A*signA;
                x_temp -= signB;
            }
            y_temp += signA;
            background[x_temp][y_temp] = MATERIAL;
        }
    }
}

void MapGenerator::drawDDA(int x1, int y1, int x2, int y2){
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    background[x2][y2] = WALL;
    while(x1 != x2 || y1 != y2){
        background[x1][y1] = WALL;
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
}

void MapGenerator::drawSpaceship(){
    drawLine(7, 7, 10, 20);
    drawLine(7, 7, 4, 20);

    drawLine(7, 4, 7, 7);

    drawLine(11, 20, 11, 45);
    drawLine(3, 20, 3, 45);

    drawLine(12, 45, 12, 64);
    drawLine(2, 45, 2, 64);

    drawLine(3, 61, 10, 61);

    drawLine(4, 61, 2, 67);
    drawLine(10, 61, 12, 67);

    drawLine(4, 61, 4, 66, FLAME);
    drawLine(5, 61, 5, 68, FLAME);
    drawLine(6, 61, 6, 69, FLAME);
    drawLine(7, 61, 7, 70, FLAME);
    drawLine(8, 61, 8, 69, FLAME);
    drawLine(9, 61, 9, 68, FLAME);
    drawLine(10, 61, 10, 66, FLAME);

    background[7][15] = DOOR;
}

void MapGenerator::drawCave(){
    for(int i = 0; i < length; i++)
        for(int j = 0; j < width; j++)
            background[i][j] = WALL;
    int x = rand() % length, y = rand() % width;
    int counter = 0;
    background[x][y] = GROUND;
    while(counter != length*width / 3){
        int dir = rand()%4;
        int dx = 0, dy = 0;
        if(dir == 0) dx = 0, dy = -1;
        if(dir == 1) dx = 0, dy = 1;
        if(dir == 2) dx = 1, dy = 0;
        if(dir == 3) dx = -1, dy = 0;

        if(x+dx > 1 && x+dx < length - 1) x += dx;
        if(y+dy > 1 && y+dy < width - 1) y += dy;
        if(background[x][y] == WALL){
            background[x][y] = GROUND;
            counter++;
        }
    }
    counter = 0;
    while(true){
        x = rand() % length, y = rand() % width;
        if(countNeighbours(x, y) > 0 && countNeighbours(x,y) < 3){
            background[x][y] = DOOR;
            return;
        }
    }
}

void MapGenerator::drawSpaceMap(){
    drawContours();
    for(int k = 0; k < rand()%4 + 3; k++){
        drawLine(rand() % length, rand() % width, rand() % length, rand() % width);
    }
    drawRoom(rand() % (length-1), rand() % (width-1), rand()%15, rand()%7);
    int i = rand() % length, j = rand() % width, doorCounter = 0;
    while(background[i][j] == WALL || doorCounter != 1){
        i = 1 + rand() % (length - 1), j = 1 + rand() % (width - 1);
        background[i][j] = DOOR;
        doorCounter++;
    }
}

void MapGenerator::drawCity(){
    drawContours();
    int bNumber = rand()%2+6;
    int bX[bNumber];
    int bY[bNumber];
    for(int i = 0; i < bNumber; i++){
        bX[i] = rand()%(length-5)+4;
        bY[i] = rand()%width;
        background[bX[i]][bY[i]] = WALL;
    }

    int Smin = 10000, Xmin = 0;
    for(int x = 0; x < length; x++){
        int sum = 0;
        for(int i = 0; i < bNumber; i++){
            sum += abs(bX[i] - x);
        }
        if (sum <= Smin){
            Smin = sum;
            Xmin = x;
        }
    }

    for(int i = 0; i < bNumber; i++){
        drawRoom(Xmin, bY[i], bX[i], bY[i]);
        drawLine(Xmin+1, bY[i], bX[i]-1, bY[i], ROAD);
    }

    drawLine(Xmin, 0, Xmin, width-2, ROAD);
    drawLine(Xmin-1, 0, Xmin-1, width-2, ROAD);
    drawLine(Xmin+1, 0, Xmin+1, width-2, ROAD);
    background[Xmin][width-4] = DOOR;
}

void MapGenerator::drawGrassland(){
    drawContours();
    for(int i = 0; i < length*width/4; i++){
        background[rand()%length][rand()%width] = WALL;
    }
    while(true){
        int x = rand() % length, y = rand() % width;
        if(countNeighbours(x, y) > 0 && countNeighbours(x,y) < 3){
            background[x][y] = DOOR;
            return;
        }
    }
}

void MapGenerator::drawIceberg(int x, int y, int vmax){ // veritacles number
    int Cx = x, Cy = y;
    int xnew = x, ynew = y;
    int vnum = 1;
    for(int i = 0; i < vmax; i++){
        while(xnew > 0 && xnew < length-1 && ynew > 0 && ynew < width && (Cx-xnew)*(Cx-xnew) + (Cy-ynew)*(Cy-ynew) <= 6 ){
            xnew = rand()%length;
            ynew = rand()%width;
        }
        int S = (Cx*vnum+xnew)/(1+vnum);
        Cx = S;
        S = (Cy*vnum+xnew)/(1+vnum);
        Cy = S;
        drawLine(x, y, Cx, Cy, WALL);
    }
}

void MapGenerator::drawArctic(){ // Не до конца готовы биомы ARCTIC и TOWN
    drawContours();
    for(int i = 0; i < 6; i++) drawIceberg(rand()%length, rand()%width, rand()%4 + 4);
    int i = rand() % length, j = rand() % width, doorCounter = 0;
    while(background[i][j] == WALL || doorCounter != 1){
        i = 1 + rand() % (length - 1), j = 1 + rand() % (width - 1);
        background[i][j] = DOOR;
        doorCounter++;
    }
}

// Получение карты (генератор очищает фон, генерирует новый мир и возвращает карту (в зависимости от биома) )

Map* MapGenerator::getRandMap(){
    int biome = rand() % 6;
    if(biome == CAVE || biome == VELVET) return getCave(biome);
    if(biome == CITY) return getCity();
    if(biome == SPACE) return getSpaceMap();
    if(biome == GRASSLAND) return getGrassland();
    if(biome == ARCTIC) return getArctic();
    else return getRandMap();
}

Map* MapGenerator::getSpaceMap(){
    drawSpaceMap();
    Map* newMap = new Map(background);
    newMap->setBiome(SPACE);
    return newMap;
}

Map* MapGenerator::getSpaceship(){
    drawSpaceship();
    Map* newMap = new Map(background);
    newMap->setBiome(SPACE);
    return newMap;
}

Map* MapGenerator::getCave(int biome){
    drawCave();
    Map* newMap = new Map(background);
    newMap->setBiome(biome);
    return newMap;
}

Map* MapGenerator::getCity(){
    drawCity();
    Map* newMap = new Map(background);
    newMap->setBiome(CITY);
    return newMap;
}

Map* MapGenerator::getGrassland(){
    drawGrassland();
    Map* newMap = new Map(background);
    newMap->setBiome(GRASSLAND);
    return newMap;
}

Map* MapGenerator::getArctic(){
    drawArctic();
    Map* newMap = new Map(background);
    newMap->setBiome(ARCTIC);
    return newMap;
}
