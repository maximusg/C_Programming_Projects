// ------------------------------------------------------
// File: mycopy.c
//
// Name: Max Geiszler
//
// Description: This program will take any text file, and 
// create a new file named by the second parameter, as
// long as that file does not exist. It then copies the
// first file into the second file capitalizing every lowercase
// char. It also spits to STDOUT metrics on number of chars coppied, 
//number lines, number of chars changed, and number of punctuation chars.
//
// Syntax:
// The first argument needs to be a file containing text. 
//The second argument needs to be a non-existent valid file name.
// ------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>



#define BUFFER 32
#define VALID_ARGS 3
#define SUCCESS 0


int main(int argc, char * argv[]) {

        errno = SUCCESS;
        FILE * rd = NULL;
        FILE * rdcheck = NULL; //used to check if write file does not yet exist
        FILE * wr = NULL;
        char buff[BUFFER];
        size_t numbbytes = 0;

        int i = 0;
        int charsCopied = 0;
        int charsChanged = 0;
        int linesInFile = 0;
        int charPunk = 0;
        char c = '\0';

        if (argc != VALID_ARGS) {
                fprintf(stderr, "Must have %d argument(s)\n", VALID_ARGS - 1);
                return EXIT_FAILURE;
        }

        printf("Will try to open '%s' read-only\n", argv[1]);
        rd = fopen(argv[1], "r");
        printf("rd: %i",rd);
        if (errno || rd == NULL) {
                perror("Unexpected error opening read file");
                fclose(rd);
                rd = NULL;
                exit(EXIT_FAILURE);
        }

        //check to see if write file exits
        rdcheck = fopen(argv[2], "r");
        if (errno || rdcheck == NULL) {
                errno = SUCCESS; //reset errno as this is expect behaviour
        } else {
                fprintf(stderr, "file \"%s\" already exists, must write to new filename\n", argv[2]);
                fclose(rd);
                fclose(rdcheck);
                rd = NULL;
                rdcheck = NULL;
                exit(EXIT_FAILURE);
        }

        printf("Will try open file '%s' read/write\n", argv[2]);
        wr = fopen(argv[2], "w");
        if (errno || wr == NULL) {
                fclose(rd);
                fclose(rdcheck);
                fclose(wr);
                rd = NULL;
                rdcheck = NULL;
                wr = NULL;
                perror("Unexpected error opening write file");
                exit(EXIT_FAILURE);
        }

        
     

        while (!feof(rd)) {
                numbbytes = fread(buff, sizeof(char), BUFFER, rd);
                if(numbbytes>0){
                
                        buff[numbbytes] = '\0'; 

                        for (i = 0; i < numbbytes; i++) {
                                c = buff[i]; //lookup char once

                                charsCopied++;
                                if (islower(c)) {
                                        buff[i] = toupper(c);
                                        charsChanged++;
                                }
                                if (ispunct(c)) {
                                        charPunk++;
                                }
                                if (c == '\n') {
                                        linesInFile++;
                                }

                        }
                        fwrite(buff, sizeof(char), numbbytes, wr); //write buff to file
                        if(ferror(wr)){
                                fprintf(stderr, "error condition writing file \n");
                                break;
                        }
                }else if(ferror(rd)){
                        fprintf(stderr, "error condition reading file\n");
                        break;
                }
        }
        printf("Number of characters copied = %d \n", charsCopied);
        printf("Number of characters changed =  %d \n", charsChanged);
        printf("Number of lines in the file =  %d \n", linesInFile);
        printf("Number of punctuation chars=  %d \n", charPunk);
        if (rd != NULL) {
                fclose(rd);
                rd = NULL;
        }
        if (rdcheck != NULL) {
                fclose(rdcheck);
                rdcheck = NULL;
        }
        if (wr != NULL) {
                fclose(wr);
                wr = NULL;
        }

        return EXIT_SUCCESS;
}
