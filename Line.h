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
using namespace std;

class Line {
private:
    list<char> chars;
    int size;

public:
    Line() : size(0) {}

    void insertChar(char ch, int pos)
    {
        auto i = chars.begin();
        advance(i, pos);
        chars.insert(i, ch);
        size++;
    }

    void deleteChar(int pos)
    {
        if (pos < size)
        {
            auto i = chars.begin();
            advance(i, pos);
            chars.erase(i);
            size--;
        }
    }

    int getSize() const
    {
        return size;
    }

    void display() const
    {
        for (char ch : chars)
        {
            cout << ch;
        }
    }

    void clear()
    {
        chars.clear();
        size = 0;
    }

    list<char>& getChars()
    {
        return chars;
    }

    const list<char>& getChars() const
    {
        return chars;
    }
};