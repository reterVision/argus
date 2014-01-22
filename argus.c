#include "argus.h"

static PyObject *
tcp_stat(PyObject* self, PyObject* args)
{
  int              sw = FALSE;
  FILE             *fp;
  char             line[1024];
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

static PyMethodDef
module_functions[] = {
  { "tcp_stat", tcp_stat, METH_VARARGS, "read TCP stats"},
  { NULL }
};

void
initargus(void)
{
  Py_InitModule3("argus", module_functions, "");
}
