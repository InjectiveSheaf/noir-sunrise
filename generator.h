#ifndef GENERATOR_H
#define GENERATOR_H

#include "mapfunctions.h"
#include <string>

class MapGenerator{
    int length;
    int width;
    vector<vector<int>> background;
public:
    MapGenerator(){;}

    // Технические функции вроде подсчёта соседей, установки размера генерируемой карты, прорисовки отрезков, комнат и так далее.

    int countNeighbours(int x, int y);

    void setSize(int _length, int _width){
        length = _length;
        width = _width;
        background = vector<vector<int>>(length, vector<int>(width, GROUND));
    }

    bool isSide(unsigned int i, unsigned int j){
        if(i == 0 || j == 0 || i == background.size()-1 || j == background[i].size()-1)
        return 1;
        else return 0;
    }

    void drawContours();

    void drawRoom(int x, int y, int l, int w);

    void drawRoomwDoor(int x, int y, int l, int w);

    void drawLine(int x1, int y1, int x2, int y2);

    void drawLine(int x1, int y1, int x2, int y2, int MATERIAL);

    void drawDDA(int x1, int y1, int x2, int y2);

    // Отрисовка крупномасштабных структур вроде карт, космического корабля, айсберга, города и т.д.

    void drawSpaceship();

    void drawIceberg(int x, int y, int vmax);

    void drawCave();

    void drawSpaceMap();

    void drawCity();

    void drawGrassland();

    void drawArctic();

    /* Получение самих карт. Пещерный биом существует в двух вариациях.
     *генератор очищает фон, генерирует новый мир и возвращает карту (в зависимости от биома)
     */

    Map* getRandMap();

    Map* getSpaceship();

    Map* getCave(int biome);

    Map* getSpaceMap();

    Map* getCity();

    Map* getGrassland();

    Map* getArctic();
};

#endif // GENERATOR_H
