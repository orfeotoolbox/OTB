/*
 * VahineSlice.txx
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
#ifndef __VAHINESLICE_TXX
#define __VAHINESLICE_TXX

#include "VahineSlice.h"

using namespace vahine;
using namespace std;

template<class TReal>
vahine::Slice<TReal>::Slice(){
  nPixel = 0;
  meanParam = 0;
  meanData = 0;
  sumPixel = PixelType();
  sumParam = 0;
  meanPixel = PixelType();
}

template<class TReal>
vahine::Slice<TReal>::Slice(const Slice& s){
  nPixel = s.nPixel;
  index = s.index;
  sumPixel = s.sumPixel;
  sumParam = s.sumParam;
  meanParam = s.meanParam;
  meanData = s.meanData;
}

template<class TReal>
vahine::Slice<TReal>::Slice(ImageIndexType idx, PixelType pixel, RealType param){
  nPixel = 1;
  index.push_back(idx);
  //sumPixel.SetSize(pixel.GetSize());
  sumPixel = pixel;
  sumParam = param;
  meanParam = 0;
  meanData = 0;
  propOfPixel = 0;
  //meanPixel.SetSize(pixel.GetSize());
}

template<class TReal>
vahine::Slice<TReal>& vahine::Slice<TReal>::operator=(const Slice& s){
  nPixel = s.nPixel;
  sumPixel = s.sumPixel;
  meanPixel = s.meanPixel;
  index = s.index;
  propOfPixel = s.propOfPixel;

  sumParam = s.sumParam;
  meanParam = s.meanParam;
  meanData = s.meanData;
}

template<class TReal>
bool vahine::Slice<TReal>::operator<(const Slice& s){
  return meanData < s.meanData;
}

template<class TReal>
void vahine::Slice<TReal>::add(ImageIndexType idx, PixelType pixel, RealType param){
  nPixel++;
  index.push_back(idx);
  sumPixel += pixel;
  sumParam += param;
}

template<class TReal>
void vahine::Slice<TReal>::calculateMean(ProjPointer projection){
  setMeanParam();
  setMeanData(projection);
}

template<class TReal>
typename  vahine::Slice<TReal>::RealType vahine::Slice<TReal>::getMeanParam(){
  return meanParam;
}

template<class TReal>
typename Slice<TReal>::RealType Slice<TReal>::getMeanData(){
  return meanData;
}

template<class TReal>
void Slice<TReal>::setMeanParam(){
  meanParam = sumParam / static_cast<RealType>(nPixel);
}

template<class TReal>
void Slice<TReal>::setPropOfPixel(unsigned int nObserv){
  propOfPixel = nPixel / static_cast<double>(nObserv);
}

template<class TReal>
double Slice<TReal>::getPropOfPixel(){
  return propOfPixel;
}

template<class TReal>
void Slice<TReal>::setMeanPixel(){
  meanPixel = sumPixel / static_cast<RealType>(nPixel);
}

template<class TReal>
typename Slice<TReal>::PixelType Slice<TReal>::getMeanPixel(){
  return meanPixel;
}

template<class TReal>
void Slice<TReal>::setMeanData(ProjPointer projection){
  typename ImageIndexListType::iterator itImageIndex = index.begin();
  IteratorProjType itProj(projection, projection->GetLargestPossibleRegion() );
  RealType         sumData = static_cast<RealType>(0);
  while( itImageIndex != index.end() ) {
    itProj.SetIndex(*itImageIndex);
    sumData += itProj.Get();
    ++itImageIndex;
    }
  meanData = sumData / static_cast<RealType>(nPixel);
}

#endif