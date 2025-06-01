#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;

struct Dice {
    string original;
    
    Dice(const string& s) : original(s) {}
};

class Monster {
public:
    string name;
    string desc;
    vector<string> colors;
    Dice speed {"0+0d0"};
    vector<string> abilities;
    Dice hp {"0+0d0"};
    Dice damage {"0+0d0"};
    char symbol = ' ';
    int rarity = 0;

    void print() const {
        cout << name << "\n"
             << desc << "\n"
             << symbol << "\n";
        
        for (size_t i = 0; i < colors.size(); ++i) {
            cout << colors[i];
            if (i != colors.size()-1) cout << " ";
        }
        cout << "\n"
             << speed.original << "\n";
        
        for (size_t i = 0; i < abilities.size(); ++i) {
            cout << abilities[i];
            if (i != abilities.size()-1) cout << " ";
        }
        cout << "\n"
             << hp.original << "\n"
             << damage.original << "\n"
             << rarity << "\n\n";
    }
};

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool parseMonsterFile(const string& path, vector<Monster>& monsters) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error opening file\n";
        return false;
    }

    string line;
    getline(file, line);
    if (line != "RLG327 MONSTER DESCRIPTION 1") {
        cerr << "Invalid file format\n";
        return false;
    }

    Monster current;
    bool inMonster = false;
    bool readingDesc = false;
    map<string, bool> fields;
    
    while (getline(file, line)) {
        if (readingDesc) {
            if (line == ".") {
                readingDesc = false;
                fields["DESC"] = true;
            } else {
                if (!current.desc.empty()) current.desc += "\n";
                current.desc += line;
            }
            continue;
        }

        if (line == "BEGIN MONSTER") {
            inMonster = true;
            current = Monster();
            fields.clear();
            continue;
        }

        if (line == "END") {
            if (inMonster && fields.size() == 9) {
                monsters.push_back(current);
            }
            inMonster = false;
            continue;
        }

        if (!inMonster) continue;

        if (line.rfind("NAME ", 0) == 0 && !fields["NAME"]) {
            current.name = line.substr(5);
            fields["NAME"] = true;
        }
        else if (line.rfind("DESC", 0) == 0) {
            readingDesc = true;
            current.desc.clear();
        }
        else if (line.rfind("COLOR ", 0) == 0 && !fields["COLOR"]) {
            current.colors = split(line.substr(6), ' ');
            fields["COLOR"] = true;
        }
        else if (line.rfind("SPEED ", 0) == 0 && !fields["SPEED"]) {
            current.speed = Dice(line.substr(6));
            fields["SPEED"] = true;
        }
        else if (line.rfind("ABIL ", 0) == 0 && !fields["ABIL"]) {
            current.abilities = split(line.substr(5), ' ');
            fields["ABIL"] = true;
        }
        else if (line.rfind("HP ", 0) == 0 && !fields["HP"]) {
            current.hp = Dice(line.substr(3));
            fields["HP"] = true;
        }
        else if (line.rfind("DAM ", 0) == 0 && !fields["DAM"]) {
            current.damage = Dice(line.substr(4));
            fields["DAM"] = true;
        }
        else if (line.rfind("SYMB ", 0) == 0 && !fields["SYMB"]) {
            current.symbol = line.substr(5)[0];
            fields["SYMB"] = true;
        }
        else if (line.rfind("RRTY ", 0) == 0 && !fields["RRTY"]) {
            current.rarity = stoi(line.substr(5));
            fields["RRTY"] = true;
        }
    }

    file.close();
    return true;
}

int main() {
    vector<Monster> monsters;
    // string path = "monster_desc.txt"; 
    string path = getenv("HOME");
    path += "/.rlg327/monster_desc.txt";
    
    if (!parseMonsterFile(path, monsters)) {
        return 1;
    }

    for (const auto& m : monsters) {
        m.print();
    }

    return 0;
}
