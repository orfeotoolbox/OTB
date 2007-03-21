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
#ifndef __otbStreamingImageFileWriter_h
#define __otbStreamingImageFileWriter_h

#include "itkMacro.h"
#include "itkImageIOBase.h"
#include "itkImageSource.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

/** \class StreamingImageFileWriter
 * \brief Writes image data to a single file with streaming process.
 *
 * StreamingImageFileWriter writes its input data to a single output file.
 * StreamingImageFileWriter interfaces with an ImageIO class to write out the
 * data whith streaming process.
 *
 * StreamingImageFileWriter will divide the output into severalpieces 
 * (controlled by SetNumberOfStreamDivisions), and call the upstream
 * pipeline for each piece, tiling the individual outputs into one large
 * output. This reduces the memory footprint for the application since
 * each filter does not have to process the entire dataset at once.
 * StreamingImageFileWriter will write directly the streaming buffer in the image file.
 * The output image is not completely allocated ; just streaming size, 
 * calculate whith the NumberOfStreamDivisions, is allocate.
 *
 * \sa ImageFileWriter
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 */
template <class TInputImage>
class ITK_EXPORT StreamingImageFileWriter : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageFileWriter  Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageFileWriter,itk::ImageToImageFilter);

  /** Some typedefs for the input and output. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType; 
  typedef typename InputImageType::PixelType InputImagePixelType; 
  typedef TInputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType; 
  typedef typename OutputImageType::PixelType OutputImagePixelType; 
  typedef typename Superclass::DataObjectPointer DataObjectPointer;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** SmartPointer to a region splitting object */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)>  SplitterType;
  typedef typename SplitterType::Pointer RegionSplitterPointer;
  
  /**  Set buffer memory size (in bytes) use to calculate the number of stream divisions */
  void SetBufferMemorySize(unsigned long );
 
  /**  Set the buffer number of lines use to calculate the number of stream divisions */
  void SetBufferNumberOfLinesDivisions(unsigned long);
 
  /**  The number of stream divisions is calculate by using 
   * OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING and 
   * OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING cmake variables.
   */
  void SetAutomaticNumberOfStreamDivisions(void);

  /** Return the string to indicate the method use to calculate number of stream divisions. */
  std::string GetMethodUseToCalculateNumberOfStreamDivisions(void);

  /** Set the number of pieces to divide the input.  The upstream pipeline
   * will be executed this many times. */
  void SetNumberOfStreamDivisions(unsigned long);
  
  /** Get the number of pieces to divide the input. The upstream pipeline
   * will be executed this many times. */
  unsigned long GetNumberOfStreamDivisions(void);

  /** Set the helper class for dividing the input into chunks. */
  itkSetObjectMacro(RegionSplitter, SplitterType);

  /** Get the helper class for dividing the input into chunks. */
  itkGetObjectMacro(RegionSplitter, SplitterType);

  /** Override UpdateOutputData() from ProcessObject to divide upstream
   * updates into pieces. This filter does not have a GenerateData()
   * or ThreadedGenerateData() method.  Instead, all the work is done
   * in UpdateOutputData() since it must update a little, execute a little,
   * update some more, execute some more, etc. */
  virtual void UpdateOutputData(itk::DataObject *itkNotUsed(output));


  /** ImageFileWriter Methods */
  
  /** Specify the name of the output file to write. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);
  
  /** Specify the region to write. If left NULL, then the whole image
   * is written. */
  void SetIORegion(const itk::ImageIORegion & region);
  itkGetConstReferenceMacro( IORegion, itk::ImageIORegion );

  /** Set the compression On or Off */
  itkSetMacro(UseCompression,bool);
  itkGetConstReferenceMacro(UseCompression,bool);
  itkBooleanMacro(UseCompression);

  /** By default the MetaDataDictionary is taken from the input image and 
   *  passed to the ImageIO. In some cases, however, a user may prefer to 
   *  introduce her/his own MetaDataDictionary. This is often the case of
   *  the ImageSeriesWriter. This flag defined whether the MetaDataDictionary 
   *  to use will be the one from the input image or the one already set in
   *  the ImageIO object. */
  itkSetMacro(UseInputMetaDataDictionary,bool);
  itkGetConstReferenceMacro(UseInputMetaDataDictionary,bool);
  itkBooleanMacro(UseInputMetaDataDictionary);

  itkSetObjectMacro(ImageIO, itk::ImageIOBase);
  itkGetObjectMacro(ImageIO, itk::ImageIOBase);
  itkGetConstObjectMacro(ImageIO, itk::ImageIOBase);

  /** Type use to define number of divisions */
  typedef enum
  { 
  	SET_NUMBER_OF_STREAM_DIVISIONS = 1,
  	SET_BUFFER_MEMORY_SIZE = 2,
  	SET_BUFFER_NUMBER_OF_LINES = 3,
  	SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS = 4
  } CalculationDivisionEnumType;

protected:
  StreamingImageFileWriter();
  ~StreamingImageFileWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Does the real work. */
  void GenerateData(void);

private:
  StreamingImageFileWriter(const StreamingImageFileWriter&); //purposely not implemented
  void operator=(const StreamingImageFileWriter&); //purposely not implemented
  
  /** This method calculate the number of stream divisions, by using the CalculationDivision type */
  unsigned long CalculateNumberOfStreamDivisions(void);
  
  /** Use to define the method used to calculate number of divisions */ 
  unsigned long m_BufferMemorySize;
  unsigned long m_BufferNumberOfLinesDivisions;
  unsigned long m_NumberOfStreamDivisions;
  
  RegionSplitterPointer m_RegionSplitter;
 
  /** Use to determine method of calculation number of divisions */
  CalculationDivisionEnumType m_CalculationDivision;
  
  /** ImageFileWriter Parameters */
  std::string        m_FileName;
  
  itk::ImageIOBase::Pointer m_ImageIO;
  
  bool m_UserSpecifiedImageIO; //track whether the ImageIO is user specified
  
  itk::ImageIORegion m_IORegion;
  bool m_UserSpecifiedIORegion; //
                                //track whether the region is user specified
  bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO
  bool m_UseCompression;
  bool m_UseInputMetaDataDictionary; // whether to use the MetaDataDictionary from the input or not.

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageFileWriter.txx"
#endif

#endif
