#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "Line.h"
#include "Document.h"
#include "Helper.h"
#include "TextEditor.h"
using namespace std;
namespace fs = experimental::filesystem;


void SetConsoleColor(int textColor, int bgColor) {
    int colorCode = (bgColor << 4) | textColor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void SetWhiteBackground() {
    system("color F0");
}

void SetDefaultTextColor() {
    SetConsoleColor(0, 15);
}

void showMenu() {
    SetConsoleColor(12, 15);
    cout << "Controls:\n";
    cout << "===================================================================\n";
    cout << "CTRL + U  : Uppercase       CTRL + L : Lowercase\n";
    cout << "CTRL + F  : Find (Case-S)   CTRL + T : Find (Case-I)\n";
    cout << "CTRL + S  : Find Sentence   CTRL + B : Find Substring\n";
    cout << "CTRL + W  : Replace All     CTRL + R : Replace First\n";
    cout << "CTRL + P  : Add Prefix      CTRL + O : Add Postfix\n";
    cout << "CTRL + A  : Avg Word Len    CTRL + D : Count Substring\n";
    cout << "CTRL + E  : Count Specials  CTRL + G : Count Sent/Para\n";
    cout << "CTRL + H  : Longest Word    CTRL + Q : Shortest Word\n";
    cout << "ESC      : Save & Exit      CTRL + BS : Replace First From Cursor\n";
    cout << "===================================================================\n";
    SetDefaultTextColor();
}


void createNewFile(string& filename) {
    cout << "Enter the name of the new document (without .txt extension): ";
    cin >> filename;

    if (filename.find(".txt") == string::npos) {
        filename += ".txt";
    }

    ofstream newFile(filename);
    newFile.close();

    cout << "\nFile \"" << filename << "\" created successfully!\n";
    this_thread::sleep_for(chrono::seconds(2));
}

void openFile(string& filename) {
    cout << "Enter the name of the file to open: ";
    cin >> filename;

    if (filename.find(".txt") == string::npos) {
        filename += ".txt";
    }

    ifstream file(filename);
    if (file.is_open()) {
        cout << "\nFile \"" << filename << "\" opened successfully.\n";
    }
    else {
        cout << "\nFile not found. Please check the file name and try again.\n";
    }
}

bool checkFileExists(const string& filename) {
    return fs::exists(filename);
}

void mergeDocuments() {
    int numDocs;
    vector<string> fileNames;
    string mergedFileName;

    cout << "\nEnter the number of documents to merge: ";
    cin >> numDocs;

    if (numDocs <= 1) {
        cout << "You need at least two documents to merge.\n";
        return;
    }

    for (int i = 0; i < numDocs; ++i) {
        string name;
        cout << "Enter the name of document " << i + 1 << " (without .txt): ";
        cin >> name;
        name += ".txt";

        if (!checkFileExists(name)) {
            cout << "File \"" << name << "\" does not exist. Aborting merge...\n";
            return;
        }

        fileNames.push_back(name);
    }

    cout << "Enter the name for the merged document (without .txt): ";
    cin >> mergedFileName;
    mergedFileName += ".txt";

    ofstream mergedFile(mergedFileName, ios::out);
    if (!mergedFile) {
        cout << "Failed to create the merged file. Aborting...\n";
        return;
    }

    for (const auto& file : fileNames) {
        ifstream inputFile(file, ios::in);
        if (inputFile) {
            mergedFile << "\n===== Content from " << file << " =====\n";
            mergedFile << inputFile.rdbuf(); 
            inputFile.close();
        }
        else {
            cout << "Error reading file \"" << file << "\". Skipping...\n";
        }
    }

    mergedFile.close();
    cout << "Files have been successfully merged into \"" << mergedFileName << "\".\n";
}

int main() {
    char choice;
    string filename;

    SetWhiteBackground();
    cout << "\n===============================\n";
    cout << "         Text Editor           \n";
    cout << "===============================\n\n";
    cout << "Do you want to create a new document (C), open an existing one (O), or merge documents (M)? (C/O/M): ";
    cin >> choice;

    if (choice == 'C' || choice == 'c') {
        createNewFile(filename);
    }
    else if (choice == 'O' || choice == 'o') {
        openFile(filename);
    }
    else if (choice == 'M' || choice == 'm') {
        mergeDocuments();
        return 0; 
    }
    else {
        cout << "\nInvalid choice! Exiting...\n";
        return 1;
    }

    system("cls");

    showMenu();
    cout << "\nPress any key to start...\n";
    _getch();

    TextEditor editor(filename);
    editor.run();

    return 0;
}

