#include "../../include/other/is_root.h"

int is_root() {
    // https://stackoverflow.com/questions/3214297/how-can-my-c-c-application-determine-if-the-root-user-is-executing-the-command
    if (getuid()) {
        return 0; // Not root
    }
    return 1; // Root
}
