/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   24 janvier 2006
  Role      :    
  $Id$

=========================================================================*/
#ifndef __otbFlst_h
#define __otbFlst_h
 
#include "itkImage.h"
#include "otbImageToTreeFilter.h"
#include "otbTreeNeighborhood.h"
#include "otbShape.h"
#include "itkPolyLineParametricPath.h"
#include "itkTreeContainer.h"

namespace otb
{

/** \class Flst
 * \brief Algorithme de Fast Level Sets Transform of an image
 *
 */
template < class TInputImage, 
 	   class TOutputTree = itk::PolyLineParametricPath<2> >
class ITK_EXPORT Flst :  public ImageToTreeFilter< TInputImage, TOutputTree >
{
public:

  typedef itk::PolyLineParametricPath<2>      PathType;
  typedef typename PathType::Pointer          PathPointer;

  typedef itk::TreeContainer<PathPointer>        TreeType;
  typedef typename TreeType::Pointer          TreePointer;


  /** Standard class typedefs. */
  typedef Flst                                              Self;
  typedef ImageToTreeFilter<TInputImage,TOutputTree>        Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(Flst, ImageToTreeFilter);
  
  itkStaticConstMacro(InputImageDimension,
  		      unsigned int,
                      TInputImage::ImageDimension);
   
  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType; 
  typedef typename Superclass::InputImagePixelType   InputImagePixelType; 
  typedef typename Superclass::InputImageSizeType    InputImageSizeType;                 
  typedef typename Superclass::InputImageValueType   InputImageValueType;
  typedef typename Superclass::InputImageIndexType   InputImageIndexType;

  typedef typename Superclass::OutputTreeType         OutputTreeType;
  typedef typename Superclass::OutputTreePointer      OutputTreePointer;
  typedef typename Superclass::OutputTreeConstPointer OutputTreeConstPointer;
  
  typedef itk::Image<int,2>                     IntImageType;
  typedef typename IntImageType::Pointer        IntImagePointer;
  typedef typename IntImageType::ConstPointer   IntImageConstPointer;
  typedef typename IntImageType::IndexType      IntImageIndexType;
  typedef typename IntImageType::PixelType      IntImagePixelType;
    
  typedef itk::Image<float,2>                   RealImageType;
  typedef typename RealImageType::Pointer       RealImagePointer;
  typedef typename RealImageType::ConstPointer  RealImageConstPointer;
  typedef typename RealImageType::IndexType     RealImageIndexType;
 
   
protected:
  Flst();
  virtual ~Flst();
  
  virtual void GenerateData();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

/* Optimization parameters. Probably should depend on image size, but these
values seem good for most images. */
  static const int INIT_MAX_AREA; 
  static const int STEP_MAX_AREA;
  
/* A 'local configuration' of the pixel is the logical 'or' of these values,
stored in one byte. Corresponding bit is set if the neighbor is in region */
  static const int EAST; 
  static const int NORTH_EAST;
  static const int NORTH;
  static const int NORTH_WEST;
  static const int WEST;
  static const int SOUTH_WEST;
  static const int SOUTH;
  static const int SOUTH_EAST;    
  
/* Gives for each configuration of the neighborhood around the pixel the number
of new cc's of the complement created (sometimes negative, since
cc's can be deleted), when the pixel is appended to the region.
Configurations are stored on one byte. 
tabPatterns[0]: set in 4-connectedness and complement in 8-connectedness.
tabPatterns[1]: set in 8-connectedness and complement in 4-connectedness. */
  static const char tabPatterns[2][256];

  void init_image_of_visited_pixels(int*** ptabtabVisitedPixels);
  void free_image_of_visited_pixels(int** tabtabVisitedPixels);
  void init_region(int iMaxArea);
  void free_region();
  void init_output_image(const InputImageType* image,
  			 float***  ptabtabPixelsOutput);
  void free_output_image(float** tabtabPixelsOutput);
  			  
  void add_neighbor(Neighborhood* pNeighborhood,short int x,
                    short int y,float value);
  void remove_neighbor(Neighborhood* pNeighborhood);

  char is_local_min(float** ou,short int x,short int y,char b8Connected);
  char is_local_max(float** ou,short int x,short int y,char b8Connected);
     
  void levelize(float** tabtabPixelsOutput,Point_plane* tabPoints,
           int iNbPoints,float newGray);
  unsigned char configuration(int** tabtabVisitedPixels,short int x,short int y);
  
  Shape* new_shape(int iCurrentArea,float currentGrayLevel, 
                   char bOfInferiorType,Shape* pChild);
  void update_smallest_shapes(Point_plane* tabPoints,int iNbPoints);
  void connect(Point_plane* tabPoints,int iNbPoints,
               Connection* tabConnections,Shape* pSmallestShape);
  void new_connection(Point_plane* pPoint,float level,Connection* tabConnections);

/* Is the neighbor pixel already stored for this exploration? */
  int NEIGHBOR_NOT_STORED(short int x,short int y) 
                 { return (tabtabVisitedNeighbors[y][x] < iExploration); };

  void store_4neighbors(float** ou,short int x,short int y,Neighborhood* pNeighborhood);
  void store_8neighbors(float** ou,short int x,short int y,Neighborhood* pNeighborhood);
	        
  char add_iso_level(Point_plane* tabPointsInShape,int* pCurrentArea,
		float currentGrayLevel, Neighborhood* pNeighborhood, 
		float** ou,int** tabtabVisitedPixels, 
		char* p8Connected, char* pIgnoreHoles);
  void find_terminal_branch(float** ou,int** tabtabVisitedPixels,
                       short int x,short int y,
	  	       char b8Connected, Neighborhood* pNeighborhood, 
		       Connection* tabConnections);
  void scan(float** tabtabPixelsOutput,int** tabtabVisitedPixels,
            Neighborhood* pNeighborhood,Connection* tabConnections);

  void PrintTree(Shapes *pTree);
  void print_PointsInShape();
  void print_visited_pixels(int** ptabtabVisitedPixels);
  void PrintPixelOutput(float **tabtabPixelsOutput);

  void insert_children(Shape* pParent, Shape* pNewChildToInsert);  
//  itkSetMacro(MinArea, int);
//  itkGetConstReferenceMacro(MinArea, int);
 
private:
  Flst(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  int iWidth;
  int iHeight;
  int iMinArea; 
  int iMaxArea; 
  int iAreaImage; 
  int iHalfAreaImage; 
  int iPerimeterImage;
  int iExploration;  /* Used to avoid reinitializing images */
  Point_plane* tabPointsInShape;
  int** tabtabVisitedNeighbors; /* Exterior boundary */
  Shapes* pGlobalTree;
  int iAtBorder; /* #points meeting at border of image */
          
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlst.txx"
#endif
  
#endif
