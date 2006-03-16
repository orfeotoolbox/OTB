#ifndef _otbVectorImageView_txx
#define _otbVectorImageView_txx

#include <otbVectorImageView.h>

namespace otb
{
  

template <class TPixel>
VectorImageView<TPixel>::VectorImageView(/*int x, int y, int w, int h, const char **/ )
{

  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(0);
    
}

template <class TPixel>
void
VectorImageView<TPixel>::Init(int x, int y, int w, int h, const char * )
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(0);
  cX = x;
  cY = y;
  cW = w;
  cH = h;
  
  cValidImData = false;
  cViewImData = false;
  cViewImDataCallBack = NULL;
  cViewImDataArg = NULL;
  cViewImDataArgCallBack = NULL;
  
  cClickMode = CM_SELECT;
  cClickSelect[0] = 0;
  cClickSelect[1] = 0;
  cClickSelect[2] = 0;
  cClickSelectV = 0;
  cClickSelectCallBack = NULL;
  cClickSelectArg = NULL;
  cClickSelectArgCallBack = NULL;
  cClickBoxCallBack = NULL;
  cClickBoxArg = NULL;
  cClickBoxArgCallBack = NULL;
  cViewClickedPoints = false;
  
  cIWMin = 0;
  cIWMax = 0;
  cIWModeMin = IW_MIN;
  cIWModeMax = IW_MAX;    
  cImageMode = IMG_VAL;
  cIWCallBack = NULL;
  cIWArg = NULL;
  cIWArgCallBack = NULL;
  
  cFlipX[0] = false;
  cFlipX[1] = false;
  cFlipX[2] = false;
  cFlipY[0] = false;
  cFlipY[1] = false;
  cFlipY[2] = false;
  cFlipZ[0] = false;
  cFlipZ[1] = false;
  cFlipZ[2] = false;
  cTranspose[0] = false;
  cTranspose[1] = false;
  cTranspose[2] = false;
  
  cWinZoom = 1;
  cWinOrder[0] = 0;
  cWinOrder[1] = 1;
  cWinOrder[2] = 2;
  cWinOrientation = 2;
  cWinOrientationCallBack = NULL;
  cWinOrientationArg = NULL;
  cWinOrientationArgCallBack = NULL;
  
  cWinCenter[0] = 0;
  cWinCenter[1] = 0;
  cWinCenter[2] = 0;
  cWinCenterCallBack = NULL;
  cWinCenterArg = NULL;
  cWinCenterArgCallBack = NULL;
  
  cSliceNumCallBack = NULL;
  cSliceNumArg = NULL;
  cSliceNumArgCallBack = NULL;
  
  cViewAxisLabel = false;
  sprintf(cAxisLabelX[0], "S");
  sprintf(cAxisLabelX[1], "L");
  sprintf(cAxisLabelX[2], "L");
  sprintf(cAxisLabelY[0], "P");
  sprintf(cAxisLabelY[1], "S");
  sprintf(cAxisLabelY[2], "P");
  
  cViewOverlayData = true;
  cViewDetails = true;
  cViewCrosshairs = true;
  cViewValue = true;
  
  cViewValuePhysicalUnits = false;
  cPhysicalUnitsName = "mm";

  cWinMinX = 0;
  cWinMaxX = 0;
  cWinSizeX = 0;
  cWinMinY = 0;
  cWinMaxY = 0;
  cWinSizeY = 0;
  cWinDataSizeX = 0;
  cWinDataSizeY = 0;
  cWinImData = NULL;
  cWinZBuffer = NULL;
  
  maxClickPoints = 0;
  fastMovVal = 1; //fast moving pace: 1 by defaut
  fastMovThresh = 10; //how many single step moves before fast moving


}

//
//
//
template <class TPixel>
VectorImageView<TPixel>::~VectorImageView()
  {
  if(cWinImData != NULL)
    delete [] cWinImData;
  cWinImData = NULL;
  if(cWinZBuffer != NULL)
    delete [] cWinZBuffer;
  cWinZBuffer = NULL;
  }



template <class TPixel>
void
VectorImageView<TPixel>
::SetInput( const ImageType * image )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast< ImageType *>( image ) );
}


//
//
template <class TPixel>
const typename VectorImageView<TPixel>::ImageType *
VectorImageView<TPixel>
::GetInput(void) const
  {
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return ( (const_cast< ImageType * >
     (this->itk::ProcessObject::GetInput(0) ) ) );

  }




template <class TPixel>
typename VectorImageView<TPixel>::ImageType * 
VectorImageView<TPixel>
::GetInput(void)
  {
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return ( static_cast< ImageType * >
     (this->itk::ProcessObject::GetInput(0) ) ) ;
  }


//
//
template <class TPixel>
unsigned char* VectorImageView<TPixel>::winImData(void)
  {
  return cWinImData;
  }


//
//

template <class TPixel>
unsigned int VectorImageView<TPixel>::winDataSizeX(void)
  {
  
  return cWinDataSizeX;
  
  }


//
//
template <class TPixel>
unsigned int VectorImageView<TPixel>::winDataSizeY(void)

  {
  
  return cWinDataSizeY;
  
  }



//
//
template <class TPixel>
void VectorImageView<TPixel>::viewImData(bool newViewImData)
  {
  cViewImData = newViewImData;
  if(cViewImDataCallBack != NULL)
    cViewImDataCallBack();
  if(cViewImDataArgCallBack != NULL)
    cViewImDataArgCallBack(cViewImDataArg);
  }




template <class TPixel>
bool VectorImageView<TPixel>::viewImData(void)
  {
  return cViewImData;
  }




template <class TPixel>
void VectorImageView<TPixel>::
viewImDataCallBack(void (* newViewImDataCallBack)(void))
  {
  cViewImDataCallBack = newViewImDataCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
viewImDataCallBack(void (* newViewImDataArgCallBack)(void *), void * newViewImDataArg)
  {
  cViewImDataArg = newViewImDataArg;
  cViewImDataArgCallBack = newViewImDataArgCallBack;
  }

template <class TPixel>
double VectorImageView<TPixel>::
dataMin(void) const
  {
  return this->cDataMin;
  }

template <class TPixel>
double VectorImageView<TPixel>::
dataMax(void) const
  {
  return this->cDataMax;
  }

//
//
template <class TPixel>
void VectorImageView<TPixel>::flipX(bool newFlipX)
  {
  cFlipX[cWinOrientation] = newFlipX;
  }




template <class TPixel>
bool VectorImageView<TPixel>::flipX()
  {
  return cFlipX[cWinOrientation];
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::flipY(bool newFlipY)
  {
  cFlipY[cWinOrientation] = newFlipY;
  }




template <class TPixel>
bool VectorImageView<TPixel>::flipY()
  {
  return cFlipY[cWinOrientation];
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::flipZ(bool newFlipZ)
  {
  cFlipZ[cWinOrientation] = newFlipZ;
  }




template <class TPixel>
bool VectorImageView<TPixel>::flipZ()
  {
  return cFlipZ[this->cWinOrientationder];
  }


//
//
template <class TPixel>
void VectorImageView<TPixel>::Transpose(bool newTranspose)
  {
  if(cTranspose[cWinOrientation] != newTranspose)
    {
    int t;
    t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }
  
  cTranspose[cWinOrientation] = newTranspose;
  }




template <class TPixel>
bool VectorImageView<TPixel>::Transpose()
  {
  return cTranspose[cWinOrientation];
  }




//
//
template <class TPixel>
unsigned int VectorImageView<TPixel>::numSlices(void)
  {
  return cDimSize[cWinOrder[2]];
  }



//
//
template <class TPixel>
void VectorImageView<TPixel>::sliceNum(unsigned int newSliceNum)
  {
  if(newSliceNum>=cDimSize[cWinOrder[2]])
    newSliceNum = cDimSize[cWinOrder[2]]-1;
  cWinCenter[cWinOrder[2]] = newSliceNum;
  //if(newSliceNum < 0)
  //  newSliceNum = 0;
  
  if(cSliceNumCallBack != NULL)
    cSliceNumCallBack();
  if(cSliceNumArgCallBack != NULL)
    cSliceNumArgCallBack(cSliceNumArg);
  }




template <class TPixel>
unsigned int VectorImageView<TPixel>::sliceNum(void)
  {
  return cWinCenter[cWinOrder[2]];
  }




template <class TPixel>
void VectorImageView<TPixel>::
sliceNumCallBack(void (* newSliceNumCallBack)(void))
  {
  cSliceNumCallBack = newSliceNumCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
sliceNumCallBack(void (* newSliceNumArgCallBack)(void *), 
                 void * newSliceNumArg)
  {
  cSliceNumArg = newSliceNumArg;
  cSliceNumArgCallBack = newSliceNumArgCallBack;
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::clickMode(ClickModeType newClickMode)
  {
  cClickMode = newClickMode;
  }




template <class TPixel>
ClickModeType VectorImageView<TPixel>::clickMode(void)
  {
  return cClickMode;
  }




template <class TPixel>
void VectorImageView<TPixel>::
clickSelectCallBack(void 
                    (*newClickSelectCallBack)(float _x, float _y, 
                                              float _z, float _v))
  {
  cClickSelectCallBack = newClickSelectCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
clickSelectCallBack(void 
                    (*newClickSelectArgCallBack)(float _x, float _y, 
                                                 float _z, float _v, 
                                                 void *clickSelectArg), 
                    void * newClickSelectArg)
  {
  cClickSelectArg = newClickSelectArg;
  cClickSelectArgCallBack = newClickSelectArgCallBack;
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::clickSelect(float newX, float newY, float newZ)
  {    
  cClickSelect[0] = newX;
  if(cClickSelect[0]<0)
    cClickSelect[0] = 0;
  if(cClickSelect[0] >= cDimSize[0])
    cClickSelect[0] = cDimSize[0]-1;
  
  cClickSelect[1] = newY;
  if(cClickSelect[1]<0)
    cClickSelect[1] = 0;
  if(cClickSelect[1] >= cDimSize[1])
    cClickSelect[1] = cDimSize[1]-1;
  
  cClickSelect[2] = newZ;
  if(cClickSelect[2]<0)
    cClickSelect[2] = 0;
  if(cClickSelect[2] >= cDimSize[2])
    cClickSelect[2] = cDimSize[2]-1;
  
  typename ImageType::IndexType ind;
  
  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
  ind[2] = (unsigned long)cClickSelect[2];
//  cClickSelectV = cImData->GetPixel(ind);
//  cClickSelectV = this->GetInput()->GetPixel(ind);
  cClickSelectV = this->GetInput()->GetPixel(ind)[0];
  
  /*if length of list is equal to max, remove the earliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    cClickedPoints.pop_back();

  ClickPoint clickedPoint(cClickSelect[0], cClickSelect[1], cClickSelect[2], cClickSelectV);
  cClickedPoints.push_front( clickedPoint ); 
  
  if(cClickSelectCallBack != NULL)
    cClickSelectCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV);
  if(cClickSelectArgCallBack != NULL)
    cClickSelectArgCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV,
    cClickSelectArg);
  }




template <class TPixel>
void VectorImageView<TPixel>::maxClickedPointsStored(unsigned int i)
  {
  maxClickPoints = i;
  }




template <class TPixel>
unsigned int VectorImageView<TPixel>::maxClickedPointsStored()
  {
  return maxClickPoints;
  }




template <class TPixel>
float VectorImageView<TPixel>::clickSelectX(void)
  {
  return cClickSelect[0];
  }




template <class TPixel>
float VectorImageView<TPixel>::clickSelectY(void)
  {
  return cClickSelect[1];
  }




template <class TPixel>
float VectorImageView<TPixel>::clickSelectZ(void)
  {
  return cClickSelect[2];
  }




template <class TPixel>
bool VectorImageView<TPixel>::getClickedPoint(unsigned int index, ClickPoint & point)
  {
  if(index >= cClickedPoints.size())
    {
    return false;
    }
  std::list<ClickPoint>::const_iterator j = cClickedPoints.begin();
  
  for(int i=0;i<static_cast<int>(index);i++,j++);
    point = *j;
  return true;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::
clickBoxCallBack(void (*newClickBoxCallBack)(float minX, float minY, 
                                             float minZ, float maxX, 
                                             float maxY, float maxZ))
  {
  cClickBoxCallBack = newClickBoxCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
clickBoxCallBack(void (*newClickBoxArgCallBack)(float minX, float minY, 
                                                float minZ, float maxX, 
                                                float maxY, float maxZ, 
                                                void * clickBoxArg), 
                 void * newClickBoxArg)
  {
  cClickBoxArg = newClickBoxArg;
  cClickBoxArgCallBack = newClickBoxArgCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::boxMin(float minX, float minY, float minZ)
  {
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
  cBoxMin[2] = minZ;
  }




template <class TPixel>
void VectorImageView<TPixel>::boxMax(float x, float y, float z)
  {
  float x0, y0, z0;
  float x1, y1, z1;
  
  x0 = (cBoxMin[0]<x) ? cBoxMin[0] : x;
  y0 = (cBoxMin[1]<y) ? cBoxMin[1] : y;
  z0 = (cBoxMin[2]<z) ? cBoxMin[2] : z;
  
  x1 = (cBoxMin[0]<x) ? x : cBoxMin[0];
  y1 = (cBoxMin[1]<y) ? y : cBoxMin[1];
  z1 = (cBoxMin[2]<z) ? z : cBoxMin[2];
  
  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
  cBoxMin[2] = z0;
  
  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
  cBoxMax[2] = z1;
  
  if(cClickBoxCallBack != NULL)
    cClickBoxCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2]);
  if(cClickBoxArgCallBack != NULL)
    cClickBoxArgCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2],
    cClickBoxArg);
  }


//
//
//
template <class TPixel>
void VectorImageView<TPixel>::iwMin(float newIWMin)
  {
  cIWMin = newIWMin;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
  }




template <class TPixel>
float VectorImageView<TPixel>::iwMin(void)
  {
  return cIWMin;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::iwMax(float newIWMax)
  {
  cIWMax = newIWMax;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
  }




template <class TPixel>
float VectorImageView<TPixel>::iwMax(void)
  {
  return cIWMax;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::iwModeMin(IWModeType newIWModeMin)
  {
  cIWModeMin = newIWModeMin;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
  }




template <class TPixel>
IWModeType VectorImageView<TPixel>::iwModeMin(void)
  {
  return cIWModeMin;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::iwModeMax(IWModeType newIWModeMax)
  {
  cIWModeMax = newIWModeMax;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
  }




template <class TPixel>
IWModeType VectorImageView<TPixel>::iwModeMax(void)
  {
  return cIWModeMax;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::imageMode(ImageModeType newImageMode)
  {
  cImageMode = newImageMode;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
  }




template <class TPixel>
ImageModeType VectorImageView<TPixel>::imageMode(void)
  {
  return cImageMode;
  
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::iwCallBack(void (* newIWCallBack)(void))
  {
  cIWCallBack = newIWCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::iwCallBack(void (* newIWArgCallBack)(void *), 
                                   void * newIWArg)
  {
  cIWArg = newIWArg;
  cIWArgCallBack = newIWArgCallBack;
  }




//
//
//
template <class TPixel>
void VectorImageView<TPixel>::winZoom(float newWinZoom)
  {
  if(newWinZoom>0.1 && newWinZoom<cDimSize[cWinOrder[0]])
    {
    cWinZoom = newWinZoom;
    }
  }




template <class TPixel>
float VectorImageView<TPixel>::winZoom(void)
  {
  return cWinZoom;
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::winCenter(void)
  {
  cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
  cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;
  
  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
  }





//
//
template <class TPixel>
void VectorImageView<TPixel>::winCenter(int newWinCenterX, 
                                  int newWinCenterY,
                                  int newWinCenterZ)
  {
  if(newWinCenterX < 0)
    newWinCenterX = 0;
  if(newWinCenterX >= (int)cDimSize[0])
    newWinCenterX = cDimSize[0] - 1;
  cWinCenter[0] = newWinCenterX;
  
  if(newWinCenterY < 0)
    newWinCenterY = 0;
  if(newWinCenterY >= (int)cDimSize[1])
    newWinCenterY = cDimSize[1] - 1;
  cWinCenter[1] = newWinCenterY;
  
  if(newWinCenterZ < 0)
    newWinCenterZ = 0;
  if(newWinCenterZ >= (int)cDimSize[2])
    newWinCenterZ = cDimSize[2] - 1;
  cWinCenter[2] = newWinCenterZ;
  
  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
  }




template <class TPixel>
unsigned int VectorImageView<TPixel>::winCenterX(void)
  {
  return cWinCenter[0];
  }




template <class TPixel>
unsigned int VectorImageView<TPixel>::winCenterY(void)
  {
  return cWinCenter[1];
  }


template <class TPixel>
unsigned int VectorImageView<TPixel>::winCenterZ(void)
  {
  return cWinCenter[2];
  }




template <class TPixel>
void VectorImageView<TPixel>::
winCenterCallBack(void (* newWinCenterCallBack)(void))
  {
  cWinCenterCallBack = newWinCenterCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
winCenterCallBack(void (* newWinCenterArgCallBack)(void *), 
                  void * newWinCenterArg)
  {
  cWinCenterArg = newWinCenterArg;
  cWinCenterArgCallBack = newWinCenterArgCallBack;
  }


//
//
template <class TPixel>
void VectorImageView<TPixel>::winShift(int newWinShiftUp, 
                                 int newWinShiftRight)
  {
  int winC[3];
  winC[cWinOrder[0]] = cWinCenter[cWinOrder[0]]+newWinShiftRight;
  winC[cWinOrder[1]] = cWinCenter[cWinOrder[1]]+newWinShiftUp;
  winC[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
  winCenter(winC[0], winC[1], winC[2]);
  }

//
//
//
template <class TPixel>
unsigned int VectorImageView<TPixel>::orientation(void)
  {
  return cWinOrientation;
  }




template <class TPixel>
void VectorImageView<TPixel>::orientation(unsigned int newOrientation)
  {
  cWinOrientation = newOrientation;
  switch(cWinOrientation) {
    case 0 :
      cWinOrder[0] = 2;
      cWinOrder[1] = 1;
      cWinOrder[2] = 0;
      break;
    case 1 :
      cWinOrder[0] = 0;
      cWinOrder[1] = 2;
      cWinOrder[2] = 1;
      break;
    default:
    case 2 :
      cWinOrientation = 2;
      cWinOrder[0] = 0;
      cWinOrder[1] = 1;
      cWinOrder[2] = 2;
      break;
    }
  
  if(cTranspose[cWinOrientation])
    {
    int t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }
  
  sliceNum((int)cClickSelect[cWinOrder[2]]);
  
  if(cWinOrientationCallBack != NULL)
    cWinOrientationCallBack();
  if(cWinOrientationArgCallBack != NULL)
    cWinOrientationArgCallBack(cWinOrientationArg);
  }




template <class TPixel>
void VectorImageView<TPixel>::
orientationCallBack(void (* newOrientationCallBack)(void))
  {
  cWinOrientationCallBack = newOrientationCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
orientationCallBack(void (* newOrientationArgCallBack)(void *), 
                    void * newOrientationArg)
  {
  cWinOrientationArg = newOrientationArg;
  cWinOrientationArgCallBack = newOrientationArgCallBack;
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::size(int w, int h)
  {
  cW = w;
  cH = h;
  }




template <class TPixel>
void VectorImageView<TPixel>::resize(int x, int y, int w, int h)
  {
  cX = x;
  cY = y;
  cW = w;
  cH = h;
  }




template <class TPixel>
int VectorImageView<TPixel>::handle(int event)
  {
  int x = Fl::event_x();
  int y = Fl::event_y();
  int button;
  int key;
  static int fastMov = 0;
  int pace;

  int imgShiftSize = (int)(cWinSizeX/10/cWinZoom);
  if(imgShiftSize<1)
    {
    imgShiftSize = 1;
    }

  double zoomBase = cW/(cDimSize[cWinOrder[0]]*(fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0])));
  if(zoomBase >
      cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]))))
    {
    zoomBase = cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0])));
    }

  double scale0 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);
  
  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
      button = Fl::event_button()-1;
      
      if(button <= 0)
        {
        if(cClickMode == CM_SELECT || cClickMode == CM_BOX) 
          {
          double originX = 0;
          double originY = 0;
          if(cWinZoom<=1)
            {
            if(cW-scale0*cDimSize[cWinOrder[0]]>0)
              {
              originX = (int)((cW-scale0*cDimSize[cWinOrder[0]])/2.0);
              }
            if(cH-scale1*cDimSize[cWinOrder[1]]>0)
              {
              originY = (int)((cH-scale1*cDimSize[cWinOrder[1]])/2.0);
              }
            }
          float p[3];
          p[cWinOrder[0]] = cWinMinX + ( (1-cFlipX[cWinOrientation])*(x-originX) 
            + (cFlipX[cWinOrientation])*(cW-x-originX) ) 
            / scale0;
          if(p[cWinOrder[0]]<cWinMinX) 
            p[cWinOrder[0]] = cWinMinX;
          if(p[cWinOrder[0]]>cWinMaxX) 
            p[cWinOrder[0]] = cWinMaxX;
          p[cWinOrder[1]] = cWinMinY + (cFlipY[cWinOrientation]*(y-originY) 
            + (1-cFlipY[cWinOrientation])*(cH-y-originY)) 
            / scale1;
          if(p[cWinOrder[1]]<cWinMinY) 
            p[cWinOrder[1]] = cWinMinY;
          if(p[cWinOrder[1]]>cWinMaxY) 
            p[cWinOrder[1]] = cWinMaxY;
          if(cImageMode != IMG_MIP)
            p[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          else
            p[cWinOrder[2]] = cWinZBuffer[(int)p[cWinOrder[0]]
            - cWinMinX 
            + ((int)p[cWinOrder[1]]
            - cWinMinY)
            * cWinDataSizeX];
          if(cClickMode == CM_SELECT)
            this->clickSelect(p[0], p[1], p[2]);
          else if(cClickMode == CM_BOX)
            if(event == FL_PUSH)
              boxMin(p[0], p[1], p[2]);
            return 1;
          }
        }
      return 0;
      break;
    case FL_RELEASE:
      if(cClickMode == CM_BOX)
        {
        double originX = 0;
        double originY = 0;
        if(cWinZoom<=1)
          {
          if(cW-scale0*cDimSize[cWinOrder[0]]>0)
            {
            originX = (int)((cW-scale0*cDimSize[cWinOrder[0]])/2.0);
            }
          if(cH-scale1*cDimSize[cWinOrder[1]]>0)
            {
            originY = (int)((cH-scale1*cDimSize[cWinOrder[1]])/2.0);
            }
          }
        float p[3];
        p[cWinOrder[0]] = cWinMinX + (cFlipX[cWinOrientation]*(cW-x-originX) 
          + (1-cFlipX[cWinOrientation])*(x-originX)) 
          / scale0;
        if(p[cWinOrder[0]]<cWinMinX) 
          p[cWinOrder[0]] = cWinMinX;
        if(p[cWinOrder[0]]>cWinMaxX) 
          p[cWinOrder[0]] = cWinMaxX;
        
        p[cWinOrder[1]] = cWinMinY + (cFlipY[cWinOrientation]*(y-originY) 
          + (1-cFlipY[cWinOrientation])*(cH-y-originY)) 
          / scale1;
        if(p[cWinOrder[1]]<cWinMinY) 
          p[cWinOrder[1]] = cWinMinY;
        if(p[cWinOrder[1]]>cWinMaxY) 
          p[cWinOrder[1]] = cWinMaxY;
        
        if(cImageMode != IMG_MIP)
          p[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
        else
          p[cWinOrder[2]] = cWinZBuffer[(int)p[cWinOrder[0]]
          - cWinMinX 
          + ((int)p[cWinOrder[1]]
          - cWinMinY)
          * cWinDataSizeX];
        boxMax(p[0], p[1], p[2]);
        return 1;
        }
      return 0;
      break;
    case FL_KEYUP:
    //when pressing down ">" or "<" key, scrolling will go faster
    //when the key is released, scrolling speed go back to 1
    fastMov = 0;
    return 1;
    break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case '0':
          orientation(0);
          cTranspose[cWinOrientation] = true;
          this->update();
          return 1;
          break;
        case '1':
          orientation(1);
          cTranspose[cWinOrientation] = false;
          this->update();
          return 1;
          break;
        case '2':
          orientation(2);
          cTranspose[cWinOrientation] = false;
          this->update();
          return 1;
          break;
        case '<':
        case ',':
          //when pressing down ">" or "<" key, scrolling will go faster
          if( fastMov < fastMovThresh)
          {
            fastMov ++;
            pace = 1;
          }
          else
          {    
            pace = fastMovVal;
          }
          if((int)cWinCenter[cWinOrder[2]]-pace<0)
          {
      if( (int)cWinCenter[cWinOrder[2]] == 0)
          return 1;
      else
        sliceNum(0);
          }
      else
      sliceNum((int)cWinCenter[cWinOrder[2]]-pace);
          this->update();
          return 1;
          break;
        case '>':
        case '.':
          //when pressing down ">" or "<" key, scrolling will go faster
          if( fastMov < fastMovThresh)
          {
            fastMov ++;
            pace = 1;
          }
          else
          {
            pace = fastMovVal;
          }
          if((int)cWinCenter[cWinOrder[2]]+pace>(int)cDimSize[cWinOrder[2]]-1)
          {
      if((int)cWinCenter[cWinOrder[2]] == (int)cDimSize[cWinOrder[2]]-1)
        return 1;
      else
        sliceNum((int)cDimSize[cWinOrder[2]]-1);
        }
      else
      sliceNum(cWinCenter[cWinOrder[2]]+pace);
          this->update();
          return 1;
          break;
        case 'r':
          {
          winZoom(1);
          winCenter();
          imageMode(IMG_VAL);
          iwMax(cDataMax);
          iwMin(cDataMin);
          this->update();
          return 1;
          break;
          }
        case '+':
        case '=':
          winZoom(cWinZoom*2);
          this->update();
          return 1;
          break;
        case '-':
        case '_':
          winZoom(cWinZoom*0.5);
          this->update();
          return 1;
          break;
        case 'x':
          flipX(!cFlipX[cWinOrientation]);
          this->update();
          return 1;
          break;
        case 'y':
          flipY(!cFlipY[cWinOrientation]);
          this->update();
          return 1;
          break;
        case 'z':
          flipZ(!cFlipZ[cWinOrientation]);
          this->update();
          return 1;
          break;
        case 'e':
          if(iwModeMax() == IW_FLIP) 
            {
            iwModeMax(IW_MAX);
            this->update();
            }
          else 
            {
            iwModeMax(IW_FLIP);
            this->update();
            }
          return 1;
          break;
        case 'd':
          if(iwModeMin() == IW_FLIP) 
            {
            iwModeMin(IW_MIN);
            this->update();
            }
          else 
            {
            iwModeMin(IW_FLIP);
            this->update();
            }
          return 1;
          break;
        case 'l':
          switch(cImageMode) 
            {
            default:
            case IMG_VAL:
              imageMode(IMG_INV);
              this->update();
              break;
            case IMG_INV:
              imageMode(IMG_LOG);
              this->update();
              break;
            case IMG_LOG:
              imageMode(IMG_DX);
              this->update();
              break;
            case IMG_DX:
              imageMode(IMG_DY);
              this->update();
              break;
            case IMG_DY:
              imageMode(IMG_DZ);
              this->update();
              break;
            case IMG_DZ:
              imageMode(IMG_BLEND);
              this->update();
              break;
            case IMG_BLEND:
              imageMode(IMG_MIP);
              this->update();
              break;
            case IMG_MIP:
              imageMode(IMG_VAL);
              this->update();
              break;
            }
            return 1;
            break;
          case 'q':
              iwMax(cIWMax-(float)0.02*(cDataMax-cDataMin));
              this->update();
              return 1;
              break;
          case 'w':
              iwMax(cIWMax+(float)0.02*(cDataMax-cDataMin));
              this->update();
              return 1;
              break;
          case 'a':
              iwMin(cIWMin-(float)0.02*(cDataMax-cDataMin));
              this->update();
              return 1;
              break;
          case 's':
              iwMin(cIWMin+(float)0.02*(cDataMax-cDataMin));
              this->update();
              return 1;
              break;
          case 'i':
              {
              int newY;
              if(cFlipY[cWinOrientation])
                newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
              else
                newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
              cWinCenter[cWinOrder[1]] = newY;
              winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
              this->update();
              return 1;
              break;
              }
          case 'm':
              {
              int newY;
              if(cFlipY[cWinOrientation])
                newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
              else
                newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
              cWinCenter[cWinOrder[1]] = newY;
              winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
              this->update();
              return 1;
              break;
              }
          case 'j':
              {
              int newX;
              if(cFlipX[cWinOrientation])
                newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
              else
                newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
              cWinCenter[cWinOrder[0]] = newX;
              winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
              this->update();
              return 1;
              break;
              }
          case 'k':
              {
              int newX;
              if(cFlipX[cWinOrientation])
                newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
              else
                newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
              cWinCenter[cWinOrder[0]] = newX;
              winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
              this->update();
              return 1;
              break;
              }
          case 't':
              Transpose(!cTranspose[cWinOrientation]);
              this->update();
              return 1;
              break;
          case 'A':
              cViewAxisLabel = !cViewAxisLabel;
              this->update();
              return 1;
              break;
          case 'C':
              cViewCrosshairs = !cViewCrosshairs;
              this->update();
              return 1;
              break;
          case 'V':
              cViewValue = !cViewValue;
              this->update();
              return 1;
              break;
          case 'P':
              cViewValuePhysicalUnits = !cViewValuePhysicalUnits;
              this->update();
              return 1;
              break;
          case 'D':
              cViewDetails = !cViewDetails;
              this->update();
              return 1;
              break;
          case 'O':
              cViewOverlayData = !cViewOverlayData;
              this->update();
              return 1;
              break;
          case 'p':
              this->saveClickedPointsStored();
              return 1;
              break;
          case 'T':
              cViewClickedPoints = !cViewClickedPoints;
              this->update();
              return 1;
              break;
          case 'h':
            ifuShowText(" \
ImageViewer\n \
===========\n \
\
This is a multi-dimensional image viewer.  It displays one 2D slice or \n \
a MIP view of that data. A variety of viewing options exist...\n \
\n \
Options: (press a key in the window)\n \
\n \
  0 - View slices along the x-axis\n \
  1 - View slices along the y-axis\n \
  2 - View slices along the z-axis (default)\n \
  \n \
  < , - View the next slice\n \
  < , - View the previous slice\n \
  \n \
  r - reset all options\n \
  \n \
  h - help (this document)\n \
  \n \
  x - Flip the x-axis\n \
  y - Flip the y-axis\n \
  z - Flip the z-axis\n \
  \n \
  q w - Decrease, Increase the upper limit of the intensity windowing\n \
  e - Toggle between clipping and setting-to-black values above IW upper limit\n \
  \n \
  a s - Decrease, Increase the lower limit of the intensity windowing\n \
  d - Toggle between clipping and setting-to-white values below IW lower limit\n \
  \n \
  + = - Zoom-in by a factor of 2 \n \
  - _ - Zoom-out by a factor of 2 \n \
  \n \
  i j k m - Shift the image in the corresponding direction\n \
  \n \
  t - Transpose the axis of the slice being viewed\n \
  \n \
  A - View axis labels: P=posterior, L=left, S=superior\n \
  C - View crosshairs that illustrate last user-click in the window\n \
  I - View image values as the user clicks in the window\n \
  T - Toggle display of clicked points\n \
  P - Toggle coordinates display between index and physical units\n \
  D - View image details as an overlay on the image\n \
  O - View a color overlay (application dependent)\n \
  \n \
  p - Save the clicked points in a file \n \
  \n \
  l - Toggle how the data is the window is viewed:\n \
        Modes cycle between the following views: \n \
        Intensity\n \
        Inverse\n \
        Log\n \
        Derivative wrt x\n \
        Derivative wrt y\n \
        Derivative wrt z\n \
        Blend with previous and next slice\n \
        MIP\n \
\n \
");
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

}; //namespace


#endif
