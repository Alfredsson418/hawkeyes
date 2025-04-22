#pragma once

#include <argp.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

#include "../packages/argtable3.h"
#include "constans.h"
#include "other/ip/convert.h"
#include "other/ip/domain.h"
#include "other/network_interface.h"
#include "other/parse_ports.h"
#include "other/permissions.h"
#include "other/port_scan_lookup.h"
#include "other/service_detection.h"
#include "other/service_parser.h"
#include "other/transfer_layers.h"
#include "scan_methods/multithread_scanning.h"
#include "scan_methods/scan_structs.h"
#include "scan_methods/tcp/connect.h"
#include "scan_methods/tcp/half_sync.h"
#include "scan_methods/udp/echo_responce.h"
#include "ui/display_file.h"
#include "ui/motd.h"
#include "ui/print_macro.h"
#include "ui/terminal_display.h"
#include "version.h"
