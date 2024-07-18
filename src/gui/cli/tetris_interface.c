#include "tetris_interface.h"

#include "../../brick_game/tetris/tetris.h"

void showStartScreen() {
  initGui();

  clear();

  mvprintw(5, (COLS - 20) / 2, "WELCOME TO TETRIS");
  mvprintw(7, (COLS - 20) / 2, "==================");

  mvprintw(10, (COLS - 30) / 2, "Press 's' to Start Game");
  mvprintw(11, (COLS - 30) / 2, "Press 'q' to Quit");

  refresh();

  int ch;
  while (1) {
    ch = getch();
    if (ch == 's') {
      initGame();
      break;
    } else if (ch == 'q') {
      endwin();
      exit(0);
    }
  }

  clear();
  refresh();
}

void showGameOverScreen(int score) {
  clear();

  mvprintw(5, (COLS - 20) / 2, "GAME OVER");
  mvprintw(7, (COLS - 20) / 2, "==================");

  mvprintw(10, (COLS - 30) / 2, "Final Score: %d", score);
  mvprintw(11, (COLS - 30) / 2, "Press 'r' to Restart the game");
  mvprintw(12, (COLS - 30) / 2, "Press 'q' to Quit");

  refresh();

  int ch;
  while (1) {
    ch = getch();
    if (ch == 'q') {
      endwin();
      exit(0);
    } else if (ch == 'r') {
      game.game_over = false;
      initGame();
      return;
    }
  }

  clear();
  refresh();
}

void initGui() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(100);
}

void drawGame() {
  clear();

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      mvprintw(y, x, game.field[y][x] ? "#" : ".");
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetrominos[current_piece.type][current_piece.rotation][i][j]) {
        mvprintw(current_piece.y + i, current_piece.x + j, "#");
      }
    }
  }

  mvprintw(0, WIDTH + 2, "Score: %d", game.score);
  mvprintw(1, WIDTH + 2, "High Score: %d", game.high_score);
  mvprintw(2, WIDTH + 2, "Level: %d", game.level);
  mvprintw(3, WIDTH + 2, "Pause: %s", game.pause ? "Yes" : "No");

  mvprintw(6, WIDTH + 2, "Next:");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetrominos[next_piece.type][next_piece.rotation][i][j]) {
        mvprintw(7 + i, WIDTH + 2 + j, "#");
      }
    }
  }

  refresh();
}

int main() {
  while (1) {
    showStartScreen();
    timeout(100 * game.speed);
    while (game.game_over == false) {
      int ch = getch();
      switch (ch) {
        case 's':
          userInput(Start, false);
          break;
        case 'p':
          userInput(Pause, false);
          break;
        case 'q':
          userInput(Terminate, false);
          break;
        case KEY_LEFT:
          userInput(Left, false);
          break;
        case KEY_RIGHT:
          userInput(Right, false);
          break;
        case KEY_DOWN:
          userInput(Down, false);
          break;
        case ' ':
          userInput(Action, false);
          break;
        default:
          break;
      }

      updateCurrentState();
      drawGame();
    }
    saveHighScore();
    cleanup();
    showGameOverScreen(game.score);
  }
  return 0;
}