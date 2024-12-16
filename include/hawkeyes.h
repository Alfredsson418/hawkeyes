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
#include "other/permissions.h"
#include "other/network_interface.h"
#include "other/parse_ports.h"
#include "scan_methods/tcp_scan.h"
#include "scan_methods/udp/udp_scan.h"
#include "scan_methods/multithread_scanning.h"
#include "other/port_scan_lookup.h"
#include "other/transfer_layers.h"
#include "other/service_detection.h"
