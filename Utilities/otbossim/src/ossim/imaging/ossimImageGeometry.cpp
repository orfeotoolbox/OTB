//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description: Class implementation of ossimImageGeometry. See .h file for class documentation.
//
//**************************************************************************************************
// $Id$

#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossim2dTo2dTransformRegistry.h>
#include <ossim/elevation/ossimElevManager.h>

RTTI_DEF1(ossimImageGeometry, "ossimImageGeometry", ossimObject);

//**************************************************************************************************
// Default constructor defaults to unity transform with no projection  
//**************************************************************************************************
ossimImageGeometry::ossimImageGeometry()
:   
m_transform (0),
m_projection(0)
{
   m_gsd.makeNan();
   m_imageSize.makeNan();
}

//**************************************************************************************************
// Copy constructor -- performs a deep copy. This is needed when an imageSource in the chain
// needs to modify the image geometry at that point in the chain. The image geometry to the left
// of that imageSource must remain unchanged. This constructor permits that imageSource to 
// maintain its own geometry based on the input geometry. All objects in the chain and to the right
// would see this alternate geometry. See ossimScaleFilter for an example of this case.
//**************************************************************************************************
ossimImageGeometry::ossimImageGeometry(const ossimImageGeometry& copy_this)
:ossimObject(copy_this)   
{
   *this = copy_this;
}

//**************************************************************************************************
//! Constructs with projection and transform objects available for referencing. Either pointer
//! can be NULL -- the associated mapping would be identity.
//**************************************************************************************************
ossimImageGeometry::ossimImageGeometry(ossim2dTo2dTransform* transform, ossimProjection* proj)
:  ossimObject(),
   m_transform(transform),
   m_projection(proj)
{
   m_imageSize.makeNan();
   m_gsd.makeNan();
   if(m_projection.valid())
   {
      // If the transform is NULL (identity), then we can simply copy the input projection's GSD.
      // Otherwise we'll need to compute a GSD specific to this geometry.
      m_gsd = m_projection->getMetersPerPixel();
   }
}

//**************************************************************************************************
// Destructor is hidden. Only accessible via ossimRefPtr centrally managed
//**************************************************************************************************
ossimImageGeometry::~ossimImageGeometry()
{
   // Nothing to do
}

void ossimImageGeometry::rnToRn(const ossimDpt& inRnPt, ossim_uint32 inResolutionLevel,
                                ossim_uint32 outResolutionLevel, ossimDpt& outRnPt) const
{
   if (inResolutionLevel != outResolutionLevel)
   {
      // Back out the decimation.
      ossimDpt pt;
      undecimatePoint(inRnPt, inResolutionLevel, pt);

      // Decimate to new level.
      decimatePoint(pt, outResolutionLevel, outRnPt);
   }
   else
   {
      outRnPt = inRnPt; // No transform required.
   }
}

void ossimImageGeometry::rnToFull(const ossimDpt& rnPt,
                                  ossim_uint32 resolutionLevel,
                                  ossimDpt& fullPt) const
{
   // Back out the decimation.
   ossimDpt localPt;
   undecimatePoint(rnPt, resolutionLevel, localPt);

   // Remove any shift/rotation.
   if ( m_transform.valid() && !localPt.hasNans() )
   {
      m_transform->forward(localPt, fullPt);
   }
   else
   {
      fullPt = localPt; // No transform (shift/rotation)
   }
}

void ossimImageGeometry::fullToRn(const ossimDpt& fullPt,
                                  ossim_uint32 resolutionLevel,
                                  ossimDpt& rnPt) const
{
   // Apply shift/rotation.
   ossimDpt localPt;
   if (m_transform.valid())
   {
      m_transform->inverse(fullPt, localPt);
   }
   else
   {
      localPt = fullPt; // No transform (shift/rotation)
   }

   // Apply the decimation.
   decimatePoint(localPt, resolutionLevel, rnPt);
}

void ossimImageGeometry::rnToWorld(const ossimDpt& rnPt,
                                   ossim_uint32 resolutionLevel,
                                   ossimGpt& wpt) const
{
   ossimDpt localPt;
   rnToRn(rnPt, resolutionLevel, 0, localPt);
   localToWorld(localPt, wpt);
}

void ossimImageGeometry::worldToRn(const ossimGpt& wpt,
                                   ossim_uint32 resolutionLevel,
                                   ossimDpt& rnPt) const
{   
   ossimDpt localPt;
   worldToLocal(wpt, localPt);
   rnToRn(localPt, 0, resolutionLevel, rnPt);
}

//**************************************************************************************************
//! Exposes the 3D projection from image to world coordinates. The caller should verify that
//! a valid projection exists before calling this method. Returns TRUE if a valid ground point
//! is available in the ground_pt argument. This method depends on the existence of elevation
//! information. If no DEM is available, the results will be incorrect or inaccurate.
//**************************************************************************************************
bool ossimImageGeometry::localToWorld(const ossimDpt& local_pt, ossimGpt& world_pt) const
{
    //! Return a NAN point of no projection is defined:
    if (!m_projection.valid())
    {
        world_pt.makeNan();
        return false;
    }

    // First transform local pixel to full-image pixel:
    ossimDpt full_image_pt;
    rnToFull(local_pt, 0, full_image_pt);

    // Perform projection to world coordinates:
    m_projection->lineSampleToWorld(full_image_pt, world_pt);
    return true;
}

//**************************************************************************************************
//! Exposes the 3D world-to-local image coordinate reverse projection. The caller should verify 
//! that a valid projection exists before calling this method. Returns TRUE if a valid image 
//! point is available in the local_pt argument.
//**************************************************************************************************
bool ossimImageGeometry::worldToLocal(const ossimGpt& world_pt, ossimDpt& local_pt) const
{
   //! Return a NAN point of no projection is defined:
   if (!m_projection.valid())
   {
      local_pt.makeNan();
      return false;
   }
   
   ossimGpt copyPt(world_pt);
   if(isAffectedByElevation())
   {
      if(world_pt.isHgtNan())
      {
         copyPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(copyPt);
      }
   }
   
   // First Perform projection from world coordinates to full-image space:
   ossimDpt full_image_pt;
   m_projection->worldToLineSample(copyPt, full_image_pt);

   // Then transform to local space:
   fullToRn(full_image_pt, 0, local_pt);
   
   return true;
}

//**************************************************************************************************
//! Sets the transform to be used for local-to-full-image coordinate transformation
//**************************************************************************************************
void ossimImageGeometry::setTransform(ossim2dTo2dTransform* transform) 
{ 
   m_transform = transform; 
   if (m_projection.valid())
   {
      m_gsd.makeNan();
   }
}

//**************************************************************************************************
//! Sets the projection to be used for local-to-world coordinate transformation
//**************************************************************************************************
void ossimImageGeometry::setProjection(ossimProjection* projection) 
{ 
   m_projection = projection; 
   if (m_projection.valid())
   {
      m_gsd.makeNan();
   }
}

//**************************************************************************************************
//! Returns TRUE if this geometry is sensitive to elevation
//**************************************************************************************************
bool ossimImageGeometry::isAffectedByElevation() const
{
    if (m_projection.valid())
        return m_projection->isAffectedByElevation();
    return false;
}

const ossimDpt& ossimImageGeometry::getMetersPerPixel() const
{
   if(m_gsd.hasNans())
   {
      if(m_projection.valid())
      {
         computeGsd();
      }
   }
   
   return m_gsd;
}

//**************************************************************************************************
//! When either the projection or the transform changes, this method recomputes the GSD.
//**************************************************************************************************
void ossimImageGeometry::computeGsd()const
{
   //---
   // We need to do the local-to-world but eliminating elevation effects,
   // hence the complication:
   //
   // Note that by doing the rnToFull the tranform and decimation are account for.
   //---

   if (m_projection.valid())
   {
      // Get three points in full image space.
      ossimDpt pL0(0,0);
      ossimDpt pLx(1,0);
      ossimDpt pLy(0,1);
      ossimDpt pF0;
      ossimDpt pFx;
      ossimDpt pFy;
      rnToFull(pL0, 0, pF0);
      rnToFull(pLx, 0, pFx);
      rnToFull(pLy, 0, pFy);
      
      ossimGpt g0, gx, gy;
      
      m_projection->lineSampleToWorld(pF0, g0);
      m_projection->lineSampleHeightToWorld(pFx, g0.height(), gx);
      m_projection->lineSampleHeightToWorld(pFy, g0.height(), gy);
      
      // Compute horizontal distance for one pixel:
      m_gsd.x = g0.distanceTo(gx);
      m_gsd.y = g0.distanceTo(gy);
   }
   else
   {
      m_gsd.makeNan();
   }
}

//**************************************************************************************************
//! Prints contents to output stream.
//**************************************************************************************************
std::ostream& ossimImageGeometry::print(std::ostream& out) const
{
   out << "type: ossimImageGeometry" << std::endl;
    if(m_transform.valid())
    {
        out << "  m_transform: ";
        m_transform->print(out);
    }
    else
    {
        out << "  No transform defined. Using identity transform.\n";
    }

    if(m_projection.valid())
    {
        out << "  m_projection: ";
        m_projection->print(out);
    }
    else
    {
        out << "  No projection defined. ";
    }
    return out;
}

//**************************************************************************************************
//! Returns TRUE if argument geometry has exactly the same instances for transform and 
//! m_projection. This may need to be expanded to permit different instances of same transforms
//**************************************************************************************************
bool ossimImageGeometry::operator==(const ossimImageGeometry& other) const
{
    return ((m_transform == other.m_transform) && (m_projection == other.m_projection) &&
            (decimationFactor(0) == other.decimationFactor(0)) );
}

//**************************************************************************************************
//! Returns the decimation factor from R0 for the resolution level specified. For r_index=0, the 
//! decimation factor is by definition 1.0. For the non-discrete case, r_index=1 returns a 
//! decimation of 0.5. If the vector of discrete decimation factors (m_decimationFactors) is 
//! empty, the factor will be computed as f=1/2^n
//**************************************************************************************************
ossimDpt ossimImageGeometry::decimationFactor(ossim_uint32 r_index) const
{
   ossim_uint32 size = (ossim_uint32)m_decimationFactors.size();
   if (size)
   {
      if (r_index < size)
      {
         return m_decimationFactors[r_index];
      }
      
      // Return the last defined decimation if the index requested exceeds list size:
      return m_decimationFactors[size-1];
   }

   // Compute the decimation factor:
   ossim_float64 factor = 1.0/(ossim_float64)(1 << r_index);
   
   return ossimDpt(factor, factor);
}

void ossimImageGeometry::decimationFactor(ossim_uint32 r_index,
                                          ossimDpt& result) const
{
   const ossim_uint32 SIZE = (ossim_uint32)m_decimationFactors.size();
   if (SIZE)
   {
      if (r_index < SIZE)
      {
         result = m_decimationFactors[r_index];
      }
      else
      {
         //---
         // Return the last defined decimation if the index requested
         // exceeds list size:
         //---
         result = m_decimationFactors[SIZE-1];
      }
   }
   else
   {
      // Compute the decimation factor:
      ossim_float64 factor = 1.0/(ossim_float64)(1 << r_index);
      result.x = factor;
      result.y = factor;
   }
}

void ossimImageGeometry::decimationFactors(
   std::vector<ossimDpt>& decimations) const
{
   decimations = m_decimationFactors;
}

//*****************************************************************************
//! Attempts to initialize a transform and a projection given the KWL.
//! Returns TRUE if no error encountered.
//*****************************************************************************
bool ossimImageGeometry::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   const char* lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      ossimString decimations   = kwl.find(prefix, "decimations");
      ossimString gsd           = kwl.find(prefix, "gsd");
      ossimString imageSize     = kwl.find(prefix, "image_size");
      
      if(!imageSize.empty())
      {
         m_imageSize.toPoint(imageSize);
      }
      if ( ossimString(lookup) == STATIC_TYPE_NAME(ossimImageGeometry) )
      {
         ossimObject::loadState(kwl, prefix);

         // First try to create a transform
         ossim2dTo2dTransform* transform = 0;
         ossimString transformPrefix = ossimString(prefix) + "transform.";
         ossimString projectionPrefix = ossimString(prefix) + "projection.";
         transform = ossim2dTo2dTransformRegistry::instance()->
            createNativeObjectFromRegistry(kwl, transformPrefix.c_str());
         
         // Now look for projection spec:
         ossimProjection* projection =
            ossimProjectionFactoryRegistry::instance()->
               createProjection(kwl, projectionPrefix.c_str());
         if(projection)
         {
            m_projection = projection;
         }
         if(transform)
         {
            m_transform = transform;
         }
      }
      else
      {
         // Now look for projection spec (for backwards compatibility):
         ossimProjection* projection = 
            ossimProjectionFactoryRegistry::instance()->
            createProjection(kwl, prefix);
         if (projection)
         {
            setProjection(projection);
         }
      }
      
      if(!decimations.empty())
      {
         m_decimationFactors.clear();
         ossim::toVector(m_decimationFactors, decimations);
      }
      if(!gsd.empty())
      {
         m_gsd.toPoint(gsd);
      }
   }
   else
   {
      //---
      // Old geometry file with no type keyword:
      //---
      ossimProjection* projection = 
         ossimProjectionFactoryRegistry::instance()->
         createProjection(kwl, prefix);
      if (projection)
      {
         setProjection(projection);
      } 
   }
   
   return true;
}

//**************************************************************************************************
//! Saves the transform (if any) and projection (if any) states to the KWL.
//**************************************************************************************************
bool ossimImageGeometry::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   bool good_save = true;
   ossimString transformPrefix = ossimString(prefix) + "transform.";
   ossimString projectionPrefix = ossimString(prefix) + "projection.";
   // Save transform if present:
   if (m_transform.valid())
   {
      good_save = m_transform->saveState(kwl, transformPrefix.c_str());
   }

   // Save projection if present:
   if (m_projection.valid())
   {
      good_save &= m_projection->saveState(kwl, projectionPrefix.c_str());
   }
   good_save &= ossimObject::saveState(kwl, prefix);

   ossimString deimations;
   if(m_decimationFactors.size())
   {
      ossimString resultPoints;
      ossim::toStringList(resultPoints,
                          m_decimationFactors);
      
      if(!resultPoints.empty())
      {
         kwl.add(prefix, "decimations", resultPoints, true);
      }
   }
   
   ossimDpt mpp = getMetersPerPixel();
   if (!mpp.hasNans())
      kwl.add(prefix, "gsd", mpp.toString(), true);
   
   if (!m_imageSize.hasNans())
      kwl.add(prefix, "image_size", m_imageSize.toString(), true);
   
   return good_save;
}

//**************************************************************************************************
//! Replaces any existing transform and projection with those in the copy_this object
//**************************************************************************************************
const ossimImageGeometry& ossimImageGeometry::operator=(const ossimImageGeometry& copy_this)
{
   if (this != &copy_this)
   {
      // Get deep copy of 2D transform if one exists:
      if (copy_this.m_transform.valid())
      {
         m_transform = (ossim2dTo2dTransform*) copy_this.m_transform->dup();
      }
      
      // Now establish a deep copy of the projection, if any:
      if (copy_this.m_projection.valid())
      {
         m_projection = (ossimProjection*) copy_this.m_projection->dup();
      }
       // the Gsd should already be solved from the source we are copying from
      m_gsd = copy_this.m_gsd;
   }
   return *this;
}

void ossimImageGeometry::undecimatePoint(const ossimDpt& rnPt,
                                         ossim_uint32 resolutionLevel,
                                         ossimDpt& outPt) const
{
   // Back out the decimation.
   ossimDpt decimation = decimationFactor(resolutionLevel);
   
   //---
   // If no nans and one or both of the factors is not 1.0 decimation should
   // be applied.
   //---
   if ( (decimation.x != 1.0) || (decimation.y != 1.0) ) 
   {
      if ( !decimation.hasNans() && !rnPt.hasNans() )
      {
         outPt.x = rnPt.x * (1.0/decimation.x);
         outPt.y = rnPt.y * (1.0/decimation.y);
      }
      else
      {
         outPt.makeNan();
      }
   }
   else
   {
      outPt = rnPt; // No decimation.
   }
}

void ossimImageGeometry::decimatePoint(const ossimDpt& inPt,
                                       ossim_uint32 resolutionLevel,
                                       ossimDpt& rnPt) const
{
   
   // Apply the decimation.
   ossimDpt decimation = decimationFactor(resolutionLevel);

   //---
   // If no nans and one or both of the factors is not 1.0 decimation should
   // be applied.
   //---
   if ( (decimation.x != 1.0) || (decimation.y != 1.0) ) 
   {
      if ( !decimation.hasNans() && !inPt.hasNans() )
      {
         rnPt.x = inPt.x * decimation.x;
         rnPt.y = inPt.y * decimation.y;
      }
      else
      {
        rnPt.makeNan(); 
      }
   }
   else
   {
      rnPt = inPt; // No decimation.
   }
}
