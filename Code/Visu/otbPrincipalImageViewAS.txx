#ifndef otbPrincipalImageViewAS_txx
#define otbPrincipalImageViewAS_txx
// OTB-FA-00026-CS

#include "otbPrincipalImageViewAS.h"


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
		  if (cMem)
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
		if (cMem)
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
		if (cMem)
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
     /*this->m_flDoubleWindow->size(          
     this->GetViewImageRegion().GetSize()[0], 
     this->GetViewImageRegion().GetSize()[1]);*/
     this->m_flDoubleWindow->label( this->GetLabel() );
     this->show();
     this->update();
     //this->PrintInfos();
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
 
}; //namespace
#endif

