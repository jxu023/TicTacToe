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
    bool operator==(const Coord c) const {
        return i == c.i && j == c.j;
    }
    bool operator!=(const Coord c) const {
        return i != c.i || j != c.j;
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
    Board(int m, int n) {
        b.resize(m, vector<Tile>(n, static_cast<Tile>(0)));
    }
    Board() {}
};

class TicTacToe {
public:
    TicTacToe();
    // Reset board to an empty state.
    void Reset();

    // Print the game state.
    void Show() const;

    // Takes a Coord to make a move.
    void Move(Coord coord);
    vector<Coord> ValidMoves() const;
    bool IsValidMove(Coord coord) const;

    // Undoes a move.
    vector<Coord> ValidUnmoves() const;
    void Unmove(Coord cood);
    bool IsValidUnmove(Coord coord) const;
    // Checks if the unmove undoes a win.
    bool IsUnwin(Coord cood) const;

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

    vector<TicTacToe> AllFullBoards() const;
private:
    Board b;
    Tile turn;
    int num_moves;
    bool game_over;
    Tile winner;
    void ForEachTile(function<void(Coord, Tile)> fn);
    void ForEachTile(function<void(Coord, Tile)> fn) const;
    int NumWinDirs(Coord coord) const;
};

char ToChar(Tile t);

extern int boards_checked;
extern unordered_map<int, int> boards_at_level;

// It's dynamic programming!
Tile SolveNaive(const TicTacToe& tic_tac_toe);
Tile SolveMemo(const TicTacToe& tic_tac_toe);
// Given that bottom up uses memory equivalent to BFS, SolveMemo which uses DFS
// memory is probably more efficient.
Tile SolveBottomUp(const TicTacToe& tic_tac_toe);
