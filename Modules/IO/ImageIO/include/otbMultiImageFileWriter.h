/*
 * Copyright (C) CS SI
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

#ifndef otbMultiImageFileWriter_h
#define otbMultiImageFileWriter_h

#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkImageBase.h"
#include "itkProcessObject.h"
#include "itkImageIOBase.h"

//#include "s2ipfImageRegionFixedSplitter.h"

#include <boost/shared_ptr.hpp>

namespace otb
{

/** \class MultiImageFileWriter
 *  \brief Streams a pipeline with multiple outputs.
 *  It optionally writes each output to a file, and can also separate each
 *  output into several granules files (use SetGranuleGenerationMode). Inputs
 *  are connected to the writer using the AddInput method.
 *  The streaming occurs by strips. Each output may have a different resolution,
 *  specified at the AddInput call, and the strip size is proportional to the
 *  resolution.
 */
class MultiImageFileWriter: public itk::ProcessObject
{
  friend class Sink;
public:
  /** Standard class typedefs. */
  typedef MultiImageFileWriter Self;
  typedef itk::ProcessObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(MultiImageFileWriter, itk::ProcessObject);

  /** Public typedefs */
  typedef itk::ImageBase<2> ImageBaseType;
  typedef ImageBaseType::RegionType RegionType;
  typedef ImageBaseType::IndexType  IndexType;
  typedef ImageBaseType::IndexValueType  IndexValueType;
  typedef ImageBaseType::SizeType   SizeType;
  typedef ImageBaseType::SizeValueType   SizeValueType;

  //typedef ImageRegionFixedSplitter<ImageBaseType::ImageDimension> SplitterType;

  /**  Return the StreamingManager object responsible for dividing
   *   the region to write */
  //~ StreamingManagerType* GetStreamingManager(void)
    //~ {
    //~ return m_StreamingManager;
    //~ }

  /**  Set a user-specified implementation of StreamingManager
   *   used to divide the largest possible region in several divisions */
  //~ void SetStreamingManager(StreamingManagerType* streamingManager)
    //~ {
    //~ m_StreamingManager = streamingManager;
    //~ }

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

  virtual void UpdateOutputData(itk::DataObject * itkNotUsed(output));

  /** Sets the number of lines used in stripped streaming for an input with
  resolutionFactor == 1.0 . This number is actually multiplied by the resolution
  factor to obtain the size of the strip for each input image. */
  //~ itkSetMacro(NumberOfLinesPerStrip, int);
  //~ itkGetMacro(NumberOfLinesPerStrip, int);

  /** Set the number of rows per granule for an image with resolution factor 1.0 */
  //~ itkSetMacro(NumberOfRowsPerGranule, int);
  //~ itkGetMacro(NumberOfRowsPerGranule, int);

  /** Specify whether granules are to be generated. If false, a single file is
  generated for each input */
  //~ itkSetMacro(GranuleGenerationMode, bool);
  //~ itkGetConstReferenceMacro(GranuleGenerationMode, bool);
  //~ itkBooleanMacro(GranuleGenerationMode);

  /** Set the compression On or Off */
  //~ itkSetMacro(UseCompression, bool);
  //~ itkGetConstReferenceMacro(UseCompression, bool);
  //~ itkBooleanMacro(UseCompression);

  /**
   * Enable/disable writing of a .geom file with the ossim keyword list along with the written image
   */
  //~ itkSetMacro(WriteGeomFile, bool);
  //~ itkGetMacro(WriteGeomFile, bool);
  //~ itkBooleanMacro(WriteGeomFile);

  /** Connect a new input to the multi-writer. Only the input pointer is
   *  required. If the filename list is empty,
   *  streaming will occur without writing. It the filename list contains more
   *  than one element, then the output will be divided into this number of
   *  granule files. The resolution factor specifies the ratio between the height of this image and the
   *  height of a reference image. The number of lines per strip class parameter will be modified according to this factor, so
   *  that images with different resolutions can be streamed together. For example, use 1.0 for a 10m pixels image, 0.5 for a 20m
   *  pixels image, and specify the number of lines per strip according to the 10m pixels image.
   *  You may specify top and bottom margins that will be removed from the input image, according to its largest possible region.
   */
  template <class TImage>
  void AddInputImage(const TImage* inputPtr, const std::string & fileName)
  {
    Sink<TImage> * sink = new Sink<TImage>(inputPtr, fileName);
    m_SinkList.push_back(SinkBase::Pointer(sink));
    unsigned int size = m_SinkList.size();
    this->SetNumberOfInputs(size);
    this->SetNthInput(size - 1, const_cast<itk::DataObject*>(dynamic_cast<const itk::DataObject*>(inputPtr)));
  }

  //~ /** Connect a new input to the multi-writer. Takes a single filename instead
  //~ of a list of filenames */
  //~ template <class TImage>
  //~ void AddInput(const TImage* inputPtr, const std::string & fileName = std::string(), double resolutionFactor = 1.0, int topMarginTrimSize = 0, int bottomMarginTrimSize = 0)
  //~ {
    //~ std::vector<std::string> fileNameList;
    //~ fileNameList.push_back(fileName);
    //~ this->AddInput(inputPtr, fileNameList, resolutionFactor, topMarginTrimSize, bottomMarginTrimSize);
  //~ }
//~ 
  //~ /** This version of AddInput takes no input filename */
  //~ template <class TImage>
  //~ void AddInput(const TImage* inputPtr, double resolutionFactor = 1.0, int topMarginTrimSize = 0, int bottomMarginTrimSize = 0)
  //~ {
    //~ this->AddInput(inputPtr, std::vector<std::string>(), resolutionFactor, topMarginTrimSize, bottomMarginTrimSize);
  //~ }

  virtual void UpdateOutputInformation();

  virtual void Update()
  {
    this->UpdateOutputInformation();
    this->UpdateOutputData(NULL);
  }

protected:
  /** SetInput is changed to protected. Use AddInput to connect the pipeline to
    * the writer
   */
//  virtual void SetInput(const ImageBaseType* image) { this->Superclass::SetInput(image); }

  /** SetInput is changed to protected. Use AddInput to connect the pipeline to
    * the writer
   */
//  virtual void SetInput(unsigned int i, const ImageBaseType* image) { this->Superclass::SetInput(i, image); }

  MultiImageFileWriter();
  virtual ~MultiImageFileWriter() {}

  /** GenerateData calls the Write method for each connected input */
  virtual void GenerateData(void);

  virtual void GenerateInputRequestedRegion();

  /** Computes the number of divisions */
  virtual void InitializeStreaming();

  /** Goes up the pipeline starting at imagePtr, resetting all requested regions to a null region */
  void ResetAllRequestedRegions(ImageBaseType* imagePtr);

  /** Returns the current stream region of the given input */
  virtual RegionType GetStreamRegion(int inputIndex);

  /** This is the number of lines used in stripped streaming for an input with
  resolutionFactor == 1.0 . This number is actually multiplied by the resolution
  factor to obtain the size of the strip for each input image. */
  //int m_NumberOfLinesPerStrip;

  void operator =(const MultiImageFileWriter&); //purposely not implemented

  void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject & event)
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
    this->UpdateProgress((m_DivisionProgress + m_CurrentDivision) / m_NumberOfDivisions);
  }

private:
  typedef otb::Image<unsigned char, 2> FakeOutputType;
  typedef StreamingManager<FakeOutputType>        StreamingManagerType;
  /** Streaming manager used for the N inputs */
  StreamingManagerType::Pointer m_StreamingManager;
  //Granule Generation mode
  //~ bool m_GranuleGenerationMode;

  //Division parameters
  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float m_DivisionProgress;

  bool m_IsObserving;
  unsigned long m_ObserverID;

  /** compression */
  //~ bool m_UseCompression;
  //~ bool m_UseInputMetaDataDictionary; // whether to use the
                                     // MetaDataDictionary from the
                                     // input or not.
  //~ bool m_WriteGeomFile; // Write a geom file to store the kwl

  /** Number of rows per granule for an image with resolution factor 1.0 (i.e. a
   * 10m band)
   */
  //~ int m_NumberOfRowsPerGranule;

  //~ SplitterType::Pointer m_Splitter;

  class SinkBase
  {
  public:
    SinkBase() {}
    SinkBase(ImageBaseType::ConstPointer inputImage) :
      m_InputImage(inputImage)
    {}
    virtual ~SinkBase() {}
    virtual ImageBaseType::ConstPointer GetInput() const { return m_InputImage; }
    virtual ImageBaseType::Pointer GetInput() { return const_cast<ImageBaseType*>(m_InputImage.GetPointer()); }
    virtual void WriteImageInformation() = 0;
    virtual void Write(const RegionType & streamRegion) = 0;
    virtual bool CanStreamWrite() = 0;
    typedef boost::shared_ptr<SinkBase> Pointer;
  protected:
    /** The image on which streaming is performed */
    ImageBaseType::ConstPointer m_InputImage;
  };

  /** \class Sink
   *  Parameters and methods specific to a single input
   */
  template <class TImage>
  class Sink : public SinkBase
  {
    //~ friend class MultiImageFileWriter;
  public:
    Sink() {}
    Sink(typename TImage::ConstPointer inputImage,
         const std::string & filename);

    virtual ~Sink() {}

    virtual void WriteImageInformation();
    virtual void Write(const RegionType & streamRegion);
    virtual bool CanStreamWrite();
    typedef boost::shared_ptr<Sink> Pointer;
  protected:
    //~ void CreateImageFile(int fileIndex);

    /** There may be several output filenames in the case of granule mode
     *  generation. Or there may be none if no output file is required.
     */
    //~ std::string m_FileName;

    /** Specifies whether image is actually written to a file */
    //~ bool m_UseImageIO;
  private:
    /** Actual writer for this image */
    typename otb::ImageFileWriter<TImage>::Pointer m_Writer;

    /** An ImageIO used to actually write data to a file */
    itk::ImageIOBase::Pointer m_ImageIO;

    /** The current file number into which data is written */
    //~ int m_CurrentFileIndex;

    /** The image region to write to the current file */
    //~ RegionType m_CurrentFileRegion;

    /** The height of the file, which can be the size of a granule in granule
    generation mode, or else the size of the input image (minus margins). */
    //~ int m_FileHeight;

    /** A pointer to the writer to gain access to its fields */
    //~ typename MultiImageFileWriter::Pointer m_Writer;
  };

  /** The list of inputs and their associated parameters, built using AddInput */
  typedef std::vector<boost::shared_ptr<SinkBase> > SinkListType;
  SinkListType m_SinkList;

  std::vector<RegionType> m_StreamRegionList;
};

} // end of namespace s2ipf

//~ #include "otbMultiImageFileWriter.txx"

#endif // otbMultiImageFileWriter_h
