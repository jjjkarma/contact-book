#include <iostream>
#include <sstream> 
#include "pull_contacts.cpp"

void ShowMenu() {
    std::cout << "\nContact Book Menu:\n";
    std::cout << "1. Add a contact\n";
    std::cout << "2. Delete a contact\n";
    std::cout << "3. Display all contacts\n";
    std::cout << "4. Search for a contact\n";
    std::cout << "5. Sort contacts\n";
    std::cout << "6. Edit contact\n";    
    std::cout << "7. Exit\n";
    std::cout << "Select an option: ";
}

int main() {
    Contacts contacts;
    int choice;

    do {
        ShowMenu();
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1: { // Add a contact
                std::string name, surname, patronymic, address, date, email, number;

                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                std::cout << "Enter surname: ";
                std::getline(std::cin, surname);
                std::cout << "Enter patronymic: ";
                std::getline(std::cin, patronymic);
                std::cout << "Enter address: ";
                std::getline(std::cin, address);
                std::cout << "Enter birth date (dd.mm.yyyy): ";
                std::getline(std::cin, date);
                std::cout << "Enter email: ";
                std::getline(std::cin, email);
                std::cout << "Enter phone number: ";
                std::getline(std::cin, number);

                try {
                    Contact newContact(name, surname, patronymic, address, date, email, number);
                    contacts.AddContact(newContact);
                    std::cout << "Contact added successfully.\n";
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    break;
                }
                break;
            }

            case 2: { // Delete a contact
                size_t index;
                std::cout << "Enter the contact index to delete: ";
                std::cin >> index;

                try {
                    contacts.DeleteContact(index - 1);
                    std::cout << "Contact deleted successfully.\n";
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    break;
                }
                break;
            }

            case 3: { // Display all contacts
                contacts.PrintContacts();
                break;
            }

            case 4: { // Search contacts
                std::cout << "Enter search terms separated by spaces: ";
                std::string input;
                std::cin.ignore(); // Clear previous newline
                std::getline(std::cin, input);

                std::vector<std::string> searchQueries;
                std::istringstream iss(input);
                std::string query;
                while (iss >> query) {
                    searchQueries.push_back(query);
                }

                Contacts results = contacts.FindContacts(searchQueries);
                results.PrintContacts();
                break;
            }

            case 5: { // Sort contacts
                std::cout << "Enter the field to sort by (name, surname, date, email, number): ";
                std::string field;
                std::cin >> field;
                contacts.SortContacts(field);
                std::cout << "Contacts sorted by " << field << ".\n";
                break;
            }

            case 6: { // Edit a contact
                size_t index;
                std::string field;
                std::string newValue;
                
                std::cout << "Enter the contact index to edit: ";
                std::cin >> index;
                std::cin.ignore();
                
                try {
                    Contact contact = contacts.GetContact(index - 1);

                    std::cout << "Enter the contact field to edit (name, surname, patronymic, address, date, email, number): ";
                    std::getline(std::cin, field);
                    
                    std::cout << "Enter new value to set: ";
                    std::getline(std::cin, newValue);
                    
                    if (field == "name") {
                        contact.SetName(newValue);
                    } else if (field == "surname") {
                        contact.SetSurname(newValue);
                    } else if (field == "patronymic") {
                        contact.SetPatronymic(newValue);
                    } else if (field == "address") {
                        contact.SetAddress(newValue);
                    } else if (field == "date") {
                        contact.SetDate(newValue);
                    } else if (field == "email") {
                        contact.SetEmail(newValue);
                    } else if (field == "number") {
                        contact.SetNumber(newValue);
                    } else {
                        throw std::invalid_argument("Invalid field name!");
                    }

                    contacts.UpdateContact(index - 1, contact);

                    std::cout << "Contact updated successfully.\n";
                    break;

                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    break;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    break;
                }

            }

            case 7: { // Exit
                std::cout << "Exiting the program.\n";
                break;
                }

                default: {
                std::cout << "Invalid input. Please try again.\n";
                }
                } } while (choice != 6);

                return 0;
            };