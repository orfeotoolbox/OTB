//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimGeoPolyCutter.h 22303 2013-07-04 18:15:52Z dburken $
#ifndef ossimGeoPolyCutter_HEADER
#define ossimGeoPolyCutter_HEADER 1
#include <ossim/imaging/ossimPolyCutter.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <vector>

class ossimImageData;
class ossimProjection;

/*!
 * ossimGeoPolyCutter.
 *
 * This classes overrides the 2-d counter part ossimPolyCutter. It will automatically
 * map 2-D to 3-D (ground) polygons if you add 2-D polygons and will automatically
 * compute the forward transform when adding a ground polygon.
 *
 * you can set the view trough the ossimViewInterface by calling setView(object, true);
 * it will cast to an ossimProjection and use that as the transformation.  For igen this is
 * set automatically and does not need to be set by the user.
 *
 * This class has no keywords but does use 2 prefix values to store out the view
 * and the ground polygon for example if we have only one polygon then here is a sample
 * keyword list for the polygons: NOTE we can have any number of polygons just increment
 * the geo_polygon0 to geo_polygon1 .. geo_polygonN for the Nth one
 *
 * <pre>
 *
 * geo_polygon0.datum:  WGE
 * geo_polygon0.number_vertices:  18
 * geo_polygon0.type:  ossimGeoPolygon
 * geo_polygon0.v0:  38.023634368488452 -122.542245818545709 nan
 * geo_polygon0.v1:  38.016619667868909 -122.548990722987568 nan
 * geo_polygon0.v2:  38.018238444934966 -122.546292761210807 nan
 * geo_polygon0.v3:  38.012932453440698 -122.547911538276864 nan
 * geo_polygon0.v4:  38.014731094625191 -122.542065954427244 nan
 * geo_polygon0.v5:  38.007446597827986 -122.541796158249582 nan
 * geo_polygon0.v6:  38.009694899308606 -122.537569351466018 nan
 * geo_polygon0.v7:  38.005108364288141 -122.537119691169892 nan
 * geo_polygon0.v8:  38.008255986361007 -122.532353292030976 nan
 * geo_polygon0.v9:  38.003039926925965 -122.531274107320272 nan
 * geo_polygon0.v10:  38.013292181677599 -122.529565398195004 nan
 * geo_polygon0.v11:  38.009874763427050 -122.526867436418243 nan
 * geo_polygon0.v12:  38.019587425823332 -122.528306349365849 nan
 * geo_polygon0.v13:  38.023904164666128 -122.537299555288328 nan
 * geo_polygon0.v14:  38.024983349376825 -122.544584052085540 nan
 * geo_polygon0.v15:  38.023904164666128 -122.548720926809878 nan
 * geo_polygon0.v16:  38.022195455540853 -122.562210735693611 nan
 * geo_polygon0.v17:  38.019317629645663 -122.562930192167400 nan
 *
 *
 * the example for the view information is
 *
 * view.central_meridian:  -123.000000000000000
 * view.datum:  WGE
 * view.decimal_degrees_per_pixel_lat:  0.000089932059225
 * view.decimal_degrees_per_pixel_lon:  0.000089932059225
 * view.ellipse_code:  WE
 * view.ellipse_name:  WGS 84
 * view.false_easting:  0.000000000000000
 * view.false_northing:  0.000000000000000
 * view.major_axis:  6378137.000000000000000
 * view.meters_per_pixel_x:  10.000000000000000
 * view.meters_per_pixel_y:  10.000000000000000
 * view.minor_axis:  6356752.314199999906123
 * view.origin_latitude:  0.000000000000000
 * view.rotation:  0.000000000000000
 * view.scale_x:  1.000000000000000
 * view.scale_y:  1.000000000000000
 * view.tie_point_easting:  0.000000000000000
 * view.tie_point_lat:  0.000000000000000
 * view.tie_point_lon:  -123.000000000000000
 * view.tie_point_northing:  0.000000000000000
 * view.translation_x:  0.000000000000000
 * view.translation_y:  0.000000000000000
 * view.type:  ossimEquDistCylProjection
 *
 * </pre> 
 */
class OSSIM_DLL ossimGeoPolyCutter : public ossimPolyCutter,
                                     public ossimViewInterface
{
public:
   ossimGeoPolyCutter();

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * Sets the polygon to the specified polygon index.
    * /param polygon A vector of points in projected 2-D.
    * /param i index of the polygon to change.
    */
   virtual void setPolygon(const vector<ossimDpt>& polygon,
                           ossim_uint32 i = 0);
   
   /*!
    * Sets the polygon to the specified polygon index.
    * /param polygon A vector of points in projected 2-D.
    * /param i index of the polygon to change.
    */
   virtual void setPolygon(const vector<ossimIpt>& polygon,
                           ossim_uint32 i = 0);

   /*!
    * Sets the polygon to the specified polygon index.
    * /param polygon A Ground polygon.
    * /param i index of the polygon to change.
    */
    virtual void setPolygon(const vector<ossimGpt>& polygon,
                            ossim_uint32 i = 0);

   virtual void setPolygon(const ossimGeoPolygon& polygon,
                           ossim_uint32 i = 0);
   /*!
    * Will invert this polygon out to world space and add it to the
    * polygon list
    */
   virtual void addPolygon(const vector<ossimIpt>& polygon);

   /*!
    * Will invert the passed in polygon to world space and add to the
    * polygon list
    */
   virtual void addPolygon(const vector<ossimDpt>& polygon);

   /*!
    * Will add the ground polygon to the list
    */
   virtual void addPolygon(const vector<ossimGpt>& polygon);

   /*!
    * Will invert te polygon to ground and add.
    */
   virtual void addPolygon(const ossimPolygon& polygon);

   /*!
    * Will add the passed in polygon.
    */
   virtual void addPolygon(const ossimGeoPolygon& polygon);

   /*!
    * Will set the number of polygons and will call the base
    * class set number of polygons.
    */
   virtual void setNumberOfPolygons(ossim_uint32 count);

   /*!
    * This should only be used for informational uses only.  If updates
    * are to be made please go through the access methods that change
    * the polygon data.
    */
   virtual std::vector<ossimGeoPolygon>& getGeoPolygonList();

   virtual const std::vector<ossimGeoPolygon>& getGeoPolygonList()const;


   /*!
    * Will set the view to the passed in object.  Once set it will project the
    * ground polygon relative to the passed in view.
    */
   virtual bool setView(ossimObject* baseObject);

   /*!
    * \return The current view for this object.
    */
   virtual ossimObject* getView();
   
   /*!
    * \return The constant pointer to the current view for this object.
    */
   virtual const ossimObject* getView()const;
   
protected:
   virtual ~ossimGeoPolyCutter();

   /*!
    * Utility method used to invert any passed in projected polygons to
    * be changed or added to the list.
    * \param polygonNumber the index of the polygon to invert to ground.
    */ 
   void invertPolygon(int polygonNumber);

   /*!
    * Transforms all vertices based on the current m_viewProjection.
    */
   void transformVertices();

   /*!
    * Transforms the specified polygon index.
    * \param i The polygon index to project.
    */
   void transformVertices(int i);

   /*!
    * The view projection used to project the ground coordinates.
    */
   ossimRefPtr<ossimImageGeometry> m_viewGeometry;

   std::vector<ossimGeoPolygon> m_geoPolygonList;

   
TYPE_DATA  
};

#endif /* #ifndef ossimGeoPolyCutter_HEADER */
