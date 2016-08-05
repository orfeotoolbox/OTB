#include <gdal.h>
#include <stdio.h>

/* http://www.gdal.org/formats_list.html */

int main(int argc, char * argv[])
{

  int i;
  int count = 0;
  FILE *fp  = NULL;
  GDALDriverH driver = NULL;

  if (argc < 2) {
    printf( "gdalFormatList.c: incorrect usage.\n");
     exit(1);
  }

  GDALAllRegister();

  count = GDALGetDriverCount();

  if( count < 1 )   {
    printf( "gdalFormatList.c: Driver count zero.\n");
    exit(1);
  }

  fp = fopen(argv[1], "w");
  if (fp == NULL) {
    printf("gdalFormatList.c: Error opening file!\n");
    exit(1);
  }

  fprintf(fp, "%s\n", "###############################");
  fprintf(fp, "# Nb of drivers: %d\n", count);
  fprintf(fp, "%s\n", "###############################");
  fprintf(fp, "%s\n", "ID,Short name,Long name");

  for (i = 0; i < count; i++) {
    driver = GDALGetDriver(i);
    fprintf(fp, "%d,%s,%s\n", i, GDALGetDriverShortName(driver), GDALGetDriverLongName(driver));
  }

  fclose(fp);

  return EXIT_SUCCESS;
}
