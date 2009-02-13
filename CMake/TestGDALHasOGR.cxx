#include "ogrsf_frmts.h"

int main(int argc,char * argv[])
{
  OGRSFDriverRegistrar::Open(argv[1], FALSE);
  return 0;
}
