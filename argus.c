#include "argus.h"

/*
 * Read TCP stats.
 */
static PyObject *
tcp_stat(PyObject* self, PyObject* args)
{
  int              sw = FALSE;
  FILE             *fp;
  char             line[LEN_1024];
  struct stats_tcp st_tcp;

  memset(&st_tcp, 0, STATS_TCP_SIZE);
  if ((fp = fopen(NET_SNMP, "r")) == NULL) {
    PyErr_SetString(PyExc_ValueError, "snmp file does not exist!");
    return NULL;
  }

  while (fgets(line, LEN_1024, fp) != NULL) {
    if (!strncmp(line, "Tcp:", 4)) {  // Locate the correct line in file.
      if (sw) {
        sscanf(line + 4, "%*u %*u %*u %*d %llu %llu "
               "%llu %llu %llu %llu %llu %llu %llu %llu",
               &st_tcp.ActiveOpens,
               &st_tcp.PassiveOpens,
               &st_tcp.AttemptFails,
               &st_tcp.EstabResets,
               &st_tcp.CurrEstab,
               &st_tcp.InSegs,
               &st_tcp.OutSegs,
               &st_tcp.RetransSegs,
               &st_tcp.InErrs,
               &st_tcp.OutRsts);
      } else {
        sw = TRUE;
      }
    }
  }

  // Close the opened fd.
  fclose(fp);

  return Py_BuildValue("{s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i}",
                       "ActiveOpens", st_tcp.ActiveOpens,
                       "PassiveOpens", st_tcp.PassiveOpens,
                       "AttemptFails", st_tcp.AttemptFails,
                       "EstabResets", st_tcp.EstabResets,
                       "CurrEstab", st_tcp.CurrEstab,
                       "InSegs", st_tcp.InSegs,
                       "OutSegs", st_tcp.OutSegs,
                       "RetransSegs", st_tcp.RetransSegs,
                       "InErrs", st_tcp.InErrs,
                       "OutRsts", st_tcp.OutRsts);
}

/*
 * Read Memory usage.
 */
static PyObject *
mem_stat(PyObject* self, PyObject* args)
{
  FILE              *fp;
  char              line[LEN_128];
  struct stats_mem  st_mem;

  memset(&st_mem, 0, sizeof(struct stats_mem));
  if ((fp = fopen(MEMINFO, "r")) == NULL) {
    PyErr_SetString(PyExc_ValueError, "meminfo file does not exist!");
    return NULL;
  }

  while (fgets(line, LEN_128, fp) != NULL) {

    if (!strncmp(line, "MemTotal:", 9)) {
      /* Read the total amount of memory in KB */
      sscanf(line + 9, "%lu", &st_mem.tlmkb);
    }

    else if (!strncmp(line, "MemFree:", 8)) {
      /* Read the amount of free memeory in KB */
      sscanf(line + 8, "%lu", &st_mem.frmkb);
    }

    else if (!strncmp(line, "Buffers:", 8)) {
      /* Read the amount of buffered memory in KB */
      sscanf(line + 8, "%lu", &st_mem.bufkb);
    }

    else if (!strncmp(line, "Cached:", 7)) {
      /* Read the amount of cached memory in KB */
      sscanf(line + 7, "%lu", &st_mem.camkb);
    }

    else if (!strncmp(line, "Active:", 7)) {
      /* Read the amount of Active memory in KB */
      sscanf(line + 7, "%lu", &st_mem.acmkb);
    }

    else if (!strncmp(line, "Inactive:", 9)) {
      /* Read the amount of Inactive memory in KB */
      sscanf(line + 9, "%lu", &st_mem.iamkb);
    }

    else if (!strncmp(line, "Slab:", 5)) {
      /* Read the amount of Slab memory in KB */
      sscanf(line + 5, "%lu", &st_mem.slmkb);
    }

    else if (!strncmp(line, "SwapCached:", 11)) {
      /* Read the amount of SwapCached memory in KB */
      sscanf(line + 11, "%lu", &st_mem.caskb);
    }

    else if (!strncmp(line, "SwapTotal:", 10)) {
      /* Read the amount of SwapTotal memory in KB */
      sscanf(line + 10, "%lu", &st_mem.tlskb);
    }

    else if (!strncmp(line, "SwapFree:", 9)) {
      /* Read the amount of free swap memory in KB */
      sscanf(line + 9, "%lu", &st_mem.frskb);
    }

    else if (!strncmp(line, "Committed_AS:", 13)) {
      /* Read the amount of commited memory in KB */
      sscanf(line + 13, "%lu", &st_mem.comkb);
    }
  }

  fclose(fp);

  return Py_BuildValue("{s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i}",
                       "MemTotal", st_mem.tlmkb,
                       "MemFree", st_mem.frmkb,
                       "Buffers", st_mem.bufkb,
                       "Cached", st_mem.camkb,
                       "Active", st_mem.acmkb,
                       "Inactive", st_mem.iamkb,
                       "Slab", st_mem.slmkb,
                       "SwapCached", st_mem.caskb,
                       "SwapTotal", st_mem.tlskb,
                       "SwapFree", st_mem.frskb,
                       "Committed_AS", st_mem.comkb);
}

/*
 * Read IO stats.
 */
#ifndef IDE_DISK_MAJOR
#define IDE_DISK_MAJOR(M) ((M) == IDE0_MAJOR || (M) == IDE1_MAJOR || \
        (M) == IDE2_MAJOR || (M) == IDE3_MAJOR || \
        (M) == IDE4_MAJOR || (M) == IDE5_MAJOR || \
        (M) == IDE6_MAJOR || (M) == IDE7_MAJOR || \
        (M) == IDE8_MAJOR || (M) == IDE9_MAJOR)
#endif  /* !IDE_DISK_MAJOR */

#ifndef SCSI_DISK_MAJOR
#ifndef SCSI_DISK8_MAJOR
#define SCSI_DISK8_MAJOR 128
#endif
#ifndef SCSI_DISK15_MAJOR
#define SCSI_DISK15_MAJOR 135
#endif
#define SCSI_DISK_MAJOR(M) ((M) == SCSI_DISK0_MAJOR || \
        ((M) >= SCSI_DISK1_MAJOR && \
         (M) <= SCSI_DISK7_MAJOR) || \
        ((M) >= SCSI_DISK8_MAJOR && \
         (M) <= SCSI_DISK15_MAJOR))
#endif  /* !SCSI_DISK_MAJOR */

#ifndef DEVMAP_MAJOR
#define DEVMAP_MAJOR 253
#endif

#ifndef COMPAQ_MAJOR
#define COMPAQ_CISS_MAJOR   104
#define COMPAQ_CISS_MAJOR7  111
#define COMPAQ_SMART2_MAJOR 72
#define COMPAQ_SMART2_MAJOR7    79
#define COMPAQ_MAJOR(M) (((M) >= COMPAQ_CISS_MAJOR && \
            (M) <= COMPAQ_CISS_MAJOR7) || \
        ((M) >= COMPAQ_SMART2_MAJOR && \
         (M) <= COMPAQ_SMART2_MAJOR7))
#endif /* !COMPAQ_MAJOR */

int
printable(unsigned int major, unsigned int minor)
{
  int print_device = 1;
  int print_partition = 0;

  if (IDE_DISK_MAJOR(major)) {
      return (!(minor & 0x3F) && print_device)
          || ((minor & 0x3F) && print_partition);
  } else if (SCSI_DISK_MAJOR(major)) {
      return (!(minor & 0x0F) && print_device)
          || ((minor & 0x0F) && print_partition);
  } else if(COMPAQ_MAJOR(major)){
      return (!(minor & 0x0F) && print_device)
          || ((minor & 0x0F) && print_partition);
  }
  return 1;   /* if uncertain, print it */
}

static PyObject *
io_stat(PyObject* self, PyObject* args)
{
  FILE              *fp;
  struct part_info  partition[MAX_PARTITIONS];
  struct blkio_info new_blkio[MAX_PARTITIONS];
  char              buffer[LEN_256];
  unsigned int      p;

  unsigned int n_partitions = 0;
  unsigned int max_partitions = MAX_PARTITIONS;

  memset(buffer, 0, LEN_256);

  if ((fp = fopen(IO_FILE, "r")) == NULL) {
    PyErr_SetString(PyExc_ValueError, "diskstats file does not exist!");
    return NULL;
  }

  /* Get partition names. Check against match list */

  while (fgets(buffer, sizeof(buffer), fp)) {
    unsigned int reads = 0;
    struct part_info curr;

    if (sscanf(buffer, "%4d %4d %31s %u", &curr.major, &curr.minor,
               curr.name, &reads) == 4) {
      unsigned int p;
      if (n_partitions >= max_partitions) {
        break;
      }

      for (p = 0; p < n_partitions
            && (partition[p].major != curr.major
              || partition[p].minor != curr.minor);
           p++);
      if (p == n_partitions && p < MAX_PARTITIONS) {
        if (reads && printable(curr.major, curr.minor)) {
          partition[p] = curr;
          n_partitions = p + 1;
        }
      }
    }
  }

  /* Get kernel stats */
  rewind(fp);
  while (fgets(buffer, sizeof(buffer), fp)) {
    int items;
    struct part_info curr;
    struct blkio_info blkio;
    memset(&blkio, 0, STATS_IO_SIZE);
    items = sscanf(buffer, "%4d %4d %*s %u %u %llu %u %u %u %llu %u %*u %u %u",
                   &curr.major, &curr.minor,
                   &blkio.rd_ios, &blkio.rd_merges,
                   &blkio.rd_sectors, &blkio.rd_ticks,
                   &blkio.wr_ios, &blkio.wr_merges,
                   &blkio.wr_sectors, &blkio.wr_ticks,
                   &blkio.ticks, &blkio.aveq);

    if (items == 6) {
      blkio.rd_sectors = blkio.rd_merges;
      blkio.wr_sectors = blkio.rd_ticks;
      blkio.rd_ios = 0;
      blkio.rd_merges = 0;
      blkio.rd_ticks = 0;
      blkio.wr_ios = 0;
      blkio.wr_merges =0;
      blkio.wr_ticks = 0;
      blkio.ticks = 0;
      blkio.aveq = 0;
      items = 12;
    }

    if (items = 12) {
      for (p = 0; p < n_partitions; p++) {
        if (partition[p].major == curr.major
              && partition[p].minor == curr.minor) {
          new_blkio[p] = blkio;
          break;
        }
      }
    }
  }

  /* Close file */
  rewind(fp);
  if (NULL != fp) {
    fclose(fp);
  }

  /* Output wrapper */
  PyDictObject* io_dict = PyDict_New();
  for (p=0; p<n_partitions; p++) {
    PyObject* partition_dict = Py_BuildValue(
      "{s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i}",
      "Read I/O operations", new_blkio[p].rd_ios,
      "Reads merged", new_blkio[p].rd_merges,
      "Sectors read", new_blkio[p].rd_sectors,
      "Time in queue + service for read", new_blkio[p].rd_ticks,
      "Write I/O operations", new_blkio[p].wr_ios,
      "Writes merged", new_blkio[p].wr_merges,
      "Sectors written", new_blkio[p].wr_sectors,
      "Time in queue + service for write", new_blkio[p].wr_ticks,
      "Time of requests in queue", new_blkio[p].ticks,
      "Average queue length", new_blkio[p].aveq);
    PyDict_SetItemString(io_dict, partition[p].name, partition_dict);
  }

  return io_dict;
}


/*
 * Define Python module function table.
 */
static PyMethodDef
module_functions[] = {
  { "tcp_stat", tcp_stat, METH_NOARGS, "read TCP stats" },
  { "mem_stat", mem_stat, METH_NOARGS, "read Memory stats" },
  { "io_stat", io_stat, METH_NOARGS, "read IO stats" },
  { NULL }
};

/*
 * Initialize Python module.
 */
void
initargus(void)
{
  Py_InitModule3("argus", module_functions, "");
}
