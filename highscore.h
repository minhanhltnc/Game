#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int loadHighScore(const string& filename) {
    ifstream file(filename);
    int highScore = 0;

    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return highScore;
    }

    file >> highScore;
    file.close();
    return highScore;
}

void saveHighScore(const string& filename, int highScore) {
    ofstream file(filename, ios::out);

    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    file << highScore;
    file.close();
}

#endif // _HIGHSCORE_H
