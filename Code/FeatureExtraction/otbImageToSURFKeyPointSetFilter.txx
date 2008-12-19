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


    double k;
    double sigma_in = 2.;
    SizeType radius;

    /*Output*/
    OutputPointSetPointerType  outputPointSet = this->GetOutput();

    /** Computing the multiplicative factor for scales iteration
     * scalar used for the wifth of the gaussian
     */
    if(m_ScalesNumber > 1 )
      k = (double)std::pow(2.0,(double)(1/(double)(m_ScalesNumber-1))) ;
    else
      k = 3;


     /* Computation loop over octaves*/
    for (int i = 0; i < m_OctavesNumber ; i++ ){

      sigma_in = 2.;
      m_ImageList = ImageListType::New();

      /*--------------------------------------------------------
	Octave per octave
	--------------------------------------------------------*/
      if(i>0) {

	m_ResampleFilter = ResampleFilterType::New();
	m_ResampleFilter ->SetInput(this->GetInput(0));

	SizeType size = this->GetInput(0)->GetLargestPossibleRegion().GetSize();
	for (int k = 0; k < 2; ++k)
	  size[k] = (unsigned int) floor(size[k]/std::pow(2.0,i) );
	m_ResampleFilter->SetSize( size );

	SpacingType spacing = this->GetInput(0)->GetSpacing();
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

      for (int j = 0 ; j < m_ScalesNumber; j++ )
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

	if ( i == 0 )m_DetHessianFilter->SetInput(this->GetInput(0));
  	else m_DetHessianFilter->SetInput(m_determinantImage );

	m_DetHessianFilter->SetSigma(sigma_in);
	m_DetHessianFilter->Update();
	m_determinantImage = m_DetHessianFilter->GetOutput() ;

	  if(i+j==0)
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

      for (int jj = 1 ; jj < (int)(m_ImageList->Size() - 1 )  ; jj++)
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

	  while(!it.IsAtEnd())
	    {

	      if(IsLocalExtremum(it.GetNeighborhood())
		 && IsLocalExtremumAround(itNeighPrev.GetNeighborhood(),m_ImageCurrent->GetPixel(it.GetIndex()))
		 && IsLocalExtremumAround(itNeighNext.GetNeighborhood(),m_ImageCurrent->GetPixel(it.GetIndex())) )
		{
		  OutputPointType keyPoint;
		  itNeighPrev.SetLocation(it.GetIndex());
		  itNeighNext.SetLocation(it.GetIndex());

		  keyPoint[0] =  it.GetIndex()[0];
		  keyPoint[1] =  it.GetIndex()[1];

		  //keyPoint[2] =  sigma_in/pow(k,(double)jj)*pow(2.,(double)i);
		  double sigmaDetected = sigma_in/pow(k,(double)jj)*pow(2.,(double)i);

		  radius.Fill(GetMin((int)(this->GetInput(0)->GetLargestPossibleRegion().GetSize()[0] - keyPoint[0]),
				     (int)(this->GetInput(0)->GetLargestPossibleRegion().GetSize()[1] - keyPoint[1]),
				     (int)(6*sigmaDetected) ) ) ; // changer le sigma detected par keypoint[2]



		  /*
		    Computing the orientation of the key point detected
		  */
		  NeighborhoodIteratorType itNeighOrientation(radius,this->GetInput(0) ,
							      this->GetInput(0)->GetLargestPossibleRegion());

		  itNeighOrientation.SetLocation(it.GetIndex());

		  /** TO DO*/
		  //keyPoint[3] = AssignOrientation( itNeighOrientation.GetNeighborhood() ,keyPoint[2] );
		  double orientationDetected = AssignOrientation( itNeighOrientation.GetNeighborhood() , sigmaDetected );

		  /*Filling the Point pointSet Part*/
		  outputPointSet->SetPoint(m_NumberOfPoints, keyPoint);


		  /*----------------------------------------*/
		  /*  Descriptor Computation                */
		  /*----------------------------------------*/

		  radius.Fill(GetMin((int)(this->GetInput(0)->GetLargestPossibleRegion().GetSize()[0] - keyPoint[0]),
				     (int)(this->GetInput(0)->GetLargestPossibleRegion().GetSize()[1] - keyPoint[1]),
				     (int)(10*sigmaDetected))); // TODO a changer sigmaDetected par Keypoint[2]

		  NeighborhoodIteratorType itNeighDescriptor(radius,this->GetInput(0),
							     this->GetInput(0)->GetLargestPossibleRegion());
		  itNeighDescriptor.SetLocation(it.GetIndex());
		  VectorType descriptor;
		  descriptor.resize(64);
		  //descriptor = ComputeDescriptor(itNeighDescriptor.GetNeighborhood(),keyPoint[3],keyPoint[2]);
		  descriptor = ComputeDescriptor(itNeighDescriptor.GetNeighborhood(),orientationDetected,sigmaDetected);



		  /*Updating the pointset data with values of the descriptor*/
		  OutputPixelType data;
		  data.SetSize(64);

		  unsigned int IndDescriptor = 0;

		  typename std::vector<double>::const_iterator  itDescriptor =
		    descriptor.begin();

		   while(itDescriptor != descriptor.end())
		    {
		      data.SetElement(IndDescriptor, *itDescriptor);
		      IndDescriptor++;
		      itDescriptor++;
		    }
		  outputPointSet->SetPointData(m_NumberOfPoints, data);

		  m_NumberOfPoints++;
		}
	      ++it;
	      ++itNeighPrev;
	      ++itNeighNext;

	    }/*End while for extremum search*/

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

    while (i!=(int)neigh.Size()){
      if(i != centerIndex ){
	if( centerValue> neigh[i] && flag_max == 0)   max = true;
	else { max = false;  flag_max = 1; }

	if(centerValue < neigh[i] && flag_min == 0 && centerValue <0)   min = true;
	else {  min = false; flag_min = 1; }
      }
      ++i;
    }

    return max || min ;
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

    while (i!=(int)neigh.Size()){

      if( CenterValue> neigh[i] && flag_max == 0)   max = true;
      else { max = false;  flag_max = 1; }

      if(CenterValue < neigh[i] && flag_min == 0)   min = true;
      else {  min = false; flag_min = 1; }

      ++i;
    }

    return max || min ;
  }

  /*-----------------------------------------------------------
   * Compute the orientation of The KeyPoint
   -----------------------------------------------------------*/

  template <class TInputImage, class TOutputPointSet>
  double
  ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
  ::AssignOrientation(const NeighborhoodType& neigh , double S)
  {

    int i= 0 ;
    int pas =( (i+S)-(int)(i+S) > 0.5 )?((int)S+1):(int)S  ;
    int Largeur = 2*neigh.GetRadius()[0]+1;                // Width & length of a neighborhood
    int rayon = neigh.GetRadius()[0];                      // radius of the neigh
    int col, raw ;
    double dist ;
    double  w;                                             // weight of the circular gaussian

    OutputPointType pt ;

    // Gradient orientation histogram
    double angle;
    int bin        = 0 ;
    int Pi         = 180;
    int LengthBin  = 60;
    int NbBins     = (2*Pi/LengthBin);
    std::vector<double> tab(NbBins*2 , 0.);

    while (i < (int)neigh.Size())
      {
	col = i%Largeur - rayon ;
	raw = i/Largeur - rayon ;
	dist = vcl_sqrt(static_cast<double>(col *col  + raw * raw) );
	col +=rayon;
	raw +=rayon;                           // Backup to the image coordinate axes

	if(dist < 6*S)
	{
	  // Haar Wavelets responses accumulated in an histogram with Pi/3 precison
	  if (( col > pas && col < Largeur - pas ) && ( raw > pas && raw < Largeur - pas) )
	  {

	    w  = vcl_exp(-((col-rayon)*(col-rayon) + (raw-rayon)*(raw-rayon))/(2*2.5*2.5*S*S) );
	    pt[0] = (neigh[(col+pas) + raw * Largeur] - neigh[(col-pas) + raw *Largeur ]) * w ;
	    pt[1] = (neigh[col + (raw+pas)* Largeur ] - neigh[col + (raw-pas)*Largeur]) * w;

	    if (pt[0] + pt[1] != 0)
	    {
	      angle = atan( pt[0]/pt[1] )*( Pi/M_PI);
	      if(angle < 0 )
		angle += 2*Pi;

	      bin = (int)(angle/LengthBin);

	      if( bin <= NbBins-1  || bin >= 0 )
	      {
		tab[2*bin]   += pt[0];
		tab[2*bin+1] += pt[1];
	      }
	    }
	  }
	}
	i+= pas;
      }

    //Find Orientation
    double  indice = 0;
    double  max    = 0;
    double  length = 0;

    //Detection de l'orientation du point courant
    for (int i = 0 ; i < NbBins*2  ; i = i+2){
      length = vcl_sqrt( tab[i]*tab[i] + tab[i+1]*tab[i+1] );
      if( length > max){
	max = length ;
	indice = i/2;
      }
    }

    return (indice+0.5)*LengthBin;
  }

  /*-----------------------------------------------------------
   * Compute the descriptor of The KeyPoint
   -----------------------------------------------------------*/

  template <class TInputImage, class TOutputPointSet>
  typename ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
  ::VectorType
  ImageToSURFKeyPointSetFilter< TInputImage, TOutputPointSet>
  ::ComputeDescriptor(const NeighborhoodType& neigh , double O , double S )
  {

    typedef itk::CenteredRigid2DTransform<double> TransformType;
    TransformType::Pointer eulerTransform = TransformType::New();
    TransformType::ParametersType  ParamVec(5);
    PointImageType pSrc , pDst;
    double angle = O * M_PI / 180;



    int i = 0,  col, raw  , Nbin, pas = 1 ;
    double xx = 0, yy = 0;
    double dx, dy , w ;
    int Largeur = 2*neigh.GetRadius()[0]+1;
    double rayon =  static_cast<double>(Largeur)/4. ;
    double r = neigh.GetRadius()[0];
    double dist = 0;
    double x0 = neigh.GetCenterNeighborhoodIndex()% Largeur ;
    double y0 = neigh.GetCenterNeighborhoodIndex()/ Largeur ;

    //std::cout << " x0 " << x0 << " y0 "  << y0 << angle << std::endl;

    VectorType  descriptorVector;
    descriptorVector.resize(64);

    /** Parameters of the transformation*/
    ParamVec[0] = angle;
    ParamVec[1] = x0;
    ParamVec[2] = y0;
    ParamVec[3] = 0;
    ParamVec[4] = 0;
    eulerTransform->SetParameters(ParamVec);

    while (i < (int)neigh.Size())
      {
	col = i % Largeur ;
	raw = i / Largeur ;

	if (( col > pas && col < Largeur - pas ) && ( raw > pas && raw < Largeur - pas) )
	  {
	    double distanceX = (raw-r);
	    double distanceY = (col-r);
	    dist = vcl_sqrt(distanceX*distanceX + distanceY*distanceY);

	    if(dist <= r )
	      {
		/* Transform point to compensate the rotation the orientation */
		pDst[0] = col;
		pDst[1] = raw;
		pSrc = eulerTransform->TransformPoint(pDst);

		/** New Coordinates (rotated) */
		col = static_cast<int>(vcl_floor(pSrc[0]));
		raw = static_cast<int>(vcl_floor(pSrc[1]));

		if(raw==0) raw =+1;
		if(col ==0) col +=1;

		xx = static_cast<int> (pSrc[1]/rayon);
		yy = static_cast<int> (pSrc[0]/rayon);
		Nbin =  static_cast<int> (xx + 4*yy) ;

		if( Nbin < 16)           //because 64 descriptor length
		  {
		    double distanceXcompensee_2 = (pSrc[0] - r)*(pSrc[0] - r);
		    double distanceYcompensee_2 = (pSrc[1] - r)*(pSrc[1] - r);

		    w = vcl_exp(-( distanceXcompensee_2 + distanceYcompensee_2 ) / (2*3.3*3.3*S*S) );

		    dx = 0.5 * (neigh[(col+pas) + raw * Largeur] - neigh[(col-pas) + raw *Largeur]) * w ;
		    dy = 0.5 * (neigh[col + (raw+ pas)* Largeur] - neigh[col + (raw-pas)*Largeur])  * w;

		    descriptorVector[4*Nbin  ] += dx ;
		    descriptorVector[4*Nbin+1] += dy ;
		    descriptorVector[4*Nbin+2] += vcl_abs(dx) ;
		    descriptorVector[4*Nbin+3] += vcl_abs(dy) ;
		  }
	      }
	  }
	i++;
      }

    double accu = 0;
    for (int i = 0 ; i < 64 ;  i++)
      accu += descriptorVector[i]*descriptorVector[i];

    for (int j = 0 ; j < 64 ;  j++)
      descriptorVector[j] /= vcl_sqrt(accu) ;

    return descriptorVector;

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
