                    /*** IN THIS FILE WE WILL HAVE ALL THE FUNCTION RELATED TO DEALING WITH FILES
                                 CREATING FILES, FETCHING LINES FROM FILES ETC               ***/

#include "file.h"


/* function will fetch the file with the fileName provided and open with mode based on file mode provided */
FILE *fetchFile(char *fileName, char *fileMode)
{
    FILE *fp;
    
    fname = (char *)safeAlloc(sCalloc, 1, strlen(fileName) + strlen(SOURCE_ET) + 1);
    strcpy(fname, fileName);
    strcat(fname, SOURCE_ET);

    fp = fopen(fname, fileMode);

    if(!fp)
    {
        printError(FAILED_OPEN, fname); /* Will print FAILED_OPEN err */ 
        fclose(fp);
        return NULL; /* name of file not found, move on to next file from argv[] if exist */
    }

    return fp;
}


/* the function read char by char from file and assign to line when \n occur
* this funcion will 'on the fly' avoid extra white chars from input, and replace with since space char, as long
as input isnt part of .string " " string, for ex: (SPACE X 4)\t AB:\t\t\t .data -> will turn into: AB: .data with one
* SPACE char in between 2 words that originally inputted with white tabs in between */
void fetchLine(FILE *fd, char **line)
{
   int ch, size=1, len=0, inString = false, firstQuote = false;

    (*line)=(char *)safeAlloc(sCalloc,1, sizeof(char)*size);
      
    /* as long as not end of file */
    while((ch=fgetc(fd)) != EOF)
    { 
        /* Is begining of string of .string directive */
        if(ch == (char)STRING_START)
        {
            /* If previous char in line is not backslash escape char && not firstQuote */
            if((*line)[len-1] != ESCAPED_BACKSLASH && firstQuote)
            inString = false;
            /* first occurence of ", begining of string */
            if(!firstQuote)
        {
            inString = true;
            firstQuote = true;
            }
        }

        /* On the fly replacing multiple inputed white chars (exclude '\n') with one space char (' '), 
        * if @ begining of string, avoid all white chars inserted */
        if(whiteCh(ch) && !inString)/* check if current char is of white char (exclude '\n') */
        { 
            if(len-1<0) /* if we are @ begining of setntence */
                continue; /* avoid the white char and move along to scan next one */
            if(ch != SPACE) /* if current white char isn't a single space */
                ch=SPACE; /* replace with single space char */
            if(whiteCh((*line)[len-1])) /* if previous string char is also of white char, avoid and continue */ 
                continue;
        }
   
        /* if 'enter' ('\n') inserted */
        if(ch == LINE_BREAK)
        {
            /* Avoiding extra white chars at end of file string before '\n' */
            while(whiteCh((*line)[--len]) && len > 0)
            {
                (*line)-1; 
            } 
            (*line)[len+1]=STRING_END;  /* assign end of string char */
            return;
        }
      
        (*line)[len++]=ch;  /* add the char to string */

        if(len==size)   /* if we need more memory allocation */
        {
            /* assign line with a new size using safeAlloc (REALLOC) */
            (*line) = (char *)safeAlloc(sRealloc,(*line),sizeof(char)*(size+=1));
        }
   }
}


/* write entry file */
void writeEntry(char *fileName, char *lbl, int *value)
{
    FILE *fp;
    char *fname;

    fp = fopen(fileName, "a");

    if(!fp)
    {
        printf("Fatal error opening/creating entry file %s", fileName);
        exit(1);
    }

    fprintf(fp, "%s\t%d\n", lbl, value);

    fclose(fp);
}
