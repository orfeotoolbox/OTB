/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLUMImageIO_h
#define otbLUMImageIO_h

#include "otbImageIOBase.h"
#include "OTBIOLUMExport.h"
#include <string>
#include <vector>
#include <fstream>

namespace otb
{

/** \class LUMImageIO
 *
 * \brief ImageIO object for reading (not writing) LUM format images
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBIOLUM
 */
class OTBIOLUM_EXPORT LUMImageIO : public otb::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef LUMImageIO              Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LUMImageIO, otb::ImageIOBase);

  /*-------- This part of the interface deals with reading data. ------ */

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

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

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
  // JULIEN: NOT USED, NOT IMPLEMENTED
  //void SampleImage(void* buffer, int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample);

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
  LUMImageIO();
  /** Destructor.*/
  ~LUMImageIO() override;

  bool OpenOneraDataFileForReading(const char* filename);
  bool OpenOneraHeaderFileForReading(const char* filename);
  void InternalReadImageInformation();

  bool OpenOneraDataFileForWriting(const char* filename);
  bool OpenOneraHeaderFileForWriting(const char* filename);

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LUMImageIO(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Internal method to read header information */
  bool InternalReadHeaderInformation(std::fstream& file, const bool reportError);
  /** This method get the LUM type */
  int CaiGetTypeLum(const char *          type_code,
                    std::string&   str_sens_code,
                    int&           inbbits,
                    std::string&   str_cod_pix);

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

#define otbSetTypeLumMacro(WeakType, CAI_VALUE_BE, CAI_VALUE_LE) \
  else if (this->GetComponentType() == WeakType) \
    { \
    if (m_ByteOrder == LittleEndian) \
      { \
      m_TypeLum = CAI_VALUE_LE; \
      } \
    else \
      { \
      m_TypeLum = CAI_VALUE_BE; \
      } \
    }

  bool                        m_FlagWriteImageInformation;
  std::vector<std::string>    m_CaiLumTyp;     //used for read
  std::string                 m_TypeLum; //used for write
  otb::ImageIOBase::ByteOrder m_FileByteOrder;
  std::fstream                m_File;

};

} // end namespace otb

#endif // otbLUMImageIO_h
