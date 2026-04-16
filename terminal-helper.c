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

int searchCommand(int argc, char *argv[], int index);

///@param argc -> argument counter (how many arguments)
///@param argv -> argument vector (what we are searching for) 
int main(int argc, char * argv[])
{
  int opt;

  while((opt = getopt(argc,argv, "sm:")) != -1)
  {
    switch (opt) {
      case 's':
        search = true;
        break;
      case 'm':
        // atoi converts ascii to int
        // optarg is the argument/number that gets addet with the m Flag
        num_of_matches = atoi(optarg); 
        break;
      // case 'h':
      //   // help
      //   break;
    }
  }

  if(search){searchCommand(argc, argv, optind);}
  
  return 0;
}

int searchCommand(int argc, char *argv[], int index) {
    // Check if we have a keyword at the index getopt left us
    if (index >= argc) {
        printf("No search term provided.\n");
        return 1;
    }

    char *keyword = argv[index];
    char *home = getenv("HOME");
    if (!home) return 3;

    char path[PATH_SIZE];
    snprintf(path, PATH_SIZE, "%s/.bash_history", home);

    FILE *file_pointer = fopen(path, "r");
    if (!file_pointer) {
        printf("Error: File not found.\n");
        return 2;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file_pointer)) {
        if (strstr(buffer, keyword)) {
            printf("%s", buffer);
            current_found++;
            if (current_found >= num_of_matches) break;
        }
    }

    // C has no garbage collector
    fclose(file_pointer);
    return 0;
}
