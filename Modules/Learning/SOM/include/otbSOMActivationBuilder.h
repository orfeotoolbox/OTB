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
#ifndef otbSOMActivationBuilder_h
#define otbSOMActivationBuilder_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class SOMActivationBuilder
 *  \brief This class takes a self organizing map and an input vector image, and
 *  produce a map counting the number of activation for each vector in the SOM map.
 *
 * SOM map can be set via the SetInput method, and vector image via the SetVectorSet
 * method.
 *
 * \sa SOMMap
 * \sa SOM
 *
 * \ingroup OTBSOM
 */
template <class TListSample, class TInputMap, class TOutputImage>
class ITK_EXPORT SOMActivationBuilder
  : public itk::ImageToImageFilter<TInputMap, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SOMActivationBuilder                             Self;
  typedef itk::ImageToImageFilter<TInputMap, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;

  /** Creation through object factory */
  itkNewMacro(Self);

  /** Runtime information macro */
  itkTypeMacro(SOMActivationBuilder, ImageToImageFilter);

  /** Template related typedefs */
  typedef TListSample  ListSampleType;
  typedef TOutputImage OutputImageType;
  typedef TInputMap    InputMapType;

  typedef typename ListSampleType::Pointer  ListSamplePointerType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;
  typedef typename InputMapType::Pointer    InputMapPointerType;

  /** Image Dimension */
  itkStaticConstMacro(ImageDimension, unsigned int, InputMapType::ImageDimension);

  /** ListSample accessor */
  itkSetObjectMacro(ListSample, ListSampleType);
  itkGetObjectMacro(ListSample, ListSampleType);

protected:
  /** Constructor */
  SOMActivationBuilder();
  /** Destructor */
  ~SOMActivationBuilder() ITK_OVERRIDE;
  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  SOMActivationBuilder(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented
  /** ListSample object */
  ListSamplePointerType m_ListSample;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMActivationBuilder.txx"
#endif

#endif
