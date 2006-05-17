#ifndef otbPrincipalImageViewAS_txx
#define otbPrincipalImageViewAS_txx
// OTB-FA-00026-CS

#include "otbPrincipalImageViewAS.h"
#include "itkPointSet.h"
#include "otbSVMPointSetModelEstimator.h"
#include "itkListSample.h"
#include "itkImageToListAdaptor.h"
#include "otbVectorImageToASImageAdaptor.h"
//#include "itkPointSetToListAdaptor.h"
#include "otbSVMClassifier.h"
//#include "itkVectorToRGBImageAdaptor.h"

namespace otb
{

template <class TPixel, class TPixelOverlay>
PrincipalImageViewAS<TPixel, TPixelOverlay>::
PrincipalImageViewAS() : GLVectorImageViewClick<TPixel, TPixelOverlay>()
  {
		
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

  int key, x, y ,z=0;
  bool mem;
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
							static_cast<int>(fabs(this->coord[2]-this->coord[0])),
							static_cast<int>(fabs(this->coord[1]-this->coord[3])));
				// Synchronize two displays in HMI
				this->GetViewer()->SynchronizeClickedPointsBox(xTmp,yTmp,z,cRectColor,
										static_cast<int>(fabs(this->coord[2]-this->coord[0])),
										static_cast<int>(fabs(this->coord[1]-this->coord[3])));	
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
  cEstimator = EstimatorType::New();
  
  // Declaration for input image values
  ImagePointer lInpuPtr = this->GetInput();
  PixelType  pixelInput;
  IndexType ind;
  int nbChannels(0);
  int channelIn(0);
  
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
  LabelPointSetType::Pointer lPSet = LabelPointSetType::New();

  typedef MeasurePointSetType::PointType                MeasurePointType;
  typedef LabelPointSetType::PointType                  LabelPointType;

  typedef MeasurePointSetType::PointsContainer          MeasurePointsContainer;
  typedef LabelPointSetType::PointsContainer            LabelPointsContainer;

  MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
  LabelPointsContainer::Pointer lCont = LabelPointsContainer::New();
	
  int pointId=0, indiceIm = 0;
  
  MeasurePointType mP;
  LabelPointType lP;
  InputVectorType measure;
  LabelPixelType label;
  
  for (it=this->cClickedPoints.begin(); it!=this->cClickedPoints.end(); it++)
  {
  	// ClickPoint is a point	
	if (!(it->isForBox))
	{
		mP[0]=it->x;	
		mP[1]=it->y;

		lP[0]=it->x;
		lP[1]=it->y;
	
		 
		// Recuperation of real values in input image, not display values 
		ind[0]=(int)((it->x)*zoomBase);
		ind[1]=(int)((it->y)*zoomBase);
		
		// Traitement sur les canaux selectionnes.
		pixelInput = lInpuPtr->GetPixel(ind);
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                measure.push_back(static_cast<InputPixelType>(pixelInput[channelIn]));
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
		indiceIm+=3;
	}
	// ClickPoint is a box
	else 
	{
		int startX = (int)it->x;
		int endX = (int)(it->x+it->w);
		int startY = (int)it->y;
		int endY = (int)(it->y+it->h);
		
		for (int i=startX; i<endX; i++)
		{
			for (int j=startY; j<endY; j++)
			{
						
				mP[0]=i;	
				mP[1]=j;

				lP[0]=i;
				lP[1]=j;
										  
				// Recuperation of real values in input image, not display values 
				ind[0]=(int)(i*zoomBase);
				ind[1]=(int)(j*zoomBase);
				
				// Traitement sur les canaux selectionnes.
				pixelInput = lInpuPtr->GetPixel(ind);
		        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        		{
                	channelIn = this->m_ChannelsWorks[nbChannels] - 1;
	                measure.push_back(static_cast<InputPixelType>(pixelInput[channelIn]));
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
				indiceIm+=3;
			}
		}
	 }
  }
  
  std::cout << "Nb de points : " << pointId << std::endl ;
  
  mPSet->SetPoints( mCont );
  lPSet->SetPoints( lCont );
  
  cEstimator->SetInputPointSet( mPSet );
  cEstimator->SetTrainingPointSet( lPSet );
  cEstimator->SetNumberOfClasses( 2 );

  cEstimator->Update();

  std::cout << "Saving model" << std::endl;
}


template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::ClassificationStep()
{
	// TODO :
	// Erreur de compil pour le passage de v2im au sample->SetImage()
	// Regarder l'objet créé par le nouvel adaptor VectorImageToASImageAdaptor	
	
	// Image conversion from reader to classification
	/*typedef itk::VectorImage<TPixel,2> ImageType;	
	typedef otb::VectorImageToASImageAdaptor<ImageType> InputTType;
	
	typename InputTType::Pointer v2im = InputTType::New();
	
	v2im->SetImage(this->GetInput());
	
	// Image definition for classification
	typedef itk::Image< itk::FixedArray<InputPixelType,3>, 2 >	InputClassifImageType;
	typedef itk::Statistics::ImageToListAdaptor< InputClassifImageType > SampleType;
	
    SampleType::Pointer sample = SampleType::New();
	
	sample->SetImage(v2im);

    std::cout << "Sample set to Adaptor" << std::endl;  


    // preparing classifier and decision rule object 
    typedef otb::SVMModel< SampleType::MeasurementVectorType::ValueType, LabelPixelType > ModelType;

    ModelType::Pointer model = cEstimator->GetModel();

    int numberOfClasses = model->GetNumberOfClasses();

    std::cout << "Classification for " << numberOfClasses << " classes " << std::endl;
    
    typedef otb::SVMClassifier< SampleType, LabelPixelType > ClassifierType ;

    ClassifierType::Pointer classifier = ClassifierType::New() ;
  
    classifier->SetNumberOfClasses(numberOfClasses) ;
    classifier->SetModel( model );
    classifier->SetSample(sample.GetPointer()) ;
    classifier->Update() ;

    // Build the class map 
    std::cout << "Output image creation" << std::endl;  

    
    std::cout << "classifier get output" << std::endl;  
    ClassifierType::OutputType* membershipSample =
      classifier->GetOutput() ;
    std::cout << "Sample iterators" << std::endl;  
    ClassifierType::OutputType::ConstIterator m_iter =
      membershipSample->Begin() ;
    ClassifierType::OutputType::ConstIterator m_last =
      membershipSample->End() ;


    double error = 0.0;
    unsigned int pointId = 0;
    while (m_iter != m_last)
      {
      ClassifierType::ClassLabelType label = m_iter.GetClassLabel();
      
      InputVectorType measure; 
      
      tPSet->GetPointData(pointId, &measure);

      ClassifierType::ClassLabelType expectedLabel;
      if(measure[0] < measure[1])
	expectedLabel= -1;
      else
	expectedLabel = 1;

      double dist = fabs(measure[0] - measure[1]);
      
      if(label != expectedLabel )
	error++;

      std::cout << int(label) << "/" << int(expectedLabel) << " --- " << dist << std::endl;
      
      
      ++pointId;
      ++m_iter ;
      }
    
    std::cout << "Error = " << error/pointId << std::endl;*/
    

	


}	


template <class TPixel, class TPixelOverlay>
void
PrincipalImageViewAS<TPixel, TPixelOverlay>::SetClickable(bool b)
{
     
	 cClickable = b ;
}


}; //namespace
#endif
