#include "ogr_api.h"

int main(int argc,char * argv[])
{
  OGRDataSourceH	hDS;
  OGRSFDriverH    *pahDriver = NULL;

  OGRRegisterAll();
  hDS = OGROpen("null.shp", FALSE, pahDriver );
  OGRReleaseDataSource( hDS );

  return EXIT_SUCCESS;
}
