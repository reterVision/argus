#include <Python.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define usable constants.
#define TRUE        1
#define FALSE       0
#define LEN_1024 1024

// Define stats file locations.
#define NET_SNMP    "/proc/net/snmp"

// TCP stat related data.
struct stats_tcp {
  unsigned long long ActiveOpens;
  unsigned long long PassiveOpens;
  unsigned long long InSegs;
  unsigned long long OutSegs;
  unsigned long long AttemptFails;
  unsigned long long EstabResets;
  unsigned long long CurrEstab;
  unsigned long long RetransSegs;
  unsigned long long InErrs;
  unsigned long long OutRsts;
};

#define STATS_TCP_SIZE (sizeof(struct stats_tcp))
