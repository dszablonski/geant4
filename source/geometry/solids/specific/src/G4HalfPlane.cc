#include "G4HalfPlane.hh"

G4HalfPlane::G4HalfPlane(const G4ThreeVector& normalIn, const G4double& distanceIn)
            :normal(normalIn / normalIn.mag()),
             distance(distanceIn) {}

G4double G4HalfPlane::ImplicitFunction(const G4ThreeVector& p) const
{
    return normal.dot(p) - distance;
}


// Returns the signed distance to surface
G4double G4HalfPlane::DistanceToSurface(const G4ThreeVector& p) const
{
    // For a plane, the implicit function is the same as the signed distance
    return this->ImplicitFunction(p);
}

G4double G4HalfPlane::DistanceToSurface(const G4ThreeVector& p, const G4ThreeVector& v) const
{
    const G4ThreeVector vUnit = v/v.mag();

    G4double projection = normal.dot(vUnit);
    G4double implicitFunction = this->ImplicitFunction(p);

    if (projection < 0 && implicitFunction < 0) return kInfinity;
    if (projection > 0 && implicitFunction > 0) return kInfinity;
    if (projection == 0) return kInfinity; // Parallel to plane

    return -implicitFunction/projection;
}
