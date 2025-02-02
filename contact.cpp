#include "contact.h"

std::regex pattern("^[A-Za-zА-Яа-я][A-Za-zА-Яа-я0-9\\- ]*[A-Za-zА-Яа-я0-9]$");
std::regex emailPattern("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z]{2,}$");
std::regex spaces("(^\\s+)|(\\s+$)");
std::regex numberPattern("^(\\+7|8)(\\(\\d{3}\\)\\d{7}|\\d{10}|\\(\\d{3}\\)-\\d{3}-\\d{2}-\\d{2})$");
std::regex datePattern("^(\\d{2})\\.(\\d{2})\\.(\\d{4})$");

Contact::Contact( //constructor with params
    std::string& Name,
    std::string& Surname,
    std::string& Patronymic,
    std::string& Address,
    std::string& Date,
    std::string& Email,
    std::string& Number
    ) {
    if (IsNameCorrect(Name)) { name = Name; } else { throw std::invalid_argument("Incorrect name!"); };
    if (IsSurnameCorrect(Surname)) { surname = Surname; } else { throw std::invalid_argument("Incorrect surname!"); };
    if (IsPatronymicCorrect(Patronymic)) { patronymic = Patronymic; } else { throw std::invalid_argument("Incorrect patronymic!"); };
    if (IsDateCorrect(Date)) { date = Date; } else { throw std::invalid_argument("Incorrect date!"); };
    if (IsEmailCorrect(Email)) { email = Email; } else { throw std::invalid_argument("Incorrect email!"); };
    if (IsNumberCorrect(Number)) { number = Number; } else { throw std::invalid_argument("Incorrect number!"); };
    address = Address;
};

Contact::~Contact() {}; //destructor

Contact::Contact(const Contact& other): //copy constructor
    name(other.GetName()),
    surname(other.GetSurname()),
    patronymic(other.GetPatronymic()),
    address(other.GetAddress()),
    date(other.GetDate()),
    email(other.GetEmail()),
    number(other.GetNumber())
{};

std::string Contact::GetName() const { return name; };
void Contact::SetName(std::string& newName) { if (IsNameCorrect(newName)) { name = newName; } else { throw std::invalid_argument("Incorrect name!"); }; };
bool Contact::IsNameCorrect(std::string& Name) const {
    Name = std::regex_replace(Name, spaces, "");
    return std::regex_match(Name, pattern);
}

std::string Contact::GetSurname() const { return surname; };
void Contact::SetSurname(std::string& newSurname) { if (IsSurnameCorrect(newSurname)) { surname = newSurname; } else { throw std::invalid_argument("Incorrect surname!"); }; };
bool Contact::IsSurnameCorrect(std::string& Surname) {
    Surname = std::regex_replace(Surname, spaces, "");
    return std::regex_match(Surname, pattern);
}

std::string Contact::GetPatronymic() const { return patronymic; };
void Contact::SetPatronymic(std::string& newPatronymic) { if (IsPatronymicCorrect(newPatronymic)) { patronymic = newPatronymic; } else { throw std::invalid_argument("Incorrect patronymic!"); }; };
bool Contact::IsPatronymicCorrect(std::string& Patronymic) {
    Patronymic = std::regex_replace(Patronymic, spaces, "");
    return std::regex_match(Patronymic, pattern);
}

std::string Contact::GetAddress() const { return address; };
void Contact::SetAddress(std::string& newAddress) { address = newAddress; };

std::string Contact::GetDate() const { return date; };
void Contact::SetDate(std::string& newDate) { if (IsDateCorrect(newDate)) { date = newDate; } else { throw std::invalid_argument("Incorrect date!"); }; };
bool Contact::IsDateCorrect(std::string& Date) {
    std::smatch dateMatch;
    if (std::regex_match(Date, dateMatch, datePattern)) {
        int day = std::stoi(dateMatch[1].str());
        int month = std::stoi(dateMatch[2].str());
        int year = std::stoi(dateMatch[3].str());

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month >= 1 && month <= 12) {
            if ((year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) && month == 2) {
                daysInMonth[1]++;
            }
            if (day >= 1 && day <= daysInMonth[month - 1]) {
                time_t t = time(nullptr);
                tm* now = localtime(&t);

                int currentYear = now->tm_year + 1900;
                int currentMonth = now->tm_mon + 1;
                int currentDay = now->tm_mday;

                return ((year < currentYear) || (year == currentYear && month < currentMonth) || (year == currentYear && month == currentMonth && day < currentDay));
            }
        }
    }
    return false;
}

std::string Contact::GetEmail() const { return email; };
void Contact::SetEmail(std::string& newEmail) { if (IsEmailCorrect(newEmail)) { email = newEmail; } else { throw std::invalid_argument("Incorrect email!"); }; };
bool Contact::IsEmailCorrect(std::string& Email) {
    Email = std::regex_replace(Email, spaces, "");
    return std::regex_match(Email, emailPattern);
}

std::string Contact::GetNumber() const { return number; };
void Contact::SetNumber(std::string& newNumber) {     if (IsNumberCorrect(newNumber)) { number = newNumber; } else { throw std::invalid_argument("Incorrect number!"); }; };
bool Contact::IsNumberCorrect(std::string& Number) {
    Number = std::regex_replace(Number, spaces, "");
    return std::regex_match(Number, numberPattern);
}
