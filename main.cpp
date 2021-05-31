#include <iostream>

#include "TicTacToe.h"

using namespace std;

int main() {
    TicTacToe t;
    cout << ToChar(SolveNaive(t)) << '\n';
    cout << boards_checked << " boards checked by SolveNaive.\n";
    for (int i = 0; i < 10; ++i) {
        cout << "level " << i << " checked " << boards_at_level[i] << " boards\n";
    }
    cout << endl;
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
    // This happens if the only way to end a game is by filling up the board.
    // With 3-in-a-row, we get this instead:
    //
    // 94978 boards checked by SolveNaive.
    // level 0 checked 1 boards
    // level 1 checked 9 boards
    // level 2 checked 72 boards
    // level 3 checked 280 boards
    // level 4 checked 1511 boards
    // level 5 checked 4380 boards
    // level 6 checked 12483 boards
    // level 7 checked 24327 boards
    // level 8 checked 30655 boards
    // level 9 checked 21260 boards

    cout << ToChar(SolveMemo(t)) << '\n';
    cout << boards_checked << " boards checked by SolveMemo.\n";
    for (int i = 0; i < 10; ++i) {
        cout << "level " << i << " checked " << boards_at_level[i] << " boards\n";
    }
    cout << endl;
    // 4804 boards checked by SolveMemo.
    // level 0 checked 1 boards
    // level 1 checked 9 boards
    // level 2 checked 72 boards
    // level 3 checked 205 boards
    // level 4 checked 688 boards
    // level 5 checked 1008 boards
    // level 6 checked 1368 boards
    // level 7 checked 1007 boards
    // level 8 checked 373 boards
    // level 9 checked 73 boards

    cout << "Showing all full boards\n";
    cout << "There are " << t.AllFullBoards().size() << " full boards\n";
    // for (auto tt : t.AllFullBoards()) {
    //     tt.Show();
    //     cout << endl;
    // }
}
