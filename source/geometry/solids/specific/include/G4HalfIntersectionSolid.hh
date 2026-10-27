#ifndef G4HALF_INTERSECTION_SOLID_HH
#define G4HALF_INTERSECTION_SOLID_HH

#include "G4HalfSurface.hh"
#include <vector>
#include "G4VSolid.hh"

class G4HalfIntersectionSolid : public G4VSolid
{
    std::vector<const G4HalfSurface*> intersectionSurfaces_;
    std::vector<const G4HalfSurface*> subtractionSurfaces_;

public:
    G4HalfIntersectionSolid(const std::vector<const G4HalfSurface*>& intersectionSurfaces,
                            const std::vector<const G4HalfSurface*>& subtractionSurfaces,
                            const G4String& name);

    void AddIntersectionSurface(const G4HalfSurface& s);
    void AddSubtractionSurface(const G4HalfSurface& s);

    G4double ImplicitFunction(const G4ThreeVector &p) const;

    const G4HalfSurface& ClosestSurfaceToOut(const G4ThreeVector& p) const;
    //const G4HalfSurface& ClosestSurfaceToIn(const G4ThreeVector& p) const;
    const G4HalfSurface& WhatSurfaceAmIOn(const G4ThreeVector& p) const;
    //G4double DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v, G4HalfSurface*) const override;
    // Returning by input argument
    // Inherited functions
    G4bool CalculateExtent(const EAxis pAxis, const G4VoxelLimits& pVoxelLimit, const G4AffineTransform& pTransform,
                           G4double& pMin, G4double& pMax) const override;
    EInside Inside(const G4ThreeVector& p) const override;
    G4ThreeVector SurfaceNormal(const G4ThreeVector& p) const override;
    G4double DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const override;
    G4double DistanceToIn(const G4ThreeVector& p) const override;
    G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v, const G4bool calcNorm, G4bool* validNorm,
                           G4ThreeVector* n) const override;
    G4double DistanceToOut(const G4ThreeVector& p) const override;
    G4GeometryType GetEntityType() const override = 0;
    void DescribeYourselfTo(G4VGraphicsScene& scene) const override;
    std::ostream& StreamInfo(std::ostream& os) const override;
};

#endif