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

int boards_checked = 0;

Tile Solve(const TicTacToe& tic_tac_toe) {
    ++boards_checked;
    if (tic_tac_toe.GameOver())
        return tic_tac_toe.Winner();

    const Tile color = tic_tac_toe.Turn();
    bool has_tie = false;
    for (const Coord m : tic_tac_toe.ValidMoves()) {
        TicTacToe t = tic_tac_toe;
        t.Move(m);
        const Tile result = Solve(t);

        if (result == color)
            return color;
        else if (result == _)
            has_tie = true;
    }
    return has_tie ? _ : Opposite(color);
}
