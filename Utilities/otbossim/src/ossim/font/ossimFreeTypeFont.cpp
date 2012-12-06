//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFreeTypeFont.cpp 21518 2012-08-22 21:15:56Z dburken $

// ossimFreeTypeFont.h should be load prior to checking for OSSIM_HAS_FREETYPE.
#include <ossim/font/ossimFreeTypeFont.h>

#if OSSIM_HAS_FREETYPE
#  include <ossim/matrix/newmatio.h>
#  include <ossim/base/ossimFilename.h>
#  include <ossim/base/ossimErrorContext.h>
#  include <ossim/base/ossimTrace.h>
#  include <cstring> // for memset
#  ifndef NULL
#    include <stddef.h>
#  endif

static ossimTrace traceDebug ("ossimFreeTypeFont:debug");

RTTI_DEF1(ossimFreeTypeFont, "ossimFreeTypeFont", ossimFont);

ossimFreeTypeFont::ossimFreeTypeFont(const ossimFilename& fontFile)
   :ossimFont(),
    theFontFace(NULL),
    theLibrary(NULL),
    theFontFile(fontFile),
    theOutputBuffer(NULL),
    theBufferSize(0),
    theKerningEnabledFlag(true),
    theNeedToLayoutGlyphsFlag(true),
    theBoundingRectIsValid(false)
{
   int error;
   if(fontFile.exists())
   {
      error = FT_Init_FreeType(&theLibrary);
      if(error)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimFreeTypeFont::ossimFreeTypeFont: Can't initialize freetype" << std::endl;
         setErrorStatus();
      }
      if(getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
         error = FT_New_Face( theLibrary,
                              fontFile.c_str(),
                              0,
                              &theFontFace );
         if (error == FT_Err_Unknown_File_Format)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "WARNING ossimFreeTypeFont::ossimFreeTypeFont: Unable to create a font face for file = "
                  << fontFile
                  << std::endl;
            }
            FT_Done_Library(theLibrary);
            theLibrary = NULL;
            setErrorStatus();
         }
         else
         {
            setBaseClassInformation();
         }
      }
   }
   else
   {
      setErrorStatus();
   }
}

ossimFreeTypeFont::ossimFreeTypeFont(const ossimFreeTypeFont& rhs)
   :ossimFont(rhs),
    theMatrix(rhs.theMatrix),
    theFontFile(rhs.theFontFile),
    theOutputBuffer(NULL),
    theBufferSize(0),
    theKerningEnabledFlag(rhs.theKerningEnabledFlag),
    theNeedToLayoutGlyphsFlag(rhs.theNeedToLayoutGlyphsFlag),
    theStringCenter(rhs.theStringCenter),
    theShift(rhs.theShift),
    thePrecomputedBoundingRect(rhs.thePrecomputedBoundingRect),
    theBoundingRectIsValid(rhs.theBoundingRectIsValid)   
{   
   if(rhs.getErrorStatus())
   {
      setErrorStatus();
   }
   else
   {
      int error;
      error = FT_Init_FreeType(&theLibrary);
      
      if(error)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimFreeTypeFoont::ossimFreeTypeFont: Can't initialize freetype in copy constructor" <<std::endl;
         setErrorStatus();
      }
      if(getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
         error = FT_New_Face( theLibrary,
                              rhs.theFontFile.c_str(),
                              0,
                              &theFontFace );
         if(error == FT_Err_Unknown_File_Format)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "WARNING ossimFreeTypeFoont::ossimFreeTypeFont: Unable to create a font face for file = "
                  << rhs.theFontFile
                  << std::endl;
            }
            
            FT_Done_Library(theLibrary);
            theLibrary = NULL;
            setErrorStatus();
         }
         else
         {
            if(rhs.theBufferSize)
            {
               theBufferSize = rhs.theBufferSize;
               theOutputBuffer = new ossim_uint8[theBufferSize];
               memcpy(theOutputBuffer, rhs.theOutputBuffer, theBufferSize);
               setBaseClassInformation();
               theNeedToLayoutGlyphsFlag = true;
               layoutGlyphs(theStringToRasterize);
            }
         }
      }
   }      
}

ossimFreeTypeFont::~ossimFreeTypeFont()
{
   if(theOutputBuffer)
   {
      delete [] theOutputBuffer;
      theOutputBuffer = NULL;
   }
   if(theFontFace)
   {
      FT_Done_Face(theFontFace);
      theFontFace = NULL;
   }
   if(theLibrary)
   {
      FT_Done_FreeType(theLibrary);
      theLibrary = NULL;
   }
}

void ossimFreeTypeFont::layoutGlyphs(const ossimString& s )
{
   if(!theNeedToLayoutGlyphsFlag||!theFontFace)
   {
      return;
   }

   
   int        n;
   FT_Vector  origin;
   FT_Pos     origin_x = 0;
   // FT_UInt    load_flags;
   // FT_UInt    num_grays;
   FT_UInt    prev_index = 0;
   FT_UInt num_glyphs = (FT_UInt)s.size();
   int error = 0;
   
   deleteGlyphs(theStringLayout);
   theStringLayout.resize(s.size());

   theMatrix.xx = (FT_Fixed)(theAffineTransform[0][0]*0x10000);
   theMatrix.xy = (FT_Fixed)(theAffineTransform[1][0]*0x10000);
   theMatrix.yx = (FT_Fixed)(theAffineTransform[0][1]*0x10000);
   theMatrix.yy = (FT_Fixed)(theAffineTransform[1][1]*0x10000);
   
   int hasKerning = FT_HAS_KERNING(theFontFace);
   
   // load_flags = FT_LOAD_DEFAULT;
   // num_grays = 256;
   
   const char* c = s.c_str();
   for ( n = 0; n < (int)num_glyphs; n++)
   {
      long charOffset = c[n];
      
      FT_ULong charIndex = 0;

      if(charOffset < 0)
      {
         charIndex = (charOffset + 256);
      }
      else
      {
         charIndex = charOffset;
      }
      theStringLayout[n].glyph_index = FT_Get_Char_Index( theFontFace,
                                                          charIndex );
      theStringLayout[n].image = NULL;
      /* compute glyph origin */
      if (hasKerning&&theKerningEnabledFlag&&prev_index)
      {
         FT_Vector  kern;
         
         
         FT_Get_Kerning( theFontFace,
                         prev_index,
                         theStringLayout[n].glyph_index,
                         ft_kerning_default,
                         &kern );
         
         origin_x += (kern.x);
      }
      
      origin.x = origin_x;
      origin.y = 0;
      
      
      /* load the glyph image (in its native format); */
      /* for now, we take a monochrome glyph bitmap   */
      error = FT_Load_Glyph( theFontFace,
                             theStringLayout[n].glyph_index,
                             FT_LOAD_DEFAULT);
      if ( error )
      {
         continue;
      }
      
      error = FT_Get_Glyph ( theFontFace->glyph, &theStringLayout[n].image );
      
      if ( error )
      {
         continue;
      }
      
      theStringLayout[n].pos = origin;
      
      origin_x  += (theFontFace->glyph->advance.x);
      prev_index = theStringLayout[n].glyph_index;
   }
   
   theStringCenter.x = origin_x / 2;
   theStringCenter.y = 0;

   FT_Vector_Transform( &theStringCenter, &theMatrix );
   theNeedToLayoutGlyphsFlag = false;
}

const ossim_uint8* ossimFreeTypeFont::rasterize()
{
   allocateBuffer();
   setupForRasterization();
   layoutGlyphs(theStringToRasterize);

   int num_glyphs = (int)theStringLayout.size();
   int        n;
   FT_Vector  delta;
   int        error;
   
   /* first of all, we must compute the general delta for the glyph set */
   delta.x = -theStringCenter.x;
   delta.y = 0;

   memset(theOutputBuffer, 0, theBufferSize);
   for ( n = 0; n < num_glyphs; n++)
   {
      FT_Glyph   image;
      FT_Vector  vec;
      
      
      if ( !theStringLayout[n].image )
      {
         continue;
      }
      
      /* copy image */
      error = FT_Glyph_Copy( theStringLayout[n].image, &image );
      if ( error )
         continue;
      
      /* transform it */
      vec = theStringLayout[n].pos;
      FT_Vector_Transform( &vec, &theMatrix );
      vec.x += (delta.x);
      vec.y += (delta.y);
      error = FT_Glyph_Transform( image, &theMatrix, &vec );
      if ( !error )
      {
         FT_BBox  bbox;
         /* check bounding box; if it is not within the display surface, */
         /* we don't need to render it                                   */
         
         FT_Glyph_Get_CBox( image, ft_glyph_bbox_pixels, &bbox );

         // since freetype coordinates are right handed we will
         // adjust the rects so they are right handed before passing
         // them to our rendering system.
         //
         ossimIrect box(bbox.xMin,
                        bbox.yMax,
                        bbox.xMax,
                        bbox.yMin,
                        OSSIM_RIGHT_HANDED);
         ossimIrect bufRect;
         ossimIrect boundingBox;;
         getBufferRect(bufRect);
         getBoundingBox(boundingBox);
         
         bufRect = ossimIrect(bufRect.ul().x,
                              boundingBox.lr().y - bufRect.ul().y,
                              bufRect.lr().x,
                              boundingBox.lr().y - bufRect.lr().y,
                              OSSIM_RIGHT_HANDED);
         box += ossimIpt(-theShift.x,
                         -theShift.y);
         if(bufRect.intersects(box))
         {
            // now let's convert it to a bitmap
            error = FT_Glyph_To_Bitmap( &image,
                                        ft_render_mode_normal,
                                        0,
                                        1 );
            if ( !error )
            {
               // cast it
               FT_BitmapGlyph  bitmap = (FT_BitmapGlyph)image;

               ossimIpt ul(bitmap->left -theShift.x,
                           bitmap->top  -theShift.y);

               // the bounding char box does not coorespond to the
               // buffer rect of rfreetype's internal sorage.
               // We will need to adjust the box and then render it
               //
               box = ossimIrect(ul.x,
                                ul.y,
                                ul.x + bitmap->bitmap.width-1,
                                ul.y - (bitmap->bitmap.rows-1),
                                OSSIM_RIGHT_HANDED);

               // draw the sub rect within our buffer.
               drawBitmap(&bitmap->bitmap,
                          bufRect,
                          box);
            }
         }
      }
      FT_Done_Glyph( image );
    }
    
    return theOutputBuffer;    
}

void ossimFreeTypeFont::getBoundingBox(ossimIrect& box)
{

   if(theBoundingRectIsValid)
   {
      box = thePrecomputedBoundingRect;
      return;
   }
   setupForRasterization();
   layoutGlyphs(theStringToRasterize);

   int num_glyphs = (int)theStringLayout.size();
   int        n;
   FT_Vector  delta;
   int        error;
   bool firstBoxSet = false;
   /* first of all, we must compute the general delta for the glyph set */
   delta.x = -theStringCenter.x;
   delta.y = 0;
   
   for ( n = 0; n < num_glyphs; n++)
   {
      FT_Glyph   image;
      FT_Vector  vec;
      
      
      if ( !theStringLayout[n].image )
      {
         continue;
      }
      
      /* copy image */
      error = FT_Glyph_Copy( theStringLayout[n].image, &image );
      if ( error )
         continue;
      
      /* transform it */
      vec = theStringLayout[n].pos;
      FT_Vector_Transform( &vec, &theMatrix );
      vec.x += (delta.x);
      vec.y += (delta.y);
      error = FT_Glyph_Transform( image, &theMatrix, &vec );
      if ( !error )
      {
         FT_BBox  bbox;
         /* check bounding box; if it is not within the display surface, */
         /* we don't need to render it                                   */
         
         FT_Glyph_Get_CBox( image, ft_glyph_bbox_pixels, &bbox );
         ossimIrect charBox(bbox.xMin,
                            bbox.yMin,
                            bbox.xMax,
                            bbox.yMax);
            
            if(firstBoxSet)
            {
               box =  box.combine(charBox);
            }
            else
            {
               firstBoxSet = true;
               box = charBox;
            }
      }
      FT_Done_Glyph( image );
   }

   theShift = box.ul();

   box += ossimIpt(-theShift.x,
                   -theShift.y);

   thePrecomputedBoundingRect = box;
}

void ossimFreeTypeFont::computeGlyphBox(FT_Glyph /* slot */,
                                        ossimIrect& box)const
{
   long yBearing = theFontFace->glyph->metrics.horiBearingY/64;
   long h        = theFontFace->glyph->metrics.height/64;
   long w        = theFontFace->glyph->metrics.width/64;

   if(h&&w)
   {
      box = ossimIrect(0,
                       yBearing-(h-1),
                       (w-1),
                       yBearing);
   }
   else
   {
      box = ossimIrect(0,0,0,0);
   }   
}

void ossimFreeTypeFont::drawBitmap(FT_Bitmap* bitmap,
                                   const ossimIrect& bufRect,
                                   const ossimIrect& glyphBox)
{
   if(bufRect.intersects(glyphBox))
   {
      unsigned char* buf   = bitmap->buffer;
      if(!theOutputBuffer||!buf) return;
      
      ossimIrect intersectionRect = bufRect.clipToRect(glyphBox);
      

      long difInX           = intersectionRect.ul().x - glyphBox.ul().x;
      long difInY           = glyphBox.ul().y - intersectionRect.ul().y;
      
      long difOutX          = intersectionRect.ul().x - bufRect.ul().x;
      long difOutY          = bufRect.ul().y - intersectionRect.ul().y;

      
      
      long startInOffset    = bitmap->width*difInY + difInX;
      long startOutOffset   = bufRect.width()*difOutY + difOutX;
      
      long outBufW = bufRect.width();
      long inBufW  = bitmap->width;

      // let's just make sure that the increment will not exceed the input
      // buffers width and height.
      //
      long height  = std::min((long)intersectionRect.height(), (long)glyphBox.height());
      long width   = std::min((long)intersectionRect.width(), (long)glyphBox.width());
      
      for(long rows = 0 ; rows < height; ++rows)
      {
         for(long cols = 0; cols < width; ++cols)
         {
            if(buf[startInOffset+cols])
            {
               theOutputBuffer[startOutOffset + cols] = buf[startInOffset+cols];
            }
         }
         startInOffset+=inBufW;
         startOutOffset+=outBufW;
      }
   }
}

void ossimFreeTypeFont::setupForRasterization()
{
   int error=0;
   if(!theFontFace) return;

   if(!theFontFace->charmap)
   {
     error = FT_Select_Charmap(theFontFace,     /* target face object */
			       ft_encoding_big5 );   /* encoding..         */   
   }

   error = FT_Set_Char_Size(theFontFace,
                            theHorizontalPixelSize*64,
                            theVerticalPixelSize*64,
                            theHorizontalDeviceUnits,
                            theVerticalDeviceUnits);
   if(error)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimFreeTypeFont::setupForRasterization: Cant set the pixel size" <<std::endl;
   }
}

void ossimFreeTypeFont::setBaseClassInformation()
{
   if(theFontFace)
   {
      theFamilyName = theFontFace->family_name;

      if(isFixed())
      {
         vector<ossimIpt> initialPointList;
         getFixedSizes(initialPointList);

         theHorizontalPixelSize = initialPointList[0].x;
         theVerticalPixelSize   = initialPointList[0].y;
      }
      else
      {
         theHorizontalPixelSize = 8;
         theVerticalPixelSize   = 8;
      }
      if((theFontFace->style_flags & FT_STYLE_FLAG_BOLD) &&
         (theFontFace->style_flags&FT_STYLE_FLAG_ITALIC))
      {
         theStyleName = "bold italic";
      }
      else if(theFontFace->style_flags & FT_STYLE_FLAG_BOLD)
      {
         theStyleName = "bold";
      }
      else if(theFontFace->style_flags & FT_STYLE_FLAG_ITALIC)
      {
         theStyleName = "italic";
      }
      else
      {
         theStyleName = "normal";
      }
   }
}


void ossimFreeTypeFont::setCurrentFace(long index)
{
   int error;
   if(theFontFace)
   {
      if((index >0) && (index < theFontFace->num_faces))
      {
         FT_Done_Face(theFontFace);
         theFontFace = NULL;
         
         error = FT_New_Face( theLibrary,
                              theFontFile.c_str(),
                              index,
                              &theFontFace );
         if((error == FT_Err_Unknown_File_Format)||error)
         {
            setErrorStatus();
         }
         else
         {
            setBaseClassInformation();
         }
         theBoundingRectIsValid = false;
      }
   }
}

void ossimFreeTypeFont::getFixedSizes(vector<ossimIpt>& sizeArray)const
{
   if(theFontFace)
   {
      sizeArray.clear();
      int i = 0;

      for(i = 0; i < theFontFace->num_fixed_sizes; ++i)
      {
         sizeArray.push_back(ossimIpt(theFontFace->available_sizes[i].width,
                                      theFontFace->available_sizes[i].height));
      }
   }
}

void ossimFreeTypeFont::allocateBuffer()
{   
   ossimIrect box;
   setupForRasterization();
   getBoundingClipBox(box);
   long bufferSize = box.width()*box.height();
   if(bufferSize != theBufferSize)
   {
      theBufferSize = bufferSize;
      
      if(theOutputBuffer)
      {
         delete theOutputBuffer;
         theOutputBuffer = NULL;
      }
      if(!theOutputBuffer&&theBufferSize)
      {
         theOutputBuffer = new ossim_uint8[theBufferSize];            
      }
   }
}

void ossimFreeTypeFont::deleteGlyphs(vector<TGlyph>& glyphs)
{
   int i = 0;

   for(i = 0; i < (int)glyphs.size(); ++i)
   {
      if(glyphs[i].image)
      {
         FT_Done_Glyph(glyphs[i].image);
      }
   }
   
   glyphs.clear();
}

#endif /* #if OSSIM_HAS_FREETYPE */
