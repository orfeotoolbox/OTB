#ifndef _otbNCCRegistrationFilter_h_
#define _otbNCCRegistrationFilter_h_

#include "itkPDEDeformableRegistrationFilter.h"
#include "itkNCCRegistrationFunction.h"

namespace otb {

/** \class NCCRegistrationFilter
 * \brief Deformably register two images using the PDEs and NCC.
 *
 * NCCRegistrationFilter implements the ####### 
 * register two images by computing the deformation field which will map a 
 * moving image onto a fixed image.
 *
 * A deformation field is represented as a image whose pixel type is some
 * vector type with at least N elements, where N is the dimension of
 * the fixed image. The vector type must support element access via operator
 * []. It is assumed that the vector elements behave like floating point
 * scalars.
 *
 * This class is templated over the fixed image type, moving image type
 * and the deformation field type.
 *
 * The input fixed and moving images are set via methods SetFixedImage
 * and SetMovingImage respectively. An initial deformation field maybe set via
 * SetInitialDeformationField or SetInput. If no initial field is set,
 * a zero field is used as the initial condition.
 *
 * The algorithm has one parameter: the number of iteration to be performed.
 *
 * The output deformation field can be obtained via methods GetOutput
 * or GetDeformationField.
 *
 * This class make use of the finite difference solver hierarchy. Update
 * for each iteration is computed in NCCRegistrationFunction.
 *
 * \warning This filter assumes that the fixed image type, moving image type
 * and deformation field type all have the same number of dimensions.
 * 
 * \sa NCCRegistrationFunction 
 * \ingroup DeformableImageRegistration MultiThreaded
 */
template<class TFixedImage, class TMovingImage, class TDeformationField>
class ITK_EXPORT NCCRegistrationFilter : 
    public itk::PDEDeformableRegistrationFilter< TFixedImage, TMovingImage,
                                            TDeformationField>
{
public:
  /** Standard class typedefs. */
  typedef NCCRegistrationFilter    Self;
  typedef itk::PDEDeformableRegistrationFilter<
    TFixedImage, TMovingImage,TDeformationField>    Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( NCCRegistrationFilter, 
                itk::PDEDeformableRegistrationFilter );

  /** Inherit types from superclass. */
  typedef typename Superclass::TimeStepType  TimeStepType;

  /** FixedImage image type. */
  typedef typename Superclass::FixedImageType   FixedImageType;
  typedef typename Superclass::FixedImagePointer  FixedImagePointer;

  /** MovingImage image type. */
  typedef typename Superclass::MovingImageType    MovingImageType;
  typedef typename Superclass::MovingImagePointer  MovingImagePointer;
  
  /** Deformation field type. */
  typedef typename Superclass::DeformationFieldType 
  DeformationFieldType;
  typedef typename Superclass::DeformationFieldPointer  
  DeformationFieldPointer;

  /** FiniteDifferenceFunction type. */
  typedef typename Superclass::FiniteDifferenceFunctionType
  FiniteDifferenceFunctionType;

  /** NCCRegistrationFilterFunction type. */
  typedef itk::NCCRegistrationFunction<FixedImageType,MovingImageType,
                                     DeformationFieldType>  NCCRegistrationFunctionType;

  typedef typename NCCRegistrationFunctionType::RadiusType RadiusType;
  
  /** Get the metric value. The metric value is computed over the the
   * overlapping region between the two images.  This is value is only
   * available for the previous iteration and NOT the current
   * iteration. */
  virtual double GetMetric() const;


  virtual void SetNCCRadius(RadiusType radius);
  virtual RadiusType GetNCCRadius() const;
  
protected:
  NCCRegistrationFilter();
  ~NCCRegistrationFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Initialize the state of filter and equation before each iteration. */
  virtual void InitializeIteration();

  /** Apply update. */
  virtual void ApplyUpdate(TimeStepType dt);

private:
  NCCRegistrationFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};


} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbNCCRegistrationFilter.txx"
#endif

#endif
