#ifdef WIN32
#define CPL_DISABLE_DLL
#endif
#include "ogr_api.h"

int main(int argc,char * argv[])
{
  OGRDataSourceH	hDS;
  OGRSFDriverH        *pahDriver;

  OGRRegisterAll();
  hDS = OGROpen("null.shp", FALSE, pahDriver );
  OGRReleaseDataSource( hDS );

  return EXIT_SUCCESS;
}
