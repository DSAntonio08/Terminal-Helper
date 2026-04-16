#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for flags
#include <stdbool.h>

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

int history_remade(int argumentCounter, char *argumentVector[]);
int searchCommand(int argc, char *argv[], int index);
void usageHelp(bool behelpful);

///@param argc -> argument counter (how many arguments)
///@param argv -> argument vector (what we are searching for) 
int main(int argc, char * argv[])
{
  history_remade(argc, argv);
  return 0;
}


int history_remade(int argumentCounter, char *argumentVector[])
{
    int opt;

  while((opt = getopt(argumentCounter,argumentVector, ":sm:h")) != -1)
  {
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
          fprintf(stderr,"Error: -m Needs a Number \n");
          fprintf(stderr,"Use the following way: -m 10 OR -sm10\n");
           
          return 1;
      break;
      default:
        wantsHelp = true;
        break;
    }
  }


  usageHelp(wantsHelp);

  if(search){searchCommand(argumentCounter, argumentVector, optind);}
  
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
    if (!home) return 3;

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
            printf( "%s", buffer);
            current_found++;
            if (current_found >= num_of_matches) break;
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
               "  -m <number>    Limit results to <number> of matches (Default: 128).\n"
               "  -h             Display this help menu.\n"
               "\n"
               "Examples:\n"
               "  ./terminal-helper -s nixos\n"
               "  ./terminal-helper -m 5 gcc\n"
               "  ./terminal-helper -sm 10 ls\n"
               "\n");
    }
}
