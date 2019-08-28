#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>//for same_file function
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>



#define BUFFER 32
#define VALID_ARGS 3

//borrowed from https://stackoverflow.com/questions/12502552/can-i-check-if-two-file-or-file-descriptor-numbers-refer-to-the-same-file
int same_file(int fd1, int fd2) {
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

int main(int argc, char *argv[])
{
        
        errno = 0;
        int rd = -1;
        int wr = -1;
        char buff[BUFFER];
        int numbbytes =0;
        int numbbytesWr =0;
        int i =0;
        int charsCopied =0;
        int charsChanged =0;
        int linesInFile =0;
        int charPunk=0;
        char c = '\0';
        int samefile;

        if (argc != VALID_ARGS) {
                printf("Must have %d argument(s)\n", VALID_ARGS - 1);
                return EXIT_FAILURE;
        }

        printf("Will try to open '%s' read-only\n", argv[1]);
        rd = open(argv[1], O_RDONLY);
        if (errno || rd==-1) {
                perror("Unexpected error opening read file");
                exit(EXIT_FAILURE);
        }
        printf("Will try open file '%s' read/write\n", argv[2]);
        wr = open(argv[2], O_CREAT);
        if (errno || wr==-1) {
                perror("Unexpected error opening write file");
                exit(EXIT_FAILURE);
        }
        
        
        //borrowed from https://stackoverflow.com/questions/12502552/can-i-check-if-two-file-or-file-descriptor-numbers-refer-to-the-same-file
        samefile=same_file(rd,wr);
        if (samefile ==1){
                perror("Writing to the same file is not allowed");
                exit(EXIT_FAILURE);
        }
        printf("samefile: %d \n",samefile);



        numbbytes = read(rd,buff,BUFFER);
       
         while (numbbytes){
                buff[numbbytes]='\0';//how does this not overwrite the last char?
               
                
                for (i=0;i<numbbytes;i++){
                        c = buff[i];//lookup char once
                        
                        charsCopied++;
                        if (islower(c)){
                                buff[i]=toupper(c);
                                charsChanged++;
                        }
                        if(ispunct(c)){
                                charPunk++;
                        }
                        if(c=='\n'){
                                linesInFile++;
                        }

                        
                }
                numbbytesWr=write(wr,buff,numbbytes);//write buff to file
                numbbytes = read(rd,buff,BUFFER);//read new bytes into buff
         }
        printf("Number of characters copied = %d \n",charsCopied);
        printf("Number of characters changed =  %d \n", charsChanged);
        printf("Number of lines in the file =  %d \n", linesInFile);
        printf("Number of punctuation chars=  %d \n", charPunk);
        printf("Success: descriptor = %d\n", rd);
        close(rd); //OS tries to clean up the issues if this is still open.
        close(wr);

        return EXIT_SUCCESS;
}

