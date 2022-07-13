#include<iostream>
#include<stdlib.h>
#include<math.h> 
#include<algorithm> 

// Move structure
// Used to pack the row, the column and the move value together
struct Move {
    int row, col, moveVal;
    Move() : row(-1), col(-1), moveVal(-1000) {}
};

// Checks if there are any free spaces left on the board
bool isMovesLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == 0)
                return true;
    return false;
}

// Evaluates the current board state
// Returns player type * 10 (-10 for O and 10 for X) if someone won
// Else returns 0
int evaluate(int b[3][3]) {
    // One of the players won horizontally
    for (short row = 0; row < 3; row++)
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2])
            if (b[row][0] != 0)
                return b[row][0] * 10;

    // One of the players won vertically
    for (short col = 0; col < 3; col++)
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col])
            if (b[0][col] != 0)
                return b[0][col] * 10;

    // One of the players won diagonally
    if ((b[0][2] == b[1][1] && b[1][1] == b[2][0]) || (b[0][0] == b[1][1] && b[1][1] == b[2][2]))
        if (b[1][1] != 0)
            return b[1][1] * 10;

    // Nobody won
    return 0;
}

// Goes through all the cells, makes a move, checks the results
// With condition passed as a function
template <typename Proc>
int traverseCells(int board[3][3], bool isMax, Proc check) {
    int best = -1000;
    int move = 1; // Move type of the board matrix (1 == X, -1 == O)
    if (!isMax) {
        best = 1000;
        move = -1;
    }

    // Iterate through all the cells
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            // Check if the cell is empty
            if (board[i][j] == 0) {
                // Make the move
                board[i][j] = move;

                // Check results of the move using the passed function
                best = check(best);

                // Undo the move
                board[i][j] = 0;
            }

    return best;
}

// Minimax algorythm
int minimax(int board[3][3], int depth, bool isMax) {
    int score = evaluate(board);

    // If one of the players has won the game
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
            return std::max(best, minimax(board, depth + 1, false)); // Chose the max from the current best and the new move
            });
    // Minimizer's score
    return traverseCells(board, isMax, [board, depth, isMax](int best) {
        return std::min(best, minimax(board, depth + 1, true)); // Chose the min from the current best and the new move
        });
}


// Returns the best possible move
Move findBestMove(int board[3][3], bool& isMax) {
    Move bestMove; // The best move
    bestMove.row = -1;
    bestMove.col = -1;
    int move = 1;   // Move type of the board matrix (1 == X, -1 == O)
    if (!isMax) {
        bestMove.moveVal = 1000;
        move = -1;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            // Check if the cell is empty
            if (board[i][j] == 0) {
                // Make a move
                board[i][j] = move;

                // Compute evaluation function for the move
                int moveVal = minimax(board, 0, !isMax);

                // Undo the move
                board[i][j] = 0;

                // Check if new move is better than the previous best move
                if ((isMax && moveVal > bestMove.moveVal) || (!isMax && moveVal < bestMove.moveVal)) {
                    // Update best move
                    bestMove.row = i;
                    bestMove.col = j;
                    bestMove.moveVal = moveVal;
                }
            }

    // Set the turn to the other player
    isMax = !isMax;
    return bestMove;
}

// Draws Tic Tac Toe board from the board matrix
void drawBoard(int board[3][3]) {
    // Clear screen
    system("CLS");
    std::cout << "-------------\n";
    // For each row
    for (int i = 0; i < 3; i++) {
        std::cout << "|";
        // For each column
        for (int j = 0; j < 3; j++) {
            // Draw X for 1, O for -1 and empty space for 0
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

// Gets move from player and returns it as a Move
Move getPlayerMove(int board[3][3], bool& playerTurn) {
    Move playerMove;
    double move = 0;  // Move on the board
    // 1  2  3
    // 4  5  6
    // 7  8  9
    std::cout << "[move (1-9)]: ";
    std::cin >> move;

    // Make sure the move is within the range 1-9
    if ((int)move < 10 && (int)move > 0) {
        // Convert the move number into the row and the column of the move
        int row = std::ceil(move / 3) - 1;
        int col = move - 3 * std::ceil(move / 3) + 2;
        // If the space is empty
        if (board[row][col] == 0) {
            // Set player move
            playerMove.row = row;
            playerMove.col = col;
            // Evaluate the board to see if anyone won
            playerMove.moveVal = evaluate(board);
            // Set the turn to the other player
            playerTurn = !playerTurn;
        }
    }

    return playerMove;
}

// Main game loop
void gameLoop(int board[3][3], int players) {
    bool gameOver = false;
    bool playerTurn = true;
    int boardValue = 0;
    while (!gameOver) {
        drawBoard(board);

        // Check the game end conditions (one of the players won/draw)
        // And end the game if any of them happened
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

        Move move;
        // Player 1
        if (playerTurn) {
            // AI
            if (players == 0) {
                // Get the move, set the move, set the new board value
                move = findBestMove(board, playerTurn);
                board[move.row][move.col] = 1;
                boardValue = move.moveVal;
            } 
            // Human
            else {
                // Get the move, set the move, set the new board value
                move = getPlayerMove(board, playerTurn);
                board[move.row][move.col] = 1;
                boardValue = move.moveVal;
            }
        }
        // Player 2
        else {
            // Human
            if (players == 2) {
                // Get the move, set the move, set the new board value
                Move move = getPlayerMove(board, playerTurn);
                board[move.row][move.col] = -1;
                boardValue = move.moveVal;
            }
            // AI
            else {
                // Get the move, set the move, set the new board value
                Move move = findBestMove(board, playerTurn);
                board[move.row][move.col] = -1;
                boardValue = move.moveVal;
            }
        }
    }
}

int main() {
    // Board matrix (0 == empty, 1 == X, -1 == O)
	int board[3][3] = {
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};
    // Number of players (0-2)
    int players = -1;
    
    // Get the number of players and make sure it's in the range 0-2
    while (players < 0 || players > 2) {
        std::cout << "Number of human players: ";
        std::cin >> players;
        system("CLS");
    }

    // Main game loop
    gameLoop(board, players);

	return 0;
}
