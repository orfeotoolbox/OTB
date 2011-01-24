/*
 * SyntheticBaseGeometricFilter.h
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

#ifndef __VAHINESYNTHETICBASEGEOMETRICFILTER_H_
#define __VAHINESYNTHETICBASEGEOMETRICFILTER_H_

#include "VahineMacro.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearConstIteratorWithIndex.h"

namespace otb {
/**
 * \class VahineSyntheticBaseGeometricFilter
 * \brief Filter to select one or more synthetic database
 *
 * This class select one or more synthetic database.
 *
 * A synthetic database is a spectral cube were each column have
 * different geometry. With this filter you can select one geometry
 * i.e. one column.
 */
template <class TImage>
class ITK_EXPORT VahineSyntheticBaseGeometricFilter
  : public itk::ImageToImageFilter<TImage, TImage> {
public:
  typedef TImage InputImageType;
  typedef TImage OutputImageType;

  typedef VahineSyntheticBaseGeometricFilter                      Self;
  typedef itk::ImageToImageFilter<InputImageType, InputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineSyntheticFilter, itk::ImageToImageFilter);

  typedef itk::ImageLinearConstIteratorWithIndex<InputImageType> LinearIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>              IteratorType;
  typedef typename OutputImageType::Pointer                      OutputImagePointer;
  typedef typename OutputImageType::IndexType                    OutputIndexType;
  typedef typename OutputImageType::SizeType                     OutputSizeType;
  typedef typename OutputImageType::RegionType                   OutputRegionType;

  itkSetMacro( ColumnIndex, unsigned int );
  itkGetMacro( ColumnIndex, unsigned int );

  bool GetDebug(){
    return m_Debug;
  };
  void SetDebug(bool debug){
    m_Debug = debug;
  };
protected:
  VahineSyntheticBaseGeometricFilter();
  virtual ~VahineSyntheticBaseGeometricFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

private:
  bool m_Debug;
  unsigned int m_ColumnIndex;

  VahineSyntheticBaseGeometricFilter(const Self&); //not implemented
  void operator=(const Self&);                     //not implemented

};

} // end namespace otb

#include "SyntheticBaseGeometricFilter.txx"

#endif /* __VAHINESYNTHETICBASEGEOMETRICFILTER_H_ */