#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <Library.h>

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printUsage();
        exit(-1);
    }

    int option;
    const char *regex = NULL;
    const char *text = NULL;

    while ((option = getopt(argc, argv, "n:E:")) != -1) {
        switch (option) {
            case 'n':
                text = optarg;
                break;
            case 'E':
                regex = optarg;
                break;
            default:
                printf("Unknown option\n");
        }
    }

    if (regexSearch(regex,text) == 1) {
        printf("Regex match\n");
        return 0;
    } else {
        printf("Regex not match\n");
        return -1;
    }
}
