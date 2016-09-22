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

#ifndef otbShiftScaleImageAdaptor_h
#define otbShiftScaleImageAdaptor_h

#include "vnl/vnl_math.h"
#include "itkImageAdaptor.h"

namespace otb
{

namespace Accessor
{
/** \class ShiftScalePixelAccessor
 * \brief Apply a shift scale operation to the value
 *
 * ShiftScalePixelAccessor shifts the input pixel by Shift
 * and then scales the pixel by Scale. All computations
 * are performed in the precision of the input pixel's RealType
 *
 * ShiftScalePixelAccessor is templated over an internal type and an
 * external type representation. This class cast the input
 * applies the function to it and cast the result according
 * to the types defined as template parameters
 *
 * \ingroup ImageAdaptors
 *
 * \ingroup OTBImageManipulation
 */
template <class TPixelType, class TExternalType>
class ITK_EXPORT ShiftScalePixelAccessor
{
public:
  /** External typedef. It defines the external aspect
   * that this class will exhibit. */
  typedef TExternalType ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data. */
  typedef typename itk::NumericTraits<TPixelType>::RealType InternalType;

  InternalType GetShift() const
  {
    return m_Shift;
  }
  void SetShift(InternalType value)
  {
    m_Shift = value;
  }

  InternalType GetScale() const
  {
    return m_Scale;
  }
  void SetScale(InternalType value)
  {
    m_Scale = value;
  }

  inline void Set(InternalType& output, const ExternalType& input)
  {
    output = static_cast<InternalType>(m_Scale * (static_cast<InternalType>(input) + m_Shift));
  }

  inline const ExternalType Get(const InternalType& input) const
  {
    return static_cast<ExternalType>(m_Scale * (static_cast<InternalType>(input) + m_Shift));
  }

private:
  InternalType m_Shift;
  InternalType m_Scale;

};

} // end namespace Accessor

/** \class ShiftScaleImageAdaptor
 * \brief Presents an image as being composed of the shift scale operation of its pixels
 *
 * Additional casting is performed according to the input and output image
 * types following C++ default casting rules.
 *
 * \ingroup ImageAdaptors
 *
 * \ingroup OTBImageManipulation
 */
template <class TImage, class TOutputPixelType>
class ITK_EXPORT ShiftScaleImageAdaptor : public
  itk::ImageAdaptor<TImage, Accessor::ShiftScalePixelAccessor<
          typename TImage::PixelType,
          TOutputPixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ShiftScaleImageAdaptor Self;
  typedef itk::ImageAdaptor<
      TImage,
      Accessor::ShiftScalePixelAccessor<typename TImage::PixelType, TOutputPixelType>
      >                                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef typename TImage::PixelType     InternalType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Accessor::ShiftScalePixelAccessor<
      typename TImage::PixelType,
      TOutputPixelType>                 AccessorType;
  typedef typename AccessorType::ExternalType PixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShiftScaleImageAdaptor, ImageAdaptor);

  typename TImage::PixelType GetShift() const
  {
    itkDebugMacro("returning " << " m_Shift of " << this->GetPixelAccessor().GetShift());
    return this->GetPixelAccessor().GetShift();
  }

  virtual void SetShift(typename TImage::PixelType value)
  {
    itkDebugMacro("setting m_Shift to " << value);
    if (this->GetPixelAccessor().GetShift() != value)
      {
      this->GetPixelAccessor().SetShift(value);
      this->Modified();
      }
  }

  typename TImage::PixelType GetScale() const
  {
    itkDebugMacro("returning " << " m_Scale of " << this->GetPixelAccessor().GetScale());
    return this->GetPixelAccessor().GetScale();
  }

  virtual void SetScale(typename TImage::PixelType value)
  {
    itkDebugMacro("setting m_Scale to " << value);
    if (this->GetPixelAccessor().GetScale() != value)
      {
      this->GetPixelAccessor().SetScale(value);
      this->Modified();
      }
  }

protected:
  ShiftScaleImageAdaptor() {}
  ~ShiftScaleImageAdaptor() ITK_OVERRIDE {}

private:
  ShiftScaleImageAdaptor(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
