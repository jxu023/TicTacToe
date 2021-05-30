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

void TicTacToe::Move(Coord coord) {
    b[coord] = turn;
    turn = Opposite(turn);
    ++num_moves;

    // update game_over and winner
    if (num_moves == 9) {
        game_over = true;
        winner = _;
    }

    // given a slope, get all valid coords along it
    // check horiz -- slope is 0, 1
    // check vert -- slope is 1, 0
    // check diags -- slope is 1, 1, slope is 1, -1
    constexpr Coord dc[4] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    const Tile color = b[coord];
    for (int k = 0; k < 4; ++k) {
        int in_a_row = 1;
        Coord nc;
        for (nc = nc + dc[k]; IsValidCoord(nc) && b[nc] == color; nc += dc[k])
            ++in_a_row;
        for (nc = nc - dc[k]; IsValidCoord(nc) && b[nc] == color; nc -= dc[k])
            ++in_a_row;

        if (in_a_row == 3) {
            winner = color;
            game_over = true;
            return;
        }
    }
}

bool TicTacToe::IsValidMove(Coord coord) {
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

vector<Coord> TicTacToe::ValidMoves() const {
    vector<Coord> ret;
    // for each valid coord
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (b.b[i][j] == _) // if empty spot
                ret.push_back({i, j});
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
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            hash += b.b[i][j] * base;
            base *= 3;
        }
    }
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

// TODO
vector<Coord> TicTacToe::ValidUnmoves() const {
    return {};
}

// TODO
void TicTacToe::Unmove(Coord coord) {
}

// TODO
static vector<TicTacToe> AllFullBoards() {
    return {};
}

// TODO
static void RemoveImpossibleGames(vector<TicTacToe>& games) {
    remove_if(games.begin(), games.end(), [] (const TicTacToe& t) {
            return false;
            });
}

Tile SolveBottomUp(const TicTacToe& tic_tac_toe) {
    // TODO only keep results for current level in tree
    unordered_map<int, Tile> hashcode_to_result;

    vector<TicTacToe> level = AllFullBoards();
    RemoveImpossibleGames(level);

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
