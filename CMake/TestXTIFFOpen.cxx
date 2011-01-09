#include <iostream>

#include <tiffio.h>
#include <xtiffio.h>

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
     std::cerr << "Usage : " << argv[0] << " tiff_filename" << std::endl;
     return 1;
   }

   TIFF* theTiffPtr = XTIFFOpen( argv[1], "rm");
   if (!theTiffPtr)
   {
      std::cerr << "XTIFFOpen failed" << std::endl;
      return 2;
   }


   unsigned int imagewidth;
   if ( !TIFFGetField( theTiffPtr,
                       TIFFTAG_IMAGEWIDTH ,
                       &imagewidth ) )
   {
      std::cerr << "TIFFGetField failed" << std::endl;
      return 3;
   }

   return 0;
}
