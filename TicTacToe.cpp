#include <algorithm>

#include "TicTacToe.h"

enum Tile : int {
    _,
    X,
    O,
};

static Tile Opposite(Tile t) {
    switch (t) {
        case X:
            return O;
        case O:
            return X;
        default:
            return _;
    }
}

char ToChar(Tile t) {
    switch(t) {
        case X:
            return 'X';
        case O:
            return 'O';
        case _:
        default:
            return '_';
    }
}

TicTacToe::TicTacToe() {
    Reset();
}

void TicTacToe::Reset() {
    b.b = {{_,_,_},
           {_,_,_},
           {_,_,_}};
    turn = X;
    num_moves = 0;
    game_over = false;
    winner = _;
}

void TicTacToe::Show() const {
    if (game_over) {
        cout << ToChar(winner) << " wins!\n";
    } else {
        cout << ToChar(turn) << " to move\n";
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << ToChar(b.b[i][j]) << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

int TicTacToe::NumWinDirs(Coord coord) const {
    int wins = 0;
    // given a slope, get all valid coords along it
    // check horiz -- slope is 0, 1
    // check vert -- slope is 1, 0
    // check diags -- slope is 1, 1, slope is 1, -1
    constexpr Coord dc[4] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    const Tile color = b[coord];
    for (int k = 0; k < 4; ++k) {
        int in_a_row = 1;
        Coord nc;
        for (nc = coord + dc[k]; IsValidCoord(nc) && b[nc] == color; nc += dc[k])
            ++in_a_row;
        for (nc = coord - dc[k]; IsValidCoord(nc) && b[nc] == color; nc -= dc[k])
            ++in_a_row;
        if (in_a_row == 3)
            ++wins;
    }
    return wins;
}

void TicTacToe::Move(Coord coord) {
    b[coord] = turn;
    turn = Opposite(turn);
    ++num_moves;

    if (num_moves == 9)
        game_over = true;

    if (NumWinDirs(coord) > 0) {
        winner = b[coord];
        game_over = true;
    }
}

bool TicTacToe::IsValidMove(Coord coord) const {
    return IsValidCoord(coord) && b[coord] == _;
}

bool TicTacToe::IsValidCoord(Coord coord) {
    auto [i, j] = coord;
    return i >= 0 && j >= 0 && i < 3 && j < 3;
}

void TicTacToe::PromptMove() {
    int i, j;
    cin >> i;
    cin >> j;
    cout << '\n';
    if (IsValidMove({i, j})) {
        Move({i, j});
    }
    else {
        cout << i << ' ' << j << " is invalid!";
        PromptMove();
    }
}

void TicTacToe::Play() {
    while (!game_over) {
        Show();
        PromptMove();
    }
    Show();
}

void TicTacToe::ForEachTile(function<void(Coord, Tile)> fn) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            fn({i, j}, b.b[i][j]);
        }
    }
}

// Man .. how do you const cast calling a void function?
void TicTacToe::ForEachTile(function<void(Coord, Tile)> fn) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            fn({i, j}, b.b[i][j]);
        }
    }
}

vector<Coord> TicTacToe::ValidMoves() const {
    vector<Coord> ret;
    // for each valid coord
    ForEachTile([&ret] (Coord c, Tile t) {
        if (t == _)
            ret.push_back(c);
    });
    return ret;
}

static int tree_level;
static Tile SolveBaseLogic(const TicTacToe& tic_tac_toe, Tile (&Recurs)(const TicTacToe&)) {
    if (tic_tac_toe.GameOver())
        return tic_tac_toe.Winner();

    const Tile color = tic_tac_toe.Turn();
    bool has_tie = false;
    for (const Coord m : tic_tac_toe.ValidMoves()) {
        TicTacToe t = tic_tac_toe;
        t.Move(m);
        const Tile result = Recurs(t);

        if (result == color)
            return color;
        else if (result == _)
            has_tie = true;
    }
    return has_tie ? _ : Opposite(color);
}

int boards_checked;
unordered_map<int, int> boards_at_level;

static Tile CountSolveBase(const TicTacToe& tic_tac_toe, Tile (&Recurs)(const TicTacToe&)) {
    ++boards_checked;
    ++boards_at_level[tree_level];
    ++tree_level;
    Tile ret = SolveBaseLogic(tic_tac_toe, Recurs);
    --tree_level;
    return ret;
}

static Tile SolveBase(const TicTacToe& tic_tac_toe, Tile (&Recurs)(const TicTacToe&)) {
    tree_level = 0;
    boards_checked = 0;
    boards_at_level.clear();
    return CountSolveBase(tic_tac_toe, Recurs);
}

Tile SolveNaiveHelper(const TicTacToe& t) {
    return CountSolveBase(t, SolveNaiveHelper);
}

Tile SolveNaive(const TicTacToe& tic_tac_toe) {
    return SolveBase(tic_tac_toe, SolveNaiveHelper);
}

// Consider counting duplicates per level.
int TicTacToe::HashCode() const {
    // Board has 9 tiles, each tile has 3 possible values. Thus there are only
    // 3^9 = 19683 possible board states. Let's interpret the board as a base
    // 3 number and convert to base 10.
    int hash = 0;
    int base = 1;
    ForEachTile([&] (Coord c, Tile t) {
        hash += b[c] * base;
        base *= 3;
    });
    return hash;
}

// This could also just be an array of size 3^9.
static unordered_map<int, Tile> memo;

static Tile SolveMemoHelper(const TicTacToe& tic_tac_toe) {
    const int hash = tic_tac_toe.HashCode();
    const auto it = memo.find(hash);
    if (auto it = memo.find(hash); it != memo.end()) {
        return it->second;
    }
    return memo[hash] = CountSolveBase(tic_tac_toe, SolveMemoHelper);
}

Tile SolveMemo(const TicTacToe& tic_tac_toe) {
    return SolveBase(tic_tac_toe, SolveMemoHelper);
}

bool TicTacToe::IsUnwin(Coord coord) const {
    Tile t = Opposite(turn);
    Coord cross_coord;
    bool cross_win = false;
    // It's possible there are multiple "separate" wins across the board
    // In that case each unwin must dismantle one of them.
    // TODO generalize unwin to dismantle groups (then it'll work on arbitrary
    // x-in-a-row).
    ForEachTile([&] (Coord c, Tile tile) {
        // There are at most 2 wins in a row
        if (tile == t && NumWinDirs(c) == 2) {
            cross_win = true;
            cross_coord = c;
        }
    });
    if (cross_win)
        return coord == cross_coord;
    else
        return NumWinDirs(coord) > 0;
}

bool TicTacToe::IsValidUnmove(Coord coord) const {
    return IsValidCoord(coord) && b[coord] == Opposite(turn)
        && (!game_over || IsUnwin(coord));
}

vector<Coord> TicTacToe::ValidUnmoves() const {
    Tile color = Opposite(turn);
    vector<Coord> ret;
    ForEachTile([&] (Coord c, Tile t) {
        if (IsValidUnmove(c))
            ret.push_back(c);
    });
    return ret;
}

void TicTacToe::Unmove(Coord coord) {
    Tile color = b[coord];
    b[coord] = _;
    turn = color;
    --num_moves;

    if (game_over) {
        game_over = false;
        ForEachTile([&] (Coord c, Tile t) {
            if (NumWinDirs(c) > 0)
                game_over = true;
        });
    }
}

static Tile NextTile(Tile t) {
    return static_cast<Tile>(static_cast<int>(t)+1);
}

vector<TicTacToe> TicTacToe::AllFullBoards() const {
    // for each of 2^9 boards, add them to a list
    vector<Board> boards;
    Board initial_board; // TODO Factor out an initial board constructor.
    initial_board.b = {{_,_,_},
                       {_,_,_},
                       {_,_,_}};
    boards.push_back(initial_board);

    for (Coord c{0, 0}; c != EndCoord(); c = NextValidCoord(c)) {
        vector<Board> children;
        for (const Board& parent : boards) {
            // TODO create a "all possible boards fn" where this loops over=
            // {X, O, _}
            for (Tile t : {X, O}) {
                Board b = parent;
                b[c] = t;
                children.push_back(b);
            }
        }
        boards = children;
    }

    // TODO or just enumerate them in such a way that xcount and ocount are
    // correct. For each parent, produce a child where one of the *tiles* has
    // x from x bag and o from o bag.
    // for each tile, for each {X, O} if !colorbag.empty(), add child.

    // TODO remove boards where not (x.count == 5 and o.count == 4)
    // remove_if();

    vector<TicTacToe> ret;
    for (auto it = boards.rbegin(); it != boards.rend(); ++it, boards.pop_back()) {
        TicTacToe t;
        t.b = *it;
        ret.push_back(t);
    }

    return ret;
}

// TODO use the count statistics to validate SolveBottomUp() checks the same
// nodes per level as SolveMemo().
Tile SolveBottomUp(const TicTacToe& tic_tac_toe) {
    // TODO only keep results for current level in tree
    unordered_map<int, Tile> hashcode_to_result;

    vector<TicTacToe> level = tic_tac_toe.AllFullBoards();
    // alternately we could use AllPossibleBoards() (allows _)

    // process states from bottom leaf nodes up to the root
    while (level.size() > 1) {
        unordered_map<int, TicTacToe> parents; // parent hashcode to parent game

        for (const TicTacToe& child : level) {
            for (Coord c : child.ValidUnmoves()) {
                TicTacToe p = child;
                p.Unmove(c);
                if (parents.find(p.HashCode()) == parents.end())
                    parents[p.HashCode()] = p;

                // TODO update p's result
            }
        }

        level.clear();
        for (auto& [hash, p] : parents)
            level.push_back(p);
    }
    return _;
}
