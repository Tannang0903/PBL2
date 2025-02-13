#include "TransactionManager.h"
#include "../CardManager/CardManager.h"
#include "../ClientManager/ClientManager.h"
#include "../../WithdrawTransaction/WithdrawTransaction.h"
#include "../../TransferTransaction/TransferTransaction.h"
#include "../../DepositTransaction/DepositTransaction.h"
#include "../../Repo/Repo.h"
#include "../../Helper/Helper.h"
#include <fstream>
#include <iomanip>
TransactionManager::TransactionManager(){
    ifstream in;
    in.open("../Data/Transaction.txt");
    int n;
    string transactionType;
    in >> n;
    for (int i = 0; i < n; i++){
        in >> transactionType;
        if (transactionType == "Withdraw"){
            Withdraw temp;
            in >> temp;
            Transaction *ptr = new Withdraw(temp);
            this -> list.addTail(ptr);
        }
        else if (transactionType == "Deposit"){
            Deposit temp;
            in >> temp;
            Transaction *ptr = new Deposit(temp);
            this -> list.addTail(ptr);
        }
        else if (transactionType == "Transfer"){
            Transfer temp;
            in >> temp;
            Transaction *ptr = new Transfer(temp);
            this -> list.addTail(ptr);
        }
    }
    in.close();
}

TransactionManager::~TransactionManager(){
    ofstream out;
    out.open("../Data/Transaction.txt");
    out << this -> list.getLength() << endl;
    Node<Transaction*> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr -> getData() -> getType() == "Withdraw"){
            Withdraw temp = *((Withdraw*)(ptr -> getData()));
            out << temp;
        }
        else if (ptr -> getData() -> getType() == "Deposit"){
            Deposit temp = *((Deposit*)(ptr -> getData()));
            out << temp;
        }
        else if (ptr -> getData() -> getType() == "Transfer"){
            Transfer temp = *((Transfer*)(ptr -> getData()));
            out << temp;
        }
        ptr = ptr -> getNext();
    }
    out.close();
}

string TransactionManager::generateID(){
    return (to_string(30000000 + this -> list.getLength()));
}

void TransactionManager::show(){
    Node<Transaction*> *ptr = this -> list.getHead();
    cout << "                                                                     DANH SACH GIAO DICH" << endl;
    transactionPanel();
    while (ptr != nullptr){
        ptr -> getData() -> show();
        ptr = ptr -> getNext();
    }
}

void TransactionManager::showByID(const string &ID){
    Node<Transaction*> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr -> getData() -> getID() == ID) ptr -> getData() -> show();
        ptr = ptr -> getNext();
    }
}

Transaction* TransactionManager::findByID(const string &ID){
    Node<Transaction*> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr -> getData() -> getID() == ID) return ptr -> getData();
        ptr = ptr -> getNext();
    }
    return nullptr;
}

int TransactionManager::indexOf(Transaction *T){
    // Node<Transaction*> *ptr = this -> list.getHead();
    // int index = 0;
    // while (ptr != nullptr){
    //     if (ptr -> getData() == Withdraw(*T) && T -> getType() == "Withdraw") return index;
    //     if (ptr -> getData() == Deposit(*T) && T -> getType() == "Deposit") return index;
    //     if (ptr -> getData() == Transfer(*T) && T -> getType() == "Transfer") return index;
    //     index++;
    //     ptr = ptr-> getNext();
    // }
    return -1;
}

int TransactionManager::indexOf(const string &ID){
    return -1;
}

bool TransactionManager::add(Transaction *T){
    return this -> list.addTail(T);
}

bool TransactionManager::remove(Transaction *T){
    return false;
}

bool TransactionManager::removeByID(const string &ID){
    int index = this -> indexOf(ID);
    if (index == -1) return false;
    else return this -> list.removeAt(index);
}
//
void TransactionManager::listByDate(const Date &D){
    Node<Transaction*> *ptr = this -> list.getHead();
    cout << "                                                                     DANH SACH GIAO DICH TRONG " << D << endl;
    transactionPanel();
    while (ptr != nullptr){
        if (Date::compareDate(ptr ->getData()->getDate(), D)) {
            ptr -> getData()->show();
        }
        ptr = ptr -> getNext();
    }
}

bool TransactionManager::makeWithdraw(const string &CardID, const long &cash, const string &PIN){
    if (Repository<Card>::getByID(CardID, "Card.txt").isNull()){
        cout << "The khong ton tai" << endl;
        return false;
    }
    CardManager CM;
    Transaction *ptr = new Withdraw(this -> generateID(), CM.findByID(CardID), cash);
    bool result = ptr -> makeTransaction(PIN);
    // Repository<Card>::findAndUpdate(ptr -> getSrcAccount(), "Card.txt");
    if (result){
        CM.updateByID(ptr -> getSrcAccount(), CardID);
    }
    this -> add(ptr);
    return result;
}

bool TransactionManager::makeTransfer(const string &SrcAccount, const string &DestAccount, const long &cash, const string &PIN){
    if (Repository<Card>::getByID(SrcAccount, "Card.txt").isNull() || Repository<Card>::getByID(DestAccount, "Card.txt").isNull()){
        cout << "The khong ton tai" << endl;
        return false;
    }
    CardManager CM;
    // Transfer T = Transfer(this -> generateID(), Repository<Card>::getByID(SrcAccount, "Card.txt"), Repository<Card>::getByID(DestAccount, "Card.txt"), cash);
    Transfer T = Transfer(this -> generateID(), CM.findByID(SrcAccount), CM.findByID(DestAccount), cash);
    bool result = T.makeTransaction(PIN);
    Transaction *ptr = new Transfer(T);
    if (result){
        CM.updateByID(T.getSrcAccount(), SrcAccount);
        CM.updateByID(T.getDestAccount(), DestAccount);
    }
    this -> add(ptr);
    return result;
}

bool TransactionManager::makeDeposit(const string &CardID, const long &cash, const string &PIN){
    if (Repository<Card>::getByID(CardID, "Card.txt").isNull()){
        cout << "The khong ton tai" << endl;
        return false;
    }
    CardManager CM;
    // Transaction *ptr = new Deposit(this -> generateID(), Repository<Card>::getByID(CardID, "Card.txt"), cash);
    Transaction *ptr = new Deposit(this -> generateID(), CM.findByID(CardID), cash);
    bool result = ptr -> makeTransaction(PIN);
    if (result){
        CM.updateByID(ptr -> getSrcAccount(), CardID);
    }
    this -> add(ptr);
    return result;
}

void TransactionManager::showWithdraw(){
    Node<Transaction*> *ptr = this->list.getHead();
    cout << "                                                                     DANH SACH GIAO DICH RUT" << endl;
    transactionPanel();
    while (ptr != nullptr){
        if (ptr -> getData() -> getType() == "Withdraw") ptr -> getData() -> show();
        ptr = ptr -> getNext();
    }
}

void TransactionManager::showTransfer(){
    Node<Transaction*> *ptr = this -> list.getHead();
    cout << "                                                                  DANH SACH GIAO DICH CHUYEN KHOAN" << endl;
    transactionPanel();
    while (ptr != nullptr){
        if (ptr -> getData() -> getType() == "Transfer") ptr -> getData() -> show();
        ptr = ptr -> getNext();
    }
}

void TransactionManager::showDeposit(){
    Node<Transaction*> *ptr = this -> list.getHead();
    cout << "                                                                       DANH SACH GIAO DICH NAP" << endl;
    transactionPanel();
    while (ptr != nullptr){
        if (ptr -> getData() -> getType() == "Deposit") ptr -> getData() -> show();
        ptr = ptr -> getNext();
    }
}

void TransactionManager::showAllClientTransaction(const string &ClientID){
    int count = 0;
    ClientManager CM;
    Client temp = CM.findByID(ClientID);
    if (temp.isNull()){
        cout << "=> Khach hang khong ton tai" << endl;
        return;
    }
    Node<Transaction*> *ptr = this -> list.getHead();
    cout << setw(70) << left << " " << "THONG TIN KHACH HANG" << endl;
    clientPanel();
    temp.show();
    cout << "                                                                                DANH SACH GIAO DICH CUA KHACH HANG \"" << ClientID << "\"" << endl << endl;
    transactionPanel();
    while (ptr != nullptr){
        if (ptr -> getData() -> getSrcAccount().getHolder().getID() == ClientID) {
            ptr -> getData() -> show();
            count++;
        }
        ptr = ptr -> getNext();
    }
    cout << "=> Co tong cong: " << count << " giao dich" << endl;
}

void TransactionManager::showAllCardTransaction(const string &CardID){
    int count = 0;
    CardManager CM;
    Node<Transaction*> *ptr = this -> list.getHead();
    CM.showByID(CardID);
    cout << endl;
    cout << "                                                                   DANH SACH GIAO DICH CUA THE \"" << CardID << "\"" << endl << endl;
    transactionPanel();
    while (ptr != nullptr){
        if(ptr -> getData()-> getSrcAccount().getID() == CardID)  {
            ptr -> getData() -> show();
            count++;
        }
        ptr = ptr -> getNext();
    }
    cout << "=> Co tong cong: " << count << " giao dich" << endl;
}

void TransactionManager::exportToCSV(const string &fileName){
    ofstream out;
    out.open("..//Output//" + fileName + ".csv");
    Node<Transaction*> *ptr = this -> list.getHead();
    out << "ID;Type;Source Account;DestAccount;Amount;Fee;Status;Date" << endl;
    while (ptr != nullptr){
        ptr -> getData() -> exportToCSV(out);
        out << endl;
        ptr = ptr -> getNext();
    }
    out.close();
}