//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

// Framework include files
#include <DD4hep/OpticalSurfaces.h>
#include <DD4hep/NamedObject.h>
#include <DD4hep/Detector.h>
#include <DD4hep/Printout.h>
#include <DD4hep/World.h>

#include <DD4hep/detail/Handle.inl>

// C/C++ includes

using namespace dd4hep;

DD4HEP_INSTANTIATE_HANDLE(TGeoSkinSurface);
DD4HEP_INSTANTIATE_HANDLE(TGeoBorderSurface);
DD4HEP_INSTANTIATE_HANDLE(TGeoOpticalSurface);

/// Initializing constructor.
OpticalSurface::OpticalSurface(Detector& detector,
                               const std::string& full_name,
                               EModel  model,
                               EFinish finish,
                               EType   type,
                               double  value)
{
  std::unique_ptr<Object> obj(new Object(full_name.c_str(), model, finish, type, value));
  detector.manager().AddOpticalSurface(m_element=obj.release());
}

/// Access to tabular properties of the surface
OpticalSurface::Property OpticalSurface::property(const char* nam)  const   {
  return access()->GetProperty(nam);
}

/// Access to tabular properties of the surface
OpticalSurface::Property OpticalSurface::property(const std::string& nam)  const   {
  return access()->GetProperty(nam.c_str());
}

/// Initializing constructor: Creates the object and registers it to the manager
SkinSurface::SkinSurface(Detector& detector, DetElement de, const std::string& nam, OpticalSurface surf, Volume vol)
{
  if ( de.isValid() )  {
    if ( vol.isValid() )  {
      if ( surf.isValid() )  {
        std::unique_ptr<Object> obj(new Object(nam.c_str(), surf->GetName(), surf.ptr(), vol.ptr()));
        detector.surfaceManager().addSkinSurface(de, m_element=obj.release());
        return;
      }
      except("SkinSurface","++ Cannot create SkinSurface %s without valid optical surface!",nam.c_str());
    }
    except("SkinSurface","++ Cannot create SkinSurface %s without valid volume!",nam.c_str());
  }
  except("SkinSurface",
         "++ Cannot create SkinSurface %s which is not connected to a valid detector element!",nam.c_str());
}

/// Access surface data
OpticalSurface SkinSurface::surface()  const    {
  return (TGeoOpticalSurface*)(access()->GetSurface());
}

/// Access to tabular properties of the optical surface
BorderSurface::Property SkinSurface::property(const char* nam)  const    {
  OpticalSurface surf(surface());
  return surf.property(nam);
}

/// Access to tabular properties of the optical surface
BorderSurface::Property SkinSurface::property(const std::string& nam)  const   {
  OpticalSurface surf(surface());
  return surf.property(nam.c_str());
}

/// Access the node of the skin surface
Volume   SkinSurface::volume()   const    {
  return access()->GetVolume();
}

/// Initializing constructor: Creates the object and registers it to the manager
BorderSurface::BorderSurface(Detector&      detector,
                             DetElement     de,
                             const std::string&  nam,
                             OpticalSurface surf,
                             PlacedVolume   lft,
                             PlacedVolume   rht)
{
  if ( de.isValid() )  {
    if ( lft.isValid() && rht.isValid() )  {
      if ( surf.isValid() )   {
        std::unique_ptr<Object> obj(new Object(nam.c_str(), surf->GetName(), surf.ptr(), lft.ptr(), rht.ptr()));
        detector.surfaceManager().addBorderSurface(de, m_element=obj.release());
        return;
      }
      except("BorderSurface","++ Cannot create BorderSurface %s without valid optical surface!",nam.c_str());
    }
    except("BorderSurface","++ Cannot create BorderSurface %s without valid placements!",nam.c_str());
  }
  except("BorderSurface",
         "++ Cannot create BorderSurface %s which is not connected to a valid detector element!",nam.c_str());
}

/// Access surface data
OpticalSurface BorderSurface::surface()  const    {
  return (TGeoOpticalSurface*)(access()->GetSurface());
}

/// Access to tabular properties of the optical surface
BorderSurface::Property BorderSurface::property(const char* nam)  const    {
  OpticalSurface surf(surface());
  return surf.property(nam);
}

/// Access to tabular properties of the optical surface
BorderSurface::Property BorderSurface::property(const std::string& nam)  const   {
  OpticalSurface surf(surface());
  return surf.property(nam.c_str());
}

/// Access the left node of the border surface
PlacedVolume   BorderSurface::left()   const    {
  return (TGeoNode*)access()->GetNode1();
}

/// Access the right node of the border surface
PlacedVolume   BorderSurface::right()  const    {
  return access()->GetNode2();
}

