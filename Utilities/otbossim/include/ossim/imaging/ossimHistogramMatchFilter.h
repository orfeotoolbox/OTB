#ifndef ossimHistogramMatchFilter_HEADER
#define ossimHistogramMatchFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>

/**
 * class ossimHistogramMatchFilter
 *
 * <pre>
 *
 * This class owns two ossimHistogramEqualization filters and will chain them together.  It will
 * make the left most filter the forward transform by transforming the input input to an equalized
 * space defined by the input histogram.  The right filter is the target histogram to match.  It will
 * take the equalized space and invert it to a target histogram.
 *
 * There is an auto load input histogram flag that if set true will always try to search for an input handler
 * and get the input handlers histogram file. and set it.  If this is enabled then the only other field that
 * is required is to specify the target histogram.  If a target histogram is not specified then the output
 *
 * will be an equalized image without the target applied.
 *
 * </pre>
 */ 
class OSSIM_DLL ossimHistogramMatchFilter : public ossimImageSourceFilter
{
public:
   ossimHistogramMatchFilter();

   /**
    * Calls the owned equalizers to create a match composite image.
    *
    * @return Histogram matched composite image
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   /**
    * Initializes the input and target equalizer filters.
    */ 
   virtual void initialize();

   /**
    * @param inputHistogram Input histogram to load
    */ 
   void setInputHistogram(const ossimFilename& inputHistogram);

   /**
    * @param targetHistogram Target histogram to match
    */ 
   void setTargetHistogram(const ossimFilename& targetHistogram);

   /**
    * @param event Connection event
    */ 
   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   /**
    * @param event Connection event
    */ 
   virtual void disconnectInputEvent(ossimConnectionEvent& event);

   /**
    * <pre>
    *
    * Valid properties are:
    *
    * auto_load_input_histogram_flag
    *                   Is a boolean property and specifes whether or not the input histogram will be auto searched from
    *                   the conected input source chain.
    *
    * input_histogram_filename
    *                   Holds the input histogram filename
    *
    * output_histogram_filename
    *                   holds the output histogram filename
    *                   
    * </pre>
    * @param property Is the property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * Returns a property given the name
    *
    * Valid names are:
    * auto_load_input_histogram_flag
    * input_histogram_filename
    * output_histogram_filename
    *
    * Example:
    *
    * ossimRefPtr<ossimProperty> prop = obj->getProperty("auto_load_input_histogram_flag");
    *
    */ 
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * Returns a list of proeprties.
    *
    * <pre>
    *
    * Valid properties are:
    *
    * auto_load_input_histogram_flag
    *                   Is a boolean property and specifes whether or not the input histogram will be auto searched from
    *                   the conected input source chain.
    *
    * input_histogram_filename
    *                   Holds the input histogram filename
    *
    * output_histogram_filename
    *                   holds the output histogram filename
    *                  
    * </pre>
    */ 
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    *
    * <pre>
    * Keywords to search and load:
    *
    * auto_load_input_histogram_flag
    * input_histogram_filename
    * output_histogram_filename
    *
    * </pre>
    * 
    * @param kwl  Holds the keywords for this object to load prefixed by the passed in prefix parameter
    * @param prefix Is the prefix value prepended to all keywords
    * 
    * @return True if succesful or flase otherwise
    * 
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   /**
    *
    * <pre>
    * Keywords:
    *
    * auto_load_input_histogram_flag
    * input_histogram_filename
    * output_histogram_filename
    *
    * </pre>
    * 
    * @param kwl  This object will store the keywords in kwl and prefix by the parameter prefix
    * @param prefix Is the prefix value prepended to all keywords
    * @return True if succesful or flase otherwise
    * 
    */
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;
   
protected:
   /**
    * Will search the input chain and get the histogram_filename property of the image handler.
    * Once retrieved it calls the setInputHistogram with the found histogram filename.  It does nothing
    * if the histogram filename does not exist
    * 
    */
   void autoLoadInputHistogram();
   
   ossimRefPtr<ossimHistogramEqualization> theInputHistogramEqualizer;
   ossimRefPtr<ossimHistogramEqualization> theTargetHistogramEqualizer;
   ossimFilename theInputHistogramFilename;
   ossimFilename theTargetHistogramFilename;
   bool          theAutoLoadInputHistogramFlag;
TYPE_DATA   
};
#endif
