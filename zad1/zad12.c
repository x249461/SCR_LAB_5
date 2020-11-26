#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[]) {
  
  pid_t pid;
  
  int pipefds[2], n, in_fd;
  char buf[BUFFER_SIZE];
  //char filename[32];

  //sprawdzenie argumentow 
  if(argc < 2){
    fprintf(stderr, "BLAD! Wprowadz: nazwaProg <nazwaPliku>\n");
    return 1;;
  }

  
  //tworzenie pipe'ow:
  if( pipe(pipefds) == -1 ){

    fprintf(stderr, "Bledne utworzenie potoku\n");
    return 1;
      
  }
  
  pid = fork(); //utworzenie procesu potomnego

  if(pid == 0){

    close(pipefds[1]); //zamyka potok write

    while (( n = read(pipefds[0], buf, sizeof(buf)-1 )) > 0){
	buf[n] = '\0';
	printf("#%s#", buf);
	/*	write(STDOUT_FILENO, "#", 1);
	write(STDOUT_FILENO, &buf, n);
	write(STDOUT_FILENO, "#", 1);	*/
    } 
      close(pipefds[0]); //zamyka potok read
  }

  else if (pid > 0){
    //rodzic
    close(pipefds[0]); //zamyka potok read 
    //printf("Input full file name:\n");
    //scanf("%s", filename);

    
    if((in_fd = open(argv[1], O_RDONLY)) < 0){

      fprintf(stderr, "Bledne otwarcie pliku\n");
      return 2;
      
    }
    
    while((n = read(in_fd, &buf, BUFFER_SIZE)) > 0){
      
      if(write(pipefds[1], &buf, n) < 0){
	fprintf(stderr, "Bledne pisanie do potoku\n");
	return 3;
      }
      
    }
    close(in_fd);
  }

  
}
