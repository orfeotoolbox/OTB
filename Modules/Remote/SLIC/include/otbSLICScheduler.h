#ifndef otbSLICScheduler_h
#define otbSLICScheduler_h

#include "itkMacro.h"
//OTB, ITK includes
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionExclusionConstIteratorWithIndex.h"

#include "itkImageToImageFilter.h"
#include "itkNeighborhoodIterator.h"
#include "itkExtractImageFilter.h"
#include "otbMPIConfig.h"

//Own includes
#include "otbSLICFilter.h"

#include <itksys/SystemTools.hxx>

#include <gdal.h>
#include <gdal_priv.h>
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wshadow"
#include <vrtdataset.h>
# pragma GCC diagnostic pop
#else
#include <vrtdataset.h>
#endif

#include <ogr_spatialref.h>




namespace otb{
  
  template <class TInputImage, class TOutputLabelImage>
  class ITK_EXPORT SLICScheduler : public itk::Object
  {
  public:
    typedef SLICScheduler                                                        Self;
    typedef itk::Object                                                          Superclass;
    typedef itk::SmartPointer<Self>                                              Pointer;
    typedef itk::SmartPointer<const Self>                                        ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(SLICScheduler, ImageToImageFilter);

    /** ImageDimension constants */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

    typedef double                                                               RealType;
    typedef typename TOutputLabelImage::PixelType                                LabelType;
    typedef otb::ImageFileReader<TInputImage>                                    ReaderType;
    typedef TOutputLabelImage                                                    OutputLabelImageType;
    typedef TInputImage                                                          VectorImageType;
    typedef typename TInputImage::RegionType                                     RegionType;
    typedef typename std::vector<RealType>                                       SampleType;
    typedef std::vector<SampleType>                                              ListSampleType;
    typedef std::vector<LabelType>                                               NeighborhoodLabelsType;
    typedef itk::ConstantBoundaryCondition<TOutputLabelImage>                    BoundaryConditionType;
    typedef itk::NeighborhoodIterator<TOutputLabelImage,BoundaryConditionType>   NeighborhoodLabelIteratorType;
    typedef typename NeighborhoodLabelIteratorType::NeighborhoodType             NeighborhoodType;
    typedef typename NeighborhoodType::ConstIterator                             NeighborhoodContainerIteratorType;
    typedef otb::ImageFileWriter<TOutputLabelImage>                              WriterType;
    typedef std::vector<otb::ProcessingTile>                                     TileListType;
    typedef otb::SLICFilter<TInputImage, TOutputLabelImage>                      SLICFilter;    
    typedef itk::ExtractImageFilter<TInputImage,TInputImage>                     ExtractFilterType;    	
    typedef otb::ImageFileReader<TOutputLabelImage>                              LabelReaderType;
    typedef itk::ExtractImageFilter<TOutputLabelImage, TOutputLabelImage>        LabelExtractFilterType;
   
    // Sets the spatial width, i.e. average root of size of the output superpixels
    itkSetMacro(SpatialWidth,unsigned int);
    
    // Sets the spatial distance weight, i.e. the compacity/homogeneity ratio of SP
    itkSetMacro(SpatialDistanceWeight,RealType);

    //Sets the max number of iterations
    itkSetMacro(MaxIterationNumber,unsigned int);

    //Sets the residual threshold for stopping the algorithm
    itkSetMacro(Threshold,RealType);
    
    //Number of tiles in X direction
    itkSetMacro(NbTilesX,unsigned int);
    
    //Number of tiles in Y direction
    itkSetMacro(NbTilesY,unsigned int);
    
    //Margin (in SP widths)
    itkSetMacro(Margin,RealType);

    //Prefix for file writing
    itkSetMacro(Prefix,std::string);
    
    //Calculate a list of regions of the input image with the appropriate margins
    static std::vector<ProcessingTile> SplitOTBImage(const TInputImage * imagePtr, // input image
						     const unsigned int tileWidth, // width of the tile
						     const unsigned int tileHeight, // height of the tile
						     const unsigned int margin, // stability margin
						     const unsigned int nbTilesX,
						     const unsigned int nbTilesY,
						     const std::string temporaryFilesPrefix);
    
    //Generate the .vrt corresponding to a list of input tiles
    static void writeVRTRegions(TileListType const& inputRegions, GDALDataset *VRTOutput, std::string prefix, unsigned int band);

    //Apply SLIC segmentation to the input image
    void Run();

    void SetInputImage(typename TInputImage::Pointer InputImagePointer);

  protected:
    SLICScheduler();

    ~SLICScheduler() ITK_OVERRIDE;
        
  private:
    SLICScheduler(const Self &); //purposely not implemented
    void operator =(const Self&); //purposely not implemented
    unsigned int m_SpatialWidth;
    RealType m_SpatialDistanceWeight;
    unsigned int m_MaxIterationNumber;
    RealType m_Threshold;
    std::string m_Prefix;
    ListSampleType m_SeedMeans;
    unsigned int m_NbTilesX;
    unsigned int m_NbTilesY;
    RealType m_Margin;
    typename TInputImage::Pointer m_InputImage;
  };
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSLICScheduler.txx"
#endif

#endif
