/*
 * Copyright (C) 2017-2019 CS Systemes d'Information (CS SI)
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
#include "OTBImageIOExport.h"

#include <boost/shared_ptr.hpp>

namespace otb
{

/** \class MultiImageFileWriter
 *  \brief Streams a pipeline with multiple outputs.
 *  It writes each output to a file. Inputs
 *  are connected to the writer using the AddInputImage method.
 *  The type of streaming can be chosen. Each output may have a different size.
 *  When the user gives a number of lines per strip or a tile size, the value
 *  is interpreted on the first input to deduce the number of streams. This
 *  number of streams is then used to split the other inputs.
 *
 * \ingroup OTBImageIO
 */
class OTBImageIO_EXPORT MultiImageFileWriter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef MultiImageFileWriter          Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(MultiImageFileWriter, itk::ProcessObject);

  /** Public typedefs */
  typedef itk::ImageBase<2>             ImageBaseType;
  typedef ImageBaseType::RegionType     RegionType;
  typedef ImageBaseType::IndexType      IndexType;
  typedef ImageBaseType::IndexValueType IndexValueType;
  typedef ImageBaseType::SizeType       SizeType;
  typedef ImageBaseType::SizeValueType  SizeValueType;

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

  virtual void UpdateOutputData(itk::DataObject* itkNotUsed(output)) override;

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
  void AddInputImage(const TImage* inputPtr, const std::string& fileName);


  /** Add a new ImageFileWriter to the multi-writer. This is an alternative method
   *  when you already have an instanciated writer.
   */
  template <class TWriter>
  void AddInputWriter(typename TWriter::Pointer writer);

  virtual void UpdateOutputInformation() override;

  virtual void Update() override
  {
    this->UpdateOutputInformation();
    this->UpdateOutputData(NULL);
  }

protected:
  MultiImageFileWriter();
  virtual ~MultiImageFileWriter()
  {
  }

  /** GenerateData calls the Write method for each connected input */
  virtual void GenerateData(void) override;

  /** GenerateInputRequestedRegion can predict approximate input regions
   *  based on the requested region of the fake output. Only useful for
   *  pipeline memory print estimation
   */
  virtual void GenerateInputRequestedRegion() override;

  /** Computes the number of divisions */
  virtual void InitializeStreaming();

  /** Goes up the pipeline starting at imagePtr, resetting all requested regions
   *  to a null region. This may be useful when mixing inputs of different
   *  resolutions. */
  void ResetAllRequestedRegions(ImageBaseType* imagePtr);

  /** Returns the current stream region of the given input */
  virtual RegionType GetStreamRegion(int inputIndex);

  void operator=(const MultiImageFileWriter&) = delete;

  void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject& event)
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
  typedef StreamingManager<FakeOutputType> StreamingManagerType;
  /** Streaming manager used for the N inputs */
  StreamingManagerType::Pointer m_StreamingManager;

  // Division parameters
  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float        m_DivisionProgress;

  bool          m_IsObserving;
  unsigned long m_ObserverID;

  /** \class SinkBase
   * Internal base wrapper class to handle each ImageFileWriter
   *
   * \ingroup OTBImageIO
   */
  class SinkBase
  {
  public:
    SinkBase()
    {
    }
    SinkBase(ImageBaseType::ConstPointer inputImage) : m_InputImage(inputImage)
    {
    }
    virtual ~SinkBase()
    {
    }
    virtual ImageBaseType::ConstPointer GetInput() const
    {
      return m_InputImage;
    }
    virtual ImageBaseType::Pointer GetInput()
    {
      return const_cast<ImageBaseType*>(m_InputImage.GetPointer());
    }
    virtual void WriteImageInformation()                 = 0;
    virtual void Write(const RegionType& streamRegion)   = 0;
    virtual bool                        CanStreamWrite() const = 0;
    typedef boost::shared_ptr<SinkBase> Pointer;


    virtual itk::ImageRegion<2> GetRegionToWrite() const = 0;

  protected:
    /** The image on which streaming is performed */
    ImageBaseType::ConstPointer m_InputImage;
  };

  /** \class Sink
   *  Wrapper class for each ImageFileWriter
   *
   * \ingroup OTBImageIO
   */
  template <class TImage>
  class Sink : public SinkBase
  {
  public:
    Sink()
    {
    }
    Sink(typename TImage::ConstPointer inputImage, const std::string& filename);
    Sink(typename otb::ImageFileWriter<TImage>::Pointer writer);

    virtual ~Sink()
    {
    }

    void WriteImageInformation() override;
    void Write(const RegionType& streamRegion) override;
    bool                    CanStreamWrite() const override;
    typedef boost::shared_ptr<Sink> Pointer;
    
    /** Get the region that should be written. By default this is the largest possible region
     * of the input image, but this might be overriden by the box extended filename parameter of 
     * the input writer */
    itk::ImageRegion<2> GetRegionToWrite() const override;
  
  private:
    /** Actual writer for this image */
    typename otb::ImageFileWriter<TImage>::Pointer m_Writer;

    /** An ImageIO used to actually write data to a file */
    otb::ImageIOBase::Pointer m_ImageIO;
  };

  /** The list of inputs and their associated parameters, built using AddInput */
  typedef std::vector<boost::shared_ptr<SinkBase>> SinkListType;
  SinkListType                                     m_SinkList;

  std::vector<RegionType> m_StreamRegionList;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiImageFileWriter.hxx"
#endif

#endif // otbMultiImageFileWriter_h
