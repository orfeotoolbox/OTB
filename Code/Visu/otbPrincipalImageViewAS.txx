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
#ifndef otbPrincipalImageViewAS_txx
#define otbPrincipalImageViewAS_txx

#include "otbPrincipalImageViewAS.h"
#include "itkPointSet.h"
#include "otbSVMPointSetModelEstimator.h"
#include "itkListSample.h"
//#include "otbVectorImageToASImageAdaptor.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkImageToListAdaptor.h"
#include "itkScalarToArrayCastImageFilter.h"
#include "itkVectorCastImageFilter.h"

//#include "itkPointSetToListAdaptor.h"
#include "otbSVMClassifier.h"
#include "itkVectorToRGBImageAdaptor.h"
#include "itkVectorCastImageFilter.h"
#include "itkCastImageFilter.h"

namespace otb
{

template <class TPixel, class TPixelOverlay>
PrincipalImageViewAS<TPixel, TPixelOverlay>::
PrincipalImageViewAS() : GLVectorImageViewClick<TPixel, TPixelOverlay>()
  {
  //cEstimator = EstimatorType::New();
		
  }

template <class TPixel, class TPixelOverlay>
PrincipalImageViewAS<TPixel, TPixelOverlay>::
~PrincipalImageViewAS()
{

}




template <class TPixel, class TPixelOverlay>
int
PrincipalImageViewAS<TPixel, TPixelOverlay>::handle(int event)
  {

  int key, x=0, y=0 ,z=0;
  ColorType colorTmp;

  std::list<ClickPoint>::iterator it;

  switch(event)
    {
    case FL_PUSH: 
	
		  nbDrag = 0;
		  x = Fl::event_x()-this->cOffSetX;	
      	  y = Fl::event_y()-this->cOffSetY;
				
		  // Click on the picture ?		  
		  if ((x<0)||(x>this->GetViewImageRegion().GetSize()[0]*this->cScale)
		    ||(y<0)||(y>this->GetViewImageRegion().GetSize()[1]*this->cScale))
		    	cMem = false ;
		  else 
				cMem = true;
		  
		  //std::cout << "Coord : " << x << " " << y << std::endl;
		  
		  // Clicked points display
		  this->ViewClickedPoints(true);
		  
		  // Test to know if mouse click is on image
		  if (cMem&&cClickable)
		  {
		  
		  	if (Fl::event_button()==FL_LEFT_MOUSE)
		  	{
		  		colorTmp = COLOR_BLUE;
				//std::cout << "Click gauche !!" << std::endl;
		  	}	
		  	else if (Fl::event_button()==FL_RIGHT_MOUSE)
		  	{
		  		colorTmp = COLOR_RED;
		  		//std::cout << "Click droit !!" << std::endl;			
		  	}
		  	this->clickSelect(x,y,z,colorTmp);
			// Synchronize two displays in HMI
		  	this->GetViewer()->SynchronizeClickedPoints(x,y,z,colorTmp);
					    		  
	        /*for (it=this->cClickedPoints.begin();it!=this->cClickedPoints.end();it++)
		  	std::cout << "Point : " << (*it).x << " " << (*it).y << std::endl ;
		  	this->cClickedPoints.pop_back();*/
		  	this->coord[0] = x;
		  	this->coord[1] = y;
		  
		  	cRectColor=colorTmp;
		  }
    case FL_DRAG:
      {
    	nbDrag++;
		if (cMem&&cClickable)
		{
			x = Fl::event_x()-this->cOffSetX;	
        	y = Fl::event_y()-this->cOffSetY;
	
		    // Drag on the picture ?
			if ((x>0)&&(x<this->GetViewImageRegion().GetSize()[0]*this->cScale)
			  &&(y>0)&&(y<this->GetViewImageRegion().GetSize()[1]*this->cScale))
			{
			this->cSelectRectangle = true ; // Display ON for box selection
			this->coord[2] = Fl::event_x()-this->cOffSetX;
			this->coord[3] = Fl::event_y()-this->cOffSetY;
			this->redraw();
			}
		}
      }
    case FL_FOCUS :
                this->update();
//THOMAS
//                this->GetViewer()->PrincipalResize();
                return 1;
                break;
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_RELEASE:
    	this->cSelectRectangle = false ; // Display OFF for box selection
		if (cMem&&cClickable)
		{
			// Release on the picture ?
			if ((x>0)&&(x<this->GetViewImageRegion().GetSize()[0]*this->cScale)
			  &&(y>0)&&(y<this->GetViewImageRegion().GetSize()[1]*this->cScale))
			{
    			this->coord[2] = Fl::event_x()-this->cOffSetX;
	    		this->coord[3] = Fl::event_y()-this->cOffSetY;
			}
			this->ViewClickedPoints(true);
			
			// If box selection
			if (nbDrag>1)
			{
				int xTmp = this->coord[0];
				int yTmp = this->coord[1];
				
				
				if (this->coord[0]>this->coord[2])
				{
					xTmp = this->coord[2];
				}
				if (this->coord[1]>this->coord[3])
				{
					yTmp = this->coord[3];
				}
			
				this->clickSelectBox(xTmp,yTmp,z,cRectColor,
							static_cast<int>(fabs(static_cast<float>(this->coord[2]-this->coord[0]))),
							static_cast<int>(fabs(static_cast<float>(this->coord[1]-this->coord[3]))));
				// Synchronize two displays in HMI
				this->GetViewer()->SynchronizeClickedPointsBox(xTmp,yTmp,z,cRectColor,
										static_cast<int>(fabs(static_cast<float>(this->coord[2]-this->coord[0]))),
										static_cast<int>(fabs(static_cast<float>(this->coord[1]-this->coord[3]))));	
			}							
		}
		return 0;
      	break;

    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
              this->update();
              return 1;
              break;
            default:
              break;
      }
    default:
      break;
  }
  
  return 0;
  
}


template <class TPixel, class TPixelOverlay>
void PrincipalImageViewAS<TPixel, TPixelOverlay>::Show(void)
{
     this->BuildWithImageRegion();
     this->m_flDoubleWindow->show();
     this->m_flDoubleWindow->label( this->GetLabel() );
     this->show();
     this->update();
}

template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::UndoPt()
{
     if (this->cClickedPoints.size()!=0)
     	this->cClickedPoints.pop_front();
     this->update();
}

template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::ResetPts()
{
     this->cClickedPoints.clear();
     this->update();
}


template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::LearnStep()
{
  
  // Declaration for input image values
  ImagePointer lInpuPtr = this->GetInput();
  IndexType     ind;
  
  // Scale calcul for correspondance between real image and displayed image
  double zoomBase = (double)this->cW / (this->cDimSize[0]);
  if(zoomBase > (double)this->cH / (this->cDimSize[1]))
  {
  		zoomBase = (double)this->cH / (this->cDimSize[1]);
  }
  zoomBase = 1/zoomBase ;
  
  
  std::list<ClickPoint>::iterator it;
 
  // Conversion from ClickedPoint structure to PointSet structure
 
  MeasurePointSetType::Pointer mPSet = MeasurePointSetType::New();
  LabelPointSetType::Pointer   lPSet = LabelPointSetType::New();

  typedef MeasurePointSetType::PointType                MeasurePointType;
  typedef LabelPointSetType::PointType                  LabelPointType;

  typedef MeasurePointSetType::PointsContainer          MeasurePointsContainer;
  typedef LabelPointSetType::PointsContainer            LabelPointsContainer;

  MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
  LabelPointsContainer::Pointer   lCont = LabelPointsContainer::New();
	
  int pointId=0, indiceIm = 0;
  
  MeasurePointType   mP;
  LabelPointType     lP;
  LabelPixelType     label;
  PixelType          pixelInput;
  int                noChannels(0);
  int                nbChannels(0);
  int                channelIn(0);

  InputVectorType    measure(this->m_ChannelsWorks.size() );


  it=this->cClickedPoints.begin();
  while(it!=this->cClickedPoints.end() )
  {
	
	// ClickPoint is a point	
	if (!(it->isForBox))
	{
		// index position for the measured point 
		mP[0]=it->x;	
		mP[1]=it->y;

		mP[0]=(int)((it->x)*zoomBase);
		mP[1]=(int)((it->y)*zoomBase);

		// index position for the label point 
		lP[0]=it->x;
		lP[1]=it->y;

		lP[0]=(int)((it->x)*zoomBase);
		lP[1]=(int)((it->y)*zoomBase);
	
		// Recuperation of real values in input image, not display values 
		ind[0]=(int)((it->x)*zoomBase);
		ind[1]=(int)((it->y)*zoomBase);
	
		/*
		std::cout <<" points : "<<  ind[0] << " , " << ind[1] << std::endl;
		*/
		
		// Traitement sur les canaux selectionnes.
		pixelInput = lInpuPtr->GetPixel(ind);
		

                for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
                {
                    channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                    measure[nbChannels] = static_cast<InputPixelType>(pixelInput[channelIn]);
                }
		if (it->color == COLOR_BLUE)
     		         label= -1;
    	        else if (it->color == COLOR_RED)
			 label = 1;
		else
			 itkExceptionMacro(<< "Unknown color");
			 
		mCont->InsertElement( pointId , mP );
    	        mPSet->SetPointData( pointId, measure ); 
		
		lCont->InsertElement( pointId , lP );
    	        lPSet->SetPointData( pointId, label ); 
		
		pointId++;
	}
	// ClickPoint is a box
	else 
	{
		int startX = (int)((it->x)*zoomBase); 
		int endX   = (int)((it->x+it->w)*zoomBase);
		int startY = (int)((it->y)*zoomBase);
		int endY   = (int)((it->y+it->h)*zoomBase);
		int temp;
		
		if(startX > endX )
		{
		temp   = endX;
		endX   = startX;
		startX = temp;
		}
		if(startY > endY )
		{
		temp   = endY;
		endY   = startY;
		startY = temp;
		}
		
		for (int i=startX; i<=endX; i++)
		{
		  for (int j=startY; j<=endY; j++)
	          {		
		        // index position for the measured point 
		        mP[0]=i;	
		        mP[1]=j;

		        // index position for the label point 
		        lP[0]=i;
		        lP[1]=j;
										  
			// Recuperation of real values in input image, not display values 
			ind[0]=i;
			ind[1]=j;
				
		        /*
			std::cout <<" box : "<<  ind[0] << " , " << ind[1] << std::endl;
			*/
			
			// Traitement sur les canaux selectionnes.
			pixelInput = lInpuPtr->GetPixel(ind);
		        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        		{
                	  channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                          measure[nbChannels] = static_cast<InputPixelType>(pixelInput[channelIn]);
    		        }
		
			if (it->color == COLOR_BLUE)
     		   		label = -1;
		    	else if (it->color == COLOR_RED)
				label = 1;
			else
				itkExceptionMacro(<< "Unknown color");
			 
			mCont->InsertElement( pointId , mP );
    			mPSet->SetPointData( pointId, measure ); 
		
			lCont->InsertElement( pointId , lP );
    			lPSet->SetPointData( pointId, label ); 
		
			pointId++;
		  }
		}
	 }
	 ++it;
  }
  
  mPSet->SetPoints( mCont );
  lPSet->SetPoints( lCont );  
  
  EstimatorType::Pointer learningEstimator = EstimatorType::New();

  learningEstimator->SetInputPointSet( mPSet );
  learningEstimator->SetTrainingPointSet( lPSet );
  learningEstimator->SetNumberOfClasses( 2 );
  learningEstimator->Modified();

  learningEstimator->Update();
  
  cEstimator = learningEstimator;
  std::cout << "End learning model" << std::endl;
}


template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::ClassificationStep()
{

     // Image conversion from reader to classification
    typedef otb::VectorImage<TPixel,2>                                   ReaderImageType;	
    typedef itk::FixedArray< float, 3 >                                  MeasurementVectorType;
    typedef otb::Image< MeasurementVectorType, 2 >                       InputClassifImageType;

    typename ReaderImageType::Pointer inputImage;
    inputImage = this->GetInput();

    typedef itk::VectorCastImageFilter<ReaderImageType,InputClassifImageType> CasterType;
    typename CasterType::Pointer caster = CasterType::New();
    caster->SetInput(inputImage);
    caster->Update();

    // Image definition for classification
    typedef itk::Statistics::ImageToListAdaptor< InputClassifImageType > SampleType;
    SampleType::Pointer sample = SampleType::New();
    sample->SetImage( caster->GetOutput() );
 
    std::cout << "Sample set to Adaptor" << std::endl;  

    // preparing classifier and decision rule object 
    typedef otb::SVMModel< SampleType::MeasurementVectorType::ValueType, LabelPixelType > ModelType;

    ModelType::Pointer model = cEstimator->GetModel();    
    int numberOfClasses = model->GetNumberOfClasses();
    
    typedef otb::SVMClassifier< SampleType, LabelPixelType > ClassifierType ;
    ClassifierType::Pointer classifier = ClassifierType::New() ;
  
    classifier->SetNumberOfClasses(numberOfClasses) ;
    classifier->SetModel( model );
    classifier->SetSample(sample.GetPointer()) ;
    classifier->Update() ;

    // Build the class map 
    std::cout << "Generating classify map" << std::endl;  

    
    ClassifierType::OutputType* membershipSample =
      classifier->GetOutput() ;
     ClassifierType::OutputType::ConstIterator m_iter =
      membershipSample->Begin() ;
    ClassifierType::OutputType::ConstIterator m_last =
      membershipSample->End() ;

    const   unsigned int        	         Dimension = 2;
    typedef ClassifierType::ClassLabelType	            OutputPixelType;
    typedef otb::Image< TPixelOverlay, Dimension >        OutputImageType;

    typename OutputImageType::Pointer outputImage = OutputImageType::New();
    typename OutputImageType::Pointer ImageClassBlue = OutputImageType::New();
    typename OutputImageType::Pointer ImageClassRed  = OutputImageType::New();

    typedef itk::Index<Dimension>         myIndexType;
    typedef itk::Size<Dimension>          mySizeType;
    typedef itk::ImageRegion<Dimension>        myRegionType;

    mySizeType size;
    size[0] = inputImage->GetRequestedRegion().GetSize()[0];
    size[1] = inputImage->GetRequestedRegion().GetSize()[1];

    myIndexType start;
    start[0] = 0;
    start[1] = 0;

    myRegionType region;
    region.SetIndex( start );
    region.SetSize( size );

    outputImage->SetRegions( region );
    outputImage->Allocate();

    ImageClassBlue->SetRegions( region );
    ImageClassBlue->Allocate();

    ImageClassRed->SetRegions( region );
    ImageClassRed->Allocate();



    std::cout << "Image iterator" << std::endl;  
    typedef itk::ImageRegionIterator< OutputImageType>  OutputIteratorType;
    OutputIteratorType  outIt(       outputImage,   outputImage->GetBufferedRegion() );
    OutputIteratorType  classRedIt(  ImageClassRed, outputImage->GetBufferedRegion() );
    OutputIteratorType  classBlueIt( ImageClassBlue,outputImage->GetBufferedRegion() );

    outIt.GoToBegin();
    classRedIt.GoToBegin();
    classBlueIt.GoToBegin();

    TPixelOverlay MaxValue = static_cast<TPixelOverlay>(255);
    TPixelOverlay MinValue = static_cast<TPixelOverlay>(0);

    
    while (m_iter != m_last && !outIt.IsAtEnd())
    {
      ClassifierType::ClassLabelType label = m_iter.GetClassLabel();
      if( label == -1 ) 
      {
      classBlueIt.Set(MaxValue);
      classRedIt.Set(MinValue);
      }	
      if( label == 1 ) 
      {
      classBlueIt.Set(MinValue);
      classRedIt.Set(MaxValue);
      }	
      
      outIt.Set(m_iter.GetClassLabel());
      
      ++m_iter ;
      ++outIt;
      ++classRedIt;
      ++classBlueIt;
    }

//  cOverlayData ....
  this->SetInputOverlay(outputImage,ImageClassRed, ImageClassBlue);
//ViewOverlayData(true);
	
std::cout << "End Classif" << std::endl;

}	


template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::SetClickable(bool b)
{
     
	 cClickable = b ;
}


}; //namespace
#endif
