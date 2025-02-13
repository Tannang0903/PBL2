#include "CardManager.h"
#include "../../Helper/Helper.h"
#include <iomanip>
int CardManager::totalCardCreated = 0;
CardManager::CardManager(){
    ifstream in;
    in.open("../Data/Card.txt", ios::in);
    int numberOfCard;
    in >> numberOfCard;
    in >> CardManager::totalCardCreated;
    for (int i = 0; i < numberOfCard;i++){
        Card temp;
        in >> temp;
        this -> list.addTail(temp);
    }
    in.close();
}

CardManager::~CardManager(){
    ofstream out;
    out.open("../Data/Card.txt", ios::out);
    out << this -> list.getLength() << endl;
    out << CardManager::totalCardCreated << endl;
    for (int i = 0; i < this -> list.getLength();i++){
        out << this -> list[i];
    }
    out.close();
}

void CardManager::show(){
    Node<Card> *ptr = this -> list.getHead();
    cardPanel();
    while (ptr != nullptr){
        ptr -> getData().show();
        cout << endl;
        ptr = ptr -> getNext();
    }
}

void CardManager::showByID(const string &ID){
    Node<Card> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr -> getData().getID() == ID) {
            cout << setw(80) << right << "THONG TIN CHU THE" << endl;
            this -> showInf(ptr -> getData().getHolder().getID());
            cout << endl << setw(78) << right << "THONG TIN THE" << endl;
            cardPanel();
            ptr -> getData().show();
            return;
        }
        ptr = ptr -> getNext();
    }
}

string CardManager::generateID(){
    return to_string(20000000 + CardManager::totalCardCreated);
}

Card CardManager::findByID(const string &ID){
    Node<Card> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr-> getData().getID() == ID) return ptr -> getData();
        ptr = ptr -> getNext();
    }
    return Card();
}

int CardManager::indexOf(const Card C){
    return this -> list.indexOf(C);
}

int CardManager::indexOf(const string &ID){
    return this -> list.indexOf(compareID, ID);
}

bool CardManager::add(Card C){
    if ((this -> countClientCard(C.getHolder().getID())) >= 3) {
        cout << "Moi khach hang chi duoc phep so huu toi da 3 the ngan hang.";
        return false;
    }
    C.setID(this -> generateID()); 
    CardManager::totalCardCreated++;
    return this -> list.addTail(C);
}

bool CardManager::remove(const Card C){
    return this -> list.remove(C);
}

bool CardManager::removeByID(const string &ID){
    return this -> list.remove(compareID, ID);
}

void CardManager::listByDate(const Date &D){
    Node<Card> *ptr = this -> list.getHead();
    cardPanel();
    while (ptr != nullptr){
        if (Date::compareDate(ptr -> getData().getCreatedAt(), D)) {
            ptr -> getData().show();
            cout << endl;
        }
        ptr = ptr -> getNext();
    }
}

bool CardManager::updateByID(Card C, const string &ID){
    int index = this -> indexOf(ID);
    if (index == -1) return false;
    this -> list[index].update(C);
    return true;
}

void CardManager::removeAll(const string &ClientID){
    Node<Card> *ptr = this -> list.getHead();
    int index = 0;
    this -> listAllClientCard(ClientID);
    while (ptr != nullptr && this -> list.getLength() > 0){
        if (ptr -> getData().getHolder().getID() == ClientID) {
            ptr = ptr -> getNext();
            this -> list.removeAt(index);
            index--;
        }else{
            ptr = ptr -> getNext();
        }
        index++;
    }
}

void CardManager::showInf(const string &ID){
    Node<Card> *ptr = this -> list.getHead();
    clientPanel();
    while (ptr != nullptr){
        if (ptr -> getData().getHolder().getID() == ID){
            ptr -> getData().getHolder().show();
            return;
        }
        ptr = ptr -> getNext();
    }
    cout << endl;
}

bool CardManager::changePin(const string &ID, const string &currentPin, const string &newPin){
    Node<Card> *ptr = this -> list.getHead();
    while (ptr != nullptr){
        if (ptr -> getData().getID() == ID){
            try{
                ptr -> getRefData().updatePin(currentPin, newPin);
            }catch(string err){
                cout << err << endl;
                return false;
            }catch(...){
                return false;
            }
            return true;
        }
        ptr = ptr -> getNext();
    }
    return true;
}

int CardManager::countClientCard(const string &ClientID){
    Node<Card> *ptr = this -> list.getHead();
    int count = 0;
    while (ptr != nullptr){
        if (ptr -> getData().getHolder().getID() == ClientID) count++;
        ptr = ptr -> getNext();
    }
    return count;
}

void CardManager::listAllClientCard(const string &ClientID){
    Node<Card> *ptr = this -> list.getHead();
    cardPanel();
    while (ptr != nullptr){
        if (ptr -> getData().getHolder().getID() == ClientID) {
            ptr -> getData().show();
            cout << endl;
        }
        ptr = ptr -> getNext();
    }
}

void CardManager::exportToCSV(const string &fileName){
    ofstream out;
    out.open("..//Output//" + fileName + ".csv");
    Node<Card> *ptr = this -> list.getHead();
    out << "ID;Holder ID;PIN;Balance;Created At;Updated At;PIN updatedAt" << endl;
    while (ptr != nullptr){
        ptr -> getData().exportToCSV(out);
        out << endl;
        ptr = ptr -> getNext();
    }
    out.close();
}