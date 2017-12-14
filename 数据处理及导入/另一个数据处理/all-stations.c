#include <stdio.h>

int main(int argc, char *argv[])
{
    int c;
    FILE *input;
    FILE *output;
    input = fopen("../data1/all-stations.txt", "r");
    if (input == NULL) {
        fprintf(stderr, "input open failed");
    }
    output = fopen("../data2/all-stations.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "output open failed");
    }
    while ((c = fgetc(input)) != EOF) {
        switch (c) {
        case ' ':
            break;
        case ',':
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
