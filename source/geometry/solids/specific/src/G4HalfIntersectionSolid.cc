#include "G4HalfIntersectionSolid.hh"
#include <algorithm>

#include "G4VGraphicsScene.hh"

G4HalfIntersectionSolid::G4HalfIntersectionSolid(const std::vector<const G4HalfSurface*>& intersectionSurfaces,
                                                 const std::vector<const G4HalfSurface*>& subtractionSurfaces,
                                                 const G4String& name)
                                                     :  G4VSolid(name),
                                                        intersectionSurfaces_(intersectionSurfaces),
                                                        subtractionSurfaces_(subtractionSurfaces)

{
}

void G4HalfIntersectionSolid::AddIntersectionSurface(const G4HalfSurface& s)
{
    intersectionSurfaces_.push_back(&s);
}

void G4HalfIntersectionSolid::AddSubtractionSurface(const G4HalfSurface& s)
{
    subtractionSurfaces_.push_back(&s);
}

G4double G4HalfIntersectionSolid::ImplicitFunction(const G4ThreeVector& p) const
{
    G4double implicitFunction = -kInfinity;

    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        implicitFunction = std::max(implicitFunction, s->ImplicitFunction(p));
    }

    for (const G4HalfSurface* s : subtractionSurfaces_)
    {
        implicitFunction = std::max(implicitFunction, -s->ImplicitFunction(p));
    }

    return implicitFunction;
}

G4double G4HalfIntersectionSolid::DistanceToOut(const G4ThreeVector& p) const
{
    if (this->Inside(p) != kInside) return 0;

    G4double distanceToOut = -kInfinity;

    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        distanceToOut = std::max(distanceToOut, s->DistanceToSurface(p));
    }
    for (const G4HalfSurface* s : subtractionSurfaces_)
    {
        distanceToOut = std::max(distanceToOut, -s->DistanceToSurface(p));
    }

    return distanceToOut;
}
G4double G4HalfIntersectionSolid::DistanceToOut(const G4ThreeVector& p, const G4HalfSurface*& s) const
{
    if (this->Inside(p) != kInside) return 0;

    G4double distanceToOut = -kInfinity;
    const G4HalfSurface* surface = nullptr;

    for (const G4HalfSurface* surface : intersectionSurfaces_)
    {
       if (s->DistanceToSurface(p) > distanceToOut)
       {
           distanceToOut = s->DistanceToSurface(p);
           s = surface;
       }
    }

    for (const G4HalfSurface* surface : subtractionSurfaces_)
    {
        if (-s->DistanceToSurface(p) > distanceToOut)
        {
            distanceToOut = -s->DistanceToSurface(p);
            s = surface;
        }
    }

    return distanceToOut;
}


const G4HalfSurface& G4HalfIntersectionSolid::WhatSurfaceAmIOn(const G4ThreeVector& p) const
{
    if (this->Inside(p) != kSurface)
    {
        throw std::invalid_argument("G4HalfIntersectionSolid::WhatSurfaceAmIOn takes surface points as argument only.");
    }

    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        if (s->Inside(p) == kSurface) return *s;
    }
    for (const G4HalfSurface* s : subtractionSurfaces_)
    {
        if (s->Inside(p) == kSurface) return *s;
    }

    throw std::logic_error("G4HalfIntersectionSolid has no surfaces.");
}

G4ThreeVector G4HalfIntersectionSolid::SurfaceNormal(const G4ThreeVector& p) const
{
    const G4HalfSurface* surface = nullptr;
    if (this->Inside(p) == kInside)
    {
        DistanceToOut(p, surface);
        return surface->SurfaceNormal(p);
    }
    if (this->Inside(p) == kOutside)
    {
        DistanceToIn(p, surface);
        return surface->SurfaceNormal(p);
    }
    return WhatSurfaceAmIOn(p).SurfaceNormal(p);
}

EInside G4HalfIntersectionSolid::Inside(const G4ThreeVector& p) const
{
    G4bool insideSubtraction = true;
    for (const G4HalfSurface* s : subtractionSurfaces_)
    {
        insideSubtraction = insideSubtraction && s->Inside(p) == kInside;
        if (not insideSubtraction) break; // Quit looping if not inside all subtractions
    }
    if (insideSubtraction) return kOutside;

    G4bool insideIntersection = true;
    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        if (s->Inside(p) == kSurface) return kSurface;

        insideIntersection = insideIntersection && s->Inside(p) == kInside;
        if (not insideIntersection) break;
    }
    if (insideIntersection) return kInside;

    return kOutside;
}

void G4HalfIntersectionSolid::DescribeYourselfTo(G4VGraphicsScene& scene) const
{
    // Standard implementation as suggested by CoPilot
    scene.AddSolid(*this);
}

std::ostream& G4HalfIntersectionSolid::StreamInfo(std::ostream& os) const
{
    os << "--- Half Intersection Solid: " << GetName() << " ---" << std::endl;
    os << "Surface Intersections: " << std::endl;
    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        os << s << std::endl;
    }
    os << "Surface Subtractions: " << std::endl;
    for (const G4HalfSurface* s : subtractionSurfaces_)
    {
        os << s << std::endl;
    }
    return os;
}