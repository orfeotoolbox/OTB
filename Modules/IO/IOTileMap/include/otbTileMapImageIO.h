/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTileMapImageIO_h
#define otbTileMapImageIO_h


/* C++ Libraries */
#include <string>
#include <vector>
//#include "stdlib.h"

/* ITK Libraries */
#include "otbImageIOBase.h"
#include "otbImageRegionTileMapSplitter.h"

#include "otbCurlHelperInterface.h"

#include "OTBIOTileMapExport.h"

namespace otb
{

namespace TileMapAddressingStyle
{
enum TileMapAddressingStyle {GM = 0, OSM = 1, NEARMAP = 2, LOCAL = 3};
}

/** \class TileMapImageIO
 *
 * \brief ImageIO object for reading and writing TileMap images
 *
 * \deprecated in OTB 7.0
 *
 * \ingroup OTBIOTileMap
 */
class OTBIOTileMap_DEPRECATED_EXPORT TileMapImageIO : public otb::ImageIOBase
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
  bool CanReadFile(const char*) override;

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  bool CanStreamRead() override
  {
    return true;
  }

  /** Set the spacing and dimension information for the set filename. */
  void ReadImageInformation() override;

  /** Reads the data from disk into the memory buffer provided. */
  void Read(void* buffer) override;

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanWriteFile(const char*) override;

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  bool CanStreamWrite() override
  {
    return true;
  }

  /** Writes the spacing and dimensions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  void WriteImageInformation() override;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  void Write(const void* buffer) override;

  /** Get the number of overviews available into the file specified
   *  This imageIO didn't support overviews */
  unsigned int GetOverviewsCount() override
  {
    // MANTIS-1154: Source image is always considered as the best
    // resolution overview.
    return 1;
  }
  
  /** Get information about overviews available into the file specified
   * This imageIO didn't support overviews */ 
  std::vector<std::string> GetOverviewsInfo() override
  {
    std::vector<std::string> desc;
    return desc;
  }
  
  /** Provide hist about the output container to deal with complex pixel
   *  type (Not used here) */ 
  void SetOutputImagePixelType( bool itkNotUsed(isComplexInternalPixelType), 
                                        bool itkNotUsed(isVectorImage)) override{}

protected:
  /** Constructor.*/
  TileMapImageIO();
  /** Destructor.*/
  ~TileMapImageIO() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Write all information on the image*/
  void InternalWriteImageInformation();

  unsigned int GetActualNumberOfSplitsForWritingCanStreamWrite(unsigned int numberOfRequestedSplits,
                                                                       const ImageIORegion& pasteRegion) const override;

  ImageIORegion GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece,
                                                               unsigned int numberOfActualSplits,
                                                               const ImageIORegion& pasteRegion) const override;

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

  TileMapImageIO(const Self &) = delete;
  void operator =(const Self&) = delete;

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
  TileMapAddressingStyle::TileMapAddressingStyle m_AddressMode;

  bool m_FlagWriteImageInformation;

  bool m_FileNameIsServerName;

  typedef otb::ImageRegionTileMapSplitter<2> SplitterType;
  SplitterType::Pointer m_TileMapSplitter;
};

} // end namespace otb

#endif // otbTileMapImageIO_h
