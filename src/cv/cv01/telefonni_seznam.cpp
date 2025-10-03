#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */
struct CContact
{
    string name;
    string surname;
    string phone;

    bool validateNumber(const string & number)
    {
        if (number.length() != 9 || number[0] == '0')
            return false;
        for (auto c : number)
            if (!isdigit(c))
                return false;
        return true;
    }
};

class CPhoneBook
{
    vector<CContact> contacts;
public:

    bool readQuery(const string & line, ostream & out)
    {
        istringstream iss(line);
        string name;
        if (line == ""){
            out << "-> 0" << endl;
            return true;
        }
        string tmp;
        if (!(iss >> name) || iss >> tmp)
            return false;
        this->printOutput(out, name);
        return true;
    }

    bool readInput(const string & fileName, ostream & out)
    {
        string line;
        ifstream fin;
        fin.open(fileName, ios::in);
        if (!fin.is_open())
            return false;
        while (true)
        {
            getline(fin, line);
            if (line.empty())
                break;
            istringstream iss(line);
            CContact contact;
            string tmp;
            // check also that after the phone number there is no other data
            if (!(iss >> contact.name >> contact.surname >> contact.phone) 
                || !contact.validateNumber(contact.phone) || iss >> tmp )
                return false;
           
            contacts.push_back(contact);
        }
        while (getline(fin, line))
        {
            if (!this->readQuery(line, out))
                return false;
        }
        if (!fin.eof())
            return false;
        fin.close();
        return true;
    }
    
    void printOutput(ostream & out, const string & name)
    {
        int count = 0;
        for (auto contact : contacts)
            if (contact.name == name || contact.surname == name){
                out << contact.name << " " << contact.surname << " " << contact.phone << endl;
                count++;
            }
        out << "-> " << count << endl;
    }

};

bool report ( const string & fileName, ostream & out )
{
    CPhoneBook phoneBook;
    if (!phoneBook.readInput(fileName, out))
        return false;

    return true;
}

#ifndef __PROGTEST__
int main ()
{
    ostringstream oss;
    oss . str ( "" );
    assert ( report( "tests/test0_in.txt", oss ) == true );
    assert ( oss . str () ==
        "John Christescu 258452362\n"
        "John Harmson 861647702\n"
        "-> 2\n"
        "-> 0\n"
        "Josh Dakhov 264112084\n"
        "Dakhov Speechley 865216101\n"
        "-> 2\n"
        "John Harmson 861647702\n"
        "-> 1\n" );
    oss . str ( "" );
    assert ( report( "tests/test1_in.txt", oss ) == false );
    oss . str ( "" );
    report( "tests/test3_in.txt", cout );
    return 0;
}
#endif /* __PROGTEST__ */
