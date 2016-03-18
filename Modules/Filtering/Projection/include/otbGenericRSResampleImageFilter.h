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
#ifndef __otbGenericRSResampleImageFilter_h
#define __otbGenericRSResampleImageFilter_h

#include "otbStreamingResampleImageFilter.h"
#include "otbPhysicalToRPCSensorModelImageFilter.h"

namespace otb
{

/** \class GenericRSResampleImageFilter
 *  \brief This class is a composite filter that allows you to project
 *  an input image from any  coordinate system to any other one. The
 *  coordinate systems can be  defined by their projection reference,
 *  the keyword list or a meta data dictionary.
 *
 *  This class uses the otb::StreamingResampleImageFilter. It defines
 *  and uses a otb::GenericRSTransform using the input/output coordinate
 *  system information listed below. This class can resample the input to an
 *  output image with the Size/Origin/Spacing/StartIndex defined by
 *  the user. Note that there are no default values for all the
 *  parmeters, so it is mandatory to set correct parameters to have a
 *  correct result.
 *
 *  The methods SetOutputParmatersFromMap can estimate the output
 *  image parameters Size/Origin/Spacing so the hole image can be
 *  reprojected without setting any output parameter.
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBProjection
 **/

template <class TInputImage, class TOutputImage>
class ITK_EXPORT GenericRSResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GenericRSResampleImageFilter                          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericRSResampleImageFilter, itk::ImageToImageFilter);

  /** Typedef parameters*/
  typedef TInputImage                                     InputImageType;
  typedef TOutputImage                                    OutputImageType;
  typedef typename OutputImageType::InternalPixelType     OutputInternalPixelType;
  typedef typename OutputImageType::PointType             OutputPointType;

  /** Internal filters typedefs*/
  typedef StreamingResampleImageFilter<InputImageType,
                                 OutputImageType>          ResamplerType;
  typedef typename ResamplerType::Pointer                  ResamplerPointerType;
  typedef typename ResamplerType::TransformType            TransformType;
  typedef typename ResamplerType::SizeType                 SizeType;
  typedef typename ResamplerType::SpacingType              SpacingType;
  typedef typename ResamplerType::OriginType               OriginType;
  typedef typename ResamplerType::IndexType                IndexType;
  typedef typename ResamplerType::RegionType               RegionType;
  typedef typename ResamplerType::InterpolatorType         InterpolatorType;

  /** Estimate the rpc model */
  typedef PhysicalToRPCSensorModelImageFilter<InputImageType>  InputRpcModelEstimatorType;
  typedef typename InputRpcModelEstimatorType::Pointer         InputRpcModelEstimatorPointerType;

  typedef PhysicalToRPCSensorModelImageFilter<OutputImageType> OutputRpcModelEstimatorType;
  typedef typename OutputRpcModelEstimatorType::Pointer        OutputRpcModelEstimatorPointerType;


  /** Specialisation of OptResampleFilter with a remote
    * sensing  transform
    */
  typedef GenericRSTransform<>                       GenericRSTransformType;
  typedef typename GenericRSTransformType::Pointer   GenericRSTransformPointerType;

  typedef itk::ImageBase<OutputImageType::ImageDimension>      ImageBaseType;

  /** The Displacement field spacing & size */
  otbSetObjectMemberMacro(Resampler, DisplacementFieldSpacing, SpacingType);

  otbGetObjectMemberConstReferenceMacro(Resampler,
                                        DisplacementFieldSpacing,
                                        SpacingType);

  /** The resampled image parameters */
  /** Output Origin */
  void SetOutputOrigin(const OriginType & origin)
  {
    m_Resampler->SetOutputOrigin(origin);
    this->Modified();
  }
  otbGetObjectMemberConstReferenceMacro(Resampler, OutputOrigin, OriginType);

  /** Output Start index */
  otbSetObjectMemberMacro(Resampler, OutputStartIndex, IndexType);
  otbGetObjectMemberConstReferenceMacro(Resampler, OutputStartIndex, IndexType);

  /** Output Size */
  otbSetObjectMemberMacro(Resampler, OutputSize, SizeType);
  otbGetObjectMemberConstReferenceMacro(Resampler, OutputSize, SizeType);

  /** Output Spacing */
  otbSetObjectMemberMacro(Resampler, OutputSpacing, SpacingType);
  otbGetObjectMemberConstReferenceMacro(Resampler, OutputSpacing, SpacingType);

  /** Methods to Set/Get the interpolator */
  void SetInterpolator(InterpolatorType * interpolator)
  {
    m_Resampler->SetInterpolator(interpolator);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(Resampler, Interpolator, const InterpolatorType *);

  /** Default Edge padding value */
  otbSetObjectMemberMacro(Resampler,
                          EdgePaddingValue,
                          typename OutputImageType::PixelType);
  otbGetObjectMemberMacro(Resampler,
                          EdgePaddingValue,
                          typename OutputImageType::PixelType);

  /**
   * Set/Get input & output projections.
   * Set/Get input & output keywordlist
   * The macro are not used here cause the input and the output are
   * inversed.
   */
  void SetInputProjectionRef(const std::string&  ref)
  {
    m_Transform->SetOutputProjectionRef(ref);
    this->Modified();
  }

  std::string GetInputProjectionRef() const
  {
    return m_Transform->GetOutputProjectionRef();
  }

  void SetOutputProjectionRef(const std::string&  ref)
  {
  m_Transform->SetInputProjectionRef(ref);
  this->Modified();
  }

  std::string GetOutputProjectionRef() const
  {
    return m_Transform->GetInputProjectionRef();
  }

  /** Set/Get Input Keywordlist*/
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetOutputKeywordList(kwl);
    this->Modified();
  }
  const ImageKeywordlist GetInputKeywordList()
  {
    return m_Transform->GetOutputKeywordList();
  }

  /** Set/Get output Keywordlist*/
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetInputKeywordList(kwl);
    this->Modified();
  }

  const ImageKeywordlist GetOutputKeywordList()
  {
    return m_Transform->GetInputKeywordList();
  }

  /** Useful to set the output parameters from an existing image*/
  void SetOutputParametersFromImage(const ImageBaseType * image);

  /** Useful to set output parmaters form an existing image with type
    * different from input or ouptut image
    */
  template <class TImageType> void SetOutputParametersFromImage(const TImageType * image);

  /** Useful to set the output parameters using a map (UTM|WGS84) and
   * a spacing set by the user
   */
  void SetOutputParametersFromMap(const std::string map, const SpacingType& spacing);

  /** Useful to set the output parameters from a projectionRef, the
    * output size, spacing and origin are estimated
    */
  void SetOutputParametersFromMap(const std::string projectionRef);

  /** Set/Get the grid size for rpc estimator*/
  void SetInputRpcGridSize(const SizeType& gridSize)
  {
    m_InputRpcEstimator->SetGridSize(gridSize);
    this->Modified();
  }
  /** unsigned int as paramater */
  void SetInputRpcGridSize(unsigned int gridSize)
  {
    m_InputRpcEstimator->SetGridSize(gridSize);
    this->Modified();
  }
  /** Get the input rpc model estimator  grid size used */
  const SizeType & GetInputRpcGridSize() const
  {
    return m_InputRpcEstimator->GetGridSize();
  }

  /** Macro to tune the EstimateInputRpcModel flag */
  itkSetMacro(EstimateInputRpcModel, bool);
  itkGetMacro(EstimateInputRpcModel, bool);
  itkBooleanMacro(EstimateInputRpcModel);

  /** Macro to Set/Get the grid size for rpc estimator*/
  void SetOutputRpcGridSize(const SizeType& gridSize)
  {
    m_OutputRpcEstimator->SetGridSize(gridSize);
    this->Modified();
  }
  /** unsigned int as paramater */
  void SetOutputRpcGridSize(unsigned int gridSize)
  {
    m_OutputRpcEstimator->SetGridSize(gridSize);
    this->Modified();
  }
  /** Get the output rpc model estimator grid size used */
  const SizeType & GetOutputRpcGridSize() const
  {
    return m_OutputRpcEstimator->GetGridSize();
  }

  /** Macro to tune the EstimateOutputRpcModel flag */
  itkSetMacro(EstimateOutputRpcModel, bool);
  itkGetMacro(EstimateOutputRpcModel, bool);
  itkBooleanMacro(EstimateOutputRpcModel);

  /** Set number of threads for Displacement field generator */
  void SetDisplacementFilterNumberOfThreads(unsigned int nbThread)
  {
    m_Resampler->SetDisplacementFilterNumberOfThreads(nbThread);
  }

  /** Override itk::ProcessObject method to let the internal filter do the propagation */
  virtual void PropagateRequestedRegion(itk::DataObject *output);

protected:
  GenericRSResampleImageFilter();
  /** Destructor */
  virtual ~GenericRSResampleImageFilter() {};

  virtual void GenerateData();

  virtual void GenerateOutputInformation();

  virtual void UpdateTransform();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GenericRSResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Method to estimate the input & output rpc model
  void EstimateOutputRpcModel();
  void EstimateInputRpcModel();

  // boolean that allow the estimation of the input rpc model
  bool                               m_EstimateInputRpcModel;
  bool                               m_EstimateOutputRpcModel;
  bool                               m_RpcEstimationUpdated;

  // Filters pointers
  ResamplerPointerType               m_Resampler;
  InputRpcModelEstimatorPointerType  m_InputRpcEstimator;
  OutputRpcModelEstimatorPointerType m_OutputRpcEstimator;
  GenericRSTransformPointerType      m_Transform;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSResampleImageFilter.txx"
#endif

#endif
