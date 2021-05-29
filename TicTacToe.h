#include <iostream>
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

    // Checks if Coord is a valid move.
    bool IsValidMove(Coord coord);
    // Checks if Coord is inBounds.
    static bool IsValidCoord(Coord coord);

    // Plays a whole game from the current state
    // with moves coming from STDIN.
    void Play();
    void PromptMove();

private:
    Board b;
    Tile turn;
    int num_moves;
    bool game_over;
    Tile winner;
};
