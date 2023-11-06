#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) {
    char buf[512];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (buf[0] == 'q') {
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }
    printf("fgets end!\r\n");
    return 0;
}