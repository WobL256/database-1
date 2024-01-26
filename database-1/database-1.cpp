#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

string filename = " ";
int selectedElement = NULL;
string elementName = "";
int cursorPos = 1;
int numberOfElementsInDB = 0;


string getLineByNum(ifstream& inputFile, int targetLine) { //function to get an entire line from a text file as a string
    string line;
    int currentLine = 0;

    inputFile.clear();  // Clear any error flags
    inputFile.seekg(0, ios::beg);  // Reset file position to the beginning

    while (getline(inputFile, line)) {
        ++currentLine;
        if (currentLine == targetLine) {
            return line;  // Return the line when the target line is found
        }
    }

    // If the target line is not found, return a placeholder line
    return "----";
}

void openDatabase(string inputFilename); //the argument is used to tell the function which database to open and display, " " is used to ask the user
void controlsView(string situation); //situation argument is used to display different controls for different contexts
char listenInput(string situation); //situation argument is used for taking input in different contexts of the controlsView
void refreshScreen(string screen, string controlsViewSituation);
void listenControls();
void openElementByNum(int elementNumber);

char listenInput() {
    char key = 0;
    key = _getch();

    return key;
}

string cinLine() {
    string input;
    getline(cin, input);
    return input;
}

void createElement(bool _refreshScreen = false) {
    elementName = cinLine();
    if (elementName != "") {
        ofstream outputFile(elementName + ".txt");

        if (outputFile.is_open()) {
            //do stuff with the newly created element's file, like adding placeholder entries
        }
        else {
            cout << "Eroare: Elementul nu a putut fii creat.";
        }
        if (_refreshScreen == true) {
            refreshScreen("db-view", "db-def");
        }
        outputFile.close();
        elementName = "";
    }
    else {
        createElement(_refreshScreen);
    }
}

void listenControls(string situation) {
    char key;
    if (situation == "db-def") {
        key = listenInput();
        if (key == 110) { //n key

            refreshScreen("db-view", "sel-nr");
        }
        else if (key == 50) { //down arrow
            cursorPos++;
            refreshScreen("db-view", "db-def");
        }
        else if (key == 56) { //up arrow
            cursorPos--;
            refreshScreen("db-view", "db-def");
        }
        else if (key == 13) { //enter key
            selectedElement = cursorPos;
            refreshScreen("elem-view", "elem-def");
        }
        else if (key == 99) { //c key

            refreshScreen("db-view", "add-elem");
        }
        else { //if none of those keys are pressed just do nothing instead of closing the program
            refreshScreen("db-view", "db-def");
        }
    }
    else if (situation == "sel-nr") {
        cin >> selectedElement;
        refreshScreen("elem-view", "elem-def");
    }
    else if (situation == "add-elem") {
        createElement(true);
    }
    else if (situation == "elem-def") {
        key = listenInput();
        if (key == 27) {
            refreshScreen("db-view", "db-def");
        }
        else {
            refreshScreen("elem-view", "elem-def");
        }
    }
}

void controlsView(string situation) {
    if (situation == "db-def") { //default controls for db-view
        cout << "\n|------------------------------------------| Comenzi |------------------------------------------|\n";
        cout << "[C] Creare element   [X] Stergere element   [sus - jos] Navigare   [Enter] Selectare element\n";
        cout << "[S] Sortare elemente   [F] Cautare element   [N] Selectare dupa numar\n";
    }
    else if (situation == "sel-nr") { //select by number contextual menu for db-view
        cout << "\n|-------------------------| Introduceti numarul elementului de deschis |-------------------------|\n";
        cout << "> ";
    }
    else if (situation == "add-elem") {
        cout << "\n|--------------------------| Introduceti numele elementului de creat |--------------------------|\n";
        cout << "> ";
    }
    else if (situation == "elem-def") { //default controls for elem-view
        cout << "\n|------------------------------------------| Comenzi |------------------------------------------|\n";
        cout << "[A] Adaugare camp nou   [X] Stergere camp   [sus - jos] Navigare   [Enter] Modificare camp\n";
    }
}

void openElement(string elementName) {
    system("cls");

    ifstream inputElementFile(elementName + ".txt");
    if (inputElementFile.is_open()) {
        cout << "| << Esc |---------------------| Element deschis: " << elementName << " |-----------------------------|" << endl;
        for (int i = 1; i <= 3; i++) {
            cout << "|" << i << "| " << getLineByNum(inputElementFile, i) << endl; //list all lines of the file
        }
    }
    else {
        cout << "Eroare: Elementul nu exista! Apasati Enter sau Escape pentru a va intoarce la meniul anterior.";
    }
    inputElementFile.close();
}

void openElementByNum(int elementNumber) {
    ifstream inputFile(filename + ".txt");
    openElement(getLineByNum(inputFile, elementNumber));
    inputFile.close();
}

void openDatabase(string inputFilename) {
    system("cls");

    char key = NULL;

    if (filename == " ") {
        cout << "Introduceti numele bazei de date pe care doriti sa o deschideti: ";
        cin >> filename;
        inputFilename = filename;
    }
    system("cls");

    ifstream inputFile(inputFilename + ".txt");

    string elementName;
    if (inputFile.is_open()) { //read the database file
        int elementNumber = 0;
        cout << "| << Esc |----------------------| Baza de date deschisa: " << filename << " |------------------------------|" << endl;

        for (int i = 1; i <= 10; i++) { //list all lines of the file with the cursor as well
            if (cursorPos != i) {
                cout << "|" << i << "| " << getLineByNum(inputFile, i) << endl;
            }
            else {
                cout << ">" << i << "<  " << getLineByNum(inputFile, i) << endl;
            }
            if (getLineByNum(inputFile, i) != "----") numberOfElementsInDB++;
        }

        inputFile.close();
    }
    else {
        cout << "Eroare: Nu s-a putut deschide baza de date [" << inputFilename << "]!" << endl;
    }
}

int main() //main function of the program
{
    openDatabase(" "); //open the database and display it, " " is used to tell the function to ask the user for a database
    controlsView("db-def"); //display the controls view on the screen, "db-def" is used to tell the function to display the default controls screen
    listenControls("db-def");
}

void refreshScreen(string screen, string controlsViewSituation) {
    if (screen == "db-view") {
        system("cls");
        openDatabase(filename);
        controlsView(controlsViewSituation);
        listenControls(controlsViewSituation);
    }
    else if (screen == "elem-view") {
        system("cls");
        openElementByNum(selectedElement);
        controlsView("elem-def");
        listenControls("elem-def");
    }
}