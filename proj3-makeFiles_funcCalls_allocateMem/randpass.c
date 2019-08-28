// ------------------------------------------------------
// File: randpass.c
//
// Name: Max Geiszler
//
// Description: This program when ran will create a easier to remember password
// with an attempt to map 4 of the chars to inside to help obviscate the 
// password more. It will then printf the password to the terminal and then
// write over the allocated memory for the string with x's, then free the
// memory.
//
// Syntax:
// Run the command with an argument as a number >8 <20
// and watch magic ensue!
// ------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>



#define CHAR_RED   "\033[31m"
#define CHAR_RESET "\033[0m"
#define SUCCESS 0
#define VALID_ARGS 2
#define FIRST_ARG 1
#define FIRST_ARG_MIN 8
#define FIRST_ARG_MAX 20
#define ALPHABET_SIZE 26//number of lower case letters in the alphabet 
#define STRINGBUFFER 5//picked 5 because it is the largest possible syllable to be added.
#define MAX_RANDOM_CHARS 4//Max number of random chars in our sting that will be changed
#define ASCII_A 97 // this is the ASCII value for "a" used for replacement from CharMap

char *Syllables[] = { 
        "bah",  "bay",  "bee",  "buy",   "boh",  "boo",
        "pah",  "pay",  "pee",  "pie",   "poh",  "poo",
        "tah",  "tay",  "tee",  "tie",   "toe",  "too",
        "dah",  "day",  "dee",  "die",   "doh",  "doo",
        "fah",  "fay",  "fee",  "fie",   "foh",  "foo",
        "vah",  "vay",  "vee",  "vie",   "voh",  "voo",
        "kah",  "kay",  "key",  "kie",   "koh",  "koo",
        "gah",  "gay",  "gee",  "guy",   "goh",  "goo",
        "shaw", "chay", "chee", "chai",  "cho",  "choo",
        "thaw", "thay", "thee", "thigh", "tho",  "thoo",
        "jaw",  "jay",  "jee",  "jie",   "joh",  "joo",
        "zhaw", "zhay", "zhee", "zhy",   "zhoh", "zhoo",
        "naw",  "nay",  "knee", "nye",   "no",   "new",
        "mah",  "may",  "mee",  "my",    "mow",  "moo",
        "wah",  "way",  "weea", "why",   "whoa", "woo",
        "yaw",  "yay",  "yee",  "yie",   "yoh",  "yoo",
        "zah",  "zay",  "zee",  "zee",   "zye",  "zoo",
        "saw",  "say",  "see",  "sigh",  "so",   "sue",  
        "rah",  "ray",  "ree",  "rye",   "roh",  "roo",
        "lah",  "lay",  "lee",  "lie",   "low",  "loo"
};
char *CharMap[] = {
        "@", "6", "C", "D", "3", "F", "G", "4", "!", "J",
        "K", "1", "M", "N", "*", "9", "&", "R", "$", "+",
        "U", "V", "#", "X", "/", "2"
};


void generate(long len) {
        long int randSyl = 0; // Used for picking a random Syllable in do-while loop
        srandom(time(NULL));
        int mallocSize = sizeof(char) * len + STRINGBUFFER; //store MallocSize to be used to create the password, then delete later
        errno = 0;
        int sylSize = sizeof(Syllables) / sizeof(Syllables[0]); //size of our Syllables Array
        int maxChars = (MAX_RANDOM_CHARS >= len) ? len : MAX_RANDOM_CHARS; //find most random Chars for cases for less than MAX_RANDOM_CHAR passwords
        int choosenLetters[MAX_RANDOM_CHARS] = {-1}; //creates array to hold char locations of random swapped values
        int printFlag=0;
        char * p = malloc(mallocSize); //password will be put in p
        p[0]='\0';//must do this to not get valgrind error for concat function.
        long int randletter =0;
        int l = 0;

        if(errno != SUCCESS){
                printf("error allocating memory for password");
                return;
        }else{
                //add random syllabels into p 
                do {
                        randSyl = random() % sylSize;
                        strcat(p, Syllables[randSyl]);
                }
                while (strlen(p) <= len);
                p[len] = '\0'; //place nullterm at the end of string 

                //put the random letters in password
                for (int i = 0; i < maxChars; i++) {
                        randletter = random() % len;
                        l = (int) p[randletter] - ASCII_A; //Takes the ASCII value of any lowercase letter found in p and subtracts 97 so that a=0, b=1, c=2...etc

                        //loop from -1 up to Alphbet size to add letters from CharMap, if letters are outside of this 
                        //bounds then they must have already been replaced, because they would not be lowercase letters a-z.
                        if (l > -1 && l < ALPHABET_SIZE) {
                                p[randletter] = * CharMap[l];
                        }
                        choosenLetters[i] = randletter;
                }

                //print letters to screen 
                for (int i = 0; i < len; i++) {
                        printFlag = 0;
                        //check to print letters with colors
                        for (int j = 0; j < maxChars; j++) {
                                //looks through length of password and checks if letter has been changed
                                if (i == choosenLetters[j] && i != -1) {
                                        //turn red print letter
                                        printf(CHAR_RED);
                                        printf("%c", p[i]);
                                        printFlag = 1; //flag to note that letter has been printed 
                                        break; //breaks out of loop after first red print
                                }
                        }
                        if (printFlag == 0) {

                                printf(CHAR_RESET);
                                //turn normal color print letter
                                printf("%c", p[i]);
                        }
                        printFlag = 0;

                }
                printf("\n"); //puts \n on the last line of the password print

                printf(CHAR_RESET); //reset color back to standard
                
                //Write over all password malloc and free the memory
                for (int i = 0; i < mallocSize; i++) {
                        p[i] = 'x';

                }
                free(p);//free memory so it can no longer be used
        }
}


int main(int argc, char * argv[]) {
        
        errno = SUCCESS;
        char * ch = NULL;
        long num_chars = 0;
        //Check number of Arguments
        if (argc != VALID_ARGS) {
                printf("Must have %d argument(s)\n", VALID_ARGS - 1);
                return EXIT_FAILURE;
        }
        num_chars = strtol(argv[FIRST_ARG], & ch, 10);
        // verify string too long function was success, also make sure no
        // other input is in ARG1 other than numbers 
        if ((errno != SUCCESS) || ( * ch != '\0')) {
                printf("error first argument must only be number\n");
                return EXIT_FAILURE;
        }
        //Check size of first argument is correct value
        if (num_chars < FIRST_ARG_MIN || num_chars > FIRST_ARG_MAX) {
                printf("error number must be >%d and <%d\n", FIRST_ARG_MIN, FIRST_ARG_MAX);
                return EXIT_FAILURE;
        }
        generate(num_chars);
        return SUCCESS;
}



