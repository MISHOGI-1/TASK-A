#include <stdio.h>
#include <string.h>

typedef struct {
    char username[16];
    int isAdmin;
} SessionState;

static void dump_state(const SessionState *state) {
    const unsigned char *raw = (const unsigned char *)state;
    size_t total = sizeof(*state);

    printf("Raw bytes (username + isAdmin): ");
    for (size_t i = 0; i < total; i++) {
        printf("%02X ", raw[i]);
    }
    printf("\n");
}

int main() {
    SessionState state;
    state.isAdmin = 0;

    // Hardcoded admin list
    const char *admins[] = {"emma", "daniel", "destiny"};
    int adminCount = 3;

    printf("=== Vulnerable Buffer Overflow Demo ===\n");
    printf("username size: %zu bytes\n", sizeof(state.username));
    printf("isAdmin initial value: %d\n", state.isAdmin);
    printf(
        "Tip: enter >16 characters (e.g., 24+ 'A's) to overflow into isAdmin.\n"
    );

    printf("Enter username: ");

    // ❌ VULNERABLE FUNCTION (no bounds checking)
    gets(state.username);

    printf("\nAfter input:\n");
    printf("username captured: %s\n", state.username);
    printf("isAdmin value now: %d (0x%08X)\n", state.isAdmin, state.isAdmin);
    dump_state(&state);

    // Check if user is admin
    for (int i = 0; i < adminCount; i++) {
        if (strcmp(state.username, admins[i]) == 0) {
            state.isAdmin = 1;
        }
    }

    // Access control decision
    if (state.isAdmin) {
        printf("Welcome to admin dashboard\n");
    } else {
        printf("Access denied. Not an admin.\n");
    }

    return 0;
}