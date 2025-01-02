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
#include "Helper.h"
using namespace std;

class Document {
private:
    vector<Line> lines;
    int cursorRow;
    int cursorCol;
    string filename;

public:
    Document(string fname) : filename(fname), cursorRow(0), cursorCol(0)
    {
        lines.push_back(Line());
    }

    void display()
    {
        system("cls");
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i].display();
            cout << endl;
        }
        gotoRowCol(cursorRow, cursorCol);
    }

    void insertChar(char ch)
    {
        if (ch >= 32 && ch <= 126)
        {
            while (cursorRow >= lines.size())
            {
                lines.push_back(Line());
            }
            lines[cursorRow].insertChar(ch, cursorCol);
            cursorCol++;
            display();
        }
    }

    void handleBackspace()
    {
        if (cursorCol > 0)
        {
            lines[cursorRow].deleteChar(cursorCol - 1);
            cursorCol--;
            display();
        }
    }

    void handleEnter()
    {
        lines.insert(lines.begin() + cursorRow + 1, Line());
        cursorRow++;
        cursorCol = 0;
        display();
    }

    void moveCursor(int deltaRow, int deltaCol)
    {
        int newRow = cursorRow + deltaRow;
        int newCol = cursorCol + deltaCol;

        if (newRow < 0)
        {
            newRow = 0;
        }
        if (newRow >= lines.size())
        {
            newRow = lines.size() - 1;
        }
        if (newCol < 0)
        {
            newCol = 0;
        }
        if (newCol > lines[newRow].getSize())
        {
            newCol = lines[newRow].getSize();
        }

        cursorRow = newRow;
        cursorCol = newCol;
        gotoRowCol(cursorRow, cursorCol);
    }

    int getCursorRow() const
    {
        return cursorRow;
    }
    int getCursorCol() const
    {
        return cursorCol;
    }

    string getFilename() const
    {
        return filename;
    }

    vector<Line>& getLines()
    {
        return lines;
    }

    void loadFile()
    {
        ifstream file(filename);
        if (file.is_open())
        {
            string lineContent;
            lines.clear();
            while (getline(file, lineContent))
            {
                Line line;
                for (char ch : lineContent)
                {
                    line.insertChar(ch, line.getSize());
                }
                lines.push_back(line);
            }
            file.close();
        }
    }

    void saveFile()
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (int i = 0; i < lines.size(); i++)
            {
                for (char ch : lines[i].getChars())
                {
                    file.put(ch);
                }
                file.put('\n');
            }
            file.close();
        }
    }

    void ToUpper()
    {
        if (cursorRow >= lines.size())
            return;

        list<char>& chars = lines[cursorRow].getChars();

        if (chars.empty())
            return;

        list<char>::iterator i = chars.begin();
        advance(i, cursorCol);

        list<char>::iterator start = i;
        while (start != chars.begin() && isalnum(*prev(start)))
        {
            start--;
        }

        list<char>::iterator end = i;
        while (end != chars.end() && isalnum(*end))
        {
            end++;
        }

        for (list<char>::iterator itr = start; itr != end; ++itr)
        {
            *itr = toupper(*itr);
        }

        display();
    }

    void ToLower()
    {
        if (cursorRow >= lines.size())
            return;

        list<char>& chars = lines[cursorRow].getChars();

        if (chars.empty())
            return;

        list<char>::iterator i = chars.begin();
        advance(i, cursorCol);

        list<char>::iterator start = i;
        while (start != chars.begin() && isalnum(*prev(start)))
        {
            start--;
        }

        list<char>::iterator end = i;
        while (end != chars.end() && isalnum(*end))
        {
            end++;
        }

        for (list<char>::iterator itr = start; itr != end; ++itr)
        {
            *itr = tolower(*itr);
        }

        display();
    }

    bool FindWordCaseSensitive(const string& word)
    {
        for (int i = 0; i < lines.size(); i++)
        {
            string lineStr;
            for (char ch : lines[i].getChars())
            {
                lineStr += ch;
            }

            int found = lineStr.find(word);
            if (found != string::npos)
            {
                SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
                cout << "At line " << i + 1 << ", word \"" << word << "\" found at position " << found << endl;
                return true;
            }
        }
        return false;
    }

    bool FindWordCaseInsensitive(const string& word)
    {
        string(*toLowerString)(const string&) = [](const string& str)
            {
                string lowerStr = str;
                for (char& ch : lowerStr)
                {
                    ch = tolower(ch);
                }
                return lowerStr;
            };


        string lowerWord = toLowerString(word);

        for (int i = 0; i < lines.size(); i++)
        {
            string lineStr;
            for (char ch : lines[i].getChars())
            {
                lineStr += ch;
            }

            string lowerLine = toLowerString(lineStr);

            int found = lowerLine.find(lowerWord);
            if (found != string::npos)
            {
                SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
                cout << "At line " << i + 1 << ", word \"" << word << "\" found at position " << found << endl;
                return true;
            }
        }
        return false;
    }

    int FindSentence(const string& sentence)
    {
        string(*toLowerString)(const string&) = [](const string& str)
            {
                string lowerStr = str;
                for (char& ch : lowerStr)
                {
                    ch = tolower(ch);
                }
                return lowerStr;
            };

        string lowerSentence = toLowerString(sentence);

        for (int i = 0; i < lines.size(); i++)
        {
            string lineStr;
            for (char ch : lines[i].getChars())
            {
                lineStr += ch;
            }

            string lowerLine = toLowerString(lineStr);

            int found = lowerLine.find(lowerSentence);
            if (found != string::npos)
            {
                SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
                cout << lowerLine << endl;
                return static_cast<int>(i + 1);
            }
        }
        return -1;
    }

    vector<int> FindSubstring(const string& substring)
    {
        vector<int> lineNumbers;
        string lowerSubstring = substring;

        transform(lowerSubstring.begin(), lowerSubstring.end(), lowerSubstring.begin(), ::tolower);

        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());
            string lowerLine = lineContent;

            transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

            int foundPos = lowerLine.find(lowerSubstring);
            if (foundPos != string::npos)
            {
                lineNumbers.push_back(static_cast<int>(i + 1));

                for (int j = 0; j < lineContent.size(); ++j)
                {
                    if (j == foundPos)
                    {
                        SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
                    }
                    cout << lineContent[j];
                    if (j == foundPos + substring.size() - 1)
                    {
                        ResetConsoleTextColor();
                    }
                }
                cout << endl;
            }
        }
        return lineNumbers;
    }

    void ReplaceFirstWord(const string& oldWord, const string& newWord)
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());

            int pos = lineContent.find(oldWord);
            while (pos != string::npos)
            {
                bool isStartBoundary = (pos == 0 || !isalnum(lineContent[pos - 1]));
                bool isEndBoundary = (pos + oldWord.length() == lineContent.size() ||
                    !isalnum(lineContent[pos + oldWord.length()]));

                if (isStartBoundary && isEndBoundary)
                {
                    lineContent.replace(pos, oldWord.length(), newWord);

                    lines[i].getChars().clear();

                    for (int j = 0; j < lineContent.size(); ++j)
                    {
                        lines[i].insertChar(lineContent[j], j);
                    }

                    return; 
                }

                pos = lineContent.find(oldWord, pos + oldWord.length());
            }
        }
    }



    void ReplaceAllInFile(const string& oldWord, const string& newWord)
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());

            int pos = 0;
            while ((pos = lineContent.find(oldWord, pos)) != string::npos)
            {
                bool isStartBoundary = (pos == 0 || !isalnum(lineContent[pos - 1]));
                bool isEndBoundary = (pos + oldWord.length() == lineContent.size() ||
                    !isalnum(lineContent[pos + oldWord.length()]));

                if (isStartBoundary && isEndBoundary)
                {
                    lineContent.replace(pos, oldWord.length(), newWord);
                    pos += newWord.length(); 
                }
                else
                {
                    pos += oldWord.length(); 
                }
            }

            lines[i].getChars().clear();

            for (int j = 0; j < lineContent.size(); ++j)
            {
                lines[i].insertChar(lineContent[j], j);
            }
        }
    }


    void AddPrefixToWord(const string& targetWord, const string& prefix) {
        string lowerTargetWord = targetWord;
        transform(lowerTargetWord.begin(), lowerTargetWord.end(), lowerTargetWord.begin(), ::tolower);

        for (Line& line : lines) {
            string lineContent(line.getChars().begin(), line.getChars().end());
            string lowerLine = lineContent;

            transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

            int pos = 0;
            while ((pos = lowerLine.find(lowerTargetWord, pos)) != string::npos)
            {
                string originalWord = lineContent.substr(pos, targetWord.size());
                string prefixedWord = prefix + originalWord;

                for (int i = 0; i < targetWord.size(); ++i)
                {
                    line.deleteChar(pos);
                }

                for (char ch : prefixedWord)
                {
                    line.insertChar(ch, pos++);
                }

                lowerLine.replace(pos - prefixedWord.size(), prefixedWord.size(), prefixedWord);
            }
        }
    }

    void AddPostfixToWord(const string& targetWord, const string& postfix)
    {
        string lowerTargetWord = targetWord;
        transform(lowerTargetWord.begin(), lowerTargetWord.end(), lowerTargetWord.begin(), ::tolower);

        for (Line& line : lines)
        {
            string lineContent(line.getChars().begin(), line.getChars().end());
            string lowerLine = lineContent;

            transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

            int pos = 0;
            while ((pos = lowerLine.find(lowerTargetWord, pos)) != string::npos)
            {
                string originalWord = lineContent.substr(pos, targetWord.size());
                string postfixedWord = originalWord + postfix;

                for (int i = 0; i < targetWord.size(); ++i)
                {
                    line.deleteChar(pos);
                }

                for (char ch : postfixedWord)
                {
                    line.insertChar(ch, pos++);
                }

                lowerLine.replace(pos - postfixedWord.size(), postfixedWord.size(), postfixedWord);
            }
        }
    }

    double AvgWordLength()
    {
        int totalCharacters = 0;
        int totalWords = 0;

        for (const Line& line : lines)
        {
            string lineContent(line.getChars().begin(), line.getChars().end());
            int i = 0;

            while (i < lineContent.size())
            {
                while (i < lineContent.size() && isspace(lineContent[i]))
                {
                    ++i;
                }

                int wordLength = 0;
                while (i < lineContent.size() && !isspace(lineContent[i]))
                {
                    ++wordLength;
                    ++i;
                }

                if (wordLength > 0)
                {
                    totalWords++;
                    totalCharacters += wordLength;
                }
            }
        }
        if (totalWords > 0)
        {
            return static_cast<double>(totalCharacters) / totalWords;
        }
        else
        {
            return 0.0;
        }
    }

    int SubstringCount(const string& substring)
    {
        int count = 0;
        string lowerSubstring = substring;

        transform(lowerSubstring.begin(), lowerSubstring.end(), lowerSubstring.begin(), ::tolower);

        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());
            string lowerLine = lineContent;

            transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

            int pos = 0;
            while ((pos = lowerLine.find(lowerSubstring, pos)) != string::npos)
            {
                ++count;
                ++pos;
            }
        }
        return count;
    }

    int FindSpecialCharactersCount()
    {
        int specialCharCount = 0;

        for (const auto& line : lines)
        {
            for (char ch : line.getChars())
            {
                if (!isalnum(ch) && !isspace(ch))
                {
                    specialCharCount++;
                }
            }
        }
        return specialCharCount;
    }

    void FindSentenceAndParagraphCount(int& sentenceCount, int& paragraphCount)
    {
        sentenceCount = 0;
        paragraphCount = 0;
        bool inParagraph = false;

        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());

            for (char ch : lineContent)
            {
                if (ch == '.' || ch == '!' || ch == '?')
                {
                    sentenceCount++;
                }
            }

            if (!lineContent.empty())
            {
                if (!inParagraph)
                {
                    paragraphCount++;
                    inParagraph = true;
                }
            }
            else
            {
                inParagraph = false;
            }
        }
    }

    int FindLargestWordLength()
    {
        int maxLength = 0;

        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());
            int currentLength = 0;

            for (char ch : lineContent)
            {
                if (isalnum(ch))
                {
                    currentLength++;
                }
                else
                {
                    if (currentLength > maxLength)
                    {
                        maxLength = currentLength;
                    }
                    currentLength = 0;
                }
            }
            if (currentLength > maxLength)
            {
                maxLength = currentLength;
            }
        }
        return maxLength;
    }

    void FindReplaceFromNext(const string& oldWord, const string& newWord)
    {
        if (cursorRow >= lines.size())
        {
            cout << "Error: Cursor row is out of bounds." << endl;
            return;
        }

        string lineContent(lines[cursorRow].getChars().begin(), lines[cursorRow].getChars().end());

        if (cursorCol < 0 || cursorCol > lineContent.length())
        {
            cout << "Error: Cursor column is out of bounds." << endl;
            return;
        }

        int pos = lineContent.find(oldWord, cursorCol);

        if (pos != string::npos)
        {
            lineContent.replace(pos, oldWord.length(), newWord);

            lines[cursorRow].getChars().clear();
            for (int i = 0; i < lineContent.size(); ++i)
            {
                lines[cursorRow].insertChar(lineContent[i], i); 
            }

            cursorCol = pos + newWord.length();
        }
        else
        {
            cout << "Word not found from the current cursor position." << endl;
        }
    }

    int FindSmallestWordLength()
    {
        int minLength = INT_MAX;

        for (int i = 0; i < lines.size(); ++i)
        {
            string lineContent(lines[i].getChars().begin(), lines[i].getChars().end());
            int currentLength = 0;

            for (char ch : lineContent)
            {
                if (isalnum(ch))
                {
                    currentLength++;
                }
                else
                {
                    if (currentLength > 0 && currentLength < minLength)
                    {
                        minLength = currentLength;
                    }
                    currentLength = 0;
                }
            }
            if (currentLength > 0 && currentLength < minLength)
            {
                minLength = currentLength;
            }
        }
        if (minLength == INT_MAX)
        {
            return 0;
        }
        else
        {
            return minLength;
        }
    }

};