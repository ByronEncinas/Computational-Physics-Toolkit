#include <stdio.h>
#include <unistd.h>
#include <limits.h> // Para PATH_MAX

int main() {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Directorio actual: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    return 0;
}
