/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbChangeLabelImageFilter_h
#define otbChangeLabelImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkConceptChecking.h"
#include "itkSimpleDataObjectDecorator.h"

#include <map>

namespace otb
{

/** \class ChangeLabelImageFilter
 *
 * \brief Change Sets of Labels
 *
 * Adapt the itkChangeLabelImageFilter for multichannel images.
 *
 *
 * \ingroup OTBImageManipulation
 */

namespace Functor {

template<class TInput, class TOutput>
class VectorChangeLabel
{
public:
  typedef typename TOutput::ValueType ValueType;

  VectorChangeLabel() {}
  virtual ~VectorChangeLabel() {}

  typedef std::map<TInput, TOutput> ChangeMapType;

  void SetNumberOfComponentsPerPixel(unsigned int nb)
  {
    m_NumberOfComponentsPerPixel = nb;
  }
  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }
  bool operator !=(const VectorChangeLabel& other) const
  {
    if (m_ChangeMap != other.m_ChangeMap)
      {
      return true;
      }
    return false;
  }
  bool operator ==(const VectorChangeLabel& other) const
  {
    return !(*this != other);
  }
  TOutput GetChange(const TInput& original)
  {
    return m_ChangeMap[original];
  }

  void SetChange(const TInput& original, const TOutput& result)
  {
    m_ChangeMap[original] = result;
  }

  void SetChangeMap(const ChangeMapType& changeMap)
  {
    m_ChangeMap = changeMap;
  }

  void ClearChangeMap()
  {
    m_ChangeMap.clear();
  }

  inline TOutput operator ()(const TInput& A)
  {
    TOutput out;
    out.SetSize(m_NumberOfComponentsPerPixel);

    if (m_ChangeMap.find(A) != m_ChangeMap.end())
      {
      out = m_ChangeMap[A];
      }
    else
      {
      out.Fill(static_cast<ValueType>(A));
      }
    return out;
  }

private:
  ChangeMapType m_ChangeMap;
  unsigned int  m_NumberOfComponentsPerPixel;
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ChangeLabelImageFilter :
  public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::VectorChangeLabel<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ChangeLabelImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::VectorChangeLabel<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType>
      >                                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ChangeLabelImageFilter, itk::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Type of the change map to use for change requests */
  typedef std::map<InputPixelType, OutputPixelType> ChangeMapType;

  /** Set up a change of a single label */
  void SetChange(const InputPixelType& original, const OutputPixelType& result);

  /** Set the entire change map */
  void SetChangeMap(const ChangeMapType& changeMap);

  /** Clears the entire change map */
  void ClearChangeMap();

  /** Set/Get the number of components per pixel */
  void SetNumberOfComponentsPerPixel(unsigned int nb)
  {
    m_NumberOfComponentsPerPixel = nb;
    this->GetFunctor().SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
    this->Modified();
  }
  itkGetMacro(NumberOfComponentsPerPixel, unsigned int);

protected:
  ChangeLabelImageFilter();
  ~ChangeLabelImageFilter() override {}
  /** Generate the output information missing */
  void GenerateOutputInformation() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ChangeLabelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /// Number of components per pixel
  unsigned int m_NumberOfComponentsPerPixel;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbChangeLabelImageFilter.hxx"
#endif

#endif
