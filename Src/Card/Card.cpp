#include "Card.h"
//Constructor
Card::Card(){

}
Card::Card(const string &IdAccount, const string &IdHolder, const string &pin, const int &balance)
{
    this->IdAccount = IdAccount;
    this->IdHolder = IdHolder;
    this->pin = pin;
    this->balance = balance;
    this->createdAt = Date::getCurrentDate();
    this->updatedAt = Date();
}
//Getter
string Card::getIdAccount()
{
    return this->IdAccount;
}
string Card::getIdHolder()
{
    return this->IdHolder;
}
string Card::getpin()
{
    return this->pin;
}
int Card::getBalance()
{
    return this->balance;
}
Date Card::getCreatedAt()
{
    return this->createdAt;
}
Date Card::getUpdatedAt()
{
    return this->updatedAt;
}
//Setter
void Card::setIdAccount(const string &IdAccount){
    this->IdAccount = IdAccount;
}
void Card::setIdHolder(const string &IdHolder){
    this->IdHolder = IdHolder;
}
void Card::setpin(const string &pin){
    this->pin = pin;
}
void Card::setBalance(const int &balance){
    this->balance = balance;
}
void Card::setCreatedAt(const Date &createdAt){
    this -> createdAt = createdAt;
}
void Card::setUpdatedAt(const Date &updatedAt){
    this -> updatedAt = updatedAt;
}

const Card& Card::operator=(const Card &card){
    this->IdAccount = card.IdAccount;
    this->IdHolder = card.IdHolder;
    this->pin = card.pin;
    this->balance = card.balance;
    this->createdAt = card.createdAt;
    this->updatedAt = card.updatedAt;
    return (*this);
}
//input
istream& operator>>(istream &in, Card &card){
    cout << "Type Card's IdAccount: ";
    in >> card.IdAccount;
    while (!card.isValidIdAccount()){
        cout << "Invalid ID, type again: ";
        in >> card.IdAccount;
    }
    cout << "Type Card's IdHolder: ";
    in >> card.IdHolder;
    while (!card.isValidIdHolder()){
        cout << "Invalid IdHolder, type again: ";
        in >> card.IdHolder;
    }
    cout << "Type Card's Pin: ";
    in >> card.pin;
    while (!card.isValidPin()){
        cout << "Invalid pin, type again: ";
        in >> card.pin;
    }
    cout << "Type Card's Balance: ";
    in >> card.balance;
    card.createdAt = Date::getCurrentDate();
    card.updatedAt = Date();
    return in;
}
//output
ostream& operator<<(ostream &out, const Card &card){
    out << "IdAccount: " << card.IdAccount << endl;
    out << "IdHolder: " << card.IdHolder << endl;
    out << "Pin: " << card.pin << endl;
    out << "Balance: " << Card(card).getBalance() << endl;
    out << "Created At: " << card.createdAt;
    if (Card(card).updatedAt.isValidDate())
        out << "Updated At: " << card.updatedAt;
    return out;
}
bool Card::isValidIdAccount(){
    if (this->IdAccount.size() < 16) return false; // IdAccount có độ dài là 16 số
    for (int i = 0; i < this->IdAccount.size();i++){
        if (this->IdAccount[i] < '0' || this->IdAccount[i] > '9') return false;
    }
    return true;
}
bool Card::isValidIdHolder(){
    if (this->IdHolder.size() < 10) return false; // IdHolder có độ dài là 10 số
    for (int i = 0; i < this->IdHolder.size();i++){
        if (this->IdHolder[i] < '0' || this->IdHolder[i] > '9') return false;
    }
    return true;
}
bool Card::isValidPin(){
    if (this->pin.size() < 6) return false; // IdHolder có độ dài là 6 số
    for (int i = 0; i < this->pin.size();i++){
        if (this->pin[i] < '0' || this->pin[i] > '9') return false;
    }
    return true;
}
void Card::show(){
    cout << "IdAccount: " << this->IdAccount << endl;
    cout << "IdHolder: " << this->IdHolder << endl;
    cout << "Pin: " << this->pin << endl;
    cout << "Created At: " << this->createdAt << endl;
    cout << "Update At: " << this->updatedAt << endl;
}
void Card::showBalance(){
    cout << "Balance: " << this->balance << endl;
}
