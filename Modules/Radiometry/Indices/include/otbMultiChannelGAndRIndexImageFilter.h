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
#ifndef __otbMultiChannelGAndRIndexImageFilter_h
#define __otbMultiChannelGAndRIndexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbSoilIndicesFunctor.h"

namespace otb
{

/** \class MultiChannelGAndRIndexImageFilter
   * \brief Implements multi channel G and R  pixel-wise generic index operation on one vector image.
   *
   * This class is parameterized over the type of the input image and
   * the type of the output image.  It is also parameterized by the
   * operation to be applied, using a Functor style.
   *
   * \sa UnaryFunctorImageFilter
   * \ingroup Radiometry
 */
template <class TInputImage, class TOutputImage,
    class TFunction = Functor::IR<typename TInputImage::InternalPixelType,
        typename TInputImage::InternalPixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT MultiChannelGAndRIndexImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelGAndRIndexImageFilter                                  Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelGAndRIndexImageFilter, UnaryFunctorImageFilter);

  /** Some typedefs. */
  typedef TFunction FunctorType;

  /** Set/Get the Green channel index. Value must be in [1...[ */
  itkSetMacro(GreenIndex, unsigned int);
  itkGetMacro(GreenIndex, unsigned int);
  /** Set/Get the Red channel index. Value must be in [1...[ */
  itkSetMacro(RedIndex, unsigned int);
  itkGetMacro(RedIndex, unsigned int);

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
  }

protected:
  /// Constructor
  MultiChannelGAndRIndexImageFilter() : m_GreenIndex(1), m_RedIndex(2) {};
  /// Destructor
  virtual ~MultiChannelGAndRIndexImageFilter() {}
  /// Before generating data, set functor parameters
  virtual void BeforeThreadedGenerateData()
  {
    unsigned int lNbChan = this->GetInput()->GetNumberOfComponentsPerPixel();
    if (m_GreenIndex < 1 || m_RedIndex < 1 ||
        m_GreenIndex > lNbChan || m_RedIndex > lNbChan)
      {
      itkExceptionMacro(<< "Channel indices must belong to range [1, ...[");
      }
    this->GetFunctor().SetGreenIndex(m_GreenIndex);
    this->GetFunctor().SetRedIndex(m_RedIndex);
  }
  /// PrintSelf Method
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Green index: " << m_GreenIndex << std::endl;
    os << indent << "Red index: " << m_RedIndex << std::endl;
  }
private:
  MultiChannelGAndRIndexImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** Green channel index */
  unsigned int m_GreenIndex;
  /** Red channel index */
  unsigned int m_RedIndex;
};
} // end namespace otb

#endif
