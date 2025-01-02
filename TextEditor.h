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
#include "Helper.h"
#include "TextEditor.h"

using namespace std;

class TextEditor
{
private:
    Document* currentDoc;
    bool isRunning;

    void handleInput()
    {
        int ch = _getch();

        if (ch == 0 || ch == 224)
        {
            ch = _getch();
            switch (ch)
            {
            case KEY_UP:
            {
                currentDoc->moveCursor(-1, 0);
                break;
            }
            case KEY_DOWN:
            {
                currentDoc->moveCursor(1, 0);
                break;
            }
            case KEY_LEFT:
            {
                currentDoc->moveCursor(0, -1);
                break;
            }
            case KEY_RIGHT:
            {
                currentDoc->moveCursor(0, 1);
                break;
            }
            }
        }
        else
        {
            switch (ch)
            {
            case KEY_ENTER:
            {
                currentDoc->handleEnter();
                break;
            }
            case KEY_BACKSPACE:
            {
                currentDoc->handleBackspace();
                break;
            }
            case KEY_ESC:
            {
                isRunning = false;
                break;
            }
            default:
            {
                switch (ch)
                {
                case 21:
                {
                    currentDoc->ToUpper();
                    break;
                }
                case 12:
                {
                    currentDoc->ToLower();
                    break;
                }
                case 6:
                {
                    string word;
                    cout << "Enter word to search (Case-Sensitive): ";
                    cin >> word;
                    if (currentDoc->FindWordCaseSensitive(word))
                    {
                        ResetConsoleTextColor();
                    }
                    else
                    {
                        ResetConsoleTextColor();
                        cout << "Word not found." << endl;
                    }
                    _getch();
                    break;
                }
                case 20:
                {
                    string word;
                    cout << "Enter word to search (Case-Insensitive): ";
                    cin >> word;
                    if (currentDoc->FindWordCaseInsensitive(word))
                    {
                        ResetConsoleTextColor();
                    }
                    else
                    {
                        ResetConsoleTextColor();
                        cout << "Word not found." << endl;
                    }
                    _getch();
                    break;
                }
                case 19:
                {
                    string sentence;
                    cout << "Enter sentence to search: ";
                    cin.ignore();
                    getline(cin, sentence);

                    int lineNo = currentDoc->FindSentence(sentence);
                    if (lineNo != -1)
                    {
                        ResetConsoleTextColor();
                        cout << "Sentence found on line " << lineNo << "." << endl;
                    }
                    else
                    {
                        ResetConsoleTextColor();
                        cout << "Sentence not found." << endl;
                    }
                    _getch();
                    break;
                }
                case 2:
                {
                    string substring;
                    cout << "Enter substring to find: ";
                    cin.ignore();
                    getline(cin, substring);

                    vector<int> lineNumbers = currentDoc->FindSubstring(substring);
                    if (!lineNumbers.empty())
                    {
                        cout << "Substring found at the following line(s): ";
                        for (int lineNo : lineNumbers)
                        {
                            cout << lineNo << " ";
                        }
                        cout << endl;
                    }
                    else
                    {
                        cout << "Substring not found." << endl;
                    }
                    system("pause");
                    break;
                }
                case 23:
                {
                    string oldWord, newWord;
                    cout << "Enter the word to replace: ";
                    cin >> oldWord;
                    cout << "Enter the new word: ";
                    cin >> newWord;
                    currentDoc->ReplaceAllInFile(oldWord, newWord);
                    break;
                }
                case 18:
                {
                    string oldWord, newWord;
                    cout << "Enter the word to replace: ";
                    cin >> oldWord;
                    cout << "Enter the new word: ";
                    cin >> newWord;
                    currentDoc->ReplaceFirstWord(oldWord, newWord);
                    break;
                }
                case 16:
                {
                    string targetWord, prefix;
                    cout << "\nEnter the word to replace: ";
                    cin >> targetWord;
                    cout << "Enter the prefix to add: ";
                    cin >> prefix;
                    currentDoc->AddPrefixToWord(targetWord, prefix);
                    break;
                }
                case 15:
                {
                    string targetWord, postfix;
                    cout << "\nEnter the word to replace: ";
                    cin >> targetWord;
                    cout << "Enter the postfix to add: ";
                    cin >> postfix;
                    currentDoc->AddPostfixToWord(targetWord, postfix);
                    break;
                }
                case 1:
                {
                    double average = currentDoc->AvgWordLength();
                    cout << "\nAverage word length: " << average << endl;
                    system("pause");
                    break;
                }
                case 4:
                {
                    cout << "Enter the substring to count: ";
                    string substring;
                    cin >> substring;

                    int count = currentDoc->SubstringCount(substring);
                    cout << "The substring '" << substring << "' was found " << count << " times in the file.\n";
                    system("pause");
                    break;
                }
                case 5:
                {
                    int specialCharCount = currentDoc->FindSpecialCharactersCount();
                    cout << "Special Characters Count: " << specialCharCount << endl;
                    _getch();
                    break;
                }
                case 7:
                {
                    int sentences = 0, paragraphs = 0;
                    currentDoc->FindSentenceAndParagraphCount(sentences, paragraphs);
                    cout << "Sentences: " << sentences << ", Paragraphs: " << paragraphs << endl;
                    _getch();
                    break;
                }
                case 8:
                {
                    int largestWordLength = currentDoc->FindLargestWordLength();
                    cout << "Largest Word Length: " << largestWordLength << endl;
                    _getch();
                    break;
                }
                default:
                    currentDoc->insertChar(ch);
                    break;
                }
                break;
            }
            case 28:
            {
                string oldWord, newWord;
                cout << "Enter the word to replace: ";
                cin >> oldWord;
                cout << "Enter the new word: ";
                cin >> newWord;
                currentDoc->FindReplaceFromNext(oldWord, newWord);
                _getch();
                break;
            }
            case 17:
            {
                int smallestWordLength = currentDoc->FindSmallestWordLength();
                cout << "Smallest word length: " << smallestWordLength << endl;
                _getch();
                break;
            }
            }
        }
    }

public:
    TextEditor(string filename) : currentDoc(nullptr), isRunning(true)
    {
        currentDoc = new Document(filename);
        currentDoc->loadFile();
    }

    void run()
    {
        while (isRunning)
        {
            currentDoc->display();
            handleInput();
        }

        currentDoc->saveFile();
    }

    ~TextEditor()
    {
        delete currentDoc;
    }
};
