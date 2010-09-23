//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description:
//
// Contains class declaration for NitfTileSource_12.
//
//*******************************************************************
//  $Id: ossimNitfTileSource_12.h 958 2010-06-03 23:00:32Z ming.su $
#ifndef ossimNitfTileSource_12_HEADER
#define ossimNitfTileSource_12_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/support_data/ossimNitfImageHeader.h>

class OSSIM_DLL ossimNitfTileSource_12 
{
public:
  static bool uncompressJpeg12Block(ossim_uint32 x, 
                                    ossim_uint32 y,
                                    ossimRefPtr<ossimImageData> cacheTile,
                                    ossimNitfImageHeader* hdr,
                                    ossimIpt cacheSize,
                                    std::vector<ossim_uint8> compressedBuf,
                                    ossim_uint32 readBlockSizeInBytes,
                                    ossim_uint32 bands);


  static bool loadJpegQuantizationTables(ossimNitfImageHeader* hdr,
                                         jpeg_decompress_struct& cinfo);

  static bool loadJpegHuffmanTables(jpeg_decompress_struct& cinfo);
};
   
#endif /* #ifndef ossimNitfTileSource_12_HEADER */
