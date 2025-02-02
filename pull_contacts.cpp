#pragma once
#include "pull_contacts.h"

void Contacts::AddContact(const Contact& newContact) {
    contacts.push_back(newContact);
}

void Contacts::DeleteContact(const size_t index) {
    if (contacts.empty()) {
        throw std::invalid_argument("Contacts list is empty!");
    }
    if (index >= contacts.size()) {
        throw std::invalid_argument("Incorrect index!");
    }
    contacts.erase(contacts.begin() + index);
}

void Contacts::SortContacts(const std::string& sortBy) {
    std::sort(contacts.begin(), contacts.end(), [&sortBy](const Contact& a, const Contact& b) {
        if (sortBy == "name") return a.GetName() < b.GetName();
        if (sortBy == "surname") return a.GetSurname() < b.GetSurname();
        if (sortBy == "patronymic") return a.GetPatronymic() < b.GetPatronymic();
        if (sortBy == "address") return a.GetAddress() < b.GetAddress();
        if (sortBy == "date") return a.GetDate() < b.GetDate();
        if (sortBy == "email") return a.GetEmail() < b.GetEmail();
        if (sortBy == "number") return a.GetNumber() < b.GetNumber();
        throw std::invalid_argument("Incorrect sort field!");
    });
}

Contacts Contacts::FindContacts(const std::vector<std::string>& searchQueries) {
    Contacts foundContacts;

    for (const auto& contact : contacts) {
        for (const auto& query : searchQueries) {
            if (contact.GetName().find(query) != std::string::npos ||
                contact.GetSurname().find(query) != std::string::npos ||
                contact.GetPatronymic().find(query) != std::string::npos ||
                contact.GetAddress().find(query) != std::string::npos ||
                contact.GetDate().find(query) != std::string::npos ||
                contact.GetEmail().find(query) != std::string::npos ||
                contact.GetNumber().find(query) != std::string::npos) {
                
                foundContacts.AddContact(contact);
                break; 
            }
        }
    }

    return foundContacts;
}


void Contacts::PrintContacts() const {
    if (contacts.empty()) {
        std::cout << "Contact list is empty!\n";
        return;
    }

    for (size_t i = 0; i < contacts.size(); ++i) {
        const auto& contact = contacts[i];
        std::cout << "Contact #" << i + 1 << ":\n";
        std::cout << "Name: " << contact.GetName() << "\n";
        std::cout << "Surname: " << contact.GetSurname() << "\n";
        std::cout << "Patronymic: " << contact.GetPatronymic() << "\n";
        std::cout << "Address: " << contact.GetAddress() << "\n";
        std::cout << "Date: " << contact.GetDate() << "\n";
        std::cout << "Email: " << contact.GetEmail() << "\n";
        std::cout << "Number: " << contact.GetNumber() << "\n";
    }
}

Contact Contacts::GetContact(const size_t index) const {
    if (index >= contacts.size()) {
        throw std::out_of_range("Invalid index: contact does not exist.");
    }
    return contacts[index];
}

void Contacts::UpdateContact(size_t index, const Contact& updatedContact) {
    if (index >= contacts.size()) {
        throw std::out_of_range("Invalid index: contact does not exist.");
    }
    contacts[index] = updatedContact;
}

void Contacts::LoadFromFile(const std::string &filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("Couldn't open file\n");
    }

    contacts.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string name, surname, patronymic, address, date, email, number;

        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        std::getline(ss, patronymic, ',');
        std::getline(ss, address, ',');
        std::getline(ss, date, ',');
        std::getline(ss, email, ',');
        std::getline(ss, number, ',');

        try {
            Contact contact;
            contact.SetName(name);
            contact.SetSurname(surname);
            contact.SetPatronymic(patronymic);
            contact.SetAddress(address);
            contact.SetDate(date);
            contact.SetEmail(email);
            contact.SetNumber(number);

            contacts.push_back(contact);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Can't upload contact: " << e.what() << "\n";
        }
    }
}

void Contacts::SaveToFile(const std::string &filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Couldn't write in file");
    }

    for (const auto &contact : contacts) {
        outFile << contact.GetName() << ","
                << contact.GetSurname() << ","
                << contact.GetPatronymic() << ","
                << contact.GetAddress() << ","
                << contact.GetDate() << ","
                << contact.GetEmail() << ","
                << contact.GetNumber() << "\n";
    }
}
