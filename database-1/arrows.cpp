/*#include <iostream>
#include <conio.h>

void displayMenu(int selectedOption) {
    system("cls"); // Clears the console screen (Windows specific)

    std::cout << "Menu Options:\n";
    for (int i = 1; i <= 5; ++i) {
        if (i == selectedOption) {
            std::cout << " -> Option " << i << " <-\n";
        }
        else {
            std::cout << "    Option " << i << "\n";
        }
    }
}

int main() {
    int selectedOption = 1;
    char key;

    do {
        displayMenu(selectedOption);

        key = _getch(); // Using _getch() from conio.h to get a key without waiting for Enter

        switch (key) {
        case 72: // Up arrow key
            selectedOption = (selectedOption > 1) ? selectedOption - 1 : 5;
            break;
        case 80: // Down arrow key
            selectedOption = (selectedOption < 5) ? selectedOption + 1 : 1;
            break;
        default:
            // Print the pressed key
            std::cout << "You pressed: " << key << "\n";
            break;
        }
    } while (key != 13); // 13 is the ASCII code for Enter

    // Perform action based on the selected option
    std::cout << "You selected Option " << selectedOption << ".\n";

    return 0;
}*/
