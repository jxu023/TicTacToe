#include <iostream>

#include "TicTacToe.h"

using namespace std;

int main() {
    TicTacToe t;
    // t.Play();
    cout << ToChar(Solve(t)) << '\n';
    cout << boards_checked << " boards checked\n";
}
