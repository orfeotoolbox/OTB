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
//  $Id: ossimNitfTileSource_12.cpp 958 2010-06-03 23:00:32Z ming.su $

//#if defined(JPEG_DUAL_MODE_8_12)
#include <fstream>

#if defined(JPEG_DUAL_MODE_8_12)
#include <jpeg12/jpeglib.h>

//ossim includes
#include <ossim//imaging/ossimJpegMemSrc.h>
#include <ossim/imaging/ossimNitfTileSource_12.h>
#include <ossim/imaging/ossimJpegDefaultTable.h>

bool ossimNitfTileSource_12::uncompressJpeg12Block(ossim_uint32 x, ossim_uint32 y,
                                                   ossimRefPtr<ossimImageData> cacheTile,
                                                   ossimNitfImageHeader* hdr,
                                                   ossimIpt cacheSize,
                                                   std::vector<ossim_uint8> compressedBuf,
                                                   ossim_uint32 readBlockSizeInBytes,
                                                   ossim_uint32 bands)
{
   jpeg_decompress_struct cinfo;

   ossimJpegErrorMgr jerr;

   cinfo.err = jpeg_std_error(&jerr.pub);
   
   jerr.pub.error_exit = ossimJpegErrorExit;

   /* Establish the setjmp return context for my_error_exit to use. */
   if (setjmp(jerr.setjmp_buffer))
   {
     jpeg_destroy_decompress(&cinfo);
   
     return false;
   }

   jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, sizeof(cinfo));
  
   //---
   // Step 2: specify data source.  In this case we will uncompress from
   // memory so we will use "ossimJpegMemorySrc" in place of " jpeg_stdio_src".
   //---
   ossimJpegMemorySrc (&cinfo,
                       &(compressedBuf.front()),
                       static_cast<size_t>(readBlockSizeInBytes));

   /* Step 3: read file parameters with jpeg_read_header() */
  
   jpeg_read_header(&cinfo, TRUE);
  
   
   // Check for Quantization tables.
   if (cinfo.quant_tbl_ptrs[0] == NULL)
   {
      // This will load table specified in COMRAT field.
      if (loadJpegQuantizationTables(hdr, cinfo) == false)
      {
        jpeg_destroy_decompress(&cinfo);
        return false;
      }
   }

   // Check for huffman tables.
   if (cinfo.ac_huff_tbl_ptrs[0] == NULL)
   {
      // This will load default huffman tables into .
      if (loadJpegHuffmanTables(cinfo) == false)
      {
        jpeg_destroy_decompress(&cinfo);
        return false;
      }
   }

   /* Step 4: set parameters for decompression */
   
   /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

   /* Step 5: Start decompressor */
   
   jpeg_start_decompress(&cinfo);
  
   const ossim_uint32 SAMPLES = cinfo.output_width;

   //---
   // Note: Some nitf will be tagged with a given number of lines but the last
   // jpeg block may go beyond that to a complete block.  So it you clamp to
   // last line of the nitf you will get a libjpeg error:
   // 
   // "Application transferred too few scanlines"
   //
   // So here we will always read the full jpeg block even if it is beyond the
   // last line of the nitf.
   //---
   const ossim_uint32 LINES_TO_READ =
      min(static_cast<ossim_uint32>(cacheSize.y), cinfo.output_height);

   /* JSAMPLEs per row in output buffer */
   const ossim_uint32 ROW_STRIDE = SAMPLES * cinfo.output_components;

   if ( (SAMPLES < cacheTile->getWidth() ) ||
        (LINES_TO_READ < cacheTile->getHeight()) )
   {
      cacheTile->makeBlank();
   }

   if ( (SAMPLES > cacheTile->getWidth()) ||
        (LINES_TO_READ > cacheTile->getHeight()) )
   {
     jpeg_finish_decompress(&cinfo);
     jpeg_destroy_decompress(&cinfo);

     return false;
   }
   
   // Get pointers to the cache tile buffers.
   std::vector<ossim_uint16*> destinationBuffer(bands);
   ossim_uint32 band = 0;
   for (band = 0; band < bands; ++band)
   {
     destinationBuffer[band] = cacheTile->getUshortBuf(band);
   }

   std::vector<ossim_uint16> lineBuffer(ROW_STRIDE);
   JSAMPROW jbuf[1];
   jbuf[0] = (JSAMPROW) &(lineBuffer.front());

   while (cinfo.output_scanline < LINES_TO_READ)
   {
     // Read a line from the jpeg file.
     jpeg_read_scanlines(&cinfo, jbuf, 1);

     ossim_uint32 index = 0;
     for (ossim_uint32 sample = 0; sample < SAMPLES; ++sample)         
     {
       for (band = 0; band < bands; ++band)
       {
         destinationBuffer[band][sample] = lineBuffer[index];
         ++index;
       }
     }

     for (band = 0; band < bands; ++band)
     {
       destinationBuffer[band] += cacheSize.x;         
     }
   }

   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);
 
   return true;
}

bool ossimNitfTileSource_12::loadJpegQuantizationTables(ossimNitfImageHeader* hdr,
                                jpeg_decompress_struct& cinfo)
{
  if (!hdr)
  {
    return false;
  }

  ossimString comrat = hdr->getCompressionRateCode();
  ossim_uint32 tableIndex = 0;
  if (comrat.size() >= 4)
  {
    // COMRAT string like: "00.2" = use table 2. (between 1 and 5).
    ossimString s;
    s.push_back(comrat[static_cast<std::string::size_type>(3)]);
    ossim_int32 comTbl = s.toInt32();
    if ( (comTbl > 0) && (comTbl < 6) )
    {
      tableIndex = comTbl-1;
    }
    else
    {
      ossimNotify(ossimNotifyLevel_WARN)
        << "ossimNitfTileSource_12::loadJpegQuantizationTables WARNING\n"
        << "\nNo quantization tables specified!"
        << endl;
      return false;  
    }
  }

  cinfo.quant_tbl_ptrs[0] = jpeg_alloc_quant_table((j_common_ptr) &cinfo);

  JQUANT_TBL* quant_ptr = cinfo.quant_tbl_ptrs[0]; // quant_ptr is JQUANT_TBL*

  for (ossim_int32 i = 0; i < 64; ++i)
  {
    /* Qtable[] is desired quantization table, in natural array order */
    quant_ptr->quantval[i] = QTABLE_ARRAY[tableIndex][i];
  }
  return true;
}

bool ossimNitfTileSource_12::loadJpegHuffmanTables(jpeg_decompress_struct& cinfo)
{
  if ( (cinfo.ac_huff_tbl_ptrs[0] != NULL) &&
    (cinfo.dc_huff_tbl_ptrs[0] != NULL) )
  {
    return false;
  }

  cinfo.ac_huff_tbl_ptrs[0] = jpeg_alloc_huff_table((j_common_ptr)&cinfo);
  cinfo.dc_huff_tbl_ptrs[0] = jpeg_alloc_huff_table((j_common_ptr)&cinfo);

  ossim_int32 i;
  JHUFF_TBL* huff_ptr;

  // Copy the ac tables.
  huff_ptr = cinfo.ac_huff_tbl_ptrs[0]; /* huff_ptr is JHUFF_TBL* */     
  for (i = 0; i < 16; ++i) 
  {
    // huff_ptr->bits is array of 17 bits[0] is unused; hence, the i+1
    huff_ptr->bits[i+1] = AC_BITS[i]; 
  }

  for (i = 0; i < 256; ++i)
  {
    huff_ptr->huffval[i] = AC_HUFFVAL[i];
  }

  // Copy the dc tables.
  huff_ptr = cinfo.dc_huff_tbl_ptrs[0]; /* huff_ptr is JHUFF_TBL* */
  for (i = 0; i < 16; ++i)
  {
    // huff_ptr->bits is array of 17 bits[0] is unused; hence, the i+1
    huff_ptr->bits[i+1] = DC_BITS[i];
  }

  for (i = 0; i < 256; i++)
  {
    /* symbols[] is the list of Huffman symbols, in code-length order */
    huff_ptr->huffval[i] = DC_HUFFVAL[i];
  }
  return true;
}

#endif /* defined(JPEG_DUAL_MODE_8_12) */
