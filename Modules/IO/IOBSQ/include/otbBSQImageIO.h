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
#ifndef __otbBSQImageIO_h
#define __otbBSQImageIO_h

#include <fstream>
#include <string>
#include <vector>

#include "otbImageIOBase.h"

namespace otb
{

/** \class BSQImageIO
 *
 * \brief ImageIO object for reading (not writing) BSQ format images
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBIOBSQ
 */
class ITK_EXPORT BSQImageIO : public otb::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef BSQImageIO              Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSQImageIO, otb::ImageIOBase);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanReadFile(const char*) ITK_OVERRIDE;

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  bool CanStreamRead() ITK_OVERRIDE
  {
    return true;
  }

  /** Set the spacing and dimention information for the set filename. */
  void ReadImageInformation() ITK_OVERRIDE;

  /** Reads the data from disk into the memory buffer provided. */
  void Read(void* buffer) ITK_OVERRIDE;

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanWriteFile(const char*) ITK_OVERRIDE;

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  bool CanStreamWrite() ITK_OVERRIDE
  {
    return true;
  }

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  void WriteImageInformation() ITK_OVERRIDE;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  void Write(const void* buffer) ITK_OVERRIDE;

  // JULIEN: NOT USED, NOT IMPLEMENTED
  // void SampleImage(void* buffer, int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample);
  
  /** Get the number of overviews available into the file specified
   *  This imageIO didn't support overviews */
  unsigned int GetOverviewsCount() ITK_OVERRIDE
  {
    // MANTIS-1154: Source image is always considered as the best
    // resolution overview.
    return 1;
  }
  
  /** Get information about overviews available into the file specified
   * This imageIO didn't support overviews */ 
  std::vector<std::string> GetOverviewsInfo() ITK_OVERRIDE
  {
    std::vector<std::string> desc;
    return desc;
  }
  
  /** Provide hist about the output container to deal with complex pixel
   *  type (Not used here) */ 
  void SetOutputImagePixelType( bool itkNotUsed(isComplexInternalPixelType), 
                                        bool itkNotUsed(isVectorImage)) ITK_OVERRIDE{}

protected:
  /** Constructor.*/
  BSQImageIO();
  /** Destructor.*/
  ~BSQImageIO() ITK_OVERRIDE;

  bool OpenOneraDataFileForReading(const char* filename);
  bool OpenOneraHeaderFileForReading(const char* filename);
  void InternalReadImageInformation();

  bool OpenOneraDataFileForWriting(const char* filename);
  bool OpenOneraHeaderFileForWriting(const char* filename);

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  BSQImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Internal method to read header information */
  bool InternalReadHeaderInformation(const std::string& file_name, std::fstream& file, const bool reportError);

#define otbSwappFileOrderToSystemOrderMacro(StrongType, buffer, buffer_size) \
    { \
    typedef itk::ByteSwapper<StrongType> InternalByteSwapperType; \
    if (m_ByteOrder != m_FileByteOrder) \
      { \
      if (m_ByteOrder == LittleEndian) \
        { \
        InternalByteSwapperType::SwapRangeFromSystemToBigEndian((StrongType *) buffer, buffer_size); \
        } \
      else if (m_ByteOrder == BigEndian) \
        { \
        InternalByteSwapperType::SwapRangeFromSystemToLittleEndian((StrongType *) buffer, buffer_size); \
        } \
      } \
    }

#define otbSwappFileToSystemMacro(StrongType, WeakType, buffer, buffer_size) \
  else if (this->GetComponentType() == WeakType) \
    { \
    otbSwappFileOrderToSystemOrderMacro(StrongType, buffer, buffer_size) \
    }

#define otbSetTypeBsqMacro(WeakType, CAI_VALUE) \
  else if (this->GetComponentType() == WeakType) \
    { \
    m_TypeBsq = CAI_VALUE; \
    }

  bool                        m_FlagWriteImageInformation;
  otb::ImageIOBase::ByteOrder m_FileByteOrder;
  std::fstream                m_HeaderFile;
  std::string                 m_TypeBsq;
  std::vector<std::string>    m_ChannelsFileName;
  std::fstream * m_ChannelsFile;

};

} // end namespace otb

#endif // __otbBSQImageIO_h
