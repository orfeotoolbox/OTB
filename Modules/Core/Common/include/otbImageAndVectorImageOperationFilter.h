/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


#ifndef otbImageAndVectorImageOperationFilter_h
#define otbImageAndVectorImageOperationFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkImageToImageFilter.h"

#include "otbMacro.h"

namespace otb
{
namespace Functor
{
template <typename TInput, typename TVectorInput, typename TOutput>
class ITK_EXPORT ImageAndVectorImageOperationFunctor
{
public:
  typedef typename TVectorInput::ValueType                        InternalInputPixelType;
  typedef typename TOutput::ValueType                             InternalOutputPixelType;
  typedef enum {MULTIPLICATION, ADDITION, DIVISION, SUBTRACTION } OperatorType;

  ImageAndVectorImageOperationFunctor()
    {
    m_Operator = ADDITION;  //1;
    };
  virtual ~ImageAndVectorImageOperationFunctor(){}

  void SetOperator(OperatorType oper)
  {
    m_Operator = oper;
  }
  OperatorType GetOperator()
  {
    return m_Operator;
  }

  inline TOutput operator ()(const TInput& inPix, const TVectorInput& vInPix)
  {
    TOutput out;
    out.SetSize(vInPix.Size());
    TVectorInput vInTmp = vInPix;

    switch (m_Operator)
      {
      case MULTIPLICATION:
        {
        vInTmp *= static_cast<InternalInputPixelType>(inPix);
        break;
        }
      case ADDITION:
        {
        vInTmp += static_cast<InternalInputPixelType>(inPix);
        break;
        }
      case DIVISION:
        {
        if (inPix != 0) vInTmp /= static_cast<InternalInputPixelType>(inPix);
        else
          {
          vInTmp.Fill(0);
          }
        break;
        }
      case SUBTRACTION:
        {
        vInTmp -= static_cast<InternalInputPixelType>(inPix);
        break;
        }
      default:
        {
        }
      }

    for (unsigned int i = 0; i < vInTmp.Size(); ++i)
      {
      out[i] = static_cast<InternalInputPixelType>(vInTmp[i]);
      }
    return out;
  }

protected:
  OperatorType m_Operator;
};
}

/** \class ImageAndVectorImageOperationFilter
 * \brief Provides simple pixel to pixel operation between Image and VectorImage.
 *
 * Apply an operation (multiplication, division, addition or subtraction) between
 * the input image and each channel of the vector input image.
 * Use SetOperation( MULTIPLICATION, ADDITION, DIVISION or SUBTRACTION ) to select the wanted operation.
 * Default is an addition.
 *
 * This class is templated over the input Image and VectorImage and output VectorImage types.
 *
 * \sa itkMultiplyImageFilter
 * \ingroup itkBinaryFunctorImageFilter
 *
 * \ingroup OTBCommon
 */

template <class TInputImage, class TVectorInputImage, class TOutputImage>
class ITK_EXPORT ImageAndVectorImageOperationFilter :
  public itk::BinaryFunctorImageFilter<TInputImage,
      TVectorInputImage,
      TOutputImage,
      Functor::ImageAndVectorImageOperationFunctor<typename TInputImage::PixelType,
          typename TVectorInputImage::
          PixelType,
          typename TOutputImage::
          PixelType> >
//ImageToImageFilter< TVectorInputImage, TOutputImage >
{
public:

  /** Standard class typedefs. */
  typedef ImageAndVectorImageOperationFilter Self;
  //typedef itk::ImageToImageFilter<TVectorInputImage, TOutputImage> Superclass;
  typedef Functor::ImageAndVectorImageOperationFunctor<typename TInputImage::PixelType,
      typename TVectorInputImage::PixelType,
      typename TOutputImage::PixelType> FunctorType;
  typedef itk::BinaryFunctorImageFilter<TInputImage, TVectorInputImage, TOutputImage, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                                  Pointer;
  typedef itk::SmartPointer<const Self>                                                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageAndVectorImageOperationFilter, itk::BinaryFunctorImageFilter);

  /** Typedef for the images.   */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef TVectorInputImage                        VectorInputImageType;
  typedef typename VectorInputImageType::PixelType VectorInputPixelType;
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::PixelType      OutputPixelType;

  /** Operation type typedef. */
  typedef typename FunctorType::OperatorType OperatorType;

  /** Set the input images of this process object.  */
  using Superclass::SetInput;
  void SetInput(const InputImageType *input) override;
  void SetVectorInput(const VectorInputImageType *input);

  /** Get the input images of this process object.  */
  const InputImageType * GetInput();
  const VectorInputImageType * GetVectorInput();

  /** Accessors */
  itkGetMacro(UseAddition, bool);
  itkGetMacro(UseMultiplication, bool);
  itkGetMacro(UseDivision, bool);
  itkGetMacro(UseSubtraction, bool);

  void UseAddition()
  {
    m_UseAddition = true;
    m_UseMultiplication = false;
    m_UseDivision = false;
    m_UseSubtraction = false;
    this->GetFunctor().SetOperator(static_cast<OperatorType>(1));
    this->Modified();
  }
  void UseMultiplication()
  {
    m_UseAddition = false;
    m_UseMultiplication = true;
    m_UseDivision = false;
    m_UseSubtraction = false;
    this->GetFunctor().SetOperator(static_cast<OperatorType>(0));
    this->Modified();
  }
  void UseDivision()
  {
    m_UseAddition = false;
    m_UseMultiplication = false;
    m_UseDivision = true;
    m_UseSubtraction = false;
    this->GetFunctor().SetOperator(static_cast<OperatorType>(2));
    this->Modified();
  }
  void UseSubtraction()
  {
    m_UseAddition = false;
    m_UseMultiplication = false;
    m_UseDivision = false;
    m_UseSubtraction = true;
    this->GetFunctor().SetOperator(static_cast<OperatorType>(3));
    this->Modified();
  }

protected:
  ImageAndVectorImageOperationFilter();
  ~ImageAndVectorImageOperationFilter() override;

  /** This is a source, so it must set the spacing, size, and largest possible
   * region for the output image that it will produce.
   * \sa ProcessObject::GenerateOutputInformation() */
  void GenerateOutputInformation() override;

private:
  ImageAndVectorImageOperationFilter(const ImageAndVectorImageOperationFilter &) = delete;
  void operator =(const ImageAndVectorImageOperationFilter&) = delete;

  bool m_UseAddition;
  bool m_UseMultiplication;
  bool m_UseDivision;
  bool m_UseSubtraction;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageAndVectorImageOperationFilter.hxx"
#endif

#endif
