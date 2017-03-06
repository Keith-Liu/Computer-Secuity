

#include <iostream>

#include <fstream>

#include <string>

#include <vector>

#include <stdio.h>
 #include <stdlib.h>

using namespace std;

bool hasMal = false;

bool malwareFound = false;

bool qFlag = false; // set up flags for commands

bool sFlag = false;

vector<string> names;

vector<string> vstrs;

int trans(char c)
{
    int t = c - '0';

    if (t >=0 && t < 10) {

        return t;
    }

    t = toupper(c) - 'A';

    if (t >=0 && t < 6) {

        return 10 + t;
    }

    return -1;
}


string transform(string old)
{

    size_t pos = old.find("\\x");

    string n = "";

    while (pos != string::npos) {

        if (pos + 3 < old.size()) {

            int x = trans(old[pos + 2]);

            int y = (old[pos + 3]);

            if (x >= 0 && y >= 0) {

                string t = "a";

                t[0] = (x << 4 ) + y;

                old.replace(pos, 4, t);
            }
        }

        pos = old.find("\\x", pos + 1);

    }


    return old;
}


void readString(string fn) // read string from file and store them
{

    ifstream in(fn.c_str());

    if(!in)
    {
        perror("file open failed");

        exit(-1);
    }

    string line;

    int count = 0;

    while (std::getline(in, line))
    {

        count++;

        if (line.size() >= 1 && line[0] == '#') {

            continue;
        }
        std::size_t found = line.find(":");

        if (found == std::string::npos) {

            if(!qFlag){

                cout << "line " << count << ": Malformed line in " << fn << endl;
            }

            hasMal = true;

        }else{

            vstrs.push_back(transform(line.substr(found + 1)));

            names.push_back(line.substr(0, found));
        }

    }

    in.close();
}


void searchFile(string fn) // search string in the file
{

    string fs;
    if(fn != "") {

        std::ifstream in(fn.c_str());

        if (!in) {

            perror("open file error"); // if open failed

            exit(-1);

        }

        fs = std::string( (std::istreambuf_iterator<char>(in) ),
                         (std::istreambuf_iterator<char>()    ) );

    }else{

        fs = std::string( (std::istreambuf_iterator<char>(cin) ),
                         (std::istreambuf_iterator<char>()    ) );

        fn = "standard input";

    }

    //    cout << fs << endl;

    size_t pos = 0;


    for (int p = 0; p < fs.size(); p++) {
        for (int i=0; i < vstrs.size(); i++) {

            pos = fs.find(vstrs[i], p);

            if (pos == p) {

                if (!qFlag) {
                    cout << names[i] << " found at " << p << " in " << fn << endl;

                }


                malwareFound = true;

                if (sFlag) {

                    return;
                }
            }
        }

    }

}





int main(int argc, const char * argv[]) {

    vector<string> scans;

    string fn = "vdetect.str";

    for (int i=1; i<argc; i++) {

        if (argv[i] == string("-q")) {

            qFlag = true;
        }else if(argv[i] == string("-s"))
        {
            sFlag = true;
        }else if(argv[i] == string("-d"))
        {
            fn = argv[i+1];
            i++;

        }else{

            scans.push_back(argv[i]);
        }

    }

    readString(fn);

    if (scans.empty()) {

        searchFile("");
    }else{

        for (int i=0; i<scans.size(); i++) {

            searchFile(scans[i]);
        }
    }



    if (malwareFound) {

        if (hasMal) {
            cout << "The exit status code is 3" << endl;
            return 3;
        }else{

            cout << "The exit status code is 1" << endl;
            return 1;
        }

    }else{

        if (hasMal) {

            cout << "The exit status code is 2" << endl;
            return 2;
        }else{

            cout << "The exit status code is 0" << endl;
            return 0;
        }


    }

}
