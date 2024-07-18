#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../../gui/cli/tetris_interface.h"

#define HIGHSCORE_FILE "record.txt"
#define WIDTH 10
#define HEIGHT 20
#define SIDEBAR_WIDTH 20
#define NUM_PIECES 7
#define PIECE_SIZE 4

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  bool game_over;
} GameInfo_t;

typedef struct {
  int x, y;
  int rotation;
  int type;
} Piece;

typedef struct {
  int type;
  int rotation;
} NextPiece_t;

extern GameInfo_t game;
extern Piece current_piece;
extern NextPiece_t next_piece;

// Function prototypes
void initGame();
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void cleanup();
void newPiece();
bool checkCollision(int x, int y, int rotation);
void placePiece();
void clearLines();
void rotatePiece();
void movePiece(int dx, int dy);
void dropPiece();
void spawnNewPiece();
void loadHighScore();
void saveHighScore();

// Tetromino shapes
extern const int tetrominos[NUM_PIECES][4][4][4];
