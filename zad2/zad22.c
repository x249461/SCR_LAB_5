#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {

    
    int pipefds[2], in_fd, n; //pipefds[2]->przechowywanie dwoch koncow pipe'a
    char buf[BUFFER_SIZE];
    char fname[32];

    //sprawdzenie argumentow
    if(argc < 2){
      fprintf(stderr, "Wprowadz: nazwaProg <nazwaPliku>");
      return 1;
    }


    //tworzenie pipeow
    if(pipe(pipefds) == -1) {

        fprintf(stderr, "Failed creating pipe\n");
        return 1;
    }

    pid_t pid = fork();

    if(pid == 0) {
        //potomek
      close(pipefds[1]); //zamykanie potoku write
        close(0);
        dup(pipefds[0]);//duplikacja potoku
        close(pipefds[0]);

	
        //Wykorzystuje funkcje 'display' progarmu ImageMagick
	// Parametr '-' sluzy do pobrania danych odczytanych przez potok
        execlp("display", "display", "-", NULL);
    }
    else if (pid > 0) {
        //rodzic
        close(pipefds[0]);
        //printf("Please input your image path along with its extension (e.g. image.jpg | ~/Desktop/image.jpg):\n");
        //scanf("%s", fname);

        if((in_fd = open(argv[1], O_RDONLY)) < 0) {
            
            fprintf(stderr, "Failed opening file\n");
            return 2;
        }
        
        while((n = read(in_fd, &buf, BUFFER_SIZE)) > 0) {
    
            if(write(pipefds[1], &buf, n) < 0) {
                fprintf(stderr, "Failed writing to pipe\n");
                return 3;
            }  
        }
	//sleep(10);
        close(in_fd);
    } 
}
