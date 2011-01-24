/*
 * FlatReshapeFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEFLATRESHAPEFILTER_H_
#define __VAHINEFLATRESHAPEFILTER_H_

#include <otb/IO/otbImage.h>
#include <otb/IO/otbVectorImage.h>
#include <itkImageToImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageLinearConstIteratorWithIndex.h>
#include <itkImageLinearIteratorWithIndex.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkExceptionObject.h>

#include "VahineMacro.h"

namespace otb {
/**
 * \class VahineFlatReshapeFilter
 * \brief Filter to apply a mask to a hyperspectral image.
 */
template <class TBaseImage, class TMaskImage>
class ITK_EXPORT VahineFlatReshapeFilter
  : public itk::ImageToImageFilter<TBaseImage, TBaseImage> {
public:
  typedef TBaseImage BaseImageType;
  typedef TMaskImage MaskImageType;

  typedef VahineFlatReshapeFilter                               Self;
  typedef itk::ImageToImageFilter<BaseImageType, BaseImageType> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  typedef typename MaskImageType::Pointer                       MaskImagePointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineFlatReshapeFilter, itk::ImageToImageFilter);

  typedef typename BaseImageType::Pointer           BaseImagePointer;
  typedef typename BaseImageType::ConstPointer      BaseImageConstPointer;
  typedef typename BaseImageType::IndexType         BaseImageIndex;
  typedef typename BaseImageType::SizeType          BaseImageSize;
  typedef typename BaseImageType::RegionType        BaseImageRegion;
  typedef typename BaseImageType::PixelType         PixelType;
  typedef typename BaseImageType::InternalPixelType InternalPixelType;
  typedef typename MaskImageType::RegionType        MaskImageRegion;
  typedef typename MaskImageType::IndexType         MaskImageIndex;
  typedef typename MaskImageType::SizeType          MaskImageSize;

  typedef itk::ImageRegionConstIterator<BaseImageType> ConstBaseIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType> ConstMaskIteratorType;
  typedef itk::ImageRegionIterator<BaseImageType>      OutputIteratorType;

  typedef itk::ImageRegionConstIteratorWithIndex<MaskImageType> ConstMaskIteratorIndexType;

  itkSetMacro( Mask, MaskImagePointer );
  itkSetMacro( HighThreshold, double );
  itkGetMacro( HighThreshold, double );
  itkSetMacro( LowThreshold, double );
  itkGetMacro( LowThreshold, double );
  itkSetMacro( FlatReshape, bool );
  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  bool GetDebug(){
    return true;
  };
protected:
  VahineFlatReshapeFilter();
  virtual ~VahineFlatReshapeFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

  //void ThreadedGenerateData(const BaseImageRegion& outputRegionForThread, int
  // threadId );
private:
  VahineFlatReshapeFilter(const Self&); //not implemented
  void operator=(const Self&);          //not implemented

  //void GetMaskRegion(BaseImageRegion outputRegion, MaskImageIndex &
  // maskImageStartIndex, MaskImageRegion & maskImageRegion);
  unsigned int GetNumberOfActivPixels();

  void CheckInputBaseAndMaskSize();

  MaskImagePointer  m_Mask;
  MaskImageRegion   m_MaskLargestPossibleRegion;
  double            m_HighThreshold;
  double            m_LowThreshold;
  BaseImageRegion   m_OutRegion;
  bool              m_FlatReshape;
  InternalPixelType m_NullValue;

};

} // end namespace otb

#include "FlatReshapeFilter.txx"

#endif /* FLATRESHAPEFILTER_H_ */