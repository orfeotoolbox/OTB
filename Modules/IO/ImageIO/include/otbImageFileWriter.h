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

#ifndef otbImageFileWriter_h
#define otbImageFileWriter_h

#include "otbImageIOBase.h"
#include "itkProcessObject.h"
#include "otbStreamingManager.h"
#include "otbExtendedFilenameToWriterOptions.h"
#include "itkFastMutexLock.h"

namespace otb
{

/** \class ImageFileWriter
 * \brief Writes image data to a single file with streaming process.
 *
 * ImageFileWriter writes its input data to a single output file.
 * ImageFileWriter interfaces with an ImageIO class to write out the
 * data with streaming process.
 *
 * ImageFileWriter will divide the output into several pieces
 * (controlled by SetNumberOfDivisionsStrippedStreaming, SetNumberOfLinesStrippedStreaming,
 * SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming or SetAutomaticTiledStreaming),
 * and call the upstream pipeline for each piece, tiling the individual outputs into one large
 * output. This reduces the memory footprint for the application since
 * each filter does not have to process the entire dataset at once.
 *
 * ImageFileWriter will write directly the streaming buffer in the image file, so
 * that the output image never needs to be completely allocated
 *
 * ImageFileWriter supports extended filenames, which allow controlling
 * some properties of the output file. See
 * http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName for more
 * information.
 *
 * \sa ImageFileReader
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 * \sa ExtendedFilenameToReaderOptions
 *
 * \ingroup OTBImageIO
 */
template <class TInputImage>
class ITK_EXPORT ImageFileWriter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageFileWriter                                   Self;
  typedef itk::ProcessObject                                Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileWriter, itk::ProcessObject);

  /** Some typedefs for the input and output. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef typename InputImageType::IndexType     InputIndexType;
  typedef TInputImage                            OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;
  typedef typename Superclass::DataObjectPointer DataObjectPointer;

  /** The Filename Helper. */
  typedef ExtendedFilenameToWriterOptions            FNameHelperType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Streaming manager base class pointer */
  typedef StreamingManager<InputImageType>       StreamingManagerType;
  typedef typename StreamingManagerType::Pointer StreamingManagerPointerType;

  /**  Return the StreamingManager object responsible for dividing
   *   the region to write */
  StreamingManagerType* GetStreamingManager(void)
    {
    return m_StreamingManager;
    }

  /**  Set a user-specified implementation of StreamingManager
   *   used to divide the largest possible region in several divisions */
  void SetStreamingManager(StreamingManagerType* streamingManager)
    {
    m_StreamingManager = streamingManager;
    }

  /**  Set the streaming mode to 'stripped' and configure the number of strips
   *   which will be used to stream the image */
  void SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions);

  /**  Set the streaming mode to 'tiled' and configure the number of tiles
   *   which will be used to stream the image */
  void SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions);

  /**  Set the streaming mode to 'stripped' and configure the number of strips
   *   which will be used to stream the image with respect to a number of line
   *   per strip */
  void SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip);

  /**  Set the streaming mode to 'stripped' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option.
   *   The bias parameter is a multiplier applied on the estimated memory size
   *   of the pipeline and can be used to fine tune the potential gap between
   *   estimated memory and actual memory used, which can happen because of
   *   composite filters for example */
  void SetAutomaticStrippedStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /**  Set the streaming mode to 'tiled' and configure the dimension of the tiles
   *   in pixels for each dimension (square tiles will be generated) */
  void SetTileDimensionTiledStreaming(unsigned int tileDimension);

  /**  Set the streaming mode to 'tiled' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Tiles will be square.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option
   *   The bias parameter is a multiplier applied on the estimated memory size
   *   of the pipeline and can be used to fine tune the potential gap between
   *   estimated memory and actual memory used, which can happen because of
   *   composite filters for example */
  void SetAutomaticTiledStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /**  Set the streaming mode to 'adaptative' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Tiles will try to match the input file tile scheme.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticAdaptativeStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /** Set the only input of the writer */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType *input);

  /** Get writer only input */
  const InputImageType* GetInput();

  /** Override Update() from ProcessObject because this filter
   *  has no output. */
  void Update() override;

  /** ImageFileWriter Methods */
  virtual void SetFileName(const char* extendedFileName);
  virtual void SetFileName(std::string extendedFileName);
  virtual const char* GetFileName () const;

  /** Specify the region to write. If left NULL, then the whole image
   * is written. */
  void SetIORegion(const itk::ImageIORegion& region);
  itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

  /** Set the compression On or Off */
  itkSetMacro(UseCompression, bool);
  itkGetConstReferenceMacro(UseCompression, bool);
  itkBooleanMacro(UseCompression);

  /** By default the MetaDataDictionary is taken from the input image and
   *  passed to the ImageIO. In some cases, however, a user may prefer to
   *  introduce her/his own MetaDataDictionary. This is often the case of
   *  the ImageSeriesWriter. This flag defined whether the MetaDataDictionary
   *  to use will be the one from the input image or the one already set in
   *  the ImageIO object. */
  itkSetMacro(UseInputMetaDataDictionary, bool);
  itkGetConstReferenceMacro(UseInputMetaDataDictionary, bool);
  itkBooleanMacro(UseInputMetaDataDictionary);

  itkSetObjectMacro(ImageIO, otb::ImageIOBase);
  itkGetObjectMacro(ImageIO, otb::ImageIOBase);
  itkGetConstObjectMacro(ImageIO, otb::ImageIOBase);

  /** This override doesn't return a const ref on the actual boolean */
  const bool & GetAbortGenerateData() const override;

  void SetAbortGenerateData(const bool val) override;

protected:
  ImageFileWriter();
  ~ImageFileWriter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Does the real work. */
  void GenerateData(void) override;

  /** Prepare the streaming and write the output information on disk */
  void GenerateOutputInformation(void) override;

private:
  ImageFileWriter(const ImageFileWriter &) = delete;
  void operator =(const ImageFileWriter&) = delete;

  void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject & event )
  {
    if (typeid(event) != typeid(itk::ProgressEvent))
      {
      return;
      }

    itk::ProcessObject* processObject = dynamic_cast<itk::ProcessObject*>(object);
    if (processObject)
      {
      m_DivisionProgress = processObject->GetProgress();
      }

    this->UpdateFilterProgress();
  }

  void UpdateFilterProgress()
  {
    this->UpdateProgress( (m_DivisionProgress + m_CurrentDivision) / m_NumberOfDivisions );
  }

  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float m_DivisionProgress;

  /** ImageFileWriter Parameters */
  std::string m_FileName;

  otb::ImageIOBase::Pointer m_ImageIO;

  bool m_UserSpecifiedImageIO; //track whether the ImageIO is user specified

  itk::ImageIORegion m_IORegion;
  bool               m_UserSpecifiedIORegion; // track whether the region is user specified
  bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO
  bool m_UseCompression;
  bool m_UseInputMetaDataDictionary; // whether to use the
                                     // MetaDataDictionary from the
                                     // input or not.

  bool m_WriteGeomFile;              // Write a geom file to store the
                                     // kwl

  FNameHelperType::Pointer m_FilenameHelper;

  StreamingManagerPointerType m_StreamingManager;

  bool          m_IsObserving;
  unsigned long m_ObserverID;
  InputIndexType m_ShiftOutputIndex;

  /** Mapping between origin components and output components (before any
   * conversion) */
  std::vector<unsigned int> m_BandList;

  /** Store the number of components to be exported to the output image
   *  This variable can be the number of components in m_ImageIO or the
   *  number of components in the m_BandList (if used) */
  unsigned int m_IOComponents;

  /** Lock to ensure thread-safety (added for the AbortGenerateData flag) */
  itk::SimpleFastMutexLock m_Lock;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileWriter.hxx"
#endif

#endif
