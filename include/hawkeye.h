#pragma once

#include <string.h>
#include <stdbool.h>
#include <argp.h>
#include <arpa/inet.h>

#include "constans.h"

#include "other/terminal_args.h"
#include "ui/print_macro.h"
#include "ui/terminal_display.h"
#include "ui/display_file.h"
#include "other/is_root.h"
#include "other/network_interface.h"
#include "other/parse_ports.h"
#include "methods/tcp_scan.h"
#include "methods/udp_scan.h"
#include "other/multithread_scanning.h"
#include "other/port_scan_loopup.h"
#include "other/transfer_layers.h"
#include "other/service_detection.h"
