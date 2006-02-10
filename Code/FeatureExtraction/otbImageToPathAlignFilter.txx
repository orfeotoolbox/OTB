/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   08 fevrier 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#ifndef __otbImageToPathAlignFilter_txx
#define __otbImageToPathAlignFilter_txx

#include "otbImageToPathAlignFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkPathIterator.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include <vector>

struct one_segment
{
  short start;    /* starting position (distance from border) */
  short end;      /* ending position (hence, length is end-start+1) */
  double nfa;     /* number of false alarms */
  char ok;       
};


namespace otb
{

/** Constructor */
template <class TInputImage, class TOutputPath>
ImageToPathAlignFilter<TInputImage,TOutputPath>
::ImageToPathAlignFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Size.Fill(0);
  
  for (unsigned int i = 0; i < InputImageDimension; i++)
    {
    // Set an image spacing for the user
    m_Spacing[i] = 1.0;
    m_Origin[i] = 0;
    }

  m_PathValue = itk::NumericTraits<ValueType>::One;
  m_BackgroundValue = itk::NumericTraits<ValueType>::Zero;
}

/** Destructor */
template <class TInputImage, class TOutputPath>
ImageToPathAlignFilter<TInputImage,TOutputPath>
::~ImageToPathAlignFilter()
{
}
  

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::SetSpacing(const double* spacing)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( spacing[i] != m_Spacing[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::SetSpacing(const float* spacing)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( (double)spacing[i] != m_Spacing[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
const double * 
ImageToPathAlignFilter<TInputImage,TOutputPath>
::GetSpacing() const
{
  return m_Spacing;
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::SetOrigin(const double* origin)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( origin[i] != m_Origin[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::SetOrigin(const float* origin)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( (double)origin[i] != m_Origin[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
const double * 
ImageToPathAlignFilter<TInputImage,TOutputPath>
::GetOrigin() const
{
  return m_Origin;
}

//----------------------------------------------------------------------------

template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::GenerateData(void)
{
  
  int i;
  
  itkDebugMacro(<< "ImageToPathAlignFilter::GenerateData() called");

  // Get the input and output pointers 
  const InputImageType * InputImage   = this->GetInput();
  OutputPathPointer    OutputPath = this->GetOutput();

  // Generate the image
  double origin[InputImageDimension];
  SizeType size;
  
  for(i=0;i<InputImageDimension;i++)
    {
    // Set Image size to the size of the path's bounding box
    //size[i] = (long unsigned int)(InputObject->GetBoundingBox()->GetMaximum()[i]
    //                              - InputObject->GetBoundingBox()->GetMinimum()[i]);
    size[i]=0;
    origin[i]=0;
    }
  
  typename InputImageType::IndexType index;
  index.Fill(0);
  typename InputImageType::RegionType region;
  
  // If the size of the output has been explicitly specified, the filter
  // will set the output size to the explicit size, otherwise the size from the spatial
  // paths's bounding box will be used as default.
  
  bool specified = false;
  for (i = 0; i < InputImageDimension; i++)
    {
    if (m_Size[i] != 0)
      {
      specified = true;
      break;
      }
    }
  
  if (specified)
    {
    region.SetSize( m_Size );
    }
  else
    {
    itkExceptionMacro( << "Currently, the user MUST specify an image size" )
    //region.SetSize( size );
    }
  region.SetIndex( index );
  
  InputImage->SetLargestPossibleRegion( region);     // 
  InputImage->SetBufferedRegion( region );           // set the region 
  InputImage->SetRequestedRegion( region );          //
  
  // If the spacing has been explicitly specified, the filter
  // will set the output spacing to that explicit spacing, otherwise the spacing from
  // the spatial object is used as default.
  
  specified = false;
  for (i = 0; i < InputImageDimension; i++)
    {
    if (m_Spacing[i] != 0)
      {
      specified = true;
      break;
      }
    }
  
  if (specified)
    {
    InputImage->SetSpacing(this->m_Spacing);         // set spacing
    }
  else
    {
    itkExceptionMacro( << "Currently, the user MUST specify an image spacing" )
    //OutputImage->SetSpacing(InputObject->GetIndexToObjectTransform()->GetScaleComponent());   // set spacing
    }
  InputImage->SetOrigin(origin);   //   and origin
  InputImage->Allocate();   // allocate the image                            
  
  RealImagePointer AngleImage = RealImageType::New();
  AngleImage->SetRegions( InputImage->GetRequestedRegion() );
  AngleImage->CopyInformation( InputImage );
  AngleImage->Allocate(); 

  typedef itk::ImageLinearIteratorWithIndex< InputImageType > IteratorType; 
  typedef itk::ImageLinearConstIteratorWithIndex< RealType > ConstIteratorType;  
  
  ConstIteratorType InputIt( InputImage, InputImage->GetRequestedRegion() );
  IteratorType      AngleIt( AngleImage, InputImage->GetRequestedRegion() );
//  InputIt.SetDirection(0);
//  AngleIt.SetDirection(0);  
  
  /** 1ere ETAPE: Compute the direction of the level line at each point*/
  
  typename RealImageType::IndexType requestedIndex = AngleImage->GetRequestedRegion().GetIndex();
  typename RealImageType::SizeType  requestedSize  = AngleImage->GetRequestedRegion().GetSize();     
  typename RealImageType::IndexType idx            = AngleIt.GetIndex();
  RealType com1,com2,gx,gy,norm;
  
  for ( AngleIt.GoToBegin(); !AngleIt.IsAtEnd(); ++AngleIt)
      {
      typename RealImageType::IndexType idx = AngleIt.GetIndex();
      if( (idx[0]!=requestedSize[0]) || (idx[1]!=requestedSize[1]) )
        { 
	RealType PixelA = static_cast<RealType>(InputImage->GetPixel(idx)   );
	RealType PixelB = static_cast<RealType>(InputImage->GetPixel(idx+1) );
	idx[0] = requestedIndex[0] + idx[0];
      	idx[1] = requestedIndex[1] + idx[1] +1;
      	RealType PixelC = static_cast<RealType>(InputImage->GetPixel(idx)   );
      	RealType PixelD = static_cast<RealType>(InputImage->GetPixel(idx+1) );
      	RealType com1 = PixelD-PixelA;
      	RealType com2 = PixelB-PixelC;
      	gx = 0.5 * (com1+com2);
      	gy = 0.5 * (com1-com2);
      	norm = gx*gx + gy*gy;
	if (norm <=m_MinGradNorm)
		AngleIt.Set(static_cast<RealType>(-1000.0));
		else AngleIt.Set( static_cast<RealType>( atan2(gx,-gy)) );
        }
	else
	{
	AngleIt.Set(static_cast<RealType>(-1000.0));
	}
      }
      
   /** 2ème ETAPE: Compute the direction of the level line at each point*/
  RealType max_nfa = pow(10.0,-m_Eps);
  int nx,ny;
  nx = requestedSize[0];
  ny = requestedSize[1];
   
  /* maximal length for a line */
  int n = (int)( ceil(hypot((double)nx,(double)ny))+1 );
  
    /*** compute P(k,l) ***/
  std::vector<float> test;
  test.resize(n+1);
  int adr1,adr2,x,y;
  float lambda,q;
  float p = 1.0/ static_cast<float>(m_NbGradDirection);
  float m = (float)(nx*ny)*(float)(nx*ny);
  
  q = 1.0-p;

  test[0] = 1.0;
  for (y=1,adr2=0;y<=n;y++) {
    adr1 = adr2;
    adr2 += n+1;    
    test[adr2] = q*test[adr1];
    for (x=1;x<=y;x++) 
      test[adr2+x] = p*test[adr1+x-1] + q*test[adr1+x];
  }  

  /*** sum to obtain proba (>=k among y) ***/
  for (y=1,adr1=n+1;y<=n;y++,adr1+=n+1) 
    for (x=y-1;x>=0;x--) 
      test[adr1+x] += test[adr1+x+1];

  /*** multiply by m (number of segments) to obtain expectation***/
  for (adr1=(n+1)*(n+1);--adr1>=0;)
    test[adr1] *= m;
   
/* Main Procedure */

  float prec = M_PI/static_cast<float>(m_NbGradDirection);
  float ntheta = m_NbLineDirection/2;  /* i.e. # directions of NON-ORIENTED lines */
  float dtheta = M_PI/(float)ntheta;
 
  int max_nblocs = n/2+1; /* maximal number of blocs */
  std::vector<int> count,startbloc,endbloc;
  
  count.resize(max_nblocs);
  startbloc.resize(max_nblocs);
  endbloc.resize(max_nblocs);
  
  int size_seg = 10000; /* initial allocation (may reallocate later) */
//  seg = (struct one_segment *)malloc(size_seg*sizeof(struct one_segment));
  
  /* counter for recorded segments (seglist) */
  int iseglist = 0;

/******************** first loop : the four sides ********************/
  int mx,my,ox,oy;
  int xx,yy,pos,posmax,nblocs,inbloc;
  int cur,j,side,tmp,l,lphase;
  int itheta;
  int iseg,size_seglist;
  float theta,theta0,dx,dy,error;
  double nfa;

  std::vector<one_segment> seg;
  std::vector<float> seglist;
  
  seg.resize(size_seg);
  seglist.resize(5*size_seglist);
  
  for (side=0;side<4;side++) {

    theta0 = 0.5*M_PI*(double)side;
    mx = ((side==0 || side==2)?1:0);
    my = ((side==1 || side==3)?1:0);
    ox = ((side==1)?nx-1:0);
    oy = ((side==2)?ny-1:0);

    posmax = nx*mx+ny*my;
    
    
    /*** second loop : angles ***/
    for (itheta = 0; itheta<ntheta; itheta++) {
      theta = theta0 + (float)(itheta)*dtheta;
      dx = (float)cos((double)theta);
      dy = (float)sin((double)theta);
      
      /*** third loop : start positions ***/
      for (pos=0;pos<posmax;pos++) {

	/* clear segment array */
	iseg = 0;

	/*** fourth loop : phase for two-spaced pixels ***/
        for (lphase=0;lphase<2;lphase++) {
	  
	  /*** detect aligned points by blocs ***/
	  inbloc = nblocs = cur = l = count[0] = 0;
	  xx = ox+pos*mx + (int)(dx*(float)(l*2+lphase));
	  yy = oy+pos*my + (int)(dy*(float)(l*2+lphase));

	  for (;xx>=0 && xx<nx && yy>=0 && yy<ny;) {

	    error = static_cast<float>( AngleImage->GetPixel(yy*nx+xx) );    
	    if (error>-100.0) {
	      error -= theta;
	      while (error<=-M_PI) error += 2.0*M_PI;
	      while (error>M_PI) error -= 2.0*M_PI;
	      if (error<0.0) error = -error;
	      if (error<prec) {
		cur++;
		if (!inbloc) {
		  startbloc[nblocs]=l;
		  inbloc=1;
		}
	      } else {
		if (inbloc) {
		  endbloc[nblocs] = l-1;
		  nblocs++;
		  count[nblocs] = cur;
		}
		inbloc=0;
	      }
	    }
	    /* compute next point */
	    l++;
	    xx = ox+pos*mx + (int)(dx*(float)(l*2+lphase));
	    yy = oy+pos*my + (int)(dy*(float)(l*2+lphase));
	  }
	  
	  /*** detect meaningful segments ***/
	  for (i=0;i<nblocs;i++) 
	    for (j=i;j<nblocs;j++) 
	      if ((nfa = test[count[j+1]-count[i]
			     +(n+1)*(1+endbloc[j]-startbloc[i])]) < max_nfa) {
		seg[iseg].start = startbloc[i]*2+lphase;
		seg[iseg].end = endbloc[j]*2+lphase;
		seg[iseg].nfa = nfa;
		seg[iseg].ok = 1;
		iseg++;
		/* reallocate if necessary */
		if (iseg==size_seg) {
		  size_seg = (size_seg*3)/2;
		  seg.resize(size_seg);
//		  if (!seg) 
//		    mwerror(FATAL,1,"Not enough memory.");
		}
	      }
	}
	/*** end of phase loop ***/

	/*** remove non-maximal segments ***/
	if (!m_isMeaningfulSegment) 
	  for (i=0;i<iseg;i++) 
	    for (j=0;j<iseg;j++)
	      if (i!=j) 
		
		/* seg[i] is included in seg[j] ? */
		if (seg[i].start>=seg[j].start && seg[i].end<=seg[j].end) {
		  
		  /* remove the less meaningful of seg[i] and seg[j] */
		  if (seg[i].nfa<seg[j].nfa) seg[j].ok=0;	
		  else seg[i].ok=0;
		  
		}
	
	/*** store detected segments ***/
	for (i=0;i<iseg;i++) 
	  if (seg[i].ok) {
	    seglist[iseglist*5  ]=(float)(ox+pos*mx)+dx*(float)(seg[i].start);
	    seglist[iseglist*5+1]=(float)(oy+pos*my)+dy*(float)(seg[i].start);
	    seglist[iseglist*5+2]=(float)(ox+pos*mx)+dx*(float)(seg[i].end);
	    seglist[iseglist*5+3]=(float)(oy+pos*my)+dy*(float)(seg[i].end);
	    seglist[iseglist*5+4]=-(float)log10(seg[i].nfa);
	    iseglist++; 
	    /* reallocate seglist if necessary */
	    if (iseglist==size_seglist) {
	      size_seglist = (size_seglist*3)/2;
	      seglist.resize(size_seglist);
//	      if (!seglist) 
//		mwerror(FATAL,1,"Not enough memory.");
	    }
	  }
      }
    }
    /*** end of second loop ***/
    
  /******************** end of first loop ********************/
    

  /* build segments list */
  seglist.resize(5*iseglist);
//  result = mw_new_flist();
//  result->size = result->max_size = iseglist;
//  result->dim = 5;
//  result->values = seglist;

  /* build curves if requested */
//  if (crv) {
//   crv = mw_change_flists(crv,iseglist,iseglist);
//   for (i=0;i<iseglist;i++) {
//     crv->list[i] = mw_change_flist(NULL,2,2,2);
//      for (j=0;j<4;j++) 
//	crv->list[i]->values[j] = seglist[i*5+j]+.5;
//    }
//  }
    
    
    
  }


      
       
//  itk::PathIterator<InputImageType,OutputPathType> pathIt(InputImage,OutputPath);
//  for( pathIt.GoToBegin(); !pathIt.IsAtEnd(); ++pathIt )
//    {
//    pathIt.Set( m_PathValue );
//    }
  
  itkDebugMacro(<< "ImageToPathAlignFilter::GenerateData() finished");

} // end update function  


template <class TInputImage, class TOutputPath>
void
ImageToPathAlignFilter<TInputImage,TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size : " << m_Size << std::endl;
  os << indent << "Path Value : " << m_PathValue << std::endl;
  os << indent << "Background Value : " << m_BackgroundValue << std::endl;
}



} // end namespace otb

#endif
