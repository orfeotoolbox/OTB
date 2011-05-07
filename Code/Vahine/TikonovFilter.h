/**
 * TikonovFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINETIKONOVFILTER_H
#define __VAHINETIKONOVFILTER_H

#include "VahineMacro.h"

#include "gdal.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingImageFileWriter.h"
#include <vnl/vnl_matrix.h>

#include "itkVariableSizeMatrix.h"
#include "itkImageToImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

namespace otb {
template <class TInputImage>
class ITK_EXPORT VahineTikonovFilter :
  public itk::ImageToImageFilter<TInputImage, TInputImage > {
public:
  typedef VahineTikonovFilter                               Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineTikonovFilter, ImageToImageFilter);

  /** Type to use for computation */
  typedef TInputImage                                              InputImageType;
  typedef typename InputImageType::Pointer                         InputImagePointer;
  typedef itk::ImageRegionConstIterator<InputImageType>            ConstIterator;
  typedef typename InputImageType::PixelType                       PixelType;
  typedef typename InputImageType::InternalPixelType               InternalPixelType;
  typedef typename InputImageType::SizeType                        SizeType;
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef itk::VariableLengthVector<RealType>                      RealPixelType;

  typedef typename itk::VariableSizeMatrix<RealType>             MatrixType;
  typedef vnl_matrix<InternalPixelType>                          VnlMatrixType;
  typedef otb::StreamingStatisticsVectorImageFilter<TInputImage> StatsFilterType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  itkSetMacro( RegularizationParam, double );
  MatrixType GetTikonovMatrix() {
    return this->m_tikonovMatrix;
  };
  MatrixType GetCovMatrix(){
    return this->m_covMatrix;
  };
  void UpdateCov();

  bool GetDebug(){
    return true;
  };
protected:
  VahineTikonovFilter();
  virtual ~VahineTikonovFilter(){
  };
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const {
    Superclass::PrintSelf(os, indent);
  };
  void GenerateData();

  void SetRegularizationMatrix(MatrixType matrix);

  typename StatsFilterType::Pointer m_statsFilter;
private:
  double m_RegularizationParam;
  MatrixType m_regulMatrix;
  MatrixType m_covMatrix;
  MatrixType m_tikonovMatrix;
  bool m_covUpdated;

  VahineTikonovFilter(const Self&); //not implemented
  void operator=(const Self&);      //not implemented

};
} // End namespace otb

#include "TikonovFilter.txx"

#endif