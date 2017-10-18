#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
 
#define BUF_SIZE 8192
 
int main(int argc, char* argv[]) {
 
    int fi, fo;    
    int i = 0;
    ssize_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE];      /* Buffer */
 
    /* Are src and dest file name arguments missing */
    if(argc != 3){
        perror ("Too few arguments\n");
        exit(-1);
    }
 
    fi = open (argv [1], O_RDONLY);
    if (fi == -1) {
        perror ("Can't open file");
        exit(-2);
    }
 
    fo = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if(fo == -1){
        perror ("Can't open file");
        exit(-3);
    }
 
    /* Copy process */
    while((ret_in = read (fi, &buffer, BUF_SIZE)) > 0){
    for (i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = toupper(buffer[i]);
    }
        ret_out = write (fo, &buffer, (ssize_t) ret_in);
        if(ret_out != ret_in){
            perror("Write error");
                exit(-4);
        }
    }
    /* Close files*/
    close (fi);
    close (fo);
 
    return (EXIT_SUCCESS);
}
