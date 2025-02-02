#include <string>
#include <iostream>
#include <fstream>
#include "contact.cpp"
#pragma once

class Contacts {
    private:
        std::vector<Contact> contacts;
    public:
        Contacts() = default;
        void AddContact(const Contact& newContact);
        Contact GetContact(const size_t index) const;
        void DeleteContact(const size_t index);
        void SortContacts(const std::string& sortBy);
        Contacts FindContacts(const std::vector<std::string>& searchQueries);
        void PrintContacts() const;
        void UpdateContact(size_t index, const Contact& updatedContact);
        void LoadFromFile(const std::string &filename);
        void SaveToFile(const std::string &filename) const;
};