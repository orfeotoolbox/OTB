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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

/* C++ Libraries */
#include <string>
#include "stdlib.h"

/* ITK Libraries */
#include "itkImageIOBase.h"


namespace otb
{

/** \class TileMapImageIO
   *
   * \brief ImageIO object for reading (not writing) TileMap images
   *
   * The streaming read is implemented.
   *
   * \ingroup IOFilters
   *
 */
class ITK_EXPORT TileMapImageIO : public itk::ImageIOBase
{
public:

  typedef unsigned char InputPixelType;

  /** Standard class typedefs. */
  typedef TileMapImageIO            Self;
  typedef itk::ImageIOBase          Superclass;
  typedef itk::SmartPointer<Self>   Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapImageIO, itk::ImageIOBase);

  /** Set/Get the level of compression for the output images.
   *  0-9; 0 = none, 9 = maximum. */
  itkSetMacro(CompressionLevel, int);
  itkGetMacro(CompressionLevel, int);

  virtual void SetCacheDirectory (const char* _arg)
  {
    if ( _arg && (_arg == this->m_CacheDirectory) )
    {
      return;
    }
    if (_arg)
    {
      this->m_CacheDirectory = _arg;
      this->m_UseCache=true;
    }
    else
    {
      this->m_CacheDirectory = "";
      this->m_UseCache=false;
    }
    this->Modified();
  }

  virtual void SetCacheDirectory (const std::string & _arg)
  {
    this->SetCacheDirectory( _arg.c_str() );
    this->m_UseCache=true;
  }

  itkSetMacro(Depth, int);
  itkGetMacro(Depth, int);

  itkGetStringMacro(CacheDirectory);

  /*-------- This part of the interface deals with reading data. ------ */

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
  /** Construtor.*/
  TileMapImageIO();
  /** Destructor.*/
  virtual ~TileMapImageIO();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Write all information on the image*/
  void InternalWriteImageInformation();
  /** Number of bands of the image*/
  int m_NbBands;

  /** Determines the level of compression for written files.
   *  Range 0-9; 0 = none, 9 = maximum , default = 4 */
  int m_CompressionLevel;

private:
  TileMapImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void InternalRead(double x, double y, void* buffer);
  void InternalWrite(double x, double y, const void* buffer);
  void BuildFileName(const std::ostringstream& quad, std::ostringstream& filename) const;
  void RetrieveTile(const std::ostringstream & filename, std::ostringstream & urlStream) const;
  void GetFromNetGM(const std::ostringstream& filename, double x, double y) const;
  void GetFromNetOSM(const std::ostringstream& filename, double x, double y) const;
  void GetFromNetNearMap(const std::ostringstream& filename, double x, double y) const;
  void FillCacheFaults(void* buffer) const;
  int XYToQuadTree(double x, double y, std::ostringstream& quad) const;
  int XYToQuadTree2(double x, double y, std::ostringstream& quad) const;


  /** Byte per pixel pixel */
  int         m_NbOctetPixel;

  /** Resolution depth*/
  int         m_Depth;
  bool        m_UseCache;
  std::string m_CacheDirectory;
  std::string m_ServerName;
  std::string m_FileSuffix;
  std::string m_AddressMode;

  bool        m_FlagWriteImageInformation;

};

} // end namespace otb

#endif // __otbTileMapImageIO_h
