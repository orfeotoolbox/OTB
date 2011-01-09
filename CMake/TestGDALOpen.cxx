#include <iostream>
#include <stdexcept>
#include <gdal.h>

int main(int argc, char * argv[])
{
   if (argc != 2)
   {
     std::cerr << "Usage : " << argv[0] << " tiff_filename" << std::endl;
     return 1;
   }

  GDALAllRegister();
  GDALOpen(argv[1], GA_ReadOnly );

  return 0;
}

