/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   17 janvier 2005
  Version   :   
  Role      :   Classe IO image pour le format GDAL
  $Id$

=========================================================================*/
#ifndef __otbGDALImageIO_h
#define __otbGDALImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

/* C++ Libraries */
#include <string>
#include "stdlib.h"

/* ITK Libraries */
#include "itkImageIOBase.h"

/* GDAL Libraries */
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_string.h"
#include "cpl_conv.h"


namespace otb
{

/** Classe otbGDALImageIO
    Based on itkImageIO, this class is add to all the IO image type
    ITK already contains (PNG, GIF, ...).
    Basically, to add and itkImageIO type, we need to set the methods:
    - CanReadFile(): that says if the file loaded can be read or not.
    - Read(): that read image put it into a buffer.
    - ReadImageInformation(): that should read spacing, origin,
    dimensions, pixel type...
    If we also need to write images of this new type, we need to set
    the methods:
    - CanWriteFile(): that says if this type of file can be write.
    - WriteImageInformation(): that write all image informations.
    - Write(): that writes the image.
*/
class ITK_EXPORT GDALImageIO : public itk::ImageIOBase
{
public:

  typedef unsigned char InputPixelType;
  /** GDAL parameters. */
  GDALDataset* poDataset;
  GDALRasterBand** poBands;
  GDALDataType PxType;
  
  /** Standard class typedefs. */
  typedef GDALImageIO            Self;
  typedef itk::ImageIOBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, itk::ImageIOBase);

  /** Set/Get the level of compression for the output images.
   *  0-9; 0 = none, 9 = maximum. */
  itkSetMacro(CompressionLevel, int);
  itkGetMacro(CompressionLevel, int);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);
  
  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();
 
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);
    
  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);

  void SampleImage(void* buffer,int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample);
  
protected:
  /** Construtor.*/
  GDALImageIO();
  /** Destructor.*/
  ~GDALImageIO();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Dimension along Ox of the image*/
  int width;
  /** Dimension along Oy of the image*/
  int height;
  /** Number of bands of the image*/
  int NbBands;
  /** Buffer*/
  //float **pafimas;
  
  /** Determines the level of compression for written files. 
   *  Range 0-9; 0 = none, 9 = maximum , default = 4 */
  int m_CompressionLevel;
  const char* m_currentfile;

private:
  GDALImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace otb

#endif // __otbGDALImageIO_h
