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

std::vector<G4ThreeVector> G4HalfPlane::Intersection(const G4ThreeVector& p, const G4ThreeVector& v)
{
    G4double t = this->ImplicitFunction(p)/v.dot(normal);
    return {v*t};
}

G4double G4HalfPlane::DistanceToIn(const G4ThreeVector& p) const
{
    EInside inside_state = this->Inside(p);
    if (inside_state == kInside || inside_state == kSurface) return 0;
    return this->ImplicitFunction(p);
}

G4double G4HalfPlane::DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const
{
    EInside inside_state = this->Inside(p);
    if (inside_state == kInside || inside_state == kSurface) return 0;

    // Check if v is pointing away from the plane
    if (v.dot(normal) >= 0) return kInfinity;

    return -this->ImplicitFunction(p)/normal.dot(v);
}

G4double G4HalfPlane::DistanceToOut(const G4ThreeVector& p) const
{
    EInside inside_state = this->Inside(p);
    if (inside_state == kOutside || inside_state == kSurface) return 0;

    return -this->ImplicitFunction(p);
}

G4double G4HalfPlane::DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v,
                                    const G4bool calcNorm, G4bool* validNorm, G4ThreeVector* n) const
{
    EInside inside_state = this->Inside(p);
    if (inside_state == kOutside || inside_state == kSurface) return 0;

    return this->ImplicitFunction(p)/normal.dot(v);
}
