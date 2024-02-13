#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <sys/stat.h>


using namespace std;

const int PRZESUNIECIE = 3;
bool zalogowany = false;
string zalogowany_uzytkownik;
string miejsce, dzisiejsza_data, nazwisko_nadawcy, adres_nadawcy, nazwa_przewoznika, nazwisko_odbiorcy, adres_odbiorcy, ilosc_sztuk, waga_brutto;

string zaszyfruj(string haslo) {
    string zaszyfrowane = haslo;
    for (size_t i = 0; i < haslo.length(); i++) {
        char c = haslo[i];
        if (isalpha(c)) {
            c = toupper(c);
            c = ((c - 'A') + PRZESUNIECIE) % 26 + 'A';
        }
        zaszyfrowane[i] = c;
    }
    return zaszyfrowane;
}

string odszyfruj(string zaszyfrowane) {
    string haslo = zaszyfrowane;
    for (size_t i = 0; i < zaszyfrowane.length(); i++) {
        char c = zaszyfrowane[i];
        if (isalpha(c)) {
            c = toupper(c);
            c = ((c - 'A') - PRZESUNIECIE + 26) % 26 + 'A';
        }
        haslo[i] = c;
    }
    return haslo;
}

bool czy_nazwa_dostepna(string login) {
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        string znak = ":";
        string zapisana_nazwa = line.substr(0, line.find(znak));
        if (zapisana_nazwa == login) {
            return false;
        }
    }
    return true;
}

bool zarejestruj() {
    string login, haslo, potwierdz_haslo;
    cout << "Login: ";
    cin >> login;
    if (!czy_nazwa_dostepna(login)) {
        cout << "Login już istnieje." << endl;
        return false;
    }
    cout << "Haslo: ";
    haslo = "";
    char c;
    while ((c = _getch()) != '\r') {
        if (isprint(c)) {
            cout << "*";
            haslo += c;
        }
    }
    cout << endl;
    cout << "Potwierdz haslo: ";
    potwierdz_haslo = "";
    while ((c = _getch()) != '\r') {
        if (isprint(c)) {
            cout << "*";
            potwierdz_haslo += c;
        }
    }
    cout << endl;
    if (haslo != potwierdz_haslo) {
        cout << "Hasla nie sa zgodne." << endl;
        return false;
    }
    ofstream file("users.txt", ios::app);
    file << login << ":" << zaszyfruj(haslo) << endl;
    file.close();

    // Tworzenie nowego katalogu dla użytkownika
    string nazwa_folderu = login + "_data";
    string polecenie = "mkdir " + nazwa_folderu;

    int result = system(polecenie.c_str());
    if (result != 0) {
        cout << "Nie udalo sie utworzyc folderu" << endl;
    }
    else {
        cout << "Folder utworzony pomyslnie" << endl;
    }

    return true;
}

bool logowanie() {
    string login, haslo;
    cout << "Login: ";
    cin >> login;
    cout << "Haslo: ";
    haslo = "";
    char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (!haslo.empty()) {
                haslo.pop_back();
                cout << "\b \b";
            }
        }
        else {
            haslo.push_back(c);
            cout << "*";
        }
    }
    cout << endl;
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        string znak = ":";
        string zapisana_nazwa = line.substr(0, line.find(znak));
        string zapisane_haslo = line.substr(line.find(znak) + 1);
        if (zapisana_nazwa == login && zaszyfruj(haslo) == zapisane_haslo) {
            zalogowany_uzytkownik = login;
            return true;
        }
    }
    return false;
}

bool wprowadz_dane() {
    bool anulowanie = false;
    do
    {
        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        cout << "Miejsce: ";
        getline(cin, miejsce);

        cout << "Data: ";
        getline(cin, dzisiejsza_data);


        cout << "Nazwisko nadawcy: ";
        getline(cin, nazwisko_nadawcy);


        cout << "Adres nadawcy: ";
        getline(cin, adres_nadawcy);


        cout << "Nazwa przewoźnika: ";
        getline(cin, nazwa_przewoznika);


        cout << "Nazwisko odbiorcy: ";
        getline(cin, nazwisko_odbiorcy);


        cout << "Adres odbiorcy: ";
        getline(cin, adres_odbiorcy);


        cout << "Ilość sztuk: ";
        getline(cin, ilosc_sztuk);


        cout << "Waga brutto: ";
        getline(cin, waga_brutto);

        char potwierdzenie;
        cout << "Czy chcesz zatwierdzić i zapisać do bazy? (T/N): ";
        cin >> potwierdzenie;


        if (potwierdzenie == 'T' || potwierdzenie == 't') {
            string nazwa_pliku;
            cout << "Podaj nazwe pliku: ";
            cin >> nazwa_pliku;

            string sciezka = zalogowany_uzytkownik + "_data/" + nazwa_pliku + ".txt";
            cout << zalogowany_uzytkownik;
            cout << sciezka;
            ofstream plik(sciezka);

            if (plik.is_open())
            {
                plik << miejsce << ";" << dzisiejsza_data << ";" << nazwisko_nadawcy << ";" << adres_nadawcy << ";"
                    << nazwa_przewoznika << ";" << nazwisko_odbiorcy << ";" << adres_odbiorcy << ";"
                    << ilosc_sztuk << ";" << waga_brutto << "." << endl;

                plik.close();
                cout << "Dane zostaly zapisane do pliku." << endl;
                anulowanie = true;
            }
            else
            {
                cout << "Blad zapisu do pliku." << endl;
                anulowanie = true;
            }
        }
        if (potwierdzenie == 'N' || potwierdzenie == 'n') {
            anulowanie = true;
        }
    } while (anulowanie != true);
    return true;
}

void generuj_list_przewozowy() {}

void wyloguj() { cout << "Wylogowano." << endl; }



int menu() {
    while (true) {
        cout << "Wybierz opcję:" << endl;
        cout << "1. Zamówienie" << endl;
        cout << "2. Stan magazynowy" << endl;
        cout << "3. Wyloguj" << endl;
        string choice;
        cin >> choice;
        if (choice == "3") {
            wyloguj();
        }
        else if (choice == "1") {
            cout << "__ZAMÓWIENIE__" << endl;
            cout << "Wybierz opcję:" << endl;
            cout << "1. Wysyłka zamówienia" << endl;
            cout << "2. Przyjęcie zamówienia" << endl;
            cout << "3. Powrót" << endl;
            string zamowienieChoice;
            cin >> zamowienieChoice;
            if (zamowienieChoice == "1") {
                cout << "__WYSYŁKA__ZAMÓWIENIA" << endl;
                cout << "Wybierz opcję:" << endl;
                cout << "1. Wygeneruj list przewozowy" << endl;
                cout << "2. Odjęcie od stanu magazynowego" << endl;
                cout << "3. Powrót" << endl;
                string wysylkaChoice;
                cin >> wysylkaChoice;
                if (wysylkaChoice == "1") {
                    generuj_list_przewozowy();
                }
                else if (wysylkaChoice == "2") {
                    //odjecie od stanu magazynowego ();
                }
                else if (wysylkaChoice == "3") {
                    //powrót do głównego wyboru
                }
            }
            else if (zamowienieChoice == "2") {
                cout << "__PRZYJĘCIE__ZAMÓWIENIA" << endl;
                cout << "Wybierz opcję:" << endl;
                cout << "1. Dodaj przyjęty towar do stanu magazynowego " << endl;
                cout << "2. Powrót" << endl;
                string przyjecieChoice;
                cin >> przyjecieChoice;
                if (przyjecieChoice == "1") {
                    wprowadz_dane();
                }
                else if (przyjecieChoice == "2") {
                    //powrot do glownego wyboru
                }
            }
        }
        else if (choice == "2") {
            cout << "__STAN__MAGAZYNOWY__" << endl;
            cout << "Wybierz opcję:" << endl;
            cout << "1. Wyszukaj towar" << endl;
            cout << "2. Usuń towar" << endl;
            cout << "3. Powrót" << endl;
            string stanmagazynowyChoice;
            cin >> stanmagazynowyChoice;
            if (stanmagazynowyChoice == "1") {
                cout << "__BAZA__DANYCH__" << endl;
                cout << "Wybierz opcję:" << endl;
                cout << "1. Szukaj" << endl;
                cout << "2. Usuń towar" << endl;
                cout << "3. Powrót" << endl;
                string bazaChoice;
                cin >> bazaChoice;
                if (bazaChoice == "1") {
                    //szukaj();
                }
                else if (bazaChoice == "2") {
                    //usuńtowar();
                }
                else if (bazaChoice == "3") {
                    //powrót do głównego wyboru
                }

            }
        }
        else {
            cout << "Nieprawidłowa opcja. Wybierz ponownie." << endl;
        }
    }
    return 0;
}
string PL(const char* text)
{
    string result;
    for (; *text; text++)
        switch (*text)
        {
        case 'ą': result += static_cast <char>(0xa5); break;
        case 'ć': result += static_cast <char>(0x86); break;
        case 'ę': result += static_cast <char>(0xa9); break;
        case 'ł': result += static_cast <char>(0x88); break;
        case 'ń': result += static_cast <char>(0xe4); break;
        case 'ó': result += static_cast <char>(0xa2); break;
        case 'ś': result += static_cast <char>(0x98); break;
        case 'ż': result += static_cast <char>(0xbe); break;
        case 'ź': result += static_cast <char>(0xab); break;
        case 'Ą': result += static_cast <char>(0xa4); break;
        case 'Ć': result += static_cast <char>(0x8f); break;
        case 'Ę': result += static_cast <char>(0xa8); break;
        case 'Ł': result += static_cast <char>(0x9d); break;
        case 'Ń': result += static_cast <char>(0xe3); break;
        case 'Ó': result += static_cast <char>(0xe0); break;
        case 'Ś': result += static_cast <char>(0x97); break;
        case 'Ż': result += static_cast <char>(0xbd); break;
        case 'Ź': result += static_cast <char>(0x8d); break;
        default: result += *text; break;
        }
    return result;
}

int main() {

    while (true) {
        cout << "Wybierz jedna z opcji:" << endl;
        cout << "1. Rejestracja" << endl;
        cout << "2. Logowanie" << endl;
        cout << "3. Wyjscie" << endl;
        int option;
        cin >> option;
        switch (option) {
        case 1:
            if (zarejestruj()) {
                cout << "Rejestracja udana." << endl;
            }
            else {
                cout << "Rejestracja nieudana." << endl;
            }
            break;
        case 2:
            if (logowanie()) {
                cout << "Logowanie udane." << endl;
                menu();
            }
            else {
                cout << "Logowanie nieudane. Sprobuj ponownie." << endl;
            }
            break;
        case 3:
            return false;
            break;
        default:
            cout << "Nie ma takiej opcji." << endl;
        }
    }
    return 0;
}

