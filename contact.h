#include <string>
#include <regex>
#include <ctime>
#include <stdexcept>
#pragma once

class Contact {
private:
    std::string name = "noname";
    std::string surname = "nosurname";
    std::string patronymic = "nopatronymic";
    std::string address = "noaddress";
    std::string date = "nodate";
    std::string email = "noemail";
    std::string number = "nonumber";
public:
    Contact() = default; //constructor without params
    Contact(    //constructor with params
        std::string& Name,
        std::string& Surname,
        std::string& Patronymic,
        std::string& Address,
        std::string& Date,
        std::string& Email,
        std::string& Number
        );
    ~Contact(); //destructor
    Contact(const Contact& other); //copy constructor

    std::string GetName() const; //name actions
    void SetName(std::string& newName);
    bool IsNameCorrect(std::string& Name) const;

    std::string GetSurname() const; //surname actions
    void SetSurname(std::string& newSurname);
    bool IsSurnameCorrect(std::string& Surname);

    std::string GetPatronymic() const; //patronymic actions
    void SetPatronymic(std::string& newPatronymic);
    bool IsPatronymicCorrect(std::string& Patronymic);

    std::string GetAddress() const; //address actions
    void SetAddress(std::string& newAddress);

    std::string GetDate() const; //date actions
    void SetDate(std::string& newDate);
    bool IsDateCorrect(std::string& Date);

    std::string GetEmail() const; //email actions
    void SetEmail(std::string& newEmail);
    bool IsEmailCorrect(std::string& Email);

    std::string GetNumber() const; //number actions
    void SetNumber(std::string& newNumber);
    bool IsNumberCorrect(std::string& Number);

};
