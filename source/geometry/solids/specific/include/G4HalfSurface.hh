#ifndef G4HALF_SURFACE_HH
#define G4HALF_SURFACE_HH

#include "G4VSolid.hh"
#include "G4Types.hh"
#include <vector>

class G4HalfSurface
{
public:
    G4HalfSurface(const G4String& name) : G4VSolid(name) {};

    EInside Inside(const G4ThreeVector& p) const override;
    G4double DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const override = 0;
    G4double DistanceToIn(const G4ThreeVector& p) const override = 0;
    G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v,
                                       const G4bool calcNorm = false, G4bool* validNorm = nullptr,
                                       G4ThreeVector* n = nullptr) const override = 0;
    G4double DistanceToOut(const G4ThreeVector& p) const override = 0;

    virtual G4double ImplicitFunction(const G4ThreeVector& p) const = 0;

    G4double Union(const G4ThreeVector& p, const G4HalfSurface &) const;
    G4double Intersection(const G4ThreeVector& p, const G4HalfSurface &) const;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& b, const G4ThreeVector& v) = 0;
    G4double Subtraction(const G4ThreeVector& p, const G4HalfSurface &) const;

    G4ThreeVector SurfaceNormal(const G4ThreeVector& p) const override = 0;
    G4bool CalculateExtent(const EAxis pAxis, const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform, G4double& pMin, G4double& pMax) const override = 0;

    G4GeometryType GetEntityType() const override = 0;

    std::ostream& StreamInfo(std::ostream& os) const override = 0;

    void DescribeYourselfTo(G4VGraphicsScene& scene) const override = 0;
};

#endif
