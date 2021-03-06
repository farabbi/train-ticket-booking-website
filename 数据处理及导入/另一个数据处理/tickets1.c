#include <stdio.h>

/* You can only use this when you have trains.txt.
   Please put trains.txt in data/ */
int main(int argc, char *argv[])
{
    int c;
    int first = 1;  //in col 8~14, is this the first item in this col
    int count = 0;  //count the number of '|'
    FILE *input;
    FILE *output;
    input = fopen("../data2/trains.txt", "r");
    if (input == NULL) {
        fprintf(stderr, "input open failed");
    }
    output = fopen("../data_temp/tickets1.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "output open failed");
    }
    while ((c = fgetc(input)) != EOF) {
        if (count >= 2 && count <= 7) {
            if (c == '|') {
                count++;
            }
            continue;
        }
        if (count >= 8 && count <= 13) {
            if (first == 1) {
                if (c == '|') {
                    fputc('0', output);
                    fputc('|', output);
                    count++;
                } else {
                    fputc('5', output);
                    first = 0;
                }
                continue;
            } else {
                if (c == '|') {
                    fputc('|', output);
                    count++;
                    first = 1;
                }
                continue;
            }
        }
        if (count == 14) {
            if (first == 1) {
                if (c == '\n') {
                    fputc('0', output);
                    fputc('\n', output);
                    count = 0;
                } else {
                    fputc('5', output);
                    first = 0;
                }
                continue;
            } else {
                if (c == '\n') {
                    fputc('\n', output);
                    count = 0;
                    first = 1;
                }
                continue;
            }
        }
        switch (c) {
        case '|':
            count++;
            fputc('|', output);
            break;
        default:
            fputc(c, output);
            break;
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
