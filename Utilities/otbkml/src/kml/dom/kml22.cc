// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the KML 2.2 element names as found in the KML 2.2 XSD.

#include "kml/dom/kml22.h"
#include "kml/dom/xsd.h"

namespace kmldom {

// There is an entry here for each global <xs:element> in the KML 2.2 XSD
static XsdElement kKml22Elements[] = {
  { "<Unknown>", XSD_UNKNOWN },  // Reserve slot 0, note impossible tag name.

  // These are the abstract substitution group elements in the XSD.
  // The name matches that of the OGC KML 2.2 (Candidate) Standard
  // The "casual" name in the comment is the corresponding name
  // used in the code.google.com/apis/kml KML 2.2 Reference.
  { "AbstractColorStyleGroup", XSD_COMPLEX_TYPE },  // "ColorStyle"
  { "Container", XSD_COMPLEX_TYPE },  // "Container"
  { "Feature", XSD_COMPLEX_TYPE },  // "Feature"
  { "Geometry", XSD_COMPLEX_TYPE },   // "Geometry"
  { "AbstractLatLonBox", XSD_COMPLEX_TYPE },  // (Not in Kml ref).
  { "AbstractObjectGroup", XSD_COMPLEX_TYPE },  // "Object"
  { "AbstractOverlayGroup", XSD_COMPLEX_TYPE },  // "Overlay"
  { "StyleSelector", XSD_COMPLEX_TYPE },  // "StyleSelector"
  { "AbstractSubStyleGroup", XSD_COMPLEX_TYPE },  // "SubStyle"
  { "TimePrimitive", XSD_COMPLEX_TYPE },  // "TimePrimitive"
  { "AbstractView", XSD_COMPLEX_TYPE },  // "AbstractView"
  { "BasicLink", XSD_COMPLEX_TYPE },  // (Not in KML ref).
  { "vec2Type", XSD_COMPLEX_TYPE },  // "vec2"

  // Most elements starting with a big letter are complex.
  { "Alias", XSD_COMPLEX_TYPE },
  { "BalloonStyle", XSD_COMPLEX_TYPE },
  { "Camera", XSD_COMPLEX_TYPE },
  { "Change", XSD_COMPLEX_TYPE },
  { "Create", XSD_COMPLEX_TYPE },
  { "Data", XSD_COMPLEX_TYPE },
  { "Delete", XSD_COMPLEX_TYPE },
  { "Document", XSD_COMPLEX_TYPE },
  { "ExtendedData", XSD_COMPLEX_TYPE },
  { "Folder", XSD_COMPLEX_TYPE },
  { "GroundOverlay", XSD_COMPLEX_TYPE },
  { "Icon", XSD_COMPLEX_TYPE },  // <Icon> in Overlay
  { "IconStyle", XSD_COMPLEX_TYPE },
  { "IconStyleIcon", XSD_COMPLEX_TYPE },  // <Icon> in <IconStyle>
  { "ImagePyramid", XSD_COMPLEX_TYPE },
  { "ItemIcon", XSD_COMPLEX_TYPE },
  { "LabelStyle", XSD_COMPLEX_TYPE },
  { "LatLonAltBox", XSD_COMPLEX_TYPE },
  { "LatLonBox", XSD_COMPLEX_TYPE },
  { "LineString", XSD_COMPLEX_TYPE },
  { "LineStyle", XSD_COMPLEX_TYPE },
  { "LinearRing", XSD_COMPLEX_TYPE },
  { "Link", XSD_COMPLEX_TYPE },
  { "ListStyle", XSD_COMPLEX_TYPE },
  { "Location", XSD_COMPLEX_TYPE },
  { "Lod", XSD_COMPLEX_TYPE },
  { "LookAt", XSD_COMPLEX_TYPE },
  { "Metadata", XSD_COMPLEX_TYPE },
  { "Model", XSD_COMPLEX_TYPE },
  { "MultiGeometry", XSD_COMPLEX_TYPE },
  { "NetworkLink", XSD_COMPLEX_TYPE },
  { "NetworkLinkControl", XSD_COMPLEX_TYPE },
  { "Orientation", XSD_COMPLEX_TYPE },
  { "Pair", XSD_COMPLEX_TYPE },
  { "PhotoOverlay", XSD_COMPLEX_TYPE },
  { "Placemark", XSD_COMPLEX_TYPE },
  { "Point", XSD_COMPLEX_TYPE },
  { "PolyStyle", XSD_COMPLEX_TYPE },
  { "Polygon", XSD_COMPLEX_TYPE },
  { "Region", XSD_COMPLEX_TYPE },
  { "ResourceMap", XSD_COMPLEX_TYPE },
  { "Scale", XSD_COMPLEX_TYPE },
  { "Schema", XSD_COMPLEX_TYPE },
  { "SchemaData", XSD_COMPLEX_TYPE },
  { "ScreenOverlay", XSD_COMPLEX_TYPE },
  { "SimpleData", XSD_COMPLEX_TYPE },
  { "SimpleField", XSD_COMPLEX_TYPE },
  { "Snippet", XSD_COMPLEX_TYPE },
  { "Style", XSD_COMPLEX_TYPE },
  { "StyleMap", XSD_COMPLEX_TYPE },
  { "TimeSpan", XSD_COMPLEX_TYPE },
  { "TimeStamp", XSD_COMPLEX_TYPE },
  { "Update", XSD_COMPLEX_TYPE },
  { "Url", XSD_COMPLEX_TYPE },
  { "ViewVolume", XSD_COMPLEX_TYPE },

  // Most elements (but not all) starting with a little letter are simple.
  { "address", XSD_SIMPLE_TYPE },
  { "altitude", XSD_SIMPLE_TYPE },
  { "altitudeMode", XSD_SIMPLE_TYPE },
  { "altitudeModeGroup", XSD_SIMPLE_TYPE },
  { "begin", XSD_SIMPLE_TYPE },
  { "bgColor", XSD_SIMPLE_TYPE },
  { "bottomFov", XSD_SIMPLE_TYPE },
  { "color", XSD_SIMPLE_TYPE },
  { "colorMode", XSD_SIMPLE_TYPE },
  { "cookie", XSD_SIMPLE_TYPE },
  { "coordinates", XSD_COMPLEX_TYPE },  // class Coordinates
  { "description", XSD_SIMPLE_TYPE },
  { "displayMode", XSD_SIMPLE_TYPE },
  { "displayName", XSD_SIMPLE_TYPE },
  { "drawOrder", XSD_SIMPLE_TYPE },
  { "east", XSD_SIMPLE_TYPE },
  { "end", XSD_SIMPLE_TYPE },
  { "expires", XSD_SIMPLE_TYPE },
  { "extrude", XSD_SIMPLE_TYPE },
  { "fill", XSD_SIMPLE_TYPE },
  { "flyToView", XSD_SIMPLE_TYPE },
  { "gridOrigin", XSD_SIMPLE_TYPE },
  { "heading", XSD_SIMPLE_TYPE },
  { "hotSpot", XSD_COMPLEX_TYPE },
  { "href", XSD_SIMPLE_TYPE },
  { "httpQuery", XSD_SIMPLE_TYPE },
  { "innerBoundaryIs", XSD_COMPLEX_TYPE },  // Implemented as class
  { "key", XSD_SIMPLE_TYPE },
  { "kml", XSD_COMPLEX_TYPE },  // <kml> is special
  { "latitude", XSD_SIMPLE_TYPE },
  { "leftFov", XSD_SIMPLE_TYPE },
  { "linkDescription", XSD_SIMPLE_TYPE },
  { "linkName", XSD_SIMPLE_TYPE },
  { "linkSnippet", XSD_COMPLEX_TYPE },  // Like Snippet this has maxLines
  { "listItemType", XSD_SIMPLE_TYPE },
  { "longitude", XSD_SIMPLE_TYPE },
  { "maxAltitude", XSD_SIMPLE_TYPE },
  { "maxFadeExtent", XSD_SIMPLE_TYPE },
  { "maxHeight", XSD_SIMPLE_TYPE },
  { "maxLength", XSD_SIMPLE_TYPE },
  { "maxLodPixels", XSD_SIMPLE_TYPE },
  { "maxSessionLength", XSD_SIMPLE_TYPE },
  { "maxSnippetLines", XSD_SIMPLE_TYPE },
  { "maxWidth", XSD_SIMPLE_TYPE },
  { "message", XSD_SIMPLE_TYPE },
  { "minAltitude", XSD_SIMPLE_TYPE },
  { "minFadeExtent", XSD_SIMPLE_TYPE },
  { "minLodPixels", XSD_SIMPLE_TYPE },
  { "minRefreshPeriod", XSD_SIMPLE_TYPE },
  { "name", XSD_SIMPLE_TYPE },
  { "near", XSD_SIMPLE_TYPE },
  { "north", XSD_SIMPLE_TYPE },
  { "open", XSD_SIMPLE_TYPE },
  { "outerBoundaryIs", XSD_COMPLEX_TYPE },  // Implemented as a class
  { "outline", XSD_SIMPLE_TYPE },
  { "overlayXY", XSD_COMPLEX_TYPE },
  { "phoneNumber", XSD_SIMPLE_TYPE },
  { "range", XSD_SIMPLE_TYPE },
  { "refreshInterval", XSD_SIMPLE_TYPE },
  { "refreshMode", XSD_SIMPLE_TYPE },
  { "refreshVisibility", XSD_SIMPLE_TYPE },
  { "rightFov", XSD_SIMPLE_TYPE },
  { "roll", XSD_SIMPLE_TYPE },
  { "rotation", XSD_SIMPLE_TYPE },
  { "rotationXY", XSD_COMPLEX_TYPE },
  { "scale", XSD_SIMPLE_TYPE },
  { "screenXY", XSD_COMPLEX_TYPE },
  { "shape", XSD_SIMPLE_TYPE },
  { "size", XSD_COMPLEX_TYPE },
  { "snippet", XSD_SIMPLE_TYPE },
  { "sourceHref", XSD_SIMPLE_TYPE },
  { "south", XSD_SIMPLE_TYPE },
  { "state", XSD_SIMPLE_TYPE },
  { "styleUrl", XSD_SIMPLE_TYPE },
  { "targetHref", XSD_SIMPLE_TYPE },
  { "tessellate", XSD_SIMPLE_TYPE },
  { "text", XSD_SIMPLE_TYPE },
  { "textColor", XSD_SIMPLE_TYPE },
  { "tileSize", XSD_SIMPLE_TYPE },
  { "tilt", XSD_SIMPLE_TYPE },
  { "topFov", XSD_SIMPLE_TYPE },
  { "units", XSD_SIMPLE_TYPE },  // For unitsEnum, not actually an element.
  { "value", XSD_SIMPLE_TYPE },
  { "viewBoundScale", XSD_SIMPLE_TYPE },
  { "viewFormat", XSD_SIMPLE_TYPE },
  { "viewRefreshMode", XSD_SIMPLE_TYPE },
  { "viewRefreshTime", XSD_SIMPLE_TYPE },
  { "visibility", XSD_SIMPLE_TYPE },
  { "west", XSD_SIMPLE_TYPE },
  { "when", XSD_SIMPLE_TYPE },
  { "width", XSD_SIMPLE_TYPE },
  { "x", XSD_SIMPLE_TYPE },
  { "y", XSD_SIMPLE_TYPE },
  { "z", XSD_SIMPLE_TYPE },

  // Atom complex elements
  { "atom:author", XSD_COMPLEX_TYPE },
  { "atom:category", XSD_COMPLEX_TYPE },
  { "atom:content", XSD_COMPLEX_TYPE },
  { "atom:entry", XSD_COMPLEX_TYPE },
  { "atom:feed", XSD_COMPLEX_TYPE },
  { "atom:link", XSD_COMPLEX_TYPE },

  // Atom simple elements
  { "atom:email", XSD_SIMPLE_TYPE },
  { "atom:id", XSD_SIMPLE_TYPE },
  { "atom:label", XSD_SIMPLE_TYPE },
  { "atom:name", XSD_SIMPLE_TYPE },
  { "atom:scheme", XSD_SIMPLE_TYPE },
  { "atom:summary", XSD_SIMPLE_TYPE },
  { "atom:term", XSD_SIMPLE_TYPE },
  { "atom:title", XSD_SIMPLE_TYPE },
  { "atom:updated", XSD_SIMPLE_TYPE },
  { "atom:uri", XSD_SIMPLE_TYPE },

  // xAL complex elements
  { "xal:AddressDetails", XSD_COMPLEX_TYPE },
  { "xal:AdministrativeArea", XSD_COMPLEX_TYPE },
  { "xal:Country", XSD_COMPLEX_TYPE },
  { "xal:Locality", XSD_COMPLEX_TYPE },
  { "xal:PostalCode", XSD_COMPLEX_TYPE },
  { "xal:SubAdministrativeArea", XSD_COMPLEX_TYPE },
  { "xal:Thoroughfare", XSD_COMPLEX_TYPE },

  // xAL simple elements
  { "xal:AdministrativeAreaName", XSD_SIMPLE_TYPE },
  { "xal:CountryNameCode", XSD_SIMPLE_TYPE },
  { "xal:LocalityName", XSD_SIMPLE_TYPE },
  { "xal:PostalCodeNumber", XSD_SIMPLE_TYPE },
  { "xal:SubAdministrativeAreaName", XSD_SIMPLE_TYPE },
  { "xal:ThoroughfareName", XSD_SIMPLE_TYPE },
  { "xal:ThoroughfareNumber", XSD_SIMPLE_TYPE },

  // Google extension abstract substitution group elements
  { "gx:AbstractTourPrimitiveGroup", XSD_COMPLEX_TYPE },  // "gx:TourPrimitive"

  // Google extension complex elements
  { "gx:AnimatedUpdate", XSD_COMPLEX_TYPE },
  { "gx:FlyTo", XSD_COMPLEX_TYPE },
  { "gx:LatLonQuad", XSD_COMPLEX_TYPE },
  { "gx:Playlist", XSD_COMPLEX_TYPE },
  { "gx:SoundCue", XSD_COMPLEX_TYPE },
  { "gx:TimeSpan", XSD_COMPLEX_TYPE },
  { "gx:TimeStamp", XSD_COMPLEX_TYPE },
  { "gx:Tour", XSD_COMPLEX_TYPE },
  { "gx:TourControl", XSD_COMPLEX_TYPE },
  { "gx:Wait", XSD_COMPLEX_TYPE },

  // Google extension simple elements
  { "gx:altitudeMode", XSD_SIMPLE_TYPE },
  { "gx:balloonVisibility", XSD_SIMPLE_TYPE },
  { "gx:duration", XSD_SIMPLE_TYPE },
  { "gx:flyToMode", XSD_SIMPLE_TYPE },
  { "gx:playMode", XSD_SIMPLE_TYPE },

  { "</Unknown>", XSD_UNKNOWN }  // Mark the end appropriately.
};

// There is an entry here for each enumeration in the XSD.
// See the declaration of xsd_simple_type_enum_t for more details.
static const char* kAltitudeModeEnums[] =
  { "clampToGround", "relativeToGround", "absolute", NULL };
static const char* kColorModeEnums[] = { "normal", "random", NULL };
static const char* kDisplayModeEnums[] = { "default", "hide", NULL };
static const char* kGridOriginEnums[] = { "lowerLeft", "upperLeft", NULL };
static const char* kItemIconStateEnums[] =
  { "open", "closed", "error", "fetching0", "fetching1", "fetching2", NULL };
static const char* kListItemTypeEnums[] =
  { "check", "radioFolder", "checkOffOnly", "checkHideChildren", NULL };
static const char* kRefreshModeEnums[] =
  { "onChange", "onInterval", "onExpire", NULL };
static const char* kShapeEnums[] = { "rectangle", "cylinder", "sphere", NULL };
static const char* kStyleStateEnums[] = { "normal", "highlight", NULL };
static const char* kUnitsEnums[] =
  { "fraction", "pixels", "insetPixels", NULL };
static const char* kViewRefreshModeEnums[] =
  { "never", "onRequest", "onStop", "onRegion", NULL };
static const char* kGxAltitudeModeEnums[] =
  { "clampToSeaFloor", "relativeToSeaFloor", NULL };
static const char* kGxFlyToViewEnums[] =
  { "bounce", "smooth", NULL };
static const char* kGxPlayModeEnums[] =
  { "pause", NULL };
static XsdSimpleTypeEnum kKml22Enums[] = {
  { Type_altitudeMode, kAltitudeModeEnums },
  { Type_colorMode, kColorModeEnums },
  { Type_displayMode, kDisplayModeEnums },
  { Type_gridOrigin, kGridOriginEnums },
  { Type_state, kItemIconStateEnums },
  { Type_listItemType, kListItemTypeEnums },
  { Type_refreshMode, kRefreshModeEnums },
  { Type_shape, kShapeEnums },
  { Type_key, kStyleStateEnums },
  { Type_units, kUnitsEnums },
  { Type_viewRefreshMode, kViewRefreshModeEnums },
  { Type_GxAltitudeMode, kGxAltitudeModeEnums },
  { Type_GxFlyToMode, kGxFlyToViewEnums },
  { Type_GxPlayMode, kGxPlayModeEnums }
};

}  // namespace kmldom
