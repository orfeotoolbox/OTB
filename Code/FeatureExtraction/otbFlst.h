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
//#include "otbTreeSource.h"
#include "itkPolyLineParametricPath.h"

namespace otb
{


typedef struct 
    {
    int x;
    int y;
    } PointPlaneType;

class ShapeType
{
public:
    char                   inferior_type;
    float                  value;
    char                   open;
    int                    area;
    char                   removed;
    std::vector<PointPlaneType>     pixels;
    std::vector<PointPlaneType>     boundary;
    ShapeType                  *parent;
    ShapeType                  *child;
    ShapeType                  *next_sibling;
       
    ShapeType();
    ~ShapeType(); 
    static void InsertChildren(ShapeType *pParent, ShapeType *pNewChildToInsert);
 
};


class ShapeList
{
public:

  int              nrow;
  int              ncol;
  int              interpolation;
  int              nb_shapes;
  ShapeType        *the_shapes;
  ShapeType        **smallest_shape;

  ShapeList();
  ~ShapeList();
  void Allocate(int nrow,int ncol,float value);
  void Delete();
  void Change(int nrow,int ncol,float value);
};



typedef struct
     {
       ShapeType *shape; //Pointeur ou pas ?
       float     level;
     } ConnectionType;

/** \class Flst
 * \brief Algorithme de Fast Level Sets Transform of an image
 *
 */
template < class TInputImage, class TOutputTree /*= itk::PolyLineParametricPath<2>*/ >
class ITK_EXPORT Flst :  public ImageToTreeFilter< TInputImage, TOutputTree >
{
public:
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
  
  
  typedef itk::Image<float,2>                   RealImageType;
  typedef typename RealImageType::Pointer       RealImagePointer;
  typedef typename RealImageType::ConstPointer  RealImageConstPointer;
  typedef typename RealImageType::IndexType     RealImageIndexType;
    
  typedef ShapeList 	          ShapeTreeType;
  typedef ShapeTreeType*          ShapeTreePointer;
  typedef const ShapeTreeType*    ShapeTreeConstPointer;
   
  typedef TreeNeighborhood               NeighborhoodType;
  typedef TreeNeighborhood::NeighborType NeighborType;
  typedef TreeNeighborhood::PointType    NeighborPointType;
  typedef std::vector<PointPlaneType>   PointPlaneListType;

  typedef std::vector<ConnectionType>   ConnectionListType;

protected:
  Flst();
  virtual ~Flst();
  
  virtual void GenerateData();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 /** Définition des paramètres d'optimisation. 
 * Ils sont dépendant de la taille des images mais ces valeurs semblent 
 * convenir à  la majoritée des images. */
  static const int INIT_MAX_AREA; 
  static const int STEP_MAX_AREA;
  
  /** configuration locale des pixels */
  static const int EAST; 
  static const int NORTH_EAST;
  static const int NORTH;
  static const int NORTH_WEST;
  static const int WEST;
  static const int SOUTH_WEST;
  static const int SOUTH;
  static const int SOUTH_EAST;    
  
  static const char tabPatterns[2][256];
  
  itkSetMacro(MinArea, int);
  itkGetConstReferenceMacro(MinArea, int);
      
  char Is_local_min(RealImagePointer ou, int x, int y, char b8Connected);
  char Is_local_max(RealImagePointer ou, int x, int y, char b8Connected);
  void Levelize(PointPlaneListType tabPoints,int iNbPoints, float newGray);
  unsigned char Configuration(int x,int y);  
  ShapeType* NewShape(int iCurrentArea, float currentGrayLevel, char bOfInferiorType,ShapeType* pChild);
  void UpdateSmallestShapes(PointPlaneListType* tabPoints,int iLastShapeArea, int iArea);
  void Connect(PointPlaneListType* tabPoints,int iNbPoints,
               ConnectionListType* tabConnections,ShapeType* pSmallestShape);
  void NewConnection(PointPlaneType* pPoint,float level,ConnectionListType* tabConnections);	       
  int NEIGHBOR_NOT_STORED(int x,int y);	   
  void Store_4neighbors(int x,int y,NeighborhoodType* pNeighborhood);
  void Store_8neighbors(int x,int y,NeighborhoodType* pNeighborhood);
  char AddIsoLevel(PointPlaneListType tabPointsInShape,int* pCurrentArea,
	          float currentGrayLevel,NeighborhoodType* pNeighborhood,
	          char* p8Connected,char* pIgnoreHoles);
  void FindTerminalBranch(int x,int y, char b8Connected, 
                          NeighborhoodType*   pNeighborhood, 
		          ConnectionListType* tabConnections);		  
  void Scan(NeighborhoodType* pNeighborhood,ConnectionListType* tabConnections);

 
private:
  Flst(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  int                   m_Width;
  int                   m_Height;
  int                   m_MinArea;
  int                   m_MaxArea;
  int                   m_AreaImage;
  int                   m_HalfAreaImage;
  int                   m_PerimeterImage;
  int                   m_AtBorder;
  int                   m_Exploration;
  IntImagePointer       m_VisitedPixels;
  RealImagePointer      m_PixelOutput;
  NeighborhoodType*     m_Neighborhood;
  ConnectionListType*   m_Connections;
  ShapeTreePointer      m_GlobalTree;
  PointPlaneListType    m_PointsInShape;
        
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlst.txx"
#endif
  
#endif
