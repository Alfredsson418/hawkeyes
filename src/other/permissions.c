#include "../../include/other/permissions.h"

int is_root() {
    // https://stackoverflow.com/questions/3214297/how-can-my-c-c-application-determine-if-the-root-user-is-executing-the-command
    if (getuid()) {
        return 0; // Not root
    }
    return 1; // Root
}

// Need to look into this in the future
/*
int has_cap_net_raw() {
    cap_t caps = cap_get_proc();

    if (!caps) {
        return -1;
    }

    cap_flag_value_t cap_net_raw;
    if (cap_get_flag(caps, CAP_NET_RAW, CAP_EFFECTIVE, &cap_net_raw) < 0) {
        cap_free(caps);
        return -1;
    }

    cap_free(caps);
    if (cap_net_raw == CAP_SET) {
        return 1;
    } else {
        return 0;
    }
}
*/
