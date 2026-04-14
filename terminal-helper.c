#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return type:
// 1 -> User Error
// 2 -> File Error
// 3 -> Unexpected Error

// Maximum searchable ASCII Characters
#define BUFFER_SIZE 1024
#define PATH_SIZE 1024


///@param argc -> argument counter (how many arguments)
///@param argv -> argument vector (what we are searching for) 
int main(int argc, char * argv[])
{
  // if user did not put what to search for
  if( argc < 2)
  {
    printf("No search Term.");
      return 1;
  }

  FILE *file_pointer; 
  char buffer [BUFFER_SIZE];
  char * home = getenv("HOME");
  if(home == NULL)
  {
    printf("Unexpected Error.");
    return 3;
  }

  char path[PATH_SIZE];
  // string numbered printf
  // we use this instead of print, so it does not get put to the stdout (standart output) instead of like here to a memory buffer
  snprintf(path, PATH_SIZE, "%s/.bash_history", home);

  // r means read-only
  file_pointer = fopen(path, "r");
  if(file_pointer == NULL)
  {
    printf("Error: File not found.");
      return 2;
  }

  // reads the file until it hits the end
  while (fgets(buffer, BUFFER_SIZE, file_pointer))
  {
    // argv[1] is the first actual word, argv[0] is the name of my program
    // string string to check if a string is in another string -> apple in pineapple for example
    if (strstr(buffer,argv[1])) {
      printf("Found %s", buffer);
    }
  }

  // C has no garbage collector
  fclose(file_pointer);

  return 0;
}

