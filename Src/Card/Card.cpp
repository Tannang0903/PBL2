#include "Card.h"
#include "../Repo/Repo.h"
#include "../Helper/Helper.h"
#include <string>
#include <iomanip>
//Constructor
Card::Card()
{
    this -> ID = "";
    this -> holder = Client();
    this -> pin = "";
    this -> balance = 0;
    this -> createdAt = "";
    this -> updatedAt = "";
    this -> pinUpdatedAt = "";
}

Card::Card(const Card &card)
{
    this -> ID = card.ID;
    this -> holder = card.holder;
    this -> pin = card.pin;
    this -> balance = card.balance;
    this -> createdAt = card.createdAt;
    this -> updatedAt = card.updatedAt;
    this -> pinUpdatedAt = card.pinUpdatedAt;
}

Card::Card(const string &ID, const Client &holder, const string &pin, const long &balance)
{
    this -> ID = (Card::isValidID(ID)) ? ID : "";
    this -> holder = holder;
    this -> pin = (Card::isValidPin(pin)) ? pin : "";
    this -> balance = (balance > 0) ? balance : 0;
    this -> createdAt = Date::getCurrentDate();
    this -> updatedAt = Date();
    this -> pinUpdatedAt = Date();
}

Card::Card(const string &ID, const Client &holder, const string &pin, const long &balance, const Date &createdAt, const Date &updatedAt, const Date &pinUpdatedAt)
{
    this -> ID = (Card::isValidID(ID)) ? ID : "";
    this -> holder = holder;
    this -> pin = (Card::isValidPin(pin)) ? pin : "";
    this -> balance = (balance > 0) ? balance : 0;
    this -> createdAt = (Date(createdAt).isValidDate()) ? createdAt : Date();
    this -> updatedAt = (Date(updatedAt).isValidDate()) ? updatedAt : Date();
    this -> pinUpdatedAt = (Date(pinUpdatedAt).isValidDate()) ? pinUpdatedAt : Date();
}

// Destructor
Card::~Card(){

}

//Getter
string Card::getID()
{
    return this -> ID;
}

Client Card::getHolder()
{
    return this -> holder;
}

string Card::getPin()
{
    return this -> pin;
}

long Card::getBalance()
{
    return this -> balance;
}

Date Card::getCreatedAt()
{
    return this -> createdAt;
}

Date Card::getUpdatedAt()
{
    return this -> updatedAt;
}

Date Card::getPinUpdatedAt(){
    return this -> pinUpdatedAt;
}
//Setter
void Card::setID(const string &ID){
    this -> ID = (Card::isValidID(ID)) ? ID : ""; 
}

void Card::setHolder(const Client &holder){
    this -> holder = holder;
}

void Card::setPin(const string &pin){
    this -> pin = pin;
}

void Card::setBalance(const long &balance){
    this -> balance = (balance > 0) ? balance : this -> balance;
}

void Card::setCreatedAt(const Date &createdAt){
    this -> createdAt = createdAt;
}
void Card::setUpdatedAt(const Date &updatedAt){
    this -> updatedAt = updatedAt;
}

const Card& Card::operator=(const Card &card){
    this -> ID = card.ID;
    this -> holder = card.holder;
    this -> pin = card.pin;
    this -> balance = card.balance;
    this -> createdAt = card.createdAt;
    this -> updatedAt = card.updatedAt;
    this -> pinUpdatedAt = card.pinUpdatedAt;
    return (*this);
}

//check
bool Card::isValidID(const string &str){
    if (str.size() != 8) return false; // ID có độ dài là 12 số
    for (int i = 0; i < str.size();i++){
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool Card::isValidPin(const string &str){
    if (str.size() != 6) return false; // ID có độ dài là 6 số
    for (int i = 0; i < str.size();i++){
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}
bool Card::isNull(){
    return ((*this) == Card());
}

void Card::show(){
    cout << "| " << left << setw(18) << this -> ID;
    cout << "| " << left << setw(13) << this -> holder.getID();
    cout << "| " << left << setw(8) << this -> pin;
    cout << "| " << left << setw(18) << moneyFormat(this -> balance);
    cout << "| " << this -> createdAt << setw(9) << ' ' << "| ";
    cout << this -> updatedAt << setw(9) << ' ' << "| ";
    cout << this -> pinUpdatedAt << setw(8) << ' ' << "| ";
    cout << endl << setfill('-') << setw(155) << '-' << setfill(' ');
}

void Card::showBalance(){
    cout << "Balance: " << this -> balance << endl;
}

int Card::withdraw(const long &money){
    if (money > this -> balance || money < 0) return -1;
    this -> balance -= money;
    this -> updatedAt = Date::getCurrentDate();
    return money;
}

void Card::deposit(const long &money){
    if (money > 0) 
        this -> balance += money;
        this -> updatedAt = Date::getCurrentDate();
}

void Card::updatePin(const string &currentPin, const string &newPin){
    if (Date::DATEDIFF(this -> pinUpdatedAt, Date::getCurrentDate()) >= 2 && Date::getCurrentDate() > this -> pinUpdatedAt){
        if (Card::isValidPin(newPin)){
            if (this -> pin == currentPin){
                this -> pin = newPin;
                this -> pinUpdatedAt = Date::getCurrentDate();
                this -> updatedAt = Date::getCurrentDate();
            }else throw string("Inccorect PIN");
        }else throw string("Invalid PIN");
    }else throw string("You have to wait " + to_string(2 - Date::DATEDIFF(this -> pinUpdatedAt, Date::getCurrentDate())) + " day left to change your PIN account");
    // if (this -> pin == currentPin)
    //     if (Card::isValidPin(pin)){
    //         if (){
    //         }else throw string("You have to wait " + to_string(2 - Date::DATEDIFF(this -> pinUpdatedAt, Date::getCurrentDate())) + " day left to change your PIN account");
    //     }else throw string("Invalid PIN");
    // else throw string("Inccorect PIN");
}

void Card::update(const Card &C){
    this -> balance = C.balance;
    this -> updatedAt = Date::getCurrentDate();
}

bool Card::operator==(const Card &newCard){
    if(this->ID == newCard.ID && this->holder == newCard.holder && this->balance == newCard.balance && this->pin == newCard.pin && this->createdAt == newCard.createdAt && this->updatedAt == newCard.updatedAt){
        return true;
    }
    return false;
}

void Card::input(){
    string clientID;
    cout << "Type Holder's ID: ";
    cin >> clientID;
    this -> holder = Repository<Client>::getByID(clientID, "Client.txt");
    while (this -> holder.isNull()){
        cout << "Client doesn't exist, type again: ";
        cin >> clientID;
        this -> holder = Repository<Client>::getByID(clientID, "Client.txt");
    }
    cout << "Type Card's Pin: ";
    cin >> this -> pin;
    while (!Card::isValidPin(this -> pin)){
        cout << "Invalid pin, type again: ";
        cin >> this -> pin;
    }
    cout << "Type Card's Balance: ";
    cin >> this -> balance;
    this -> createdAt = Date::getCurrentDate();
    this -> updatedAt = Date();
    this -> pinUpdatedAt = Date();
}

ofstream& operator<<(ofstream &out, const Card &card){
    out << card.ID << endl;
    out << Client(card.holder).getID() << endl;
    out << card.pin << endl;
    out << card.balance << endl;
    out << Date(card.createdAt).toString() << endl;
    out << Date(card.updatedAt).toString() << endl;
    out << Date(card.pinUpdatedAt).toString() << endl;
    return out;
}

ifstream& operator>>(ifstream &in, Card &card){
    string data;
    in >> card.ID;
    in >> data;
    card.holder = Repository<Client>::getByID(data, "Client.txt");
    in >> card.pin;
    in >> card.balance;
    in.ignore();
    getline(in, data);
    card.createdAt = Date(data.c_str());
    getline(in, data);
    card.updatedAt = Date(data.c_str());
    getline(in, data);
    card.pinUpdatedAt = Date(data.c_str());
    return in;
}   

void Card::exportToCSV(ofstream &out){
    out << this -> ID << ";";
    out << this -> holder.getID() << ";";
    out << this -> pin << ";";
    out << this -> balance << ";";
    out << this -> createdAt.toString() << ";";
    out << this -> updatedAt.toString() << ";";
    out << this -> pinUpdatedAt.toString() << ";";
}