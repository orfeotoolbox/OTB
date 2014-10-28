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
#ifndef __otbDescriptorsListSampleGenerator_h
#define __otbDescriptorsListSampleGenerator_h

#include <vector>

#include "itkVariableLengthVector.h"
#include "itkImageRegion.h"

#include "otbListSampleSource.h"
#include "itkListSample.h"
#include "otbVectorData.h"

#include "itkSimpleDataObjectDecorator.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

template <class TOutputPrecision>
class DefaultDescriptorsType
{
public:
  typedef itk::VariableLengthVector<TOutputPrecision> Type;
};

/** \class PersistentDescriptorsListSampleGenerator
 *  \brief [internal] Helper class for the implementation of DescriptorsListSampleGenerator
 *
 *  This class inherits PersistentImageFilter and provides the Reset/Synthesize functions,
 *  plus the ThreadedGenerateData function implementing the image function evaluation
 */
template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabel>
class ITK_EXPORT PersistentDescriptorsListSampleGenerator :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentDescriptorsListSampleGenerator        Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentDescriptorsListSampleGenerator, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                      InputImageType;
  typedef typename TInputImage::Pointer    InputImagePointer;
  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::SizeType   SizeType;
  typedef typename TInputImage::IndexType  IndexType;
  typedef typename TInputImage::PixelType  PixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Input VectorData */
  typedef TVectorData                                     VectorDataType;
  typedef typename VectorDataType::Pointer                VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType           VectorDataNodeType;
  typedef typename VectorDataType::DataNodePointerType    VectorDataNodePointerType;
  typedef typename VectorDataType::DataTreeType           VectorDataTreeType;
  typedef typename VectorDataType::DataTreePointerType    VectorDataTreePointerType;
  typedef typename VectorDataNodeType::PointType          VectorDataPointType;
  typedef typename VectorDataNodeType::LineType           VectorDataLineType;
  typedef typename VectorDataNodeType::PolygonType        VectorDataPolygonType;
  typedef itk::PreOrderTreeIterator<VectorDataTreeType>   VectorDataTreeIteratorType;

  /** Function type for descriptors extraction */
  typedef TFunctionType                                   DescriptorsFunctionType;
  typedef typename DescriptorsFunctionType::Pointer       DescriptorsFunctionPointerType;
  typedef typename DescriptorsFunctionType::InputType     DescriptorsFunctionPointType;

  typedef itk::ContinuousIndex
        <typename DescriptorsFunctionPointType::ValueType,
         itkGetStaticConstMacro(InputImageDimension)>     ContinuousIndexType;

  /** ListSample output */
  typedef TListSample                                     ListSampleType;
  typedef typename ListSampleType::Pointer                ListSamplePointerType;
  typedef typename ListSampleType::MeasurementVectorType  SampleMeasurementVectorType;
  typedef typename ListSampleType::MeasurementType        SampleMeasurementType;

  /** LabelListSample output */
  typedef TLabel                                          LabelType;
  typedef itk::FixedArray<LabelType,1>                    LabelSampleType;
  typedef itk::Statistics::ListSample<LabelSampleType>    LabelListSampleType;
  typedef typename LabelListSampleType::Pointer           LabelListSamplePointerType;
  typedef typename LabelListSampleType::MeasurementVectorType LabelMeasurementVectorType;
  typedef typename LabelListSampleType::MeasurementType   LabelMeasurementType;

  /** ListSamplePositions output */
  typedef std::vector<DescriptorsFunctionPointType>               SamplesPositionType;
  typedef itk::SimpleDataObjectDecorator<SamplesPositionType>     SamplesPositionObjectType;
  typedef typename SamplesPositionObjectType::Pointer             SamplesPositionObjectPointerType;

  void SetSamplesLocations(VectorDataType * input);
  VectorDataType * GetSamplesLocations(void);

  /** The function to evaluate */
  itkSetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetConstObjectMacro(DescriptorsFunction, DescriptorsFunctionType);

  /** Output sample list */
  ListSampleType*             GetListSample();

  /** Output label list */
  LabelListSampleType*        GetLabelListSample();

  /** Output sample position list */
  SamplesPositionType&             GetSamplesPositions();
  SamplesPositionObjectType*       GetSamplesPositionsObject();

  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  itk::DataObject::Pointer MakeOutput(unsigned int idx);

  void AllocateOutputs();
  void GenerateOutputInformation();
  void Reset(void);
  void Synthetize(void);

  void AddInput(itk::DataObject * dataObject)
  {
    Superclass::AddInput(dataObject);
  }

protected:
  PersistentDescriptorsListSampleGenerator();
  virtual ~PersistentDescriptorsListSampleGenerator();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateInputRequestedRegion();

  void BeforeThreadedGenerateData();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread,
                             itk::ThreadIdType threadId);

private:
  PersistentDescriptorsListSampleGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  bool IsInsideWithNeighborhoodRadius(const RegionType& region, const ContinuousIndexType &index) const
    {
    typedef typename RegionType::IndexType     IndexType;
    typedef typename IndexType::IndexValueType IndexValueType;
    typedef typename ContinuousIndexType::ValueType ContinuousIndexValueType;

    for(unsigned int i=0; i<ImageDimension; ++i)
      {
      if( itk::Math::RoundHalfIntegerUp<IndexValueType>(index[i]) < static_cast<IndexValueType>( region.GetIndex(i) ) + static_cast<IndexValueType>(m_NeighborhoodRadius) )
       //Comment this instruction after itkv4 migration (correct
       //usage of centered-pixel coordinates)
       //if( index[i] < static_cast<IndexValueType>( region.GetIndex(i) ) + m_NeighborhoodRadius )
        {
        return false;
        }
      // bound is the last valid pixel location
      const ContinuousIndexValueType bound = static_cast<ContinuousIndexValueType>(
          region.GetIndex(i) + region.GetSize(i) - 0.5);
      //Comment this instruction after itkv4 migration (correct
      //usage of centered-pixel coordinates)
      //const ContinuousIndexValueType bound = static_cast<ContinuousIndexValueType>(
      //    region.GetIndex(i) + static_cast<IndexValueType>(region.GetSize(i)) - 1);

      if( index[i] > bound - m_NeighborhoodRadius )
        {
        return false;
        }
      }
    return true;
    }
  typedef std::vector<ListSamplePointerType>      ListSampleArray;
  typedef std::vector<LabelListSamplePointerType> LabelListSampleArray;
  typedef std::vector<SamplesPositionType>        SamplesPositionArray;

  ListSampleArray                m_ThreadListSample;
  LabelListSampleArray           m_ThreadLabelListSample;
  SamplesPositionArray           m_ThreadSamplesPosition;

  DescriptorsFunctionPointerType m_DescriptorsFunction;

  unsigned int m_NeighborhoodRadius;
};


/** \class DescriptorsListSampleGenerator
 *  \brief This class generates a ListSample by computing local descriptors
 *  on an image at specified positions.
 *
 *  Given an image (by SetInputImage()) and a vector data containing points (by SetSamplesLocations()),
 *  this class evaluates an ImageFunction (set by SetDescriptorsFunction()) at each point of the vector data
 *  over the image.
 *  It generates a ListSample containing the descriptors values for each sample,
 *  along with a label ListSample containing the label associated with each sample.
 *  The label is read from the vector data using the data field "Class", read as an int
 *
 *  This class is streaming capable and multithreaded
 */
template <class TInputImage, class TVectorData, class TListSample, class TLabel, class TOutputPrecision = double, class TCoordRep = double>
class ITK_EXPORT DescriptorsListSampleGenerator :
    public PersistentFilterStreamingDecorator<
              PersistentDescriptorsListSampleGenerator< TInputImage,
                                                        TVectorData,
                                                        itk::FunctionBase< itk::Point<TCoordRep, 2>,
                                                                           typename DefaultDescriptorsType<TOutputPrecision>::Type >,
                                                        TListSample,
                                                        TLabel > >
{
public:
    /** Standard Self typedef */
    typedef DescriptorsListSampleGenerator      Self;
    typedef PersistentFilterStreamingDecorator
              < PersistentDescriptorsListSampleGenerator
                  <TInputImage,
                   TVectorData,
                   itk::FunctionBase< itk::Point<TCoordRep, 2>, typename DefaultDescriptorsType<TOutputPrecision>::Type >,
                   TListSample,
                   TLabel> >          Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Type macro */
    itkNewMacro(Self);

    /** Creation through object factory macro */
    itkTypeMacro(DescriptorsListSampleGenerator, PersistentFilterStreamingDecorator);

    typedef TInputImage                              InputImageType;
    typedef TVectorData                              InputVectorDataType;
    typedef TListSample                              ListSampleType;
    typedef TCoordRep                                CoordRepType;
    typedef TOutputPrecision                         OutputPrecision;

    /** The coordinates used when evaluating function */
    typedef itk::Point<TCoordRep, 2> PointType;

    /** The return value of the function */
    typedef typename DefaultDescriptorsType<TOutputPrecision>::Type DescriptorType;

    /** The function evaluated on the input image at locations specified by the vector data */
    typedef itk::FunctionBase<PointType, DescriptorType> DescriptorsFunctionType;
    typedef typename DescriptorsFunctionType::Pointer    DescriptorsFunctionPointerType;

    typedef typename Superclass::FilterType                           PersistentFilterType;
    typedef typename PersistentFilterType::LabelListSampleType        LabelListSampleType;
    typedef typename PersistentFilterType::SamplesPositionObjectType  SamplesPositionObjectType;
    typedef typename PersistentFilterType::SamplesPositionType        SamplesPositionType;


    /** Input image to extract feature */
    void SetInputImage(InputImageType * input)
    {
      this->GetFilter()->SetInput(input);
    }

    /** Input image to extract feature */
    InputImageType * GetInputImage()
    {
      return this->GetFilter()->GetInput();
    }

    void AddInput(itk::DataObject * dataObject)
      {
        this->GetFilter()->AddInput(dataObject);
      }

    /** Sample locations as a VectorData of points. The label is in the ClassKey feature */
    void SetSamplesLocations(InputVectorDataType * input)
    {
      this->GetFilter()->SetSamplesLocations(input);
    }

    /** Sample locations as a VectorData of points. The label is in the ClassKey feature  */
    InputImageType * GetSamplesLocations()
    {
      return this->GetFilter()->GetSamplesLocations();
    }

    /** The function to evaluate */
    void SetDescriptorsFunction(DescriptorsFunctionType * input)
    {
      this->GetFilter()->SetDescriptorsFunction(input);
    }

    /** The function to evaluate */
    DescriptorsFunctionType * GetDescriptorsFunction()
    {
      return this->GetFilter()->GetDescriptorsFunction();
    }

    /** Final sample list */
    ListSampleType*       GetListSample()
    {
      return this->GetFilter()->GetListSample();
    }

    /** Final label list */
    LabelListSampleType*        GetLabelListSample()
    {
      return this->GetFilter()->GetLabelListSample();
    }

    /** Final label list */
    SamplesPositionType&        GetSamplesPositions()
    {
      return this->GetFilter()->GetSamplesPositions();
    }

    SamplesPositionObjectType*  GetSamplesPositionsObject()
    {
      return this->GetFilter()->GetSamplesPositionsObject();
    }

    otbSetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);
    otbGetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);

  protected:
    /** Constructor */
    DescriptorsListSampleGenerator();

    /** Destructor */
    virtual ~DescriptorsListSampleGenerator();

  private:
    DescriptorsListSampleGenerator(const Self &); //purposely not implemented
    void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDescriptorsListSampleGenerator.txx"
#endif

#endif
