/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
