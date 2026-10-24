#include "G4HalfIntersectionSolid.hh"
#include <algorithm>

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
    if (this->Inside(p) == kOutside || this->Inside(p) == kSurface) return 0;

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

const G4HalfSurface& G4HalfIntersectionSolid::ClosestSurfaceToOut(const G4ThreeVector& p) const
{ // Determines closest surface when inside (or closest surface you must pass through to be outside)
    if (this->Inside(p) == kOutside || this->Inside(p) == kSurface)
    {
        throw std::invalid_argument("G4HalfIntersectionSolid::ClosestSurfaceToOut takes kInside points only");
    }

    const G4HalfSurface* resultingSurface = nullptr;
    G4double distanceToOut = -kInfinity;

    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
       if (s->DistanceToSurface(p) > distanceToOut)
       {
           distanceToOut = s->DistanceToSurface(p);
           resultingSurface = s;
       }
    }

    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        if (-s->DistanceToSurface(p) > distanceToOut)
        {
            distanceToOut = -s->DistanceToSurface(p);
            resultingSurface = s;
        }
    }

    if (resultingSurface == nullptr)
    {
        throw std::logic_error("G4HalfIntersectionSolid has no surfaces");
    }

    return *resultingSurface;
}

G4ThreeVector G4HalfIntersectionSolid::SurfaceNormal(const G4ThreeVector& p) const
{
    // Determine the closest surface, choose it, and return that surfaces .SurfaceNormal() function
    if (this->Inside(p) == kInside) return ClosestSurfaceToOut(p).SurfaceNormal(p);
    if (this->Inside(p) == kOutside) return ClosestSurfaceToIn(p).SurfaceNormal(p);
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
    G4bool surfaceIntersection = true;
    for (const G4HalfSurface* s : intersectionSurfaces_)
    {
        surfaceIntersection = surfaceIntersection && s->Inside(p) == kSurface;
        insideIntersection = insideIntersection && s->Inside(p) == kInside;
        if (not insideIntersection) break;
    }
    if (insideIntersection) return kInside;
    if (surfaceIntersection) return kSurface;

    return kOutside;
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