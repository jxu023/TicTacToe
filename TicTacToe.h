#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>

using namespace std;

enum Tile : int;

struct Coord {
    int i;
    int j;

    Coord operator+(const Coord c) {
        return Coord{i+c.i, j+c.j};
    }
    Coord operator-(const Coord c) {
        return Coord{i-c.i, j-c.j};
    }
    Coord& operator+=(const Coord c) {
        i += c.i;
        j += c.j;
        return *this;
    }
    Coord& operator-=(const Coord c) {
        i -= c.i;
        j -= c.j;
        return *this;
    }
};

struct Board {
    Tile& operator[](Coord c) {
        return b[c.i][c.j];
    }
    const Tile& operator[](Coord c) const {
        return b.at(c.i).at(c.j);
    }
    vector<vector<Tile>> b;
};

class TicTacToe {
public:
    TicTacToe();
    // Reset board to an empty state.
    void Reset();

    // Print the board state.
    void Show() const;

    // Takes a Coord to make a move.
    void Move(Coord coord);
    vector<Coord> ValidMoves() const;

    // Undoes a move.
    vector<Coord> ValidUnmoves() const;
    void Unmove(Coord cood);

    // Checks if Coord is a valid move.
    bool IsValidMove(Coord coord);
    // Checks if Coord is inBounds.
    static bool IsValidCoord(Coord coord);

    // Plays a whole game from the current state
    // with moves coming from STDIN.
    void Play();
    void PromptMove();

    int HashCode() const;

    Tile Turn() const {
        return turn;
    }
    bool GameOver() const {
        return game_over;
    }
    Tile Winner() const {
        return winner;
    }
private:
    Board b;
    Tile turn;
    int num_moves;
    bool game_over;
    Tile winner;
    void ForEachTile(function<void(Coord, Tile)> fn);
    void ForEachTile(function<void(Coord, Tile)> fn) const;
};

char ToChar(Tile t);

extern int boards_checked;
extern unordered_map<int, int> boards_at_level;

// It's dynamic programming!
Tile SolveNaive(const TicTacToe& tic_tac_toe);
Tile SolveMemo(const TicTacToe& tic_tac_toe);
Tile SolveBottomUp(const TicTacToe& tic_tac_toe);
