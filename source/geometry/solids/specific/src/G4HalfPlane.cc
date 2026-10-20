#include "G4HalfPlane.hh"

G4HalfPlane::G4HalfPlane(const G4String& name, const G4ThreeVector& normal,
                         const G4double& distance)
    : G4HalfSurface(name),
      normal(normal / normal.mag()),
      distance(distance) {}

G4double G4HalfPlane::ImplicitFunction(const G4ThreeVector& p) const
{
    return normal.dot(p) - distance;
}

G4double G4HalfPlane::DistanceToIn(const G4ThreeVector& p) const
{
    return this->ImplicitFunction(p);
}

G4double G4HalfPlane::DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const
{
    // Check if the point is on the surface
    if (this->Inside(p) == kSurface) return 0;

    // Check if v is pointing away from the plane
    if (v.dot(normal) >= 0) return kInfinity;

    return this->ImplicitFunction(p)/(normal.dot(v));
}

