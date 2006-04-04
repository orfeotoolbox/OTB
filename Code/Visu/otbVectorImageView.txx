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
//  cClickSelect[2] = 0;
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
/*  
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
*/  
  cWinZoom = 1;
/*
  cWinOrder[0] = 0;
  cWinOrder[1] = 1;
  cWinOrder[2] = 2;
  cWinOrientation = 2;
  cWinOrientationCallBack = NULL;
  cWinOrientationArg = NULL;
  cWinOrientationArgCallBack = NULL;
*/  
  cWinCenter[0] = 0;
  cWinCenter[1] = 0;
//  cWinCenter[2] = 0;
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
/*
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
  return cFlipZ[this->cWinOrientation];
  }
*/

//
//
/*
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
*/


/*
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

*/


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
                                              /*float _z,*/ float _v))
  {
  cClickSelectCallBack = newClickSelectCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
clickSelectCallBack(void 
                    (*newClickSelectArgCallBack)(float _x, float _y, 
                                                 /*float _z,*/ float _v, 
                                                 void *clickSelectArg), 
                    void * newClickSelectArg)
  {
  cClickSelectArg = newClickSelectArg;
  cClickSelectArgCallBack = newClickSelectArgCallBack;
  }




//
//
template <class TPixel>
void VectorImageView<TPixel>::clickSelect(float newX, float newY/*, float newZ*/)
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

/*  
  cClickSelect[2] = newZ;
  if(cClickSelect[2]<0)
    cClickSelect[2] = 0;
  if(cClickSelect[2] >= cDimSize[2])
    cClickSelect[2] = cDimSize[2]-1;
*/  
  typename ImageType::IndexType ind;
  
  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
//  ind[2] = (unsigned long)cClickSelect[2];
//  cClickSelectV = cImData->GetPixel(ind);
//  cClickSelectV = this->GetInput()->GetPixel(ind);
  cClickSelectV = this->GetInput()->GetPixel(ind)[0];
  
  /*if length of list is equal to max, remove the earliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    cClickedPoints.pop_back();

  ClickPoint clickedPoint(cClickSelect[0], cClickSelect[1], /*cClickSelect[2],*/ cClickSelectV);
  cClickedPoints.push_front( clickedPoint ); 
  
  if(cClickSelectCallBack != NULL)
    cClickSelectCallBack(cClickSelect[0], cClickSelect[1], 
    /*cClickSelect[2],*/ cClickSelectV);
  if(cClickSelectArgCallBack != NULL)
    cClickSelectArgCallBack(cClickSelect[0], cClickSelect[1], 
    /*cClickSelect[2],*/ cClickSelectV,
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



/*
template <class TPixel>
float VectorImageView<TPixel>::clickSelectZ(void)
  {
  return cClickSelect[2];
  }
*/



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
                                             /*float minZ,*/ float maxX, 
                                             float maxY/*, float maxZ*/))
  {
  cClickBoxCallBack = newClickBoxCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::
clickBoxCallBack(void (*newClickBoxArgCallBack)(float minX, float minY, 
                                                /*float minZ,*/ float maxX, 
                                                float maxY, /*float maxZ,*/ 
                                                void * clickBoxArg), 
                 void * newClickBoxArg)
  {
  cClickBoxArg = newClickBoxArg;
  cClickBoxArgCallBack = newClickBoxArgCallBack;
  }




template <class TPixel>
void VectorImageView<TPixel>::boxMin(float minX, float minY/*, float minZ*/)
  {
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
//  cBoxMin[2] = minZ;
  }




template <class TPixel>
void VectorImageView<TPixel>::boxMax(float x, float y/*, float z*/)
  {
  float x0, y0, z0;
  float x1, y1, z1;
  
  x0 = (cBoxMin[0]<x) ? cBoxMin[0] : x;
  y0 = (cBoxMin[1]<y) ? cBoxMin[1] : y;
//  z0 = (cBoxMin[2]<z) ? cBoxMin[2] : z;
  
  x1 = (cBoxMin[0]<x) ? x : cBoxMin[0];
  y1 = (cBoxMin[1]<y) ? y : cBoxMin[1];
//  z1 = (cBoxMin[2]<z) ? z : cBoxMin[2];
  
  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
//  cBoxMin[2] = z0;
  
  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
//  cBoxMax[2] = z1;
  
  if(cClickBoxCallBack != NULL)
    cClickBoxCallBack(cBoxMin[0], cBoxMin[1], /*cBoxMin[2],*/
    cBoxMax[0], cBoxMax[1]/*, cBoxMax[2]*/);
  if(cClickBoxArgCallBack != NULL)
    cClickBoxArgCallBack(cBoxMin[0], cBoxMin[1], /*cBoxMin[2],*/
    cBoxMax[0], cBoxMax[1]/*, cBoxMax[2]*/,
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
//  if(newWinZoom>0.1 && newWinZoom<cDimSize[cWinOrder[0]])
//On interdit le zoom < 1 
  if(newWinZoom>=1 && newWinZoom<cDimSize[0])
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
//  cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
//  cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;
  cWinCenter[0] = cDimSize[0]/2;
  cWinCenter[1] = cDimSize[1]/2;
  
  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
  }





//
//
template <class TPixel>
void VectorImageView<TPixel>::winCenter(int newWinCenterX, 
                                  int newWinCenterY/*,
                                  int newWinCenterZ*/)
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
  
/*  if(newWinCenterZ < 0)
    newWinCenterZ = 0;
  if(newWinCenterZ >= (int)cDimSize[2])
    newWinCenterZ = cDimSize[2] - 1;
  cWinCenter[2] = newWinCenterZ;
*/  
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

/*
template <class TPixel>
unsigned int VectorImageView<TPixel>::winCenterZ(void)
  {
  return cWinCenter[2];
  }
*/



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
  winCenter( cWinCenter[0]+newWinShiftRight , cWinCenter[1]+newWinShiftUp );
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

}; //namespace


#endif
