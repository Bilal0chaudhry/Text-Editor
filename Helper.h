#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <conio.h>
#include <fstream> 
#include <thread>
#include <chrono>
#include "Line.h"
#include "Document.h"
using namespace std;


const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;
const int KEY_ESC = 27;




void gotoRowCol(int rpos, int cpos)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { (SHORT)cpos, (SHORT)rpos };
    SetConsoleCursorPosition(h, position);
}

void SetConsoleTextColor(WORD color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void ResetConsoleTextColor()
{
    SetConsoleTextColor(7);
}