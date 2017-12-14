#include <stdio.h>
#include <ctype.h>

void get_trainid(char *trainid, char *filename)
{
    while ((*trainid = *filename) != '.') {
        trainid++;
        filename++;
    }
    *trainid = '\0';
}

/* Open the file one by one and do it.
   To make filelist, use ls > 1.txt, and delete 1.txt in 1.txt. */
int main(int argc, char *argv[])
{
    int c;
    char filename[20];
    char trainid[20];
    /* count = the number of comma have been read in a line
       firstline = is this the first row in a file
       firstdigit = is this the first digit of a number in a col
       count_ = the number of '|' have been made */
    int count = -1, firstline = 1, firstdigit = 1, count_ = 0;
    FILE *input;
    FILE *output;
    FILE *filelist;
    filelist = fopen("1.txt", "r");
    if (filelist == NULL) {
        fprintf(stderr, "filelist open failed");
    }
    output = fopen("../data2/trains.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "output open failed");
    }

    while (fscanf(filelist, "%s", filename) != EOF) {
        get_trainid(trainid, filename);
        input = fopen(filename, "r");
        if (input == NULL) {
            fprintf(stderr, "input %s open failed", filename);
        }
        while ((c = fgetc(input)) != EOF) {
            //delete the first row
            if (firstline) {
                if (c == '\n') {
                    firstline = 0;
                }
                continue;
            }
            //print trainid
            if (count == -1) {
                fprintf(output, "%s|", trainid);
                count++;
            }
            //let traveltime and traveldistance have value
            if ((count == 5 || count == 6) && c == '-') {
                fputc('0', output);
                continue;
            }
            //let the col of 0 price be NULL in database
            if ((count == 7 || count == 8 || count == 9) && firstdigit == 1 && c == '0') {
                firstdigit = 0;
                continue;
            }
            //some replace
            switch (c) {
            case ' ':
                break;
            case '-':
                break;
            case '/':
                count_++;
                firstdigit = 1;
                fputc('|', output);
                break;
            case ',':
                count_++;
                count++;
                firstdigit = 1;
                fputc('|', output);
                break;
            case '\n':
                //fprintf(output, "----%d----", count_);
                //for some row, to make the number of '|' same as others 
                while (count_ < 13) {
                    fputc('|', output);
                    count_++;
                }
                count_ = 0;
                count = -1;
                firstdigit = 1;
                fputc(c, output);
                break;
            default:
                fputc(c, output);
                break;
            }
        }
        fclose(input);
        firstline = 1;
    }
    fclose(output);
    return 0;
}
