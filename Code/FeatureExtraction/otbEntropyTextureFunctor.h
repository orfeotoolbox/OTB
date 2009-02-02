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
#ifndef __otbEntropyTextureFunctor_h
#define __ootbEntropyTextureFunctor_h

#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class EntropyTextureFunctor
 *  \brief This functor calculates the local energy of an image
 *
 *   Computes the sqaure gradient which is computed using offset and
 *   angle pixel of the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class EntropyTextureFunctor
{
public:
  EntropyTextureFunctor()
  {
    m_Offset.Fill(1);
    //m_BinLength = IntVectyorType(0, 1);
  };
  ~EntropyTextureFunctor() {};

  typedef TIterInput1                    IterType1;
  typedef TIterInput2                    IterType2;
  typedef TOutput                        OutputType;
  typedef typename IterType1::OffsetType OffsetType;
  typedef typename IterType1::RadiusType RadiusType;
  typedef typename OutputType::ValueType OutputPixelType;
  typedef std::vector<double>            DoubleVectorType;
  typedef std::vector<int>               IntVectorType;
  typedef std::vector<IntVectorType>    IntVectorVectorType;

  void SetOffset(OffsetType off)
  {
    m_Offset=off;
  };
  OffsetType GetOffset()
  {
    return m_Offset;
  };

  IntVectorVectorType StatComputation(const TIterInput1 &it, const TIterInput2 &itOff)
  {
    IntVectorVectorType output;
    IntVectorType binLength;
    IntVectorType binLengthOff;
    RadiusType radius = it.GetRadius();
    unsigned int nbComp = it.GetCenterPixel().GetSize();
    m_Mini = DoubleVectorType(nbComp, itk::NumericTraits<double>::max());
    m_MiniOff = DoubleVectorType(nbComp, itk::NumericTraits<double>::max());
    m_Maxi = DoubleVectorType(nbComp, itk::NumericTraits<double>::NonpositiveMin());
    m_MaxiOff = DoubleVectorType(nbComp, itk::NumericTraits<double>::NonpositiveMin());
    double area = static_cast<double>(radius[0]*radius[1]);
    double areaInv = 1/area;

    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0]+m_Offset[0]-1;
    offsetOffInit[1] = -radius[1]+m_Offset[1]-1;
    IntVectorType mean;
    IntVectorType meanOff;
    for ( unsigned int i=0; i<nbComp; i++ )
    {
      offsetOff = offsetOffInit;
      double meanCh = 0.;
      double meanChOff = 0.;
      for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
      {
        offsetOff[0]++;
        offsetOff[1] = offsetOffInit[1];
        offset[0] = l;
        for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
        {
          //std::cout<<it.GetPixel(offset)[i]<<std::endl;
          offsetOff[1]++;
          offset[1] = k;
          meanCh += static_cast<double>(it.GetPixel(offset)[i]);
          meanChOff += static_cast<double>(itOff.GetPixel(offsetOff)[i]);

          m_Mini[i] = std::min(static_cast<double>(it.GetPixel(offset)[i]),   m_Mini[i]);
          m_MiniOff[i] = std::min(static_cast<double>(itOff.GetPixel(offsetOff)[i]),   m_MiniOff[i]);
          m_Maxi[i] = std::max(static_cast<double>(it.GetPixel(offset)[i]),   m_Maxi[i]);
          m_MaxiOff[i] = std::max(static_cast<double>(itOff.GetPixel(offsetOff)[i]),   m_MaxiOff[i]);
        }
        mean.push_back(meanCh * areaInv);
        meanOff.push_back(meanChOff * areaInv);
      }
    }

    for ( unsigned int i=0; i<nbComp; i++ )
    {
      offsetOff = offsetOffInit;
      double stdCh = 0.;
      double stdChOff = 0.;
      for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
      {
        offsetOff[0]++;
        offsetOff[1] = offsetOffInit[1];
        offset[0] = l;
        for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
        {
          offsetOff[1]++;
          offset[1] = k;
          stdCh += vcl_pow( (mean[i]-static_cast<double>(it.GetPixel(offset)[i])), 2);
          stdChOff += vcl_pow( (meanOff[i]-static_cast<double>(itOff.GetPixel(offsetOff)[i])), 2);
        }
      }
      stdCh *= areaInv;
      stdCh = vcl_sqrt( stdCh );
      stdChOff *= areaInv;
      stdChOff = vcl_sqrt( stdChOff );
      binLength.push_back( (3.5*stdCh) /(vcl_pow(area, 1/3) ) );
      binLengthOff.push_back( (3.5*stdChOff) /(vcl_pow(area, 1/3) ) );
    }

    output.push_back(binLength);
    output.push_back(binLengthOff);

    return output;
  }


  inline TOutput operator()(const TIterInput1 &it, const TIterInput2 &itOff)
  {
    //std::cout<<"operator"<<std::endl;
    IntVectorVectorType binsLength = this->StatComputation(it, itOff);

    RadiusType radius = it.GetRadius();
    unsigned int nbComp = it.GetCenterPixel().GetSize();
    double area = static_cast<double>(radius[0]*radius[1]);
    double areaInv = 1/area;
    OutputType outPix;
    outPix.SetSize( nbComp );
    outPix.Fill(0);


    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0]+m_Offset[0]-1;
    offsetOffInit[1] = -radius[1]+m_Offset[1]-1;

    int histoIdX = 0;
    int histoIdY = 0;

    for ( unsigned int i=0; i<nbComp; i++ )
    {
      //std::cout<<"i :"<<i<<" "<<std::endl;

      IntVectorType histoTemp;
      IntVectorVectorType histo;
      if (binsLength[0][i] != 0)
        histoTemp = IntVectorType( vcl_floor( static_cast<double>(m_Maxi[i]-m_Mini[i])/binsLength[0][i])+1., 0);
      else
        histoTemp = IntVectorType( 1, 0 );
      if (binsLength[1][i] != 0)
        histo = IntVectorVectorType( vcl_floor(static_cast<double>(m_MaxiOff[i]-m_MiniOff[i])/binsLength[1][i])+1., histoTemp );
      else
        histo = IntVectorVectorType( 1, histoTemp );

      //std::cout<<"m_Maxi[i]-m_Mini[i]  "<<m_Maxi[i]<<"  "<<m_Mini[i]<<std::endl;
      //std::cout<<"binsLength[0][i] "<<binsLength[0][i]<<std::endl;
      //std::cout<<histoTemp.size()<<"   "<<histo.size()<<std::endl;

      offsetOff = offsetOffInit;
      for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
      { //std::cout<<"i :"<<i<<" "<<l<<std::endl;
        offsetOff[0]++;
        offsetOff[1] = offsetOffInit[1];
        offset[0] = l;
        for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
        {
          //std::cout<<i<<" "<<l<<" "<<k<<std::endl;
          offsetOff[1]++;
          offset[1] = k;
          if ( binsLength[1][i] != 0)
            histoIdX = static_cast<int>(vcl_floor( (itOff.GetPixel(offsetOff)[i]-m_MiniOff[i]) / binsLength[1][i] ));
          else
            histoIdX = 0;
          if ( binsLength[0][i] !=0 )
            histoIdY = static_cast<int>(vcl_floor( (it.GetPixel(offset)[i]-m_Mini[i]) / binsLength[0][i] ));
          else
            histoIdY = 0;
          histo[histoIdX][histoIdY]++;

        }
      }
      for (unsigned r = 0; r<binsLength.size(); r++)
      {
        for (unsigned s = 0; s<binsLength[r].size(); s++)
        {
          double p = binsLength[r][s] * areaInv;
          if (p != 0)
            outPix[i] += static_cast<OutputPixelType>(p * vcl_log(p));
        }
      }

    }

    //std::cout<<"operator FIN"<<std::endl;
    return outPix;

  }

private:
  OffsetType m_Offset;
  DoubleVectorType m_Mini;
  DoubleVectorType m_MiniOff;
  DoubleVectorType m_Maxi;
  DoubleVectorType m_MaxiOff;
};




} // namespace Functor
} // namespace otb

#endif

