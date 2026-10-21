#ifndef G4HALFPLANE_HH
#define G4HALFPLANE_HH

#include "G4HalfSurface.hh"
#include "G4Types.hh"

class G4HalfPlane : public G4HalfSurface
{
private:
    G4ThreeVector normal;
    G4double distance;

public:
    G4HalfPlane(const G4String& name, const G4ThreeVector &normal, const G4double &distance);
    G4double ImplicitFunction(const G4ThreeVector& p) const override;


    std::vector<G4ThreeVector> Intersection(const G4ThreeVector& b, const G4ThreeVector& v) override;
    G4double DistanceToIn(const G4ThreeVector& p) const override;
    G4double DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const override;

    G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v,
                                           const G4bool calcNorm = false, G4bool* validNorm = nullptr,
                                           G4ThreeVector* n = nullptr) const override;
    G4double DistanceToOut(const G4ThreeVector& p) const override;
};

#endif