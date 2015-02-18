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
#ifndef __otbMultiChannelRAndGAndNIRIndexImageFilter_h
#define __otbMultiChannelRAndGAndNIRIndexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class MultiChannelRAndGAndNIRIndexImageFilter
   * \brief Implements multi channel R and G and NIR pixel-wise generic index operation on one vector image.
   *
   * This class is parameterized over the type of the input image and
   * the type of the output image.  It is also parameterized by the
   * operation to be applied, using a Functor style.
   *
   * \sa UnaryFunctorImageFilter
   * \ingroup Radiometry
 */
template <class TInputImage, class TOutputImage,
    class TFunction = Functor::AVI<typename TInputImage::InternalPixelType,
        typename TInputImage::InternalPixelType,
        typename TInputImage::InternalPixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT MultiChannelRAndGAndNIRIndexImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelRAndGAndNIRIndexImageFilter                            Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelRAndGAndNIRIndexImageFilter, UnaryFunctorImageFilter);

  /** Some typedefs. */
  typedef TFunction FunctorType;

  /** Set/Get the red channel index. Value must be in [1...[ */
  itkSetMacro(RedIndex, unsigned int);
  itkGetMacro(RedIndex, unsigned int);
  /** Set/Get the green channel index. Value must be in [1...[ */
  itkSetMacro(GreenIndex, unsigned int);
  itkGetMacro(GreenIndex, unsigned int);
  /** Set/Get the nir channel index. Value must be in [1...[ */
  itkSetMacro(NIRIndex, unsigned int);
  itkGetMacro(NIRIndex, unsigned int);

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_RedIndex = channel;
      }
    if (band == BandName::GREEN)
      {
      m_GreenIndex = channel;
      }
    if (band == BandName::NIR)
      {
      m_NIRIndex = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_RedIndex;
      }
    if (band == BandName::GREEN)
      {
      return m_GreenIndex;
      }
    if (band == BandName::NIR)
      {
      return m_NIRIndex;
      }
  }

protected:
  /// Constructor
  MultiChannelRAndGAndNIRIndexImageFilter() : m_RedIndex(3), m_GreenIndex(2), m_NIRIndex(4) {};
  /// Destructor
  virtual ~MultiChannelRAndGAndNIRIndexImageFilter() {}
  /// Before generating data, set functor parameters
  virtual void BeforeThreadedGenerateData()
  {

    unsigned int lNbChan = this->GetInput()->GetNumberOfComponentsPerPixel();
    if (m_RedIndex < 1 || m_GreenIndex < 1 || m_NIRIndex < 1 ||
        m_RedIndex > lNbChan || m_GreenIndex > lNbChan || m_NIRIndex > lNbChan)
      {
      itkExceptionMacro(<< "Channel indices must belong to range [1, ...[");
      }
    this->GetFunctor().SetRedIndex(m_RedIndex);
    this->GetFunctor().SetGreenIndex(m_GreenIndex);
    this->GetFunctor().SetNIRIndex(m_NIRIndex);
  }
  /// PrintSelf
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Red  index: " << m_RedIndex << std::endl;
    os << indent << "Green index: " << m_GreenIndex << std::endl;
    os << indent << "NIR  index: " << m_NIRIndex << std::endl;
  }

private:
  MultiChannelRAndGAndNIRIndexImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Red channel index */
  unsigned int m_RedIndex;
  /** Green channel index */
  unsigned int m_GreenIndex;
  /** NIR channel index */
  unsigned int m_NIRIndex;
};

} // end namespace otb

#endif
