#include <stdio.h>

/* We use this to make insert(sql) to update tickets.
   You have to make tickets1.txt first, and put it in data/. */ 
int main(int argc, char *argv[])
{
    int c;
    char date[] = "date '2016-11-24'";
    int head = 1;  //is the head of a row
    int ticketid = 1; //is the ticketid term
    FILE *input;
    FILE *output;
    input = fopen("../data_temp/tickets1.txt", "r");
    if (input == NULL) {
        fprintf(stderr, "input open failed");
    }
    output = fopen("../data2/tickets2.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "output open failed");
    }
    while ((c = fgetc(input)) != EOF) {
        if (head) {
            fprintf(output, "insert into tickets values (%s, '", date);
            head = 0;
        }
        switch (c) {
        case '|':
            if (ticketid == 1) {
                fprintf(output, "',");
                ticketid = 0;
            } else {
                fprintf(output, ",");
            }
            break;
        case '\n':
            fprintf(output, ");");
            fputc('\n', output);
            head = 1;
            ticketid = 1;
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
