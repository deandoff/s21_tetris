#include "tetris.h"

#include "../../gui/cli/tetris_interface.h"

GameInfo_t game;
Piece current_piece;
NextPiece_t next_piece;

const int tetrominos[NUM_PIECES][4][4][4] = {
    // I piece
    {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},

     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},

     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},

     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}},
    // O piece
    {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    // T piece
    {{{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},

     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}}},
    // L piece
    {{{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}},

     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}},

     {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}},
    // J piece
    {{{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},

     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}}},
    // S piece
    {{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},

     {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}}},
    // Z piece
    {{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},

     {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

     {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}}};

void loadHighScore() {
  FILE *file = fopen(HIGHSCORE_FILE, "r");
  if (file) {
    int high_score;
    fscanf(file, "%d", &high_score);
    game.high_score = high_score;
    fclose(file);
  } else {
    game.high_score = 0;
  }
}

void saveHighScore() {
  FILE *file = fopen(HIGHSCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", game.high_score);
    fclose(file);
  }
}

void initGame() {
  game.field = (int **)malloc(HEIGHT * sizeof(int *));
  for (int i = 0; i < HEIGHT; ++i) {
    game.field[i] = (int *)calloc(WIDTH, sizeof(int));
  }

  game.next = (int **)malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; ++i) {
    game.next[i] = (int *)calloc(4, sizeof(int));
  }

  game.score = 0;
  game.high_score = 0;
  game.level = 1;
  game.speed = 1;
  game.pause = 0;
  game.game_over = false;

  loadHighScore();

  srand(time(NULL));

  newPiece();
  spawnNewPiece();
}

void cleanup() {
  for (int i = 0; i < HEIGHT; ++i) {
    free(game.field[i]);
  }
  free(game.field);

  for (int i = 0; i < 4; ++i) {
    free(game.next[i]);
  }
  free(game.next);

  endwin();
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;

  if (game.game_over) return;

  switch (action) {
    case Start:
      break;
    case Pause:
      game.pause = !game.pause;
      break;
    case Terminate:
      cleanup();
      exit(0);
      break;
    case Left:
      movePiece(-1, 0);
      break;
    case Right:
      movePiece(1, 0);
      break;
    case Up:
      /// not used in this game///
      break;
    case Down:
      dropPiece();
      break;
    case Action:
      rotatePiece();
      break;
  }
}

GameInfo_t updateCurrentState() {
  static int tick = 0;
  if (!game.pause && !game.game_over) {
    tick++;
    if (tick >= game.speed) {
      tick = 0;
      movePiece(0, 1);
    }
  }

  game.speed = 10 - game.level;
  if (game.speed < 1) {
    game.speed = 1;
  }

  return game;
}

void newPiece() {
  current_piece.x = WIDTH / 2 - 2;
  current_piece.y = -1;
  current_piece.rotation = 0;
  current_piece.type = next_piece.type;

  next_piece.type = rand() & NUM_PIECES;
  next_piece.rotation = 0;

  if (checkCollision(current_piece.x, current_piece.y,
                     current_piece.rotation)) {
    game.game_over = true;
  }
}

bool checkCollision(int x, int y, int rotation) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetrominos[current_piece.type][rotation][i][j]) {
        int newX = x + j;
        int newY = y + i;
        if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT ||
            game.field[newY][newX]) {
          return true;
        }
      }
    }
  }
  return false;
}

void placePiece() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetrominos[current_piece.type][current_piece.rotation][i][j]) {
        int newY = current_piece.y + i;
        int newX = current_piece.x + j;
        if (newY >= 0 && newY <= HEIGHT && newX >= 0 && newX < WIDTH) {
          game.field[current_piece.y + i][current_piece.x + j] = 1;
        }
        if (newY < 0) {
          game.game_over = true;
        }
      }
    }
  }
}

void clearLines() {
  int count = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    bool full = true;
    for (int j = 0; j < WIDTH; ++j) {
      if (!game.field[i][j]) {
        full = false;
        break;
      }
    }
    if (full) {
      for (int k = i; k > 0; --k) {
        for (int l = 0; l < WIDTH; ++l) {
          game.field[k][l] = game.field[k - 1][l];
        }
      }
      for (int l = 0; l < WIDTH; ++l) {
        game.field[0][l] = 0;
      }
      count++;
    }
  }
  if (count == 1) {
    game.score += 100;
  } else if (count == 2) {
    game.score += 300;
  } else if (count == 3) {
    game.score += 700;
  } else if (count == 4) {
    game.score += 1500;
  }

  int new_level = (game.score / 600) + 1;
  if (new_level > 0 && new_level <= 10) {
    game.level = new_level;
  }

  if (game.score > game.high_score) {
    game.high_score = game.score;
    saveHighScore();
  }
}

void rotatePiece() {
  int newRotation = (current_piece.rotation + 1) % 4;

  if (!checkCollision(current_piece.x, current_piece.y, newRotation)) {
    current_piece.rotation = newRotation;
  } else {
    if (!checkCollision(current_piece.x - 1, current_piece.y, newRotation)) {
      current_piece.x -= 1;
      current_piece.rotation = newRotation;
    } else if (!checkCollision(current_piece.x + 1, current_piece.y,
                               newRotation)) {
      current_piece.x += 1;
      current_piece.rotation = newRotation;
    } else if (!checkCollision(current_piece.x - 2, current_piece.y,
                               newRotation)) {
      current_piece.x -= 2;
      current_piece.rotation = newRotation;
    } else if (!checkCollision(current_piece.x + 2, current_piece.y,
                               newRotation)) {
      current_piece.x += 2;
      current_piece.rotation = newRotation;
    }
  }
}

void movePiece(int dx, int dy) {
  if (!checkCollision(current_piece.x + dx, current_piece.y + dy,
                      current_piece.rotation)) {
    current_piece.x += dx;
    current_piece.y += dy;
  } else if (dy == 1) {
    placePiece();
    clearLines();
    newPiece();
    if (checkCollision(current_piece.x, current_piece.y,
                       current_piece.rotation)) {
      game.game_over = true;
    }
    spawnNewPiece();
  }
}

void dropPiece() {
  while (!checkCollision(current_piece.x, current_piece.y + 1,
                         current_piece.rotation)) {
    current_piece.y++;
  }
  movePiece(0, 1);
}

void spawnNewPiece() {
  next_piece.type = rand() % NUM_PIECES;
  next_piece.rotation = 0;
}