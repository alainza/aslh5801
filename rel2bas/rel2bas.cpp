// rel2bas.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

void usage(const char *name)
{
    cerr << "Usage: " << name << " program.rel" << endl;
}

int main(int argc, char* argv[])
{
    // usage rel2bas program.rel
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }

    vector<int> bytes;
    int address = 0;

    bool first = true;
    ifstream relfile(argv[1]);
    string line;
    while (getline(relfile, line)) {
        if (line[0] == 'T') {
            // Text line
            std::istringstream parser(line);
            char c;
            int b1, b2;
            parser >> std::hex >> c >> b1 >> b2;
            if (first) {
                address = b1 * 256 + b2;
                first = false;
            }
            while (!parser.eof()) {
                parser >> b1;
                bytes.push_back(b1);
            }
        }
    }
    cout << "10 N = " << bytes.size() << endl;
    if (address != 0) {
        cout << "20 A = &" << hex << uppercase << address << endl;
    }
    else {
        cout << "20 INPUT \"Address=\";A" << endl;
    }
    cout << "30 FOR I=0 TO N-1" << endl;
    cout << "40 READ B" << endl;
    cout << "50 POKE A+I, B" << endl;
    cout << "60 NEXT I" << endl;
    cout << "70 END" << endl;

//    cout << setw(2) << setfill('0');
    int linenum = 100;
    first = true;
    for (unsigned int i = 0; i < bytes.size(); i++) {
        if (first) {
            cout << dec << linenum << hex << uppercase << " DATA &" << setw(2) << setfill('0') << bytes[i];
            first = false;
        }
        else {
            cout << ", &" << setw(2) << setfill('0') << bytes[i];
        }
        if (i % 8 == 7) {
            first = true;
            linenum += 10;
            cout << endl;
        }
    }
    cout << endl;
    return 0;
}

