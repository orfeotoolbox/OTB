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
#ifndef __otbLineSegmentDetector_txx
#define __otbLineSegmentDetector_txx

#include "otbLineSegmentDetector.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbPolygon.h"

#include "otbMath.h"

#include "itkMatrix.h"
#include "itkSymmetricEigenAnalysis.h"

#define MAXIT 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

namespace otb
{

/**
 *
 */
template <class TInputImage, class TPrecision>
LineSegmentDetector<TInputImage,TPrecision >
::LineSegmentDetector()
{
  m_Threshold = 2.;

  /** Compute the modulus and the orientation gradient images */
  m_GradientFilter = GradientFilterType::New();
  m_MagnitudeFilter = MagnitudeFilterType::New();
  m_OrientationFilter = OrientationFilterType::New();

  /** */
  m_UsedPointImage  = LabelImageType::New();

  /** */
  LineSpatialObjectListType::Pointer m_LineList = LineSpatialObjectListType::New();
}


/**
 *
 */

template <class TInputImage, class TPrecision >
void
LineSegmentDetector<TInputImage,TPrecision >
::GenerateData()
{
  /** The Output*/
  m_LineList = this->GetOutput();
  
  /** Allocate memory for the temporary label Image*/
  m_UsedPointImage->SetRegions(this->GetInput()->GetRequestedRegion());
  m_UsedPointImage->Allocate();
  m_UsedPointImage->FillBuffer(0);

  /** Compute the modulus and the orientation gradient image*/
  m_GradientFilter->SetInput(this->GetInput());
  m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
  m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());
  m_MagnitudeFilter->Update();
  m_OrientationFilter->Update();
  
  /** Comupute the seed histogram to begin the search*/
  CoordinateHistogramType   CoordinateHistogram;
  CoordinateHistogram = this->SortImageByModulusValue(m_MagnitudeFilter->GetOutput());
  
  /** Serach the segments on the image by growing a region from a seed   */
  this->LineSegmentDetection(&CoordinateHistogram);
  
  /** 
   * Compute The rectangles 
   * Out : a List of rectangles : m_RectangleList
   * 
   */
  this->ComputeRectangles();

}

/**************************************************************************************************************/
/**
 * 
 */

template <class TInputImage, class TPrecision >
typename LineSegmentDetector<TInputImage,TPrecision >
::CoordinateHistogramType
LineSegmentDetector<TInputImage,TPrecision >
::SortImageByModulusValue(OutputImageType * modulusImage)
{
  /* Size of the images **/
  SizeType     SizeInput = this->GetInput()->GetRequestedRegion().GetSize();
  m_NumberOfImagePixels  = SizeInput[0]*SizeInput[1];

  /** Define constants 
   *  prec = M_PI/8  : 8 is the number off direction allowed
   *  p =1/8 
   */
  float logNT = 5.*(vcl_log10(static_cast<float>(SizeInput[0])) + vcl_log10(static_cast<float>(SizeInput[1])))/2.;
  float log1_p = vcl_log10(0.125);
  float rapport = logNT/log1_p;
  m_MinimumRegionSize = -1*static_cast<unsigned  int>(rapport);

  
  /** Definition of the min & the max of an image*/
  OutputPixelType   min = itk::NumericTraits<MagnitudePixelType >::Zero;
  OutputPixelType   max = itk::NumericTraits<MagnitudePixelType >::Zero;
  
  /** Computing the min & max of the image*/
  typedef  itk::MinimumMaximumImageCalculator<OutputImageType>  MinMaxCalculatorFilter;
  typename MinMaxCalculatorFilter::Pointer minmaxCalculator =   MinMaxCalculatorFilter::New();
  
  minmaxCalculator->SetImage(modulusImage);
  minmaxCalculator->ComputeMinimum();
  min = minmaxCalculator->GetMinimum();
  minmaxCalculator->ComputeMaximum();
  max = minmaxCalculator->GetMaximum();
  
  /** Compute the threshold on the gradient*/
  float d = 8.;                                                    // d : the number of allowed directions
  m_Threshold = 4*m_Threshold /vcl_sin(M_PI/d)*((max-min)/255.);     // threshold normalized with min & max of the values

  /** Computing the length of the bins*/  
  unsigned int NbBin = 10;  
  MagnitudePixelType lengthBin = (max - min)/static_cast<MagnitudePixelType>(NbBin-1) ;
  CoordinateHistogramType  tempHisto(NbBin);  /** Initializing the histogram */

  
  itk::ImageRegionIterator<OutputImageType> it(modulusImage, 
					       modulusImage->GetRequestedRegion());
    
  //std::cout << "min "<< min<<  " max " << max  << " lengthBin " << lengthBin <<  " et thresh" << m_Threshold << std::endl;
  it.GoToBegin();
  while(!it.IsAtEnd())
    {
      OutputIndexType index = it.GetIndex();
      if(static_cast<unsigned int>(index[0]) > 0 && static_cast<unsigned int>(index[1]) <SizeInput[0]-1 
	 && static_cast<unsigned int>(index[1]) >0 && static_cast<unsigned int>(index[1]) <SizeInput[1]-1 )
	{
	  unsigned int bin = static_cast<unsigned int> (it.Value()/lengthBin);
	  if( it.Value()- m_Threshold >1e-10 )
	    tempHisto[NbBin-bin-1].push_back(it.GetIndex());
	}
      ++it;
    }
 
  return tempHisto;
}

/**************************************************************************************************************/
/**
 * Method used to search the segments 
 */

template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::LineSegmentDetection(CoordinateHistogramType *CoordinateHistogram)
{
    
  /** Begin the search of the segments*/
  CoordinateHistogramIteratorType  ItCoordinateList = CoordinateHistogram->begin();

  while(ItCoordinateList != CoordinateHistogram->end())
    {
      typename IndexVectorType::iterator   ItIndexVector = (*ItCoordinateList).begin(); 
      while(ItIndexVector != (*ItCoordinateList).end())
	{
	  InputIndexType index = *ItIndexVector;
	  
	  /** If the point is not yet computed */
	  if(!this->IsUsed(index))
	  {
	    this->GrowRegion(index); 
	  }
	  ++ItIndexVector;
	}
      ++ItCoordinateList;
    }

}

/**************************************************************************************************************/
/**
 * Method used to compute rectangles from region  
 * Here you can access the NFA for each region
 */
template <class TInputImage, class TPrecision  >
int
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRectangles()
{
  /** Check the size of the region list */
  unsigned int sizeRegion = m_RegionList.size();
  if(sizeRegion ==0)
    return EXIT_FAILURE;
  
  /** Compute the rectangle*/
  CoordinateHistogramIteratorType     ItRegion = m_RegionList.begin();
  DirectionVectorIteratorType         ItDir    =  m_DirectionVector.begin();
  std::cout << " NB DE REGIONS : "<< m_RegionList.size()<< std::endl;
  
  while(ItRegion != m_RegionList.end() && ItDir !=m_DirectionVector.end() )
    {

      this->Region2Rect(*ItRegion, *ItDir );
      ++ItRegion;
      ++ItDir;
    }

  /** Improve the rectangles & store the lines*/
  RectangleListTypeIterator            itRec = m_RectangleList.begin();
  while(itRec != m_RectangleList.end())
    {
      float NFA = this->ComputeRectNFA(*itRec);
      //float NFAImproved = this->ImproveRectangle(&(*itRec) , NFA);
      /**
       * Here we start building the OUTPUT :a LineSpatialObjectList. 
       */
      if(NFA > 0./** eps */)
	{
	  //std::cout << (*itRec)[0] << " " << (*itRec)[1] << " " << (*itRec)[2] << " " << (*itRec)[3]<<std::endl;
	  PointListType pointList;
	  PointType     point;
	  
	  point.SetPosition((*itRec)[0],(*itRec)[1]);
	  pointList.push_back(point);
	  point.SetPosition((*itRec)[2],(*itRec)[3]);
	  pointList.push_back(point);
	  
	  typename LineSpatialObjectType::Pointer line = LineSpatialObjectType::New();
	  line->SetId(0);
	  line->SetPoints( pointList );
	  line->ComputeBoundingBox();
	  m_LineList->push_back(line);
	  pointList.clear();
	}
      
      ++itRec;
    }


  return EXIT_SUCCESS;
}

/**************************************************************************************************************/
/**
 *
 */
template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::CopyRectangle(RectangleType * rDst , RectangleType  *rSrc )
{
  RectangleIteratorType     itSrc = (*rSrc).begin();

  while(itSrc != (*rSrc).end())
    {
      (*rDst).push_back(*itSrc);
      ++itSrc;
    }
}

/**************************************************************************************************************/
/**
 * Method used to compute improve the NFA of The rectangle
 */
template <class TInputImage, class TPrecision  >
float
LineSegmentDetector<TInputImage, TPrecision>
:: ImproveRectangle(RectangleType  * rec , float NFA)
{
  int n = 0;
  float nfa_new;
  float delta = 0.5;
  float delta_2 = delta / 2.0;
  RectangleType r;

  if( NFA > 0. ) return NFA;
 
  /*Try to improve the precison of the oriented */
  CopyRectangle(&r ,rec );
  for(n=0; n<5; n++)
    {
      r[7] /= 2.0;
      r[6] = M_PI * r[7];              // prec = rec[6]
      nfa_new = this->ComputeRectNFA(r);
      if( nfa_new > NFA )
        {
          NFA = nfa_new;
	  CopyRectangle(rec ,&r );
	}
    }
  if( NFA > 0. ) return NFA;
  
  /*Try to improve the width of the rectangle*/
  CopyRectangle(&r ,rec );
  for(n=0; n<5; n++)
    {
      r[4] -= delta;   //r[4] is stored as the width
      nfa_new = this->ComputeRectNFA(r);
      if( nfa_new > NFA )
        {
          NFA = nfa_new;
	  CopyRectangle(rec ,&r );
	}
    }
  if( NFA > 0. ) return NFA;
  
  /*Try to improve the extremity of the segments*/
  CopyRectangle(&r ,rec );
  for(n=0; n<5; n++)
    {
      if( (r[4] - delta) >= 0.5 )
        {
	  r[0] += -vcl_sin(r[5]) * delta_2;
	  r[1] +=  vcl_cos(r[5])* delta_2;
	  r[2] += -vcl_sin(r[5])* delta_2;
	  r[3] +=  vcl_cos(r[5])* delta_2;
	  r[4] -= delta;
	  
	  nfa_new = this->ComputeRectNFA(r);
	  if( nfa_new > NFA )
	    {
	      NFA = nfa_new;
	      CopyRectangle(rec ,&r );
	    }
	}
    }
  if( NFA > 0. ) return NFA;
  
  CopyRectangle(&r ,rec );
  for(n=0; n<5; n++)
    {
      if( (r[4] - delta) >= 0.5 )
        {
	  r[0] -= -vcl_sin(r[5]) * delta_2;
	  r[1] -=  vcl_cos(r[5])* delta_2;
	  r[2] -= -vcl_sin(r[5])* delta_2;
	  r[3] -=  vcl_cos(r[5])* delta_2;
	  r[4] -= delta;
	  
	  nfa_new = this->ComputeRectNFA(r);
	  if( nfa_new > NFA )
	    {
	      NFA = nfa_new;
	      CopyRectangle(rec ,&r );
	    }
	}
    }
  if( NFA > 0. ) return NFA;
  
  /*Try to improve the precision again */
  CopyRectangle(&r ,rec );
  for(n=0; n<5; n++)
    {
      r[7] /= 2.0;
      r[6] = M_PI * r[7];              // prec = rec[]
      nfa_new = this->ComputeRectNFA(r);
      if( nfa_new > NFA )
        {
	  NFA = nfa_new;
	  CopyRectangle(rec ,&r );
	}
    }
  if( NFA > 0. ) return NFA;
  
  return NFA;

}

/**************************************************************************************************************/
/**
 * Method IsUsed : Determine if a point exists in the pointSet of already computed points.
 */
template <class TInputImage, class TPrecision  >
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsUsed(InputIndexType  index) 
{
  bool isUsed = false;
  
  typedef itk::NeighborhoodIterator<LabelImageType>   NeighborhoodLabelIteratorType;
  typename NeighborhoodLabelIteratorType::SizeType    radiusLabel;
  radiusLabel.Fill(0);
  NeighborhoodLabelIteratorType                       itLabel(radiusLabel,m_UsedPointImage,
							      m_UsedPointImage->GetRequestedRegion());
  
  itLabel.SetLocation(index);
  if(*(itLabel.GetCenterValue()) == 1)
    isUsed = true;
  
  return isUsed;
}

/**
 * Method SetPixelToUsed : Determine if a point exists in the pointSet of already computed points.
 */
template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::SetPixelToUsed(InputIndexType  index) 
{
  typedef itk::NeighborhoodIterator<LabelImageType>   NeighborhoodLabelIteratorType;
  typename NeighborhoodLabelIteratorType::SizeType    radiusLabel;
  radiusLabel.Fill(0);
  NeighborhoodLabelIteratorType                       itLabel(radiusLabel,m_UsedPointImage,
							      m_UsedPointImage->GetRequestedRegion());
  itLabel.SetLocation(index);
  itLabel.SetCenterPixel(1);     // 1 : Set the point status to : Used Point
}

/**************************************************************************************************************/
/**
 * Method GrowRegion : 
 */
template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::GrowRegion(InputIndexType  index ) 
{
  /** Add the point to the used list point*/
  this->SetPixelToUsed(index);
  
  /** Neighborhooding */
  typedef itk::ConstNeighborhoodIterator<InputImageType>  NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType             radius;
  radius.Fill(1);
  NeighborhoodIteratorType                                itNeigh(radius,m_MagnitudeFilter->GetOutput(),
								  m_MagnitudeFilter->GetOutput()->GetRequestedRegion());
  NeighborhoodIteratorType                                itNeighDir(radius,m_OrientationFilter->GetOutput() ,
								     m_OrientationFilter->GetOutput()->GetRequestedRegion());
  
  /** Vector where to store the point belonging to the current region*/
  unsigned int neighSize  = itNeigh.GetSize()[0]*itNeigh.GetSize()[1];
  IndexVectorType                                 reg;

  /** Angle of the region*/
  float regionAngle = 0;
  
  /** Add the first point to the region */
  reg.push_back(index);
  float sumX = 0.;
  float sumY = 0.;
  
  /** 
   * Loop for searching regions 
   */
  for (unsigned int cpt = 0; cpt < reg.size() ; cpt++ )
    {
      itNeigh.SetLocation(reg[cpt]);
      itNeighDir.SetLocation(reg[cpt]);
      sumX += vcl_cos(*(itNeighDir.GetCenterValue()));
      sumY += vcl_sin(*(itNeighDir.GetCenterValue()));
      regionAngle = vcl_atan2(sumY,sumX);
      
      unsigned int s = 0;
      while(s < neighSize )
	{
	  InputIndexType NeighIndex = itNeigh.GetIndex(s);
	  float angleComp =   itNeighDir.GetPixel(s);
	  float prec = M_PI/8;

	  if( !this->IsUsed(NeighIndex) && this->IsAligned(angleComp, regionAngle ,prec) )
	    {
	      if(this->GetInput()->GetRequestedRegion().IsInside(NeighIndex))  /** Check if the index is inside the image*/
		{
		  this->SetPixelToUsed(NeighIndex);
		  reg.push_back(NeighIndex);
		}
	    }
	  s++;
	}
    }/** End Searching loop*/
    
  /** Store the region*/
  if(reg.size()> m_MinimumRegionSize && reg.size() < static_cast<unsigned int>(m_NumberOfImagePixels/2))
  {
      m_RegionList.push_back(reg);
      m_DirectionVector.push_back(regionAngle);
  }
}

/**************************************************************************************************************/
/**
 *
 */
template <class TInputImage, class TPrecision  >
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsAligned(float Angle, float regionAngle, float prec)
{

 //  if(Angle < 0.) Angle = - Angle;
//   if(regionAngle < 0.)  regionAngle = -regionAngle ;

//   while(Angle > M_PI) Angle -= M_PI;
//   while( regionAngle > M_PI)  regionAngle -= M_PI;

//   if(Angle < 0.) Angle = - Angle;
//   if(regionAngle < 0.)  regionAngle = -regionAngle ;

  float diff = Angle - regionAngle;
  
  if( diff < 0.0 ) diff = -diff;

  if( diff > 1.5*M_PI )
    {
      diff -= 2*M_PI;
      if( diff < 0.0 ) diff = -diff;
    }




  return diff < prec;
}

/**************************************************************************************************************/
/**
 *
 */
template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::Region2Rect(IndexVectorType  region , float angleRegion)
{
  /** Local Variables*/
  float weight = 0.,sumWeight = 0.; 
  float  x = 0., y = 0.;
  float l_min = 0., l_max = 0.,l =0., w=0. , w_min = 0. , w_max =0.;
  
  /** Neighborhooding again*/
  typedef itk::ConstNeighborhoodIterator<InputImageType>  NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType             radius;
  radius.Fill(0);
  NeighborhoodIteratorType                                itNeigh(radius,m_MagnitudeFilter->GetOutput(),
								  m_MagnitudeFilter->GetOutput()->GetRequestedRegion());

  /** Computing the center of the rectangle*/
  IndexVectorIteratorType    it = region.begin();
  while(it != region.end())
    {
      itNeigh.SetLocation(*it);
      weight = *itNeigh.GetCenterValue(); 
      x += static_cast<float>((*it)[0])* weight;
      y += static_cast<float>((*it)[1])* weight;
      sumWeight +=  weight;
      ++it;
    }
  if(sumWeight < 1e-10)
    {
      x = 0.; y = 0.;
    }
  else
    {
      x/= sumWeight ;
      y/= sumWeight ;
    }
  
  /** Compute the orientation of the region*/
  float theta = this->ComputeRegionOrientation(region , x , y , angleRegion); 
    
  /* Lenght & Width of the rectangle **/
  
  typedef std::vector<MagnitudePixelType>                          MagnitudeVector;
  SizeType    sizeInput = this->GetInput()->GetRequestedRegion().GetSize();

  int Diagonal =  static_cast<unsigned int>(vcl_sqrt(sizeInput[0]*sizeInput[0] +sizeInput[1]*sizeInput[1])) + 2;
  MagnitudeVector sum_l( 2*Diagonal, itk::NumericTraits<MagnitudePixelType>::Zero);
  MagnitudeVector sum_w( 2*Diagonal, itk::NumericTraits<MagnitudePixelType>::Zero);
  
  float dx = vcl_cos(theta);
  float dy = vcl_sin(theta);
  
  it = region.begin();
  while(it != region.end())
    {
      itNeigh.SetLocation(*it);
      weight = *itNeigh.GetCenterValue(); 
      l =  ( static_cast<float>((*it)[0]) - x )*dx + ( static_cast<float>((*it)[1]) - y )*dy;
      w = -( static_cast<float>((*it)[0]) - x )*dy + ( static_cast<float>((*it)[1]) - y )*dx;
      	
      if(l<l_min) l_min = l;   if(l>l_max)  l_max = l;
      if(w<w_min) w_min = w;   if(w>w_max)  w_max = w;

      sum_l[static_cast< int>(vcl_floor(l)+0.5)+ Diagonal ] +=  weight;
      sum_w[static_cast< int>(vcl_floor(w)+0.5)+ Diagonal ] +=  weight;
      
      ++it;
    }
  
  /** Thresholdinq the width and the length*/
  
  float sum_th = 0.01 * sumWeight; /* weight threshold for selecting region */
  float s = 0.;
  int i = 0;
  
  for( s=0.0,i = static_cast<int>(l_min); s<sum_th && i<=static_cast<int>(l_max) ; i++) 
    s += sum_l[ Diagonal + i];
  
  float lb = (static_cast<float>(i-1) - 0.5 ) ;
  
  for(s=0.0,i=static_cast<int>(l_max); s<sum_th && i>=static_cast<int>(l_min); i--) 
    s += sum_l[ Diagonal  + i];
  float lf =  (static_cast<float>(i-1) - 0.5 ); 

  for(s=0.0,i=static_cast<int>(w_min); s<sum_th && i<=static_cast<int>(w_max); i++) 
    s += sum_w[ Diagonal + i];
  
  float wr = (static_cast<float>(i-1) - 0.5)  ;  

  for(s=0.0,i=static_cast<int>(w_max); s<sum_th && i>=static_cast<int>(w_min); i--) 
    s += sum_w[Diagonal  + i];
  
  float wl = (static_cast<float>(i-1) - 0.5 ) ;  


  /** Finally store the rectangle in vector this way : 
   *  vec[0] = x1 
   *  vec[1] = y1
   *  vec[2] = x2 
   *  vec[3] = y2
   *  vec[4] = width
   *  vec[5] = theta
   *  vec[6] = prec = Pi/8
   *  vec[7] = p =  1/8
   */
  
  RectangleType          rec(8 ,0.);     // Definition of a rectangle : 8 components
  rec[0] = (x + lb*dx >0)?x + lb*dx:0.;
  rec[1] = (y + lb*dy>0 )?y + lb*dy:0.;
  rec[2] = (x + lf*dx >0)?x + lf*dx:0.;
  rec[3] = (y + lf*dy >0)? y + lf*dy:0;
  rec[4] = vcl_abs(wl - wr);
  rec[5] = theta;
  rec[6] = M_PI/8;
  rec[7] = 1./8.;
  
  if(rec[4] - 1. <1e-10) rec[4] = 1.;
  m_RectangleList.push_back(rec);
}

/**************************************************************************************************************/
template <class TInputImage, class TPrecision  >
float
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRegionOrientation(IndexVectorType  region , float x,float y  , float angleRegion) 
{

  float Ixx = 0.0;
  float Iyy = 0.0;
  float Ixy = 0.0;
  float theta = 0.;
  float weight = 0.,sum = 0.;
  
  /** Neighborhooding again*/
  typedef itk::ConstNeighborhoodIterator<InputImageType>  NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType             radius;
  radius.Fill(0);
  NeighborhoodIteratorType                                itNeigh(radius,m_MagnitudeFilter->GetOutput(),
								  m_MagnitudeFilter->GetOutput()->GetRequestedRegion());
  
  /** Computing the center iof the rectangle*/
  IndexVectorIteratorType    it = region.begin();
  while(it != region.end())
    {
      itNeigh.SetLocation(*it);
      weight = *itNeigh.GetCenterValue(); 
      float Iyy2 = static_cast<float>((*it)[0]) - x;
      float Ixx2 = static_cast<float>((*it)[1]) - y;
      
      Ixx += Ixx2*Ixx2*weight;
      Iyy += Iyy2*Iyy2*weight;
      Ixy -= Ixx2*Iyy2*weight;
      sum +=  weight;
      ++it;
    }

  /** using te itk Eigen analysis*/
  typedef itk::Matrix<float , 2, 2>    MatrixType;
  typedef std::vector<float >    MatrixEigenType;
  MatrixType Inertie, eigenVector;
  MatrixEigenType  eigenMatrix(2,0.);
  Inertie[0][0] =Ixx;
  Inertie[1][1] =Iyy;
  Inertie[0][1] =Ixy;
  Inertie[1][0] =Ixy;
  
  typedef itk::SymmetricEigenAnalysis<MatrixType,MatrixEigenType>   EigenAnalysisType;
  EigenAnalysisType eigenFilter(2) ;
  eigenFilter.ComputeEigenValuesAndVectors(Inertie,eigenMatrix,eigenVector  );
  theta = vcl_atan2(eigenVector[1][1], -eigenVector[1][0]);
  
  /* the previous procedure don't cares orientations,
     so it could be wrong by 180 degrees.
     here is corrected if necessary */

  float prec = M_PI/8;
  if( this->angle_diff(theta,angleRegion) > prec ) theta += M_PI;

  return theta;
}

/**************************************************************************************************************/
/**
 * Compute the difference betwenn 2 angles modulo 2_PI
 */
template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::angle_diff(float a, float b)
{
  a -= b;
  while( a <= -M_PI ) a += 2*M_PI;
  while( a >   M_PI ) a -= 2*M_PI;
  if( a < 0.0 ) a = -a;
  return a;
}

/**
 * compute the number of false alarm of the rectangle 
 */
template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRectNFA(RectangleType  rec)
{
  int NbAligned = 0;
  float nfa_val = 0.;
  
  float dx = vcl_cos(rec[5]);
  float dy = vcl_sin(rec[5]);
  float halfWidth = rec[4]/2;
  
  /** Determine the four corners of the rectangle*/
  /** Remember : 
   *  vec[0] = x1 
   *  vec[1] = y1
   *  vec[2] = x2 
   *  vec[3] = y2
   *  vec[4] = width
   *  vec[5] = theta
   *  vec[6] = prec = Pi/8
   *  vec[7] = p =  1/8
   */

  RectangleType        X(4,0.) , Y(4,0.);
  
  X[0] = rec[0] + dy* halfWidth ;
  Y[0] = rec[1] - dx* halfWidth ;
  X[1] = rec[0] - dy* halfWidth ;
  Y[1] = rec[1] + dx* halfWidth ;
  X[2] = rec[2] + dy* halfWidth ;
  Y[2] = rec[3] - dx* halfWidth ;
  X[3] = rec[2] - dy* halfWidth ;
  Y[3] = rec[3] + dx* halfWidth ;

  /** Compute the NFA of the rectangle  
   *  We Need : The number of : Points in the rec  (Area of the rectangle)
   *                            Aligned points  with theta in the rectangle
   */
  
  /**  Compute the number of points aligned */
  typedef otb::Polygon<float>       PolygonType;
  PolygonType::Pointer  rectangle = PolygonType::New();

  /** Fill the rectangle with the points*/
  for (int i = 0; i<static_cast<int>(X.size());  i++)
    {
      InputIndexType   vertex;
      vertex[0] = X[i] ; vertex[1] = Y[i];
      rectangle->AddVertex(vertex);
    }
  
  /** Get The Bounding Region*/
  OutputImageDirRegionType   region = rectangle->GetBoundingRegion();
  
  itk::ImageRegionIterator<OutputImageDirType> it(m_OrientationFilter->GetOutput(), region/*m_OrientationFilter->GetOutput()->GetRequestedRegion()*/);
  it.GoToBegin();
  
  int pts = 0;
  
  while(!it.IsAtEnd())
    {
      if( rectangle->IsInside( it.GetIndex() ))
	{
	  pts++;

	  if(this->IsAligned(it.Get(), rec[5] /*theta*/ ,rec[6] /*Prec*/))
	    NbAligned++;
	}
      ++it;
    }
  
  /** Compute the NFA from the rectangle computed below*/
  SizeType     SizeInput = this->GetInput()->GetRequestedRegion().GetSize();
  float logNT = 5.*(vcl_log10(static_cast<float>(SizeInput[0])) + vcl_log10(static_cast<float>(SizeInput[1])))/2.;
  
  nfa_val = NFA(pts,NbAligned ,0.125,logNT);
  
  return nfa_val;
}

/**************************************************************************************************************/
/*
   compute logarithm of binomial NFA
   NFA = NT.b(n,k,p)
   log10 NFA = log10( NFA )

   n,k,p - binomial parameters.
   logNT - logarithm of Number of Tests
 */
template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::NFA(int n, int k, double p, double logNT)
{
  float val;

  if(k==0) return -logNT;

  val = -logNT - log10(betai((double)k,(double)(n-k+1),p));

  if(isinf(val)) /* approximate by the first term of the tail */        
    val = -logNT - ( factln(n) - factln(k) - factln(n-k) ) / M_LN10
          - (double)k * log10(p) - (double)(n-k) * log10(1.0-p);

  return val;
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TPrecision  >
void
LineSegmentDetector<TInputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );

}


/**************************************************************************************************************/
/**************************************************************************************************************/
/*
  rutines betacf, gammln, betai, and factln
  taken from "Numerical Recipes in C", Second Edtion, 1992 by
  W.H. Press, S.A. Teukolsky, W.T.Vetterling and B.P. Flannery
*/
/**************************************************************************************************************/
/**************************************************************************************************************/
template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::betacf(float a, float b, float x)
{
  int m,m2;
  float aa,c,d,del,h,qab,qam,qap;

  qab=a+b;
  qap=a+1.0;
  qam=a-1.0;
  c=1.0;
  d=1.0-qab*x/qap;
  if (fabs(d) < FPMIN) d=FPMIN;
  d=1.0/d;
  h=d;
  for (m=1;m<=MAXIT;m++) {
    m2=2*m;
    aa=m*(b-m)*x/((qam+m2)*(a+m2));
    d=1.0+aa*d;
    if (fabs(d) < FPMIN) d=FPMIN;
    c=1.0+aa/c;
    if (fabs(c) < FPMIN) c=FPMIN;
    d=1.0/d;
    h *= d*c;
    aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
    d=1.0+aa*d;
    if (fabs(d) < FPMIN) d=FPMIN;
    c=1.0+aa/c;
    if (fabs(c) < FPMIN) c=FPMIN;
    d=1.0/d;
    del=d*c;
    h *= del;
    if (fabs(del-1.0) < EPS) break;
  }

  return h;
}

template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::gammln(float xx)
{
  double x,y,tmp,ser;
  static double cof[6]={76.18009172947146,-86.50532032941677,
			24.01409824083091,-1.231739572450155,
			0.1208650973866179e-2,-0.5395239384953e-5};
  int j;

  y=x=xx;
  tmp=x+5.5;
  tmp -= (x+0.5)*log(tmp);
  ser=1.000000000190015;
  for (j=0;j<=5;j++) ser += cof[j]/++y;
  return -tmp+log(2.5066282746310005*ser/x);
}

template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::betai(float a, float b, float x)
{
  float betacf(float a, float b, float x);
  float gammln(float xx);
  float bt;

  if (x == 0.0 || x == 1.0) bt=0.0;
  else
    bt=exp(this->gammln(a+b)-this->gammln(a)-this->gammln(b)+a*log(x)+b*log(1.0-x));
  if (x < (a+1.0)/(a+b+2.0))
    return bt*this->betacf(a,b,x)/a;
  else
    return 1.0-bt*this->betacf(b,a,1.0-x)/b;
}


template <class TInputImage, class TPrecision  >
float 
LineSegmentDetector<TInputImage, TPrecision>
::factln(int n)
{
  float gammln(float xx);
  static float a[101];

  if (n <= 1) return 0.0;
  if (n <= 100) return a[n] ? a[n] : (a[n]=this->gammln(n+1.0));
  else return this->gammln(n+1.0);
}
/*--------------------------------------------------------------------------------------------------------*/
} // end namespace otb

#endif
