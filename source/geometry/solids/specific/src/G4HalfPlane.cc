#include "G4HalfPlane.hh"

G4HalfPlane::G4HalfPlane(const G4ThreeVector& normalIn, const G4double& distanceIn)
            :normal(normalIn / normalIn.mag()),
             distance(distanceIn) {}

G4double G4HalfPlane::ImplicitFunction(const G4ThreeVector& p) const
{
    return normal.dot(p) - distance;
}

std::vector<G4ThreeVector> G4HalfPlane::Intersection(const G4ThreeVector& p, const G4ThreeVector& v)
{
    if (normal.dot(v) == 0) return {};
    
    const G4double t = -this->ImplicitFunction(p)/normal.dot(v);
    return {v*t + p};
}

