#include <scws.h>
#include <stdio.h>
main()
{
  scws_t s;
  s = scws_new();
  scws_free(s);
  printf("test ok!\n");
}
