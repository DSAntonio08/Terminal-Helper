#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // provides terminal interface for POSIX
#include <unistd.h>  // for flags

// return type:
// 1 -> User Error
// 2 -> File Error
// 3 -> Unexpected Error

// Maximum searchable ASCII Characters
#define BUFFER_SIZE 1024
#define PATH_SIZE 1024
#define MATCH_LIMIT 128

int num_of_matches = MATCH_LIMIT;
int current_found = 0;
bool search = false;
int wantsHelp = true;
bool isRawModeenabled = false;
char *arrayofMatches[MATCH_LIMIT];

struct termios original;

int history_remade(int argumentCounter, char *argumentVector[]);
int searchCommand(int argc, char *argv[], int index);
void usageHelp(bool behelpful);
char *storeMatch(char *buffer);
// need to switch to raw mode to "read keys"
void enableRawMode();
void disableRawMode();
int readInput();
void displaySelected(int selected, bool firstDraw);

///@param argc -> argument counter (how many arguments)
///@param argv -> argument vector (what we are searching for)
int main(int argc, char *argv[]) {
  history_remade(argc, argv);
  enableRawMode();
  readInput();
  disableRawMode();

  for (int i = 0; i < current_found; i++) {
    free(arrayofMatches[i]);
  }

  return 0;
}

int history_remade(int argumentCounter, char *argumentVector[]) {
  int opt;

  while ((opt = getopt(argumentCounter, argumentVector, ":sm:h")) != -1) {
    switch (opt) {
    case 's':
      search = true;
      wantsHelp = false;
      break;
    case 'm':
      // atoi converts ascii to int
      // optarg is the argument/number that gets addet with the m Flag
      num_of_matches = atoi(optarg);

      // for unreasonable stuff - default
      if (num_of_matches > MATCH_LIMIT || num_of_matches < 1) {
        num_of_matches = MATCH_LIMIT;
      }

      wantsHelp = false;

      // so that the s flag is Optional
      search = true;
      break;
    case 'h':
      wantsHelp = true;
      break;
    case ':':
      fprintf(stderr, "Error: -m Needs a Number \n");
      fprintf(stderr, "Use the following way: -m 10 OR -sm10\n");

      return 1;
      break;
    default:
      wantsHelp = true;
      break;
    }
  }

  usageHelp(wantsHelp);
  if (search) {
    searchCommand(argumentCounter, argumentVector, optind);
  }
  return 0;
}

int searchCommand(int argc, char *argv[], int index) {
  // Check if we have a keyword at the index getopt left us
  if (index >= argc) {
    fprintf(stderr, "No search term provided.\n");
    return 1;
  }

  char *keyword = argv[index];
  char *home = getenv("HOME");
  if (!home)
    return 3;

  char path[PATH_SIZE];
  snprintf(path, PATH_SIZE, "%s/.bash_history", home);

  FILE *file_pointer = fopen(path, "r");
  if (!file_pointer) {
    fprintf(stderr, "Error: File not found.\n");
    return 2;
  }

  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, file_pointer)) {
    if (strstr(buffer, keyword)) {
      arrayofMatches[current_found] = storeMatch(buffer);
      current_found++;
      if (current_found >= num_of_matches)
        break;
    }
  }

  // C has no garbage collector
  fclose(file_pointer);
}

void usageHelp(bool behelpful) {
  if (behelpful) {
    printf("\n"
           "Usage: terminal-helper [FLAGS] <keyword>\n"
           "\n"
           "A lightweight utility to search and execute bash history.\n"
           "\n"
           "Flags:\n"
           "  -s             Search mode (default if keyword is provided).\n"
           "  -m <number>    Limit results to <number> of matches (Default: "
           "128).\n"
           "  -h             Display this help menu.\n"
           "\n"
           "Examples:\n"
           "  ./terminal-helper -s nixos\n"
           "  ./terminal-helper -m 5 gcc\n"
           "  ./terminal-helper -sm 10 ls\n"
           "\n");
  }
}

char *storeMatch(char *buffer) {
  // +1 for hidden "end of string" character
  int sizeofcommand = (strlen(buffer) + 1);
  char *foundCommand = malloc(sizeofcommand);
  if (foundCommand == NULL)
    return NULL;
  // stringcopy(destination, source)
  strcpy(foundCommand, buffer);
  return foundCommand;
}

void enableRawMode() {
  struct termios orignal_copy;
  // get original config first
  // 0 = stdin
  tcgetattr(0, &original);
  orignal_copy = original;
  // turn of these local flags
  orignal_copy.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &orignal_copy);
  isRawModeenabled = true;
}

void disableRawMode() {
  tcsetattr(0, TCSANOW, &original);
  isRawModeenabled = false;
}

int readInput() {
  /*
   q = quit
   n = enter
   Up arrow    → \x1b  [  A
   Down arrow  → \x1b  [  B
  */

  if (isRawModeenabled) {
    char character;
    int selected = 0;
    displaySelected(selected, true);

    while (true) {
      // 0 = stdin
      // variable
      // 1 = byte size
      read(0, &character, 1);
      if (character == '\x1b') {
        // Arrow Key (maybe)
        char sequence[2];
        read(0, &sequence[0], 1); // should be [
        read(0, &sequence[1], 1);

        if (sequence[0] == '[') {
          switch (sequence[1]) {
          case 'A':
            // go up
            if (selected > 0) {
              selected--;
            }
            break;
          case 'B':
            // go down
            if (selected < current_found - 1) {
              selected++;
            }
            break;
          }
        }
      } else if (character == '\n') {
        // Enter was pressed
        printf("%s", arrayofMatches[selected]);
        // then after executing/putting into terminal - quit
        break;
        break;
      } else if (character == 'q') {
        // quit
        break;
      }
      displaySelected(selected, false);
    }
  } else {
    fprintf(stderr, "Error: Terminal not in Raw Mode.\n");
    return 3;
  }
}

void displaySelected(int selected, bool firstDraw) {
  if (!firstDraw) {
    // move cursor to the top
    for (int i = 0; i < current_found; i++) {
      printf("\033[A\r"); // up one line, back to start of line
    }
  }
  for (int i = 0; i < current_found; i++) {
    if (i == selected)
      printf("> %s", arrayofMatches[i]); // highlighted
    else
      printf("  %s", arrayofMatches[i]); // normal
  }
}
