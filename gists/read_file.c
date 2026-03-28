#include <stdio.h>

int main() {
    FILE *fptr = fopen("file.txt", "r");
    char buff[100];

    if (fptr == NULL) {
        printf("File not opened!\n");
        return 1;
    }

    // Read line by line until EOF
    while (fgets(buff, sizeof(buff), fptr) != NULL) {
        printf("%s", buff);  // Print each line
    }

    fclose(fptr);
    return 0;
}
