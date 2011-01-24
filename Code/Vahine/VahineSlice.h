/*
 * VahineSlice.h
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

#ifndef __VAHINESLICE_H
#define __VAHINESLICE_H

#include "otbImage.h"
#include "itkVariableLengthVector.h"
#include "itkImageRegionIterator.h"

namespace vahine {
/** Slice Class :
   *  nPixel, count the number of observation in the slice
   *  index, list of all image index in the current slice
   *  sumPixel, sum of centering data foreach band
   *  sumParam, sum of parameter value for all index in current slice
   *  meanParam,
   *  meanData,
   */
template<class TReal>
class Slice {
public:
  typedef  TReal                                        RealType;
  typedef typename itk::VariableLengthVector<RealType>  PixelType;
  typedef typename otb::Image<RealType, 2>              ImageType;
  typedef typename ImageType::IndexType                 ImageIndexType;
  typedef std::list<ImageIndexType>                     ImageIndexListType;
  typedef typename ImageType::Pointer                   ProjPointer;
  typedef  typename itk::ImageRegionIterator<ImageType> IteratorProjType;

  Slice();
  Slice(const Slice& s);
  Slice(ImageIndexType idx, PixelType pixel, RealType param);
  ~Slice(){
  };
  Slice& operator=(const Slice& s);

  bool operator<(const Slice& s);

  void add(ImageIndexType idx, PixelType pixel, RealType param);

  void calculateMean(ProjPointer projection);

  RealType getMeanParam();

  RealType getMeanData();

  PixelType getMeanPixel();

  bool GetDebug(){
    return true;
  }
  std::string GetNameOfClass(){
    return "Slice";
  }
  void setMeanPixel();

  void setPropOfPixel(unsigned int nObserv);

  double getPropOfPixel();

private:

  int                nPixel;
  PixelType          sumPixel;
  PixelType          meanPixel;
  ImageIndexListType index;
  RealType           sumParam;
  RealType           meanParam;
  RealType           meanData;
  double             propOfPixel;

  void setMeanParam();

  void setMeanData(ProjPointer projection);

};

} //end namespace vahine

#include "VahineSlice.txx"

#endif /* VAHINESLICE_H_ */