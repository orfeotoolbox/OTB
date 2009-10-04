/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) CS systèmes d'information. All rights reserved.
See CSCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbImageToSURFKeyPointSetFilter.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkImageRegionIterator.h"
#include "otbImageFileWriter.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template < class TInputImage, class TOutputPointSet >
ImageToSURFKeyPointSetFilter<  TInputImage, TOutputPointSet >
::ImageToSURFKeyPointSetFilter()
{
  m_OctavesNumber = 1;
  m_ScalesNumber = 3;
  m_NumberOfPoints = 0;
  m_DetHessianFilter = ImageToDetHessianImageType::New();
  m_IntegralImage    = InputImageType::New();
}


/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/

template <class TInputImage, class TOutputPointSet>
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::~ImageToSURFKeyPointSetFilter()
{}

/*-------------------------------------------------------
 * Generate Data
 --------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet>
void
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::GenerateData(void)
{

  /**Integral Image */
  std::cout << "Begin Integral Image"<< std::endl;
  this->IntegralImage();
  std::cout << "Done"<< std::endl;

  double k;
  double sigma_in = 2.;
  SizeType radius;

  /*New Adds*/
  double fscale = 1.;
  double initSigma = 1.6;

  /*Output*/
  OutputPointSetPointerType  outputPointSet = this->GetOutput();

  /** Computing the multiplicative factor for scales iteration
   * scalar used for the wifth of the gaussian
   */
  if (m_ScalesNumber > 1 )
    k = (double)std::pow(2.0,(double)(1/(double)(m_ScalesNumber-1)));
  else
    k = 3;


  /* Computation loop over octaves*/
  for (int i = 0; i < m_OctavesNumber; ++i )
  {

    sigma_in = 2.;
    m_ImageList = ImageListType::New();


    /*--------------------------------------------------------
    Octave per octave
    --------------------------------------------------------*/
    if (i>0)
    {

      m_ResampleFilter = ResampleFilterType::New();
      m_ResampleFilter ->SetInput(this->GetInput());

      SizeType size = this->GetInput()->GetLargestPossibleRegion().GetSize();
      for (int k = 0; k < 2; ++k)
        size[k] = (unsigned int) floor(size[k]/std::pow(2.0,i) );
      m_ResampleFilter->SetSize( size );

      SpacingType spacing = this->GetInput()->GetSpacing();
      for (int k = 0; k < 2; ++k)
        spacing[k] = (spacing[k] * std::pow(2.0,i));
      m_ResampleFilter->SetOutputSpacing( spacing );

      m_ResampleFilter->SetDefaultPixelValue( 0 );
      m_ResampleFilter->Update();
      m_determinantImage = m_ResampleFilter->GetOutput();

      otbGenericMsgDebugMacro( <<"ImageToSURFKeyPointSetFilter:: Size of the image at the octave : " \
                               << i << " is " \
                               <<m_determinantImage->GetLargestPossibleRegion().GetSize() );

    }

    for (int j = 0; j < m_ScalesNumber; ++j )
    {
      /** Incrementation of the gaussian width
       *  the width of the gaussian have to be doubled for
       *  each iteration over octaves
       */

      if ((i != 0 && j !=0 ) || (i == 0  && (i+j !=0) ) || ( m_ScalesNumber == 1 && i!=0 ))
        sigma_in *= k;

      /**
       * For each octave, we serach for the key points
       */

      /** Hessian Determinant Image */
      m_DetHessianFilter = ImageToDetHessianImageType::New();

      if ( i == 0 )m_DetHessianFilter->SetInput(this->GetInput());
      else m_DetHessianFilter->SetInput(m_determinantImage );

      m_DetHessianFilter->SetSigma(sigma_in);
      m_DetHessianFilter->Update();
      m_determinantImage = m_DetHessianFilter->GetOutput();

      if (i+j==0)
      {
        otbGenericMsgDebugMacro( <<"ImageToSURFKeyPointSetFilter:: Size of the image at the octave : "
                                 << i << " is "
                                 <<m_determinantImage->GetLargestPossibleRegion().GetSize() );
      }

      /** For each octave, we fill the imageList for the extremum search*/
      m_ImageList->PushBack(m_determinantImage);
    }

    /*----------------------------------------------------*/
    /*           extremum  Search over octave's scales    */
    /*----------------------------------------------------*/

    for (int jj = 1; jj < (int)(m_ImageList->Size() - 1 ); ++jj)
    {
      m_ImageCurrent = m_ImageList->GetNthElement(jj);
      m_ImageMovedPrev = m_ImageList->GetNthElement(jj-1);
      m_ImageMovedNext = m_ImageList->GetNthElement(jj+1);


      /** NeighboorhoodIterator parameters*/
      radius.Fill(1);
      NeighborhoodIteratorType it(radius, m_ImageCurrent,m_ImageCurrent->GetLargestPossibleRegion());
      it.GoToBegin();


      /* NeighboorhoodIterator Adjacents parameters*/
      NeighborhoodIteratorType itNeighPrev(radius, m_ImageMovedPrev,m_ImageMovedPrev->GetLargestPossibleRegion());
      itNeighPrev.GoToBegin();

      NeighborhoodIteratorType itNeighNext(radius, m_ImageMovedNext,m_ImageMovedNext->GetLargestPossibleRegion());
      itNeighNext.GoToBegin();

      while (!it.IsAtEnd())
      {

        if (IsLocalExtremum(it.GetNeighborhood())
            && IsLocalExtremumAround(itNeighPrev.GetNeighborhood(),m_ImageCurrent->GetPixel(it.GetIndex()))
            && IsLocalExtremumAround(itNeighNext.GetNeighborhood(),m_ImageCurrent->GetPixel(it.GetIndex())) )
        {
          OutputPointType keyPoint;
          itNeighPrev.SetLocation(it.GetIndex());
          itNeighNext.SetLocation(it.GetIndex());

          keyPoint[0] =  it.GetIndex()[0];
          keyPoint[1] =  it.GetIndex()[1];

          //keyPoint[2] =  sigma_in/pow(k,(double)jj)*pow(2.,(double)i);
          //double sigmaDetected = sigma_in/pow(k,(double)jj)*pow(2.,(double)i);

      double fSize         = initSigma*pow(2.,((double)i+1.)/m_OctavesNumber);
      double sigmaDetected = fSize * fscale;

        radius.Fill((int)sigmaDetected*6);

          /*
            Computing the orientation of the key point detected
          */
          NeighborhoodIteratorType itNeighOrientation(radius,m_IntegralImage/*this->GetInput()*/ ,
                              /*this->GetInput()*/m_IntegralImage->GetLargestPossibleRegion());

          itNeighOrientation.SetLocation(it.GetIndex());

          /** TO DO*/
          //keyPoint[3] = AssignOrientation( itNeighOrientation.GetNeighborhood() ,keyPoint[2] );
      double orientationDetected = AssignOrientation( itNeighOrientation.GetNeighborhood() , sigmaDetected , it.GetIndex());

          /*Filling the Point pointSet Part*/
          outputPointSet->SetPoint(m_NumberOfPoints, keyPoint);


          /*----------------------------------------*/
          /*  Descriptor Computation                */
          /*----------------------------------------*/
      radius.Fill(vcl_floor(sigmaDetected*10));

          NeighborhoodIteratorType itNeighDescriptor(radius,this->GetInput(),
              this->GetInput()->GetLargestPossibleRegion());
          itNeighDescriptor.SetLocation(it.GetIndex());
          VectorType descriptor;
          descriptor.resize(64);
          //descriptor = ComputeDescriptor(itNeighDescriptor.GetNeighborhood(),keyPoint[3],keyPoint[2]);
          descriptor = ComputeDescriptor(itNeighDescriptor.GetNeighborhood(),orientationDetected,sigmaDetected, it.GetIndex());

          /*Updating the pointset data with values of the descriptor*/
          OutputPixelType data;
          data.SetSize(64);

          unsigned int IndDescriptor = 0;

          typename std::vector<double>::const_iterator  itDescriptor =
            descriptor.begin();

          while (itDescriptor != descriptor.end())
          {
            data.SetElement(IndDescriptor, *itDescriptor);
            ++IndDescriptor;
            ++itDescriptor;
          }
          outputPointSet->SetPointData(m_NumberOfPoints, data);

          ++m_NumberOfPoints;
        }
        ++it;
        ++itNeighPrev;
        ++itNeighNext;

      }/*End while for extremum search*/

      fscale += fscale;

    } /*End Iteration over scales */

    m_ImageList->Clear();

  } /* End  Key Points search*/

  otbGenericMsgDebugMacro( <<"ImageToSURFKeyPointSetFilter:: Total Number of Key points "\
                           << m_NumberOfPoints  );

}/** End of GenerateData()*/

template <class TInputImage, class TOutputPointSet>
int
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::GetMin( int a , int b , int c)
{
  return std::min(a,std::min(b,c));
}

/*-----------------------------------------------------------
 * Find Local Extremum
 -----------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet>
bool
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::IsLocalExtremum(const NeighborhoodType& neigh)
{
  int centerIndex = neigh.GetCenterNeighborhoodIndex(), i = 0;
  double centerValue = neigh[centerIndex];
  bool max = false, min = false;
  int flag_min = 0, flag_max = 0;

  while (i!=(int)neigh.Size())
  {
    if (i != centerIndex )
    {
      if ( centerValue> neigh[i] && flag_max == 0)   max = true;
      else
      {
        max = false;
        flag_max = 1;
      }

      if (centerValue < neigh[i] && flag_min == 0 && centerValue <0)   min = true;
      else
      {
        min = false;
        flag_min = 1;
      }
    }
    ++i;
  }

  return max || min;
}

/*-----------------------------------------------------------
 *Find Local Extremum Around
 -----------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet>
bool
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::IsLocalExtremumAround(const NeighborhoodType& neigh , double CenterValue)
{

  int i = 0;
  bool max = false, min = false;
  int flag_min = 0, flag_max = 0;

  while (i!=(int)neigh.Size())
  {

    if ( CenterValue> neigh[i] && flag_max == 0)   max = true;
    else
    {
      max = false;
      flag_max = 1;
    }

    if (CenterValue < neigh[i] && flag_min == 0)   min = true;
    else
    {
      min = false;
      flag_min = 1;
    }

    ++i;
  }

  return max || min;
}

/*-----------------------------------------------------------
 * Compute the orientation of The KeyPoint
 -----------------------------------------------------------*/

template <class TInputImage, class TOutputPointSet>
double
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::AssignOrientation(const NeighborhoodType& neigh , double S, IndexType cindex)
{
  SizeType size  = this->GetInput()->GetLargestPossibleRegion().GetSize();

  unsigned int scale = vcl_floor(S + 0.5 );
  std::vector<double> resX, resY, Angs;

  for(int i = -6; i <= 6; ++i)
  {
    for(int j = -6; j <= 6; ++j)
    {
      if(i*i + j*j < 36)
      {
    double w = 1./(2.*CONST_PI*4*S*S)*vcl_exp(-(  i*i+j*j  ) / (2.*4.*S*S) );
    resX.push_back( w*this->HaarX(cindex[1] + j*scale ,cindex[0] + i*scale , 4*scale));
    resY.push_back( w*this->HaarY(cindex[1] + j*scale ,cindex[0] + i*scale , 4*scale));
    Angs.push_back(GetAngle(resX.back(), resY.back()));
      }
    }
  }

  //Calculate the dominant direction
  double sumX, sumY;
  double max=0., orientation = 0.;
  double ang1, ang2, ang;

  // loop slides pi/3 window around feature point
  for(ang1 = 0; ang1 < 2*CONST_PI;  ang1 += 0.15)
    {
      ang2 = ( (ang1 + CONST_PI/3. > 2*CONST_PI )  ? ang1 - 5.*CONST_PI/3. : ang1+CONST_PI/3.0f);
      sumX = sumY = 0;
      for(unsigned int k = 0; k < Angs.size(); k++)
    {
      // get angle from the x-axis of the sample point
      ang = Angs[k];

      // determine whether the point is within the window
      if (ang1 < ang2 && ang1 < ang && ang < ang2)
        {
          sumX+=resX[k];
          sumY+=resY[k];
        }
      else if (ang2 < ang1 &&
           ((ang > 0 && ang < ang2) || (ang > ang1 && ang < 2*CONST_PI) ))
        {
          sumX+=resX[k];
          sumY+=resY[k];
        }
    }

      // if the vector produced from this window is longer than all
      // previous vectors then this forms the new dominant direction
      if (sumX*sumX + sumY*sumY > max)
    {
      // store largest orientation
      max = sumX*sumX + sumY*sumY;
      orientation = GetAngle(sumX, sumY);
    }
    }
  return orientation;
}


/*-----------------------------------------------------------
 * HaarX wavelet reponse in the X axis
 -----------------------------------------------------------*/

template <class TInputImage, class TOutputPointSet>
double
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::HaarX( unsigned int row , unsigned int col, double S )
{
  return this->BoxIntegral(row-S/2, col, S, S/2) -1 * this->BoxIntegral(row-S/2, col-S/2, S, S/2);
}

// /*-----------------------------------------------------------
//  * HaarY wavelet reponse in the Y axis
//  -----------------------------------------------------------*/

template <class TInputImage, class TOutputPointSet>
double
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::HaarY(unsigned int row , unsigned int col, double S )
{
 return BoxIntegral(row, col-S/2, S/2, S) -1 * BoxIntegral(row-S/2, col-S/2, S/2, S);

}


/*BoxIntegral */

template <class TInputImage, class TOutputPointSet>
double
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::BoxIntegral( int row,  int col,  int nbRow ,  int nbCol)
{
  //Size of the image
  InputImagePointerType intimage = m_IntegralImage;
  SizeType  size = intimage->GetLargestPossibleRegion().GetSize();

  // The subtraction by one for row/col is because row/col is inclusive.
  int r1 = std::min((double)row,           (double)size[1]) - 1;
  int c1 = std::min((double)col,           (double)size[0]) - 1;
  int r2 = std::min((double)row + nbRow,   (double)size[1]) - 1;
  int c2 = std::min((double)col + (double)nbCol,   (double)size[0]) - 1;

  double A(0.), B(0.), C(0.), D(0.);

  IndexType i1,i2,i3,i4;
  i1[1] = r1; i1[0] = c1;
  i2[1] = r1; i2[0] = c2;
  i3[1] = r2; i3[0] = c1;
  i4[1] = r2; i4[0] = c2;

  if (r1 >= 0 && c1 >= 0)  A = intimage->GetPixel(i1);//data[r1 * size[0] + c1];
  if (r1 >= 0 && c2 >= 0)  B = intimage->GetPixel(i2);//data[r1 * size[0] + c2];
  if (r2 >= 0 && c1 >= 0)  C = intimage->GetPixel(i3);//data[r2 * size[0] + c1];
  if (r2 >= 0 && c2 >= 0)  D = intimage->GetPixel(i4);//data[r2 * size[0] + c2];

  return std::max(0., A - B - C + D);
}

/*-----------------------------------------------------------
 * Compute the descriptor of The KeyPoint
 -----------------------------------------------------------*/

template <class TInputImage, class TOutputPointSet>
typename ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::VectorType
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::ComputeDescriptor(const NeighborhoodType& neigh , double O , double S , IndexType cindex)
{

//   int y, x, sample_x, sample_y, count=0;
//   int i = 0, ix = 0, j = 0, jx = 0, xs = 0, ys = 0;
//   double  dx, dy, mdx, mdy, co, si;

//   VectorType  descriptorVector;
//   descriptorVector.resize(64);

//   double gauss_s1 = 0.f, gauss_s2 = 0.f;
//   double rx = 0.f, ry = 0.f, rrx = 0.f, rry = 0.f, len = 0.f;
//   double cx = -0.5f, cy = 0.f; //Subregion centers for the 4x4 gaussian weighting



//   x = cindex[0];
//   y = cindex[1];
//   co = vcl_cos(O);
//   si = vcl_sin(O);

//   i = -8;

//   //Calculate descriptorVectorriptor for this interest point
//   //Area of size 24 s x 24 s
//   //***********************************************
//   while(i < 12)
//   {
//     j = -8;
//     i = i-4;

//     cx += 1.f;
//     cy = -0.5f;

//     while(j < 12)
//     {
//       dx=dy=mdx=mdy=0.f;
//       cy += 1.f;

//       j = j - 4;

//       ix = i + 5;
//       jx = j + 5;

//       xs = vcl_floor(x + ( -jx*sigma*si + ix*sigma*co) +0.5);
//       ys = vcl_floor(y + ( jx*sigma*co + ix*sigma*si)  +0.5 );

//       for (int k = i; k < i + 9; ++k)
//       {
//         for (int l = j; l < j + 9; ++l)
//         {
//           //Get coords of sample point on the rotated axis
//           sample_x =vcl_floor(x + (-l*sigma*si + k*sigma*co) + 0.5 );
//           sample_y = vcl_floor(y + ( l*sigma*co + k*sigma*si) + 0.5 );

//           //Get the gaussian weighted x and y responses
//       gauss_s1= 1./(2.0f*CONST_PI*sigma*sigma)*vcl_exp(-((xs-sample_x)*(xs-sample_x) +  (ys-sample_y)*(ys-sample_y) ) / (2*2.5*2.5*sigma*sigma) );

//           rx = this->HaarX(sample_y, sample_x, 2*sigma);
//           ry = this->HaarY(sample_y, sample_x, 2*sigma);

//           //Get the gaussian weighted x and y responses on rotated axis
//  //          rrx = -rx*si + ry*co;
// //           rry = rx*co + ry*si;

// //           rrx = gauss_s1*rrx;
// //           rry = gauss_s1*rry;

//            rrx = gauss_s1*rx;
//            rry = gauss_s1*ry;

//           dx += rrx;
//           dy += rry;
//           mdx += fabs(rrx);
//           mdy += fabs(rry);

//         }
//       }

//       //Add the values to the descriptorVectorriptor vector
//       //gauss_s2 = 1./(2.0f*CONST_PI*1.5*1.5)*vcl_exp(-((cx-2.)*(cx-2.) +  (cy-2.)*(cy-2.) ) / (2*1.5*1.5) );
//       gauss_s2 = 1.;
//       descriptorVector[count++] = dx*gauss_s2;
//       descriptorVector[count++] = dy*gauss_s2;
//       descriptorVector[count++] = mdx*gauss_s2;
//       descriptorVector[count++] = mdy*gauss_s2;

//       len += dx*dx + dy*dy + mdx*mdx + mdy*mdy;

//       j += 9;
//     }
//     i += 9;
//   }

//   //Convert to Unit Vector
//   len = sqrt(len);
//   for(int i = 0; i < 64; i++)
//     {
//     descriptorVector[i] /= len;
//     }

//   return descriptorVector;



  typedef itk::CenteredRigid2DTransform<double> TransformType;
  TransformType::Pointer eulerTransform = TransformType::New();
  TransformType::ParametersType  ParamVec(5);
  PointImageType pSrc , pDst;

  VectorType  descriptorVector;
  descriptorVector.resize(64);

  /** Parameters of the transformation*/
  ParamVec[0] = O;
  ParamVec[1] = cindex[0];
  ParamVec[2] = cindex[1];
  ParamVec[3] = 0;
  ParamVec[4] = 0;
  eulerTransform->SetParameters(ParamVec);

  int sigma = vcl_floor(S+0.5);
  unsigned int count = 0;
  double cx = -0.5;
  double cy = 0.;
  double si = vcl_sin(O);
  double co = vcl_cos(O);

  for(int i = -10 * sigma ; i < 10 * sigma ; i+= 5*sigma )
  {
    cx += 1.;
    cy = -0.5;
    for(int j = -10 * sigma ; j < 10 * sigma ; j+= 5*sigma )
    {
      cy += 1.;
      double dx = 0.,dy =0., mdx = 0.,mdy = 0.;

      for(int  k = i ; k< i+5*sigma ; k+= sigma )
        for(int  l = j ; l< j+5*sigma ; l+= sigma )
        {
          double distance = vcl_sqrt(static_cast<double>(k*k  + i*i));
          if(distance < 10*S)
          {
            typename InputImageType::OffsetType offset ;
            offset[0] = k;
            offset[1] = l;

            IndexType currentI = cindex + offset;
            //m_IntegralImage->TransformIndexToPhysicalPoint(currentI,pSrc);
            //pDst = eulerTransform->TransformPoint(pSrc);
            //Get coords of sample point on the rotated axis
            pDst[0] = vcl_floor( (cindex[0] + (-l*si + k*co)) + 0.5 );
            pDst[1] = vcl_floor( (cindex[1] + ( l*co + k*si)) + 0.5 );

            double w = 1./(2.*3.3*CONST_PI*S*S)*vcl_exp(-(  offset[0]*offset[0]+ offset[1]*offset[1]) / (2.*3.3*3.3*S*S) );
            double rx = this->HaarX(pDst[1],pDst[0],2*sigma);
            double ry = this->HaarY(pDst[1],pDst[0],2*sigma);

            //Get the gaussian weighted x and y responses on rotated axis
            double rrx = -rx*vcl_sin(O) + ry*vcl_cos(O);
            double rry = rx*vcl_cos(O) + ry*vcl_sin(O);
            rrx *= w;
            rry *= w;

            dx += rrx;
            dy += rry;
            mdx += vcl_abs(rrx);
            mdy += vcl_abs(rry);
          }
        }

      //Edit the descriptor
      double w2 = 1./(2.*CONST_PI*1.5*1.5)*vcl_exp(-(   (cx -2.)*(cx -2.)+ (cy-2.)*(cy-2.)) / (2.*1.5*1.5 ));
      descriptorVector[count++] = w2*dx;
      descriptorVector[count++] = w2*dy;
      descriptorVector[count++] = w2*mdx;
      descriptorVector[count++] = w2*mdy;
    }
  }

//   int i = 0,  col, raw  , Nbin, pas = 1;
//   double xx = 0, yy = 0;
//   double dx, dy , w;
//   int Largeur = 2*neigh.GetRadius()[0]+1;
//   double rayon =  static_cast<double>(Largeur)/4.;
//   double r = neigh.GetRadius()[0];
//   double dist = 0;
//   double x0 = neigh.GetCenterNeighborhoodIndex()% Largeur;
//   double y0 = neigh.GetCenterNeighborhoodIndex()/ Largeur;


//   VectorType  descriptorVector;
//   descriptorVector.resize(64);

//   /** Parameters of the transformation*/
//   ParamVec[0] = angle;
//   ParamVec[1] = x0;
//   ParamVec[2] = y0;
//   ParamVec[3] = 0;
//   ParamVec[4] = 0;
//   eulerTransform->SetParameters(ParamVec);

//   while (i < (int)neigh.Size())
//   {
//     col = i % Largeur;
//     raw = i / Largeur;

//     if (( col > pas && col < Largeur - pas ) && ( raw > pas && raw < Largeur - pas) )
//     {
//       double distanceX = (raw-r);
//       double distanceY = (col-r);
//       dist = vcl_sqrt(distanceX*distanceX + distanceY*distanceY);

//       if (dist <= r )
//       {
//         /* Transform point to compensate the rotation the orientation */
//         pDst[0] = col;
//         pDst[1] = raw;
//         pSrc = eulerTransform->TransformPoint(pDst);

//         /** New Coordinates (rotated) */
//         col = static_cast<int>(vcl_floor(pSrc[0]));
//         raw = static_cast<int>(vcl_floor(pSrc[1]));

//         if (raw==0) raw =+1;
//         if (col ==0) col +=1;

//         xx = static_cast<int> (pSrc[1]/rayon);
//         yy = static_cast<int> (pSrc[0]/rayon);
//         Nbin =  static_cast<int> (xx + 4*yy);

//         if ( Nbin < 16)          //because 64 descriptor length
//         {
//           double distanceXcompensee_2 = (pSrc[0] - r)*(pSrc[0] - r);
//           double distanceYcompensee_2 = (pSrc[1] - r)*(pSrc[1] - r);

//           w = 1./(2.*CONST_PI*S*S)*vcl_exp(-( distanceXcompensee_2 + distanceYcompensee_2 ) / (2*3.3*3.3*S*S) );

//           dx = 0.5 * (neigh[(col+pas) + raw * Largeur] - neigh[(col-pas) + raw *Largeur]) * w;
//           dy = 0.5 * (neigh[col + (raw+ pas)* Largeur] - neigh[col + (raw-pas)*Largeur])  * w;

//       //dx = this->HaarX(pSrc[1]   ,pSrc[0] , 2*vcl_floor(S+0.5));
//       //dy = this->HaarY(pSrc[1] ,pSrc[0]  , 2*vcl_floor(S+0.5));

//           descriptorVector[4*Nbin  ] += dx;
//           descriptorVector[4*Nbin+1] += dy;
//           descriptorVector[4*Nbin+2] += vcl_abs(dx);
//           descriptorVector[4*Nbin+3] += vcl_abs(dy);
//         }
//       }
//     }
//     ++i;
//   }

  double accu = 0;
  for (int i = 0; i < 64;  ++i)
    accu += descriptorVector[i]*descriptorVector[i];

  for (int j = 0; j < 64;  ++j)
    descriptorVector[j] /= vcl_sqrt(accu);

  return descriptorVector;
}

/*----------------------------------------------------------------
  Compute Integral Images
  -----------------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet  >
void
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::IntegralImage()
{
  //InputImagePointerType  m_IntegralImage    = InputImageType::New();
  InputImagePointerType  sourceImage = const_cast<InputImageType*>(this->GetInput());

  m_IntegralImage->SetRegions(sourceImage->GetLargestPossibleRegion());
  m_IntegralImage->Allocate();
  m_IntegralImage->FillBuffer(0.);

  itk::ImageRegionIterator<InputImageType>   itSource(sourceImage,
                               sourceImage->GetLargestPossibleRegion());

  itk::ImageRegionIterator<InputImageType>   itInt(m_IntegralImage,
                            m_IntegralImage->GetLargestPossibleRegion());

  itSource.GoToBegin();
  itInt.GoToBegin();
  double rowSum = 0.;

  while(!itSource.IsAtEnd())
    {
      IndexType index = itSource.GetIndex();

      if(index[0]  == 0)
      rowSum = 0.;

      if(index[1]  == 0 )
    {
      rowSum += itSource.Get();
      itInt.Set(rowSum);
    }
      else
    {
      //Index of the upper pixel (raw -1 & col)
      IndexType prevIndex = index;
      prevIndex[1] -= 1;
      itInt.SetIndex(prevIndex);
      double prevValue = itInt.Get();

      //Edit the integral image
      itInt.SetIndex(index);
      rowSum += itSource.Get();
      itInt.Set(prevValue + rowSum);
    }
      ++itSource;
      ++itInt;
    }

}

/*----------------------------------------------------------------
 * GetAngle
 * -----------------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet  >
double
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::GetAngle(unsigned int  X, unsigned int Y)
{
  if(X > 0 && Y >= 0)
    return vcl_atan2(Y,X);

  if(X < 0 && Y >= 0)
    return CONST_PI - vcl_atan2(-Y,X);

  if(X < 0 && Y < 0)
    return  CONST_PI + vcl_atan2(Y,X);

  if(X > 0 && Y < 0)
    return 2*CONST_PI - vcl_atan2(-Y,X);


  return 0;
}

/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputImage, class TOutputPointSet  >
void
ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of Key Points  " << m_NumberOfPoints  << std::endl;
}
}
