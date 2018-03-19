#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <set>

using namespace std;

// Function prototypes
void playGame(void);

int roll(bool isPlayerTurn);

char getActionFromUser(void);

void printDisplay(int playerScore, int playerTurnPoints, int devilScore);

int devilTurn(int playerScore, int devilScore);

void updateWinHistory(bool playerWon);

/**
 * Main entry point for the Devil's Dice game
 * @return 0 on successful execution
 */
int main() {
#ifdef JARVIS
  srand(0);
#else
  srand(time(NULL));
#endif

  playGame();

  return 0;
}

/**
 * Plays a game of Devil's Dice
 */
void playGame(void) {
  int devilTotalScore = 0;
  int playerTotalScore = 0;
  int playerCurrentTurnPoints = 0;
  bool devilGetsATurn = false;
  bool didPlayerWin = false;
  bool isGameOver = false;

  cout << "---- Welcome to Devil's Dice! ----" << endl;

  while (!isGameOver) {
    char action = getActionFromUser();
    switch (action) {
      case 'r':
        int diceRoll;
        diceRoll = roll(true);
        if (diceRoll == 1) {
          playerCurrentTurnPoints = 0;
          devilGetsATurn = true;
        } else {
          playerCurrentTurnPoints = playerCurrentTurnPoints + diceRoll;
        }
        break;
      case 'h':
        playerTotalScore = playerTotalScore + playerCurrentTurnPoints;
        cout << "You banked " << playerCurrentTurnPoints << " points" << endl;
        devilGetsATurn = true;
        playerCurrentTurnPoints = 0;
        break;
      case 'f':
        cout << "Game Over!" << endl;
        isGameOver = true;
        break;
      default:
        break;
    }
    if (playerTotalScore >= 100){
      cout << "You Win!" << endl;
      devilGetsATurn = false;
      isGameOver = true;
      didPlayerWin = true;
    }


    if (devilGetsATurn) {
      devilTotalScore += devilTurn(playerTotalScore, devilTotalScore);
      devilGetsATurn = false;
    }

    if (!isGameOver) {
      printDisplay(playerTotalScore, playerCurrentTurnPoints, devilTotalScore);
    }

    if (devilTotalScore >= 100) {
      cout << "Devil Wins!" << endl;
      isGameOver = true;
    }
  }

  updateWinHistory(didPlayerWin);
}

/**
 * Rolls a dice, returning a random value between 1 and 6
 * @param isPlayerTurn True if it is the player rolling, false otherwise
 * @return Random dice roll
 */
int roll(bool isPlayerTurn) {
  int roll = 0;
  roll = rand() % 6 + 1;
  if (isPlayerTurn == true) {
    cout << "You rolled a " << roll << endl;
  } else if (isPlayerTurn == false) {
    cout << "Devil rolled a " << roll << endl;
  }
  return roll;
}

/**
 * Prompts for and gets an action from the user
 * @return The action selected by the user
 */
char getActionFromUser(void) {
  char input;
  cout << "Hold[h], roll[r], or forfeit[f]: ";
  cin >> input;

  return input;
}

/**
 * Prints out the Devil's Dice display
 * @param playerScore Current total player score
 * @param playerTurnPoints Points player has accumulated so far this turn
 * @param devilScore Current total devil's score
 */
void printDisplay(int playerScore, int playerTurnPoints, int devilScore) {
  cout << right << setw(3) << "   " << setw(2) << "  " << left << setw(6) << "Player" << setw(2) << "  " << setw(3) << "   " << right << setw(15) << "Devil" << left << setw(5) << "     " << endl;
  cout << right << setw(3) << "   " << setw(2) << "  " << left << setw(6) << "------" << setw(2) << "  " << setw(3) << "   " << right << setw(15) << "-----" << left << setw(5) << "     " << endl;
  for (int count = 100; count >= 0; count = count - 10) {
    if (playerScore >= count && playerScore < count + 10) {
      cout << right << setw(3) << playerScore << setw(2) << " >";
    } else {
      cout << right << setw(3) << "   " << setw(2) << "  ";
    }
    cout << left << setw(3) << "--" << left << setw(3) << count;

    if ((playerScore + playerTurnPoints) >= count && (playerScore + playerTurnPoints) < count + 10) {
      cout << left << setw(2) << "< " << setw(3) << (playerScore + playerTurnPoints);
    } else {
      cout << left << setw(2) << "  " << setw(3) << "   ";
    }
    cout << right << setw(12) << count << right << setw(3) << "--";
    if (devilScore >= count && devilScore < count + 10) {
      cout << left << setw(2) << "< " << setw(3) << devilScore << endl;
    } else {
      cout << left << setw(2) << "  " << setw(3) << "   " << endl;
    }
  }
  cout << endl;

}

/**
 * Updates the win/loss history stored in a file
 * @param playerWon True if the player won the game, false otherwise
 */
void updateWinHistory(bool playerWon) {
  ifstream inputFile;
  inputFile.open("games.txt");
  int wins, losses;
  if (!inputFile.is_open()) {
    cout << "something went wrong" << endl;
    return;
  } else {
    inputFile >> wins >> losses;
    inputFile.close();
  }
  if (playerWon) {
    wins = wins + 1;
  } else if (!playerWon) {
    losses = losses + 1;
  }
  ofstream outputFile;
  outputFile.open("games.txt");
  outputFile << wins << " " << losses << endl;
  outputFile.close();
  cout << "Total Wins: " << wins << endl;
  cout << "Total Losses: " << losses << endl;
}

/**
 * The devil take's his turn
 * @param playerScore Current total player score
 * @param devilScore Current total devil score
 * @return The devil's new total score after his turn has completed
 */
int devilTurn(int playerScore, int devilScore) {
  int devilTurnScore = 0;
  bool devilTurnOver = false;
  int devilGoal = 21;

  if (playerScore > devilScore) {
    devilGoal = 30;
  }

  cout << endl;

  while (!devilTurnOver) {
    if ((devilTurnScore < devilGoal) && (devilScore + devilTurnScore < 100)) {
      int devilRoll = roll(false);

      if (devilRoll == 1) {
        devilTurnScore = 0;
        devilTurnOver = true;
      } else {
        devilTurnScore += devilRoll;
      }
    } else {
      devilTurnOver = true;
    }
  }

  cout << "Devil got " << devilTurnScore << " points" << endl << endl;
  return devilTurnScore;
}