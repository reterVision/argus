#include <Python.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/param.h>
#include <linux/major.h>

// Define usable constants.
#define TRUE        1
#define FALSE       0
#define LEN_128   128
#define LEN_256   256
#define LEN_1024 1024

// Define stats file locations.
#define NET_SNMP    "/proc/net/snmp"
#define MEMINFO     "/proc/meminfo"
#define IO_FILE     "/proc/diskstats"

// TCP stats.
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

// Memeory usage stats.
struct stats_mem {
  unsigned long tlmkb;
  unsigned long frmkb;
  unsigned long bufkb;
  unsigned long camkb;
  unsigned long acmkb;
  unsigned long iamkb;
  unsigned long slmkb;
  unsigned long frskb;
  unsigned long tlskb;
  unsigned long caskb;
  unsigned long comkb;
};

// IO stats.
#define MAX_PARTITIONS 64

struct part_info {
  unsigned int major;
  unsigned int minor;
  char name[32];
};

struct blkio_info {
  unsigned long long rd_ios;      /* Read I/O operations */
  unsigned long long rd_merges;   /* Reads merged */
  unsigned long long rd_sectors;  /* Sectors read */
  unsigned long long rd_ticks;    /* Time in queue + service for read */
  unsigned long long wr_ios;      /* Write I/O operations */
  unsigned long long wr_merges;   /* Writes merged */
  unsigned long long wr_sectors;  /* Sectors written */
  unsigned long long wr_ticks;    /* Time in queue + service for write */
  unsigned long long ticks;       /* Time of requests in queue */
  unsigned long long aveq;        /* Average queue length */
};

#define STATS_IO_SIZE (sizeof(struct blkio_info))
