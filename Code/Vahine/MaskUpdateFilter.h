/*
 * MaskUpdateFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __MASKUPDATEFILTER_H_
#define __MASKUPDATEFILTER_H_

#include <otb/IO/otbImage.h>
#include <itkImageToImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include "VahineMacro.h"
#include "VahineException.h"

namespace otb {
template <class TMaskImage>
class ITK_EXPORT VahineMaskUpdateFilter : public itk::ImageToImageFilter<TMaskImage, TMaskImage> {
public:
  const static unsigned int Dimension = 2;
  typedef TMaskImage                                            MaskImageType;
  typedef VahineMaskUpdateFilter                                Self;
  typedef itk::ImageToImageFilter<MaskImageType, MaskImageType> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  typedef typename MaskImageType::PixelType                     MaskImagePixelType;
  typedef typename MaskImageType::Pointer                       MaskImagePointer;
  typedef itk::ImageRegionConstIterator<MaskImageType>          ConstMaskIteratorType;
  typedef itk::ImageRegionIterator<MaskImageType>               MaskIteratorType;
  typedef typename MaskImageType::RegionType                    MaskRegionType;
  typedef typename MaskImageType::InternalPixelType             MaskInternalPixelType;
  typedef Image<MaskInternalPixelType, Dimension>               BandType;
  typedef typename BandType::PixelType                          BandPixelType;
  typedef itk::ImageRegionConstIterator<BandType>               ConstBandIteratorType;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineFlatReshapeFilter, itk::ImageToImageFilter);

  itkSetMacro( RemedialMask, MaskImagePointer );
  itkSetMacro( InputBand, unsigned int);
  itkSetMacro( RemedialBand, unsigned int);
  bool GetDebug(){
    return true;
  };
protected:
  VahineMaskUpdateFilter();
  virtual ~VahineMaskUpdateFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

  //void ThreadedGenerateData(const BaseImageRegion& outputRegionForThread, int
  // threadId );
private:
  VahineMaskUpdateFilter(const Self&); // not implemented
  void operator=(const Self&);         //not implemented

  MaskImagePointer m_RemedialMask;
  unsigned int     m_InputBand;
  unsigned int     m_RemedialBand;
  double           m_LowThreshold;
  double           m_HighThreshold;
};

} // end namesapce otb

#include "MaskUpdateFilter.txx"

#endif /* MASKUPDATEFILTER_H_ */