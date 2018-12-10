#ifndef otbSLICFilter_h
#define otbSLICFilter_h

#include "itkMacro.h"
//OTB, ITK includes
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbStreamingTraits.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionExclusionConstIteratorWithIndex.h"
#include "itkNeighborhoodIterator.h"

#include "itkProcessObject.h"
//Own includes
#include "simplePointCalculator.h"

//STD includes
#include <algorithm>//std::find
#include <unordered_set>
#include <unordered_map>
namespace otb{
  enum RelativePosition{
    POS_TOP,
    POS_RIGHT,
    POS_BOTTOM,
    POS_LEFT
  };

  enum NeighborhoodRelativePosition{
    NBH_TOP,
    NBH_TOP_RIGHT,
    NBH_RIGHT,
    NBH_BOTTOM_RIGHT,
    NBH_BOTTOM,
    NBH_BOTTOM_LEFT,
    NBH_LEFT,
    NBH_TOP_LEFT
  };
  struct ProcessingTile
  {
    long int rows[2]; // lower and upper rows (-1 means that the row has not be considered)
    long int columns[2]; // lower and upper columns (-1 means that the row has not be considered)
    long int tileNeighbors[8]; // tile Neighbors at (top, top right, right, bottom right, bottom, bottom left, left, top left)
    long int margin[4]; // Is there a margin at top, left, bottom or right
    otb::VectorImage<double>::RegionType region; // The image region

    // Temporary files
    std::string nodeFileName;
    std::string edgeFileName;
    std::string nodeMarginFileName;
    std::string edgeMarginFileName;
  };
  template <class TInputImage, class TOutputLabelImage>
  class ITK_EXPORT SLICFilter : public itk::ProcessObject
  {
  public:
    typedef SLICFilter                                                        Self;
    typedef itk::ProcessObject                                                   Superclass;
    typedef itk::SmartPointer<Self>                                              Pointer;
    typedef itk::SmartPointer<const Self>                                        ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(SLICFilter, itk::ProcessObject);

    /** ImageDimension constants */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

    typedef double                                                               RealType;
    typedef typename TOutputLabelImage::PixelType                                LabelType;
    typedef TOutputLabelImage                                                    OutputLabelImageType;
    typedef TInputImage                                                          VectorImageType;
    typedef typename TInputImage::RegionType                                     RegionType;
    typedef typename std::vector<double>                                         SampleType;
    typedef typename VectorImage<double>::PixelType                              PixelType;
    typedef std::unordered_map<LabelType,SampleType>                             ListSampleType;
    typedef std::unordered_map<LabelType,unsigned int>                           CountContainerType;
    typedef std::vector<LabelType>                                               NeighborhoodLabelsType;
    typedef itk::ConstantBoundaryCondition<TOutputLabelImage>                    BoundaryConditionType;
    typedef itk::NeighborhoodIterator<TOutputLabelImage,BoundaryConditionType>   NeighborhoodLabelIteratorType;
    typedef typename NeighborhoodLabelIteratorType::NeighborhoodType             NeighborhoodType;
    typedef typename NeighborhoodType::ConstIterator                             NeighborhoodContainerIteratorType;
    
    // Sets the spatial width, i.e. average root of size of the output superpixels
    itkSetMacro(SpatialWidth,unsigned int);
    
    // Sets the spatial distance weight, i.e. the compacity/homogeneity ratio of SP
    itkSetMacro(SpatialDistanceWeight,RealType);

    //Sets the max number of iterations
    itkSetMacro(MaxIterationNumber,unsigned int);

    //Sets the residual threshold for stopping the algorithm
    itkSetMacro(Threshold,RealType);
    
    //Desired number of superpixels along the X axis of the current tile
    itkSetMacro(NbSPx, unsigned int);
    
    //Margin in superpixels
    itkSetMacro(Margin, unsigned int);

    //Sets the input
    using Superclass::SetInput;
    void SetInput(const TInputImage* InputImagePtr);
    
    //Add image of margin to list of margins
    void AddInputMargin(typename TOutputLabelImage::Pointer InputSegPtr);
    //Set information regarding the current tile (size, origin, neighbors)
    void SetTile(ProcessingTile const& _t);
    //Clear the margin images
    void ClearMargins();
    // //Returns the const image of region labels
    const OutputLabelImageType * GetOutput() const;
    // //Returns the image of region labels
    OutputLabelImageType * GetOutput();
    //Write the segmentation of a region
    template <class TImage> static void WriteImageRegion(typename TImage::Pointer inputPtr, typename TImage::Pointer inputImage, typename TImage::RegionType const& region);

  protected:
    SLICFilter();
    ~SLICFilter() ITK_OVERRIDE;
    void GenerateData() ITK_OVERRIDE;
    void GenerateInputRequestedRegion() ITK_OVERRIDE;
    TInputImage* GetInput();
    
  private:
    SLICFilter(const Self &); //purposely not implemented
    void operator =(const Self&); //purposely not implemented
    unsigned int m_SpatialWidth;
    RealType m_SpatialDistanceWeight;
    unsigned int m_MaxIterationNumber;
    RealType m_Threshold;
    ProcessingTile m_Tile;
    unsigned int m_NbSPx;
    unsigned int m_Margin;
    std::vector<typename OutputLabelImageType::Pointer> m_InputSeg;
    //Test to check if a pixel is in the corner of the tile (with margins)
    static bool inImageCorner(std::vector<RegionType> const& corners, typename TOutputLabelImage::IndexType index);
    //Method to recompute the new coordinates of a centroid when removing a value
    static void removeFromCentroid(PixelType const& currentPixel, typename TInputImage::IndexType index, LabelType label, CountContainerType & count,  ListSampleType & means);  
    //Method to recompute the new coordinates of a centroid when adding a value
    static void addToCentroid(PixelType const& currentPixel, typename TInputImage::IndexType index, LabelType label, CountContainerType & count,  ListSampleType & means);

  };
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSLICFilter.txx"
#endif

#endif
