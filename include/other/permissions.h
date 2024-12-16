#pragma once

#include <unistd.h>
#include <sys/capability.h>

int is_root();

int has_cap_net_raw();
