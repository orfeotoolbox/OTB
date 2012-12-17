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
#ifndef __otbImageFileWriter_h
#define __otbImageFileWriter_h

#include "itkImageFileWriter.h"
#include "otbImageIOFactory.h"
#include "otbStreamingTraits.h"
#include "otbExtendedFilenameToWriterOptions.h"

namespace otb
{

/** \class ImageFileWriter
 * \brief This class writes image file format (a variety of formats are supported)
 *
 * \sa ExtendedFilenameToWriterOptions
 * \sa ImageSeriesWriter
 * \sa ImageIOBase
 *
 * \ingroup IO
 *
 */

template <class TInputImage>
class ITK_EXPORT ImageFileWriter : public itk::ImageFileWriter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageFileWriter                   Self;
  typedef itk::ImageFileWriter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileWriter, itk::ImageFileWriter);

  /** Some convenient typedefs. */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Streaming traits helper typedef */
  typedef StreamingTraits<InputImageType> StreamingTraitsType;

  /** SmartPointer to a region splitting object */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)> SplitterType;
  typedef typename SplitterType::Pointer                                        RegionSplitterPointer;

  /** Type use to define number of divisions */
  typedef StreamingMode CalculationDivisionEnumType;

  /** The Filename Helper. */
  typedef ExtendedFilenameToWriterOptions            FNameHelperType;

  /** A special Write() method. It invokes the creating of the
    * specific otb::ImageIOFactory.
    */
  virtual void Write(void);

  /**  Set buffer memory size (in bytes) use to calculate the number of stream divisions */
  void SetBufferMemorySize(unsigned long);

  /**  Set the buffer number of lines use to calculate the number of stream divisions */
  void SetBufferNumberOfLinesDivisions(unsigned long);

  /**  Set the number of stream divisions */
  void SetNumberOfStreamDivisions(unsigned long nb_divisions);

  /** Get the number of stream divisions computed */
  unsigned long GetNumberOfStreamDivisions(void);

  /**  The number of stream divisions is calculate by using
   * OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING and
   * OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING cmake variables.
   */
  void SetAutomaticNumberOfStreamDivisions(void);

  /** Set the tiling automatic mode for streaming division */
//  void SetTilingStreamDivisions(void);
/** Choose number of divisions in tiling streaming division */
//  void SetTilingStreamDivisions(unsigned long);

  /** Return the string to indicate the method use to calculate number of stream divisions. */
  std::string GetMethodUseToCalculateNumberOfStreamDivisions(void);

  virtual void SetFileName(const char* extendedFileName);
  virtual void SetFileName(std::string extendedFileName);
  virtual const char* GetFileName () const;

  /**
   * Enable/disable writing of a .geom file with the ossim keyword list along with the written image
   */
  itkLegacyMacro( itkSetMacro(WriteGeomFile, bool) );
  itkLegacyMacro( itkGetMacro(WriteGeomFile, bool) );
  itkLegacyMacro( itkBooleanMacro(WriteGeomFile) );

protected:
  ImageFileWriter();
  virtual ~ImageFileWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFileWriter(const Self &); //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  /** This method calculate the number of stream divisions, by using the CalculationDivision type */
  unsigned long CalculateNumberOfStreamDivisions(void);

  /** Use to define the method used to calculate number of divisions */
  unsigned long m_BufferMemorySize;
  unsigned long m_BufferNumberOfLinesDivisions;

  RegionSplitterPointer m_RegionSplitterUseToEstimateNumberOfStreamDivisions;

  /** Use to determine method of calculation number of divisions */
  CalculationDivisionEnumType m_CalculationDivision;

  FNameHelperType::Pointer m_FilenameHelper;

  bool m_WriteGeomFile;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileWriter.txx"
#endif

#endif // __otbImageFileWriter_h
