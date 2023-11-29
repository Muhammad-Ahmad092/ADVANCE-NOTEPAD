#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include"TrieTree.h"
#include <windows.h>
using namespace std;

stack<string> previousTexts; // Stack to store previous texts
stack<string> deletedTexts; // Stack to store previous texts
int x = 0, y = 0;
string copiedText;
TrieTree t;


void AddText()
{
    string data = "";
    int temp = 0;
    cout << "Input the Location of Your word" << endl;
    cout << "Enter X position: ";
   // cin >> x;
    try {
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();

            cout << "Your Input is worng" << endl;
            throw 1;
        }

    }
    catch (int i) {
        cout << "X - axis Position =  ";
        cin >> x;
    }
    
    cout << "Enter Y position: ";
    cin >> y;
    try {
        if (cin.fail()) {
            cin.clear();
            cin.ignore();

            cout << "Your Input is worng" << endl;
            throw 1;
        }

    }
    catch (int i) {
        cout << "Y - axis Position =  ";
        cin >> y;
    }

    ofstream file;
    y--;
    y = 191 * y;
    y = y + x;
    bool check = false;

    while (true)
    {
        if (temp == -1)
        {
            break;
        }
        char keypress;
        file.open("editior.txt", ios::app);
        cout << "Enter data: ";
        cin.ignore(); // Ignore the newline character in the input buffer
        getline(cin, data);


        string* suggested = t.suggest(data);

        for (int i = 0; suggested[i] != "NULL" && i < 10; i++) {
            cout << i << ":" << suggested[i] << endl;
        }
        int ch;
        if (suggested != NULL) {
            cout << "Enter Choice: or -1 to not choose ";
            cin >> ch;
            if (ch >= 0) {
                data = suggested[ch];
            }
        }


        if (check == false)
        {
            for (int i = 0; i < y; i++)
            {
                if ((i) % 191 == 0 && i != 0 && check == false)
                {
                    file << endl;
                }
                file << ' ';
            }
        }
        else
        {
            file << ' ';
        }



        check = true;
        file << data;
        previousTexts.push(data); // Store the entered data in the previousTexts stack
        cout << " (enter -1 to exit or press 1 to continue): ";
        cin >> temp;
        file.close();
    }
}
void UndoText()
{
    if (!previousTexts.empty())
    {
        string lastText = previousTexts.top();
        previousTexts.pop();
        deletedTexts.push(lastText); // Store the deleted text in the deletedTexts stack
        std::cout << "Undo successful." << endl;

        // Rewrite the file with remaining texts from previousTexts stack
        ofstream file("editior.txt", ios::trunc); // Truncate the file
        bool check2 = false;
        while (!previousTexts.empty())
        {
            if (check2 == false)
            {
                for (int i = 0; i < y; i++)
                {
                    if ((i) % 191 == 0 && i != 0 && check2 == false)
                    {
                        file << endl;
                    }
                    file << ' ';
                }
            }
            else
            {
                file << ' ';
            }
            file << previousTexts.top() << " ";
            previousTexts.pop();
            check2 = true;
        }
        file.close();
    }
    else
    {
        std::cout << "No text to undo." << endl;
    }
}


void CountWords()
{
    ifstream file("editior.txt");
    int words = -1;

    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    int characters = 0;
    bool check = false;

    while (!file.eof())
    {
        file >> line;
        words++;
        characters += line.size();
    }

    file.close();
    std::cout << "Total Words: " << words << endl;
    std::cout << "Total Characters: " << characters << endl;
}

void SimilarityChecker()
{
    ifstream file1("file1.txt"); // Replace "file1.txt" with the path to the first file
    ifstream file2("file2.txt"); // Replace "file2.txt" with the path to the second file

    if (!file1 || !file2)
    {
        cerr << "Failed to open one or both files." << endl;
        return;
    }

    char char1, char2;
    int similarChars = 0;
    int totalChars = 0;

    while (file1.get(char1) && file2.get(char2))
    {
        totalChars++;
        if (char1 == char2)
        {
            similarChars++;
        }
    }

    file1.close();
    file2.close();

    float similarityPercentage = (static_cast<float>(similarChars) / totalChars) * 100;

    std::cout << "Similarity percentage: " << similarityPercentage << "%" << endl;
}

void UpdateSugesstions() {
    string searchWord, replaceWord;
    std::cout << "Enter the word to search: ";
    std::cin >> searchWord;
    std::cout << "Enter the word to replace with: ";
    std::cin >> replaceWord;

    ifstream file("dictionary.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool found = false;

    while (getline(file, line))
    {
        size_t pos = line.find(searchWord);
        while (pos != string::npos)
        {
            found = true;
            line.replace(pos, searchWord.length(), replaceWord);
            pos = line.find(searchWord, pos + replaceWord.length());
        }

        fileContents.push_back(line);
    }

    file.close();

    if (found)
    {
        ofstream outFile("dictionary.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        std::cout << "Word replaced successfully." << endl;
    }
    else
    {
        std::cout << "Word not found." << endl;
    }
}

void CopyText()
{
    int startX, startY, endX, endY;
    std::cout << "Enter starting position (X, Y): ";
    std::cin >> startX >> startY;
    std::cout << "Enter ending position (X, Y): ";
    std::cin >> endX >> endY;

    ifstream file("editior.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    string copiedString;
    int lineNumber = 0;
    bool copyInProgress = false;

    while (getline(file, line))
    {
        lineNumber++;

        if (lineNumber >= startY && lineNumber <= endY)
        {
            if (lineNumber == startY)
            {
                // Copy from the starting position
                int startPos = startX - 1;
                int endPos = (lineNumber == endY) ? (endX - 1) : line.size();
                copiedString += line.substr(startPos, endPos - startPos + 1);
            }
            else if (lineNumber == endY)
            {
                // Copy until the ending position
                int endPos = endX - 1;
                copiedString += line.substr(0, endPos + 1);
            }
            else
            {
                // Copy the whole line
                copiedString += line;
            }

            copyInProgress = true;
        }
        else if (copyInProgress)
        {
            // Copying has been completed
            break;
        }
    }

    file.close();
    copiedText = copiedString;

    std::cout << "Text copied successfully." << endl;
}

void PasteText()
{
    int pasteX, pasteY;
    std::cout << "Enter paste position (X, Y): ";
    std::cin >> pasteX >> pasteY;

    ifstream file("editior.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    int lineNumber = 0;
    bool pasteSuccessful = true;

    while (getline(file, line))
    {
        lineNumber++;

        if (lineNumber == pasteY)
        {
            int pastePos = pasteX - 1;
            if (pastePos >= 0 && pastePos <= line.size())
            {
                if (line[pastePos] != ' ')
                {
                    // Text already found at the paste position
                    char choice;
                    std::cout << "Text already found. Do you still want to paste the text? (y/n): ";
                    std::cin >> choice;
                    if (choice != 'y' && choice != 'Y')
                    {
                        pasteSuccessful = false;
                        break;
                    }
                }
                line.replace(pastePos, copiedText.size(), copiedText);
            }
        }

        fileContents.push_back(line);
    }

    file.close();

    if (pasteSuccessful)
    {
        ofstream outFile("editior.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        std::cout << "Text pasted successfully." << endl;
    }
}

void DeleteText()
{
    string deleteString;
    std::cout << "Enter the string to delete: ";
    std::cin.ignore();
    getline(std::cin, deleteString);

    ifstream file("editior.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool deleteSuccessful = false;

    while (getline(file, line))
    {
        if (line.find(deleteString) != string::npos)
        {
            // Match found, delete the line
            deleteSuccessful = true;
        }
        else
        {
            fileContents.push_back(line);
        }
    }

    file.close();

    if (deleteSuccessful)
    {
        ofstream outFile("editior.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        std::cout << "Text deleted successfully." << endl;
    }
    else
    {
        std::cout << "No matching text found." << endl;
    }
}

void SearchWords()
{
    string searchString;
    std::cout << "Enter the string to search: ";
    std::cin.ignore();
    getline(std::cin, searchString);

    ifstream file("editior.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    bool found = false;

    while (getline(file, line))
    {
        lineNumber++;

        size_t pos = line.find(searchString);
        while (pos != string::npos)
        {
            found = true;
            std::cout << "String found at position (" << pos + 1 << ", " << lineNumber << ")" << endl;
            pos = line.find(searchString, pos + 1);
        }
    }

    file.close();

    if (!found)
    {
        std::cout << "String not found." << endl;
    }
}

void ReplaceWords()
{
    string searchWord, replaceWord;
    std::cout << "Enter the word to search: ";
    std::cin >> searchWord;
    std::cout << "Enter the word to replace with: ";
    std::cin >> replaceWord;

    ifstream file("editior.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool found = false;

    while (getline(file, line))
    {
        size_t pos = line.find(searchWord);
        while (pos != string::npos)
        {
            found = true;
            line.replace(pos, searchWord.length(), replaceWord);
            pos = line.find(searchWord, pos + replaceWord.length());
        }

        fileContents.push_back(line);
    }

    file.close();

    if (found)
    {
        ofstream outFile("editior.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        std::cout << "Word replaced successfully." << endl;
    }
    else
    {
        std::cout << "Word not found." << endl;
    }
}


int main()
{

    fstream dictionary;
    dictionary.open("dictionary.txt", ios::in);
    int choice = -1;
    if (!dictionary) {
        std::cout << "File not found";
    }
    string s;
    // loading a dictionary words from file
    while (!dictionary.eof())
    {
        dictionary >> s;
        t.LoadData(s);
    }
    dictionary.close();

    std::cout << "                   :--------------------------------------------------:" << endl << endl;  Sleep(250);
    std::cout << "                         FAST NATIONAL UNIVERSITY (CFD CAMPUS)     " << endl << endl;      Sleep(300);
    std::cout << "                                Advance Notepad System              " << endl;
    std::cout << "                   :--------------------------------------------------:" << endl;          Sleep(500);
    std::cout << "                            Muhammad Ahmad   21F _ 9195" << endl;                          Sleep(400);
    std::cout << "                            Muhammad Umer    21F _ 9291" << endl << endl << endl;          Sleep(400);
    std::cout << "                                      Assigned By :" << endl;                              Sleep(400);
    std::cout << "                                     SIR MUGHEES ISMAIL " << endl;                         Sleep(400);
    std::cout << "                                            START" << endl;                                Sleep(300);
    std::cout << "                                          Loading..." << endl;
    Sleep(1200);
    system("cls");
    system("color F0");
    while (choice != 0)
    {
        Sleep(400);
        std::cout << "                                Advance Notepad System              " << endl;
        std::cout << "       MAIN MENU" << endl;
        Sleep(400);
        std::cout << "1-  Insert Text" << endl;      Sleep(350);
        std::cout << "2-  Delete Text" << endl;      Sleep(350);
        std::cout << "3-  Copy   Text" << endl;      Sleep(350);
        std::cout << "4-  Paste  Text" << endl;      Sleep(350);
        std::cout << "5-  Undo   Text" << endl;      Sleep(350);
        std::cout << "6-  Search Words" << endl;     Sleep(350);
        std::cout << "7-  Count  Words" << endl;     Sleep(350);
        std::cout << "8-  Similarity checker" << endl;           Sleep(350);
        std::cout << "9- Find and Replace " << endl;            Sleep(350);
        std::cout << "10- Update suggestion.txt file: " << endl; Sleep(350);
        std::cout << "0-  Exit" << endl;
        std::cout << "Enter numbers from 1-10 to go to the menu functions, else press 0 to exit\n";
        
        try {
            std::cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore();

                cout << "Your Input is worng" << endl;
                throw 1;
            }

        }
        catch (int i) {
            cout << "Input Choice again";
            cin >> choice;
        }
        switch (choice)
        {
        case 1:
            AddText();
            break;

        case 2:
            DeleteText();

            break;

        case 3:
            CopyText();

            break;

        case 4:
            PasteText();

            break;

        case 5:
            UndoText();

            break;

        case 6:
            SearchWords();
            break;

        case 7:
            CountWords();
            break;

        case 8:
            SimilarityChecker();
            break;

        case 9:
            ReplaceWords();
            break;
        case 10:
            UpdateSugesstions();
            break;
        default:
            break;
        }

        system("pause");
        system("cls");
    }

    return 0;
}