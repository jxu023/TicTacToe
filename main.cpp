#include <iostream>

#include "TicTacToe.h"

using namespace std;

int main() {
    TicTacToe t;
    // t.Play();
    cout << ToChar(SolveNaive(t)) << '\n';
    cout << boards_checked << " boards checked\n";
    cout << "SolveNaive checked " << boards_at_level.size() << " levels.\n";
    for (int i = 0; i < 10; ++i) {
        cout << "level " << i << " checked " << boards_at_level[i] << " boards\n";
    }
    // results in "986410 boards checked"
    //
    // SolveNaive() executes a large tree, with root node as initial board
    // and each possible move corresponds to an edge to a child node.
    //
    // Level 0 has 1 node (root node)
    // Level 1 has 9 nodes (9 possible moves)
    // Level 2 has 9*8 nodes (8 possibles moves)
    // Level 3 has 9*8*7 nodes
    // ...
    // Level 8 has 9*8..*2 nodes
    // Level 9 has up to 9! nodes
    //
    // Taking the sum of all nodes in the tree, we get a total of 986,410
    // note, levels 8 and 9 have the same number of nodes.
    cout << endl;

    cout << ToChar(SolveMemo(t)) << '\n';
    cout << boards_checked << " boards checked\n";
    for (int i = 0; i < 10; ++i) {
        cout << "level " << i << " checked " << boards_at_level[i] << " boards\n";
    }
}
