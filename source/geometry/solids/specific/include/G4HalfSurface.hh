#ifndef G4HALF_SURFACE_HH
#define G4HALF_SURFACE_HH

#include "G4Types.hh"
#include "G4VSolid.hh"
#include <vector>

class G4HalfSurface
{
public:
    G4HalfSurface()= default;
    virtual ~G4HalfSurface() = default;

    EInside Inside(const G4ThreeVector& p) const;

    virtual G4double ImplicitFunction(const G4ThreeVector& p) const = 0;

    virtual G4double Union(const G4ThreeVector& p, const G4HalfSurface& s) const;
    virtual G4double Intersection(const G4ThreeVector& p, const G4HalfSurface& s) const;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector& v) = 0;
    virtual G4double Subtraction(const G4ThreeVector& p, const G4HalfSurface& s) const;

    virtual G4ThreeVector SurfaceNormal(const G4ThreeVector& p) const = 0;
};

#endif