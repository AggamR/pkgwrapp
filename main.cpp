// this code is under the MIT license, see `LICENSE` file in this repository

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define strhas(og, str) og.find(str) != string::npos 

typedef struct packageManager
{
    string name, upgrade, install, remove, root;

    string to_string() {
        return "name: " + name + ", upgrade: " + upgrade + ", install: " + install + ", remove: " + remove + ", root: " + root; 
    }
} packageManager;


int runPkgmCmd(string cmdbase, vector<packageManager> pkgManagers, string packages, int i) {
    string cmd;
    int retCode;

    cout << "trying " << pkgManagers[i].name << ".\n";

    if (pkgManagers[i].root == "true") 
        cmd = "su -c \"";
    else 
        cmd = "";

    cmd += cmdbase;
    cmd += " " + packages;

    if (pkgManagers[i].root == "true")
        cmd += "\"";

    retCode = system(cmd.c_str());
    if (retCode == 0) {
        cout << pkgManagers[i].name << " ran sucessfuly.";
        return 0;
    }

    cout << pkgManagers[i].name << " failed.\n";
    return 1;
}

string strReplace (string s, string toReplace, string dest) {
    s.replace(s.find(toReplace), toReplace.length(), dest);
    return s;
}

int main(int argc, char *argv[]){
    // ========================================================================
    // reading of config file
    ifstream configFile ("/etc/pkgwrapp");
    string line, property, propertyVal;
    vector<packageManager> pkgManagers;
    
    while (getline (configFile, line)) {
        if (line.length() == 0) continue;

        if (line == "PKG") {
            pkgManagers.push_back(packageManager());
            continue;
        }

        property = line.substr(0, line.find(":"));
        propertyVal = line.substr(line.find(":") + 1, line.length());
        if (propertyVal.at(0) == ' ')
            propertyVal = propertyVal.substr(1,propertyVal.length());

        // yeah this is disgusting... sory about that...
        if (property == "name")
            pkgManagers[pkgManagers.size() - 1].name = propertyVal;
        else if (property == "install")
            pkgManagers[pkgManagers.size() - 1].install = propertyVal;
        else if (property == "upgrade")
            pkgManagers[pkgManagers.size() - 1].upgrade = propertyVal;
        else if (property == "remove")
            pkgManagers[pkgManagers.size() - 1].remove = propertyVal;
        else if (property == "root")
            pkgManagers[pkgManagers.size() - 1].root = propertyVal;
    }

    configFile.close();

    

    // end reading config file
    // ========================================================================


    // didn't work otherwise, idk what to do, had to maker a stringstream

    if (argc < 2) {
        cout << "USAGE: pkg [option] [arguments]";
        return 1;
    }

    stringstream sStream;
    sStream << argv[1];
    string option;
    sStream >> option;

    if (argc < 3 && option != "upgrade") {
        cout << "USAGE: pkg [option] [arguments]";
        return 1;
    }

    string packages = "";
    for (int i = 2; i < argc; i++) {   
        packages += argv[i];
        packages += " ";
    }
     
    int retCode;
    if (!strcmp(argv[1], "install")) {
        for (int i = 0; i < pkgManagers.size(); i++) {
            retCode = runPkgmCmd(pkgManagers[i].install, pkgManagers, packages, i);
            if (retCode == 0) break;
        }
    } else if (!strcmp(argv[1], "upgrade")) {
        for (int i = 0; i < pkgManagers.size(); i++) {
            retCode = runPkgmCmd(pkgManagers[i].upgrade, pkgManagers, "", i);
            if (retCode == 0) break;
        }
    } else if (!strcmp(argv[1], "remove")) {
        for (int i = 0; i < pkgManagers.size(); i++) {
            retCode = runPkgmCmd(pkgManagers[i].remove, pkgManagers, packages, i);
            if (retCode == 0) break;
        }
    }

    return 0;
}