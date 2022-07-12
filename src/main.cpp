#include<iostream>
#include<stdlib.h>
#include<math.h> 
#include<algorithm> 

struct Move {
    int row, col, moveVal;
    Move() : row(-1), col(-1), moveVal(-1000) {}
};

bool isMovesLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == 0)
                return true;
    return false;
}

int evaluate(int b[3][3]) {
    for (short row = 0; row < 3; row++)
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2])
            if (b[row][0] != 0)
                return b[row][0] * 10;

    for (short col = 0; col < 3; col++)
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col])
            if (b[0][col] != 0)
                return b[0][col] * 10;

    if ((b[0][2] == b[1][1] && b[1][1] == b[2][0]) || (b[0][0] == b[1][1] && b[1][1] == b[2][2]))
        if (b[1][1] != 0)
            return b[1][1] * 10;

    return 0;
}

template <typename Proc>
int traverseCells(int board[3][3], bool isMax, Proc check) {
    int best = -1000;
    int move = 1;
    if (!isMax) {
        best = 1000;
        move = -1;
    }

    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            // Check if the cell is empty
            if (board[i][j] == 0) {
                // Make the move
                board[i][j] = move;

                best = check(best);

                // Undo the move
                board[i][j] = 0;
            }

    return best;
}

// Minimax algorythm
int minimax(int board[3][3], int depth, bool isMax) {
    int score = evaluate(board);

    // If 1 has won the game
    if (score == 10)
        return score - depth;
    else if (score == -10)
        return score + depth;

    // If there are no moves left and no winner
    if (isMovesLeft(board) == false)
        return 0;

    // Maximizer's move
    if (isMax)
        return traverseCells(board, isMax, [board, depth, isMax](int best) {
            return std::max(best, minimax(board, depth + 1, false));
            });
    // Minimizer's score
    return traverseCells(board, isMax, [board, depth, isMax](int best) {
        return std::min(best, minimax(board, depth + 1, true));
        });
}


// Returns the best possible move for the player
Move findBestMove(int board[3][3], bool isMax) {
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    if (!isMax)
        bestMove.moveVal = 1000;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            // Check if cell is empty
            if (board[i][j] == 0) {
                // Make the move
                if(isMax)
                    board[i][j] = 1;
                else
                    board[i][j] = -1;

                // compute evaluation function for this
                // move.
                int moveVal = minimax(board, 0, !isMax);

                // Undo the move
                board[i][j] = 0;

                if ((isMax && moveVal > bestMove.moveVal) || (!isMax && moveVal < bestMove.moveVal)) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestMove.moveVal = moveVal;
                }
            }

    return bestMove;
}

// Draws Tic Tac Toe board from the board 
void drawBoard(int board[3][3]) {
    system("CLS");
    std::cout << "-------------\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "|";
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 1)
                std::cout << " X |";
            else if (board[i][j] == -1)
                std::cout << " O |";
            else
                std::cout << "   |";
        }
        std::cout << std::endl;
        std::cout << "-------------\n";
    }
    std::cout << std::endl;
}

// Main game loop
void gameLoop(int board[3][3], int players) {
    bool gameOver = false;
    bool playerTurn = true;
    int boardValue = 0;
    while (!gameOver) {
        drawBoard(board);

        if (boardValue == 10) {
            if (players == 2)
                std::cout << "Player 1 won\n";
            gameOver = true;
            break;
        }
        else if (boardValue == -10) {
            if (players == 2)
                std::cout << "Player 2 won\n";
            if (players == 1)
                std::cout << "AI won\n";
            gameOver = true;
            break;
        }
        else if (!isMovesLeft(board)) {
            std::cout << "draw\n";
            gameOver = true;
            break;
        }
        if (playerTurn) {
            if (players == 2 || players == 1) {
                double move = 0;
                std::cout << "[move (1-9)]: ";
                std::cin >> move;
                if ((int)move < 10 && (int)move > 0) {
                    int& moveOnBoard = board[(int)std::ceil(move / 3) - 1][(int)(move - 3 * std::ceil(move / 3)) + 2];
                    if (moveOnBoard == 0) {
                        moveOnBoard = 1;
                        boardValue = evaluate(board);
                        playerTurn = false;
                    }
                }
            } 
            else {
                Move AImove = findBestMove(board, true);
                boardValue = AImove.moveVal;
                board[AImove.row][AImove.col] = 1;
                playerTurn = false;
            }
        }
        else {
            if (players == 1 || players == 0) {
                Move AImove = findBestMove(board, false);
                boardValue = AImove.moveVal;
                board[AImove.row][AImove.col] = -1;
                playerTurn = true;
            }
            else {
                double move = 0;
                std::cout << "[move (1-9)]: ";
                std::cin >> move;
                if ((int)move < 10 && (int)move > 0) {
                    int& moveOnBoard = board[(int)std::ceil(move / 3) - 1][(int)(move - 3 * std::ceil(move / 3)) + 2];
                    if (moveOnBoard == 0) {
                        moveOnBoard = -1;
                        boardValue = evaluate(board);
                        playerTurn = true;
                    }
                }
            }
        }
    }
}

int main() {
	int board[3][3] = {
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};
    int players = -1;
    
    while (players < 0 || players > 2) {
        std::cout << "Number of human players: ";
        std::cin >> players;
        system("CLS");
    }

     gameLoop(board, players);

	return 0;
}
