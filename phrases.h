#ifndef PHRASES_H
#define PHRASES_H

#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

enum English{
    START,
    NAME,
    BEGIN,
    WEAPONS,
    HELP_0,
    HELP,
    ATTACK,
    DEFENCE,
    PICK,
    DELETE
};

void type_text(const std::string& text)
{
    for (std::size_t i = 0; i < text.size(); ++i)
    {
        std::cout << text[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
}

// Ну тут, думаю, довольно понятно - обычный вывод фраз.

void Print(English phrase){
    string begin;
    switch (phrase) {
    case START:
        cout << "Hello, Space Cowboy." << endl;
        cout << "What is your name?" << endl;
        break;
    case BEGIN:
        type_text("You know, Cowboy, the situation around our system is not so good. ");
        cout << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1000));
        type_text("Ever since the Trade Federation started a war with powerful industrial empire ");
        cout << endl;
        type_text("called Machine Cult, armies of rogues, bandits and other creatures began to ");
        cout << endl;
        type_text("gather round the Earth. ");
        cout << endl;
        this_thread::sleep_for(std::chrono::milliseconds(2000));
        type_text("There are not many of us left. Each of us is important. We need to save the Earth.");
        cout << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1000));
        type_text("Good luck, Space Cowboy.");
        cout << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1500));
        break;
    case WEAPONS:
        cout << "Weapons has been initialized. Pick up one of them: \n 1 - Beginner's sword \n 2 - Beginner's wand \n 3 - Beginner's bow." << endl;
        break;
    case HELP:
        cout << "Help page: \n There are no help there... \n";
        break;
    case HELP_0:
        cout << "To open the help page press \"h\", to exit press \"e\" \n";
        break;
    default:
        break;
    }
}

#endif // PHRASES_H
