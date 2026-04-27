#include <stdio.h>
#include <string.h>

/*
 * Secure version of the admin-check program:
 * - Keeps the same data model/flow as vulnerable.c.
 * - Replaces unsafe input function with bounded fgets.
 * - Validates and normalizes input before authorization checks.
 * - Preserves least-privilege default (isAdmin = 0 unless exact match).
 */
typedef struct {
    char username[16];
    int isAdmin;
} SessionState;

int main(void) {
    SessionState state = {0};
    const char *admins[] = {"root", "admin", "superuser"};
    int adminCount = 3;

    printf("=== Fixed Secure Input Demo ===\n");
    printf("username buffer size: %zu bytes\n", sizeof(state.username));
    printf("isAdmin initial value: %d\n", state.isAdmin);
    printf("Enter username: ");

    if (fgets(state.username, sizeof(state.username), stdin) == NULL) {
        fprintf(stderr, "Input error.\n");
        return 1;
    }

    /* Remove trailing newline if present and keep state for overflow check. */
    char *newline = strchr(state.username, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    /*
     * If newline was not read and buffer is full, input was too long.
     * Flush remaining characters and handle as denied access.
     */
    if (newline == NULL && strlen(state.username) == sizeof(state.username) - 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard extra bytes */
        }
        printf("Captured username length: > %zu bytes (truncated input detected)\n",
               sizeof(state.username) - 1);
        printf("isAdmin remains safe value: %d\n", state.isAdmin);
        printf("Access denied. Not an admin.\n");
        return 0;
    }

    printf("Captured username: %s\n", state.username);
    printf("Captured username length: %zu\n", strlen(state.username));
    printf("isAdmin before auth check: %d\n", state.isAdmin);

    for (int i = 0; i < adminCount; i++) {
        if (strcmp(state.username, admins[i]) == 0) {
            state.isAdmin = 1;
            break;
        }
    }

    printf("isAdmin after auth check: %d\n", state.isAdmin);

    if (state.isAdmin) {
        printf("Welcome to admin dashboard\n");
    } else {
        printf("Access denied. Not an admin.\n");
    }

    return 0;
}
