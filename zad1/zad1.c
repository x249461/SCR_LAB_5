#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 5

int main() {
  
  pid_t pid;
  
  int pipefds[2], n, in_fd;
  char buf[BUFFER_SIZE];
  char filename[32];

  //tworzenie pipe'ow:
  if( pipe(pipefds) < 0 ){

    fprintf(stderr, "Failed creating pipe\n");
    return 1;
      
  }
  
  pid = fork(); //utworzenie procesu potomnego

  if(pid == 0){

    close(pipefds[1]); //zamyka potok write

      while (( n = read(pipefds[0], buf, sizeof(buf) - 1)) > 0){
	buf[n] = '\0';
	printf("#%s#", buf);

    } 
    close(pipefds[0]); 
  }

  else if (pid > 0){

    close(pipefds[0]);
    printf("Input full file name:\n");
    scanf("%s", filename);

    if((in_fd = open(filename, O_RDONLY)) < 0){

      fprintf(stderr, "Failed opening file\n");
      return 2;
      
    }
    
    while((n = read(in_fd, &buf, BUFFER_SIZE)) > 0){
      
      if(write(pipefds[1], &buf, n) < 0){
	fprintf(stderr, "Failed writing to pipe\n");
	return 3;
      }
      
    }
    close(in_fd);
  }

  
}
