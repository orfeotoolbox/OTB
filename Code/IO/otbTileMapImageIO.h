/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTileMapImageIO_h
#define __otbTileMapImageIO_h


/* C++ Libraries */
#include <string>
#include <vector>
//#include "stdlib.h"

/* ITK Libraries */
#include "otbMacro.h"
#include "otbImageIOBase.h"
#include "otbImageRegionTileMapSplitter.h"

#include "otbCurlHelperInterface.h"

namespace otb
{

namespace TileMapAdressingStyle
{
enum TileMapAdressingStyle {GM = 0, OSM = 1, NEARMAP = 2, LOCAL = 3};
}

/** \class TileMapImageIO
   *
   * \brief ImageIO object for reading and writing TileMap images
   *
   *
 */
class ITK_EXPORT TileMapImageIO : public otb::ImageIOBase
{
public:

  typedef unsigned char InputPixelType;

  /** Standard class typedefs. */
  typedef TileMapImageIO          Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapImageIO, otb::ImageIOBase);

  typedef itk::ImageIORegion ImageIORegion;

  /** Set/Get the level of compression for the output images.
   *  0-9; 0 = none, 9 = maximum. */
  itkSetMacro(CompressionLevel, int);
  itkGetMacro(CompressionLevel, int);

  /** Set/Get the maximum number of connections */
  itkSetMacro(MaxConnect, int);
  itkGetMacro(MaxConnect, int);

  virtual void SetCacheDirectory(const char* _arg);
  virtual void SetCacheDirectory(const std::string& _arg);

  itkGetStringMacro(CacheDirectory);

  itkSetMacro(Depth, int);
  itkGetMacro(Depth, int);

  itkGetStringMacro(FileSuffix);
  itkSetStringMacro(FileSuffix);

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead()
  {
    return true;
  }

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite()
  {
    return true;
  }

  /** Writes the spacing and dimensions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);

protected:
  /** Constructor.*/
  TileMapImageIO();
  /** Destructor.*/
  virtual ~TileMapImageIO();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Write all information on the image*/
  void InternalWriteImageInformation();

  virtual unsigned int GetActualNumberOfSplitsForWritingCanStreamWrite(unsigned int numberOfRequestedSplits,
                                                                       const ImageIORegion& pasteRegion) const;

  virtual ImageIORegion GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece,
                                                               unsigned int numberOfActualSplits,
                                                               const ImageIORegion& pasteRegion) const;

  /** Number of bands of the image*/
  int m_NbBands;

  /** Determines the level of compression for written files.
   *  Range 0-9; 0 = none, 9 = maximum , default = 4 */
  int m_CompressionLevel;

private:
  /** Struct to save filename & tile associates */
  typedef struct
  {
    int numTileX;
    int numTileY;
    double x;
    double y;
    std::string filename;
  } TileNameAndCoordType;

  TileMapImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void InternalWrite(double x, double y, const void* buffer);
  void BuildFileName(const std::ostringstream& quad, std::ostringstream& filename, bool inCache = true) const;
  void FillCacheFaults(void* buffer) const;
  int XYToQuadTree(double x, double y, std::ostringstream& quad) const;
  int XYToQuadTree2(double x, double y, std::ostringstream& quad) const;

  /** CURL Multi */
  void GenerateTileInfo(double x, double y, int numTileX, int numTileY);
  bool CanReadFromCache(const std::string& filename);
  void GenerateURL(double x, double y);
  void GenerateBuffer(unsigned char * p);
  void ReadTile(const std::string& filename, void * buffer);

  std::vector<std::string>          m_ListFilename;
  std::vector<std::string>          m_ListURLs;
  std::vector<TileNameAndCoordType> m_ListTiles;
  int                               m_MaxConnect;

  CurlHelperInterface::Pointer      m_Curl;

  /** Byte per pixel pixel */
  int m_BytePerPixel;

  /** Tile size (heavily tested for the default 256 only) */
  int m_TileSize;

  /** Resolution depth*/
  int                                          m_Depth;
  bool                                         m_UseCache;
  std::string                                  m_CacheDirectory;
  std::string                                  m_ServerName;
  std::string                                  m_FileSuffix;
  TileMapAdressingStyle::TileMapAdressingStyle m_AddressMode;

  bool m_FlagWriteImageInformation;

  bool m_FileNameIsServerName;

  typedef otb::ImageRegionTileMapSplitter<2> SplitterType;
  SplitterType::Pointer m_TileMapSplitter;
};

} // end namespace otb

#endif // __otbTileMapImageIO_h
