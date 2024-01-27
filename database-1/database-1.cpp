#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <filesystem>
#include <nlohmann/json.hpp>


using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

string filename = " ";
int selectedElement = NULL;
string selectedElementName = "";
int cursorPos = 1;
int cursorPosElem = 1;
int numberOfElementsInDB = 0;

string newAttribute = "";
string newAttributeValue = "";

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

void viewDatabase(string inputFilename); //the argument is used to tell the function which database to open and display, " " is used to ask the user
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
    selectedElementName = cinLine();
    if (selectedElementName != "") {
        
        ifstream dbFile(filename + ".json");
        json jsonData;
        dbFile >> jsonData;
        dbFile.close();

        int highestId = 0; //the highest id in the elements array, basically the number of elements present in the db
        for (const auto& element : jsonData["database"]["elements"]) {
            int currentId = element["id"];
            if (currentId > highestId) highestId = currentId;  
        }

        json newElement = {
        {"id", highestId + 1},
        {"nume", selectedElementName}
        };


        jsonData["database"]["elements"].push_back(newElement);

        ofstream output(filename + ".json");
        output << setw(4) << jsonData;
        output.close();

        selectedElementName = "";
        if (_refreshScreen == true) {
            refreshScreen("db-view", "db-def");
        }
    }
    else {
        createElement(_refreshScreen);
    }
}

void createAttribute(bool _refreshScreen = false) {
    ifstream dbFile(filename + ".json");
    json jsonData;
    dbFile >> jsonData;
    dbFile.close();

    int targetId = 1;
    auto& elementsArray = jsonData["database"]["elements"];
    auto it = find_if(elementsArray.begin(), elementsArray.end(), [targetId](const auto& element) {
        return element["id"] == targetId;
    });

    if (it != elementsArray.end()) {
        (*it)[newAttribute] = newAttributeValue;

        ofstream output(filename + ".json");
        output << setw(4) << jsonData;  // pretty print with indentation
        output.close();
    }
    else {
        //throw an error
        cout << "Eroare: elementul nu a fost gasit.";
    }

    refreshScreen("elem-view", "elem-def");
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
        else if (key == 50) { //down arrow
            cursorPosElem++;
            refreshScreen("elem-view", "elem-def");
        }
        else if (key == 56) { //up arrow
            cursorPosElem--;
            refreshScreen("elem-view", "elem-def");
        }
        else if (key == 97) { //a key
            refreshScreen("elem-view", "elem-add-atr");
        }
        else {
            refreshScreen("elem-view", "elem-def");
        }
    }
    else if (situation == "elem-add-atr") {
        newAttribute = cinLine();
        refreshScreen("elem-view", "elem-add-val");
    }
    else if (situation == "elem-add-val") {
        newAttributeValue = cinLine();
        createAttribute(true);
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
    else if (situation == "elem-add-atr") { //default controls for elem-view
        cout << "\n|--------------------------| Introduceti numele campului de creat |--------------------------|\n";
        cout << "> ";
    }
    else if (situation == "elem-add-val") { //default controls for elem-view
        cout << "\n|--------------------------| Introduceti valoarea campului de creat |--------------------------|\n";
        cout << "> ";
    }
}

void openElement(string elementName) {
    system("cls");

    ifstream inputElementFile(filename + ".json");
    json data;
    inputElementFile >> data;

    if (inputElementFile.is_open()) {
        cout << "| << Esc |---------------------| Element deschis: " << selectedElementName << " |-----------------------------|" << endl;
        
        // Check if the "database" key exists in the JSON structure
        if (data.contains("database")) {
            // Access the "elements" array within the "database" object
            auto elementsArray = data["database"]["elements"];

            int targetId = cursorPos; 

            // Find the element with the desired "id"
            auto targetElement = find_if(elementsArray.begin(), elementsArray.end(),
                [targetId](const auto& element) {
                    return element["id"] == targetId;
                });

            if (targetElement != elementsArray.end()) {
                // Print attributes of the found element
                int index = 1;
                for (auto it = targetElement->begin(); it != targetElement->end(); ++it) {
                    if (it.key() != "id") {
                        if (cursorPosElem != index) {
                            cout << "| " << it.key() << ": " << it.value() << endl;
                        }
                        else {
                            cout << "> " << it.key() << ": " << it.value() << endl;
                        }
                        index++;
                    }
                    
                }
            }
            else {
                cout << "Eroare: Elementul cu id-ul " << targetId << " nu a fost gasit." << endl;
            }
        }
        else {
            cout << "Eroare: Structura bazei de date este invalida. Cheia 'elements' lipseste." << endl;
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

void viewDatabase(string inputFilename) {
    system("cls");

    char key = NULL;

    if (filename == " ") {
        cout << "Introduceti numele bazei de date pe care doriti sa o deschideti: ";
        cin >> filename;
        inputFilename = filename;
    }
    system("cls");

    ifstream inputFile(inputFilename + ".json");
    json data;
    inputFile >> data;

    if (inputFile.is_open()) {
        int elementNumber = 0;
        cout << "| << Esc |----------------------| Baza de date deschisa: " << filename << " |------------------------------|" << endl;

        // Check if the "database" key exists in the JSON structure
        if (data.contains("database")) {
            // Access the "elements" array within the "database" object
            auto elementsArray = data["database"]["elements"];

            for (const auto& element : elementsArray) {
                // Access the "nume" key within each element
                string name = element["nume"];
                if (cursorPos != element["id"]) {
                    cout << "[" << element["id"] << "] " << name << endl;
                }
                else {
                    cout << ">" << element["id"] << "<  " << name << endl;
                    selectedElementName = element["nume"];
                }
            }
        }
        else {
            cout << "Eroare: Structura bazei de date este invalida. Cheia 'database' lipseste." << endl;
        }
    }
    else {
        cout << "Eroare: Nu s-a putut deschide baza de date." << endl;
    }
}

int main() //main function of the program
{
    viewDatabase(" "); //open the database and display it, " " is used to tell the function to ask the user for a database
    controlsView("db-def"); //display the controls view on the screen, "db-def" is used to tell the function to display the default controls screen
    listenControls("db-def");
}

void refreshScreen(string screen, string controlsViewSituation) {
    if (screen == "db-view") {
        system("cls");
        viewDatabase(filename);
        controlsView(controlsViewSituation);
        listenControls(controlsViewSituation);
    }
    else if (screen == "elem-view") {
        system("cls");
        openElementByNum(selectedElement);
        controlsView(controlsViewSituation);
        listenControls(controlsViewSituation);
    }
}