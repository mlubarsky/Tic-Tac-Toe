#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ROWS 3
#define COLUMNS 3
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

//function that checks whether X or O has won, or neither
char who_won(char board[3][3]){
	//check rows
	for (int row = 0; row < 3; row++) {
		if (board[row][0] == board[row][1] && board[row][1] == board[row][2]
		&& (board[row][0] == 'X' || board[row][0] == 'O')) {
			return board[row][0];
		}
	}
	
	//check columns
	for (int col = 0; col < 3; col++) {
		if (board[0][col] == board[1][col] && board[1][col] == board[2][col]
		&& (board[0][col] == 'X' || board[0][col] == 'O')) {
			return board[0][col];
		}
	}

	//check diagonals
	if ((board[0][0] == board[1][1] &&
		 board[1][1] == board[2][2] &&
		(board[0][0] == 'X' || board[0][0] == 'O')) ||
		(board[0][2] == board[1][1] &&
	     board[1][1] == board[2][0] &&
		(board[0][2] == 'X' || board[0][2] == 'O'))) {
			return board[1][1];
		} else {
			return '_';
		}
}

//check if a space on the board is empty
bool empty(char b[ROWS][COLUMNS], int r, int c){
	return b[r][c] == '_';
}

//check for occupation of all board spaces
bool full(char b[ROWS][COLUMNS]){
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			if (empty(b, r ,c)) {
				return false;
			}
		}
	}
	return true;
}

//function that finds and returns the best score using minimax algorithm
int minimax(char b[ROWS][COLUMNS], char player, int depth){
	//initialize a score variable, another player, and a winner
	int score;
	char other_player = player == 'X' ? 'O' : 'X';
	char winner = who_won(b);

	//base cases that return a score
	if (winner == 'X') {
		return score = 10 - depth;
	} else if (winner == 'O') {
		return score = -10 + depth;
	} else if (winner == '_' && full(b)) {
		return score = 0;
	}

	//recursive case
	if (player == 'X') {
		score = -100;
		for (int row = 0; row < ROWS; row++) {
			for (int column = 0; column < COLUMNS; column++) {
				if (empty(b, row, column)) {
					//put X in that space, call minimax
					b[row][column] = player;
					score = MAX(score, minimax(b, other_player, depth + 1));
					b[row][column] = '_'; //undo the move
				}
			}
		}
	} else {
		score = 100;
		for (int row = 0; row < ROWS; row ++) {
			for (int column = 0; column < COLUMNS; column++) {
				if (empty(b, row, column)) {
					//put O in that space, call minimax
					b[row][column] = player;
					score = MIN(score, minimax(b, other_player, depth + 1));
					b[row][column] = '_'; //undo the move
				}
			}
		}
	}
	return score;
}

//function that finds the best row and column by using the score from minimax
void find_best_move(char b[ROWS][COLUMNS], char player, int *r, int *c){
	//initialize another player and best score variable to store the highest score
	char other_player = player == 'X' ? 'O' : 'X';
	int best_score = player == 'X' ? - 100 : 100;
	
	for (int row = 0; row < ROWS; row++) {
		for (int column = 0; column < COLUMNS; column++) {
			//if the board is not empty we continue, otherwise dont
			if (!empty(b, row, column)) {
				continue;
			}
			//put the player into the space, call minimax, undo move
			b[row][column] = player;
			int score = minimax(b, other_player, 0);
			b[row][column] = '_';
			//compare scores
			if (player == 'X') {
				if (score > best_score) {
					best_score = score;
					*r = row;
					*c = column;
				}
			} else {
				if (score < best_score) {
					best_score = score;
					*r = row;
					*c = column;
				}
			}
		}
	}
}

//function to check if X or O won, or if its a draw
bool terminal_state(char b[ROWS][COLUMNS]){
	char winner = who_won(b);
	//return true if the winner is either X or O, or if the board is full
	return winner == 'X' || winner == 'O' || full(b);
}

//function to check if the move is valid
bool legal(char b[ROWS][COLUMNS], int r, int c){
	//return true if the row and column are within bounds 0 and 2, and if the space is empty
	return 0 <= r && r < ROWS && 0 <= c && c < COLUMNS && empty(b, r, c);
}

//function that prints the current state of the board
void print_board(char b[ROWS][COLUMNS]){
	//loop through rows and columns
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLUMNS; col++) {
			printf(" %c ", b[row][col]);
			if (col < 2) {
				printf("|");
			}
		}
		if (row < 2) {
			printf("\n---+---+---");
		}
	printf("\n");
	}
}

int main(int argc, char **argv){
	int r, c;
	//initialize board to be underscores signaling that its empty
	char b[ROWS][COLUMNS] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
	if (argc == 1) {
		//forever scan input, will break out of the loop if the board reaches a terminal state
		while ("true") {
			print_board(b);
			printf("X: ");
			fscanf(stdin, "%d %d", &r, &c);
			if (legal(b, r, c)) {
				b[r][c] = 'X';
				find_best_move(b, 'O', &r, &c);
				b[r][c] = 'O';
				printf("O moves to: %d %d\n", r, c);
				//O wins
				if (terminal_state(b) && who_won(b) == 'O') {
					print_board(b);
					printf("O wins\n");
					break;
				//X wins
				} else if (terminal_state(b) && who_won(b) == 'X') {
					print_board(b);
					printf("X wins\n");
					break;
				//draw
				} else if (terminal_state(b) && who_won(b) == '_') {
					print_board(b);
					printf("draw\n");
					break;
				}
			} else {
				printf("illegal move\n");
			}
		}
	} else {
		//loop over rows and columns to print the board
		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLUMNS; col++) {
				//assign an index value for the coordinate
				int index = row * 3 + (col + 1);
				//take in each argument from command line and store it on the board
				b[row][col] = *argv[index];
				printf(" %c ", b[row][col]);
				if (col < 2) {
					printf("|");
				}
			}
			if (row < 2) {
				printf("\n---+---+---");
			}
		printf("\n");
		}
		find_best_move(b, 'O', &r, &c);
		printf("O: %d %d\n", r, c);
	}
	return 0;
}
