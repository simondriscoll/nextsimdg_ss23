/*!
 * @file SlabOcean.hpp
 *
 * @date 27 Jan 2023
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#ifndef SLABOCEAN_HPP
#define SLABOCEAN_HPP

#include "include/ModelComponent.hpp"

namespace Nextsim {

class SlabOcean : public ModelComponent {
public:
    SlabOcean()
        : sstSlab(ModelArray::Type::H)
        , sssSlab(ModelArray::Type::H)
        , qdw(ModelArray::Type::H)
        , fdw(ModelArray::Type::H)
        , sstExt(getProtectedArray())
        , sssExt(getProtectedArray())
        , mld(getProtectedArray())
        , cpml(getProtectedArray())
        , emp(getProtectedArray())
        , qio(getSharedArray())
        , qow(getSharedArray())
        , deltaHice(getSharedArray())
        , deltaSmelt(getSharedArray())

    {
    }

    void setData(const ModelState::DataMap& ms) override;
    ModelState getState() const override;
    ModelState getState(const OutputLevel&) const override;
    std::string getName() const override { return "SlabOcean"; }

    std::unordered_set<std::string> hFields() const override;
    void update(const TimestepTime&);

private:
    // Owned shared fields
    HField sstSlab;
    HField sssSlab;
    HField qdw;
    HField fdw;

    // Input fields
    ModelArrayRef<ProtectedArray::SST, MARConstBackingStore> sstExt;
    ModelArrayRef<ProtectedArray::SSS, MARConstBackingStore> sssExt;
    ModelArrayRef<ProtectedArray::MLD, MARConstBackingStore> mld;
    ModelArrayRef<ProtectedArray::ML_BULK_CP, MARConstBackingStore> cpml;
    ModelArrayRef<ProtectedArray::EVAP_MINUS_PRECIP, MARConstBackingStore> emp;
    ModelArrayRef<SharedArray::Q_IO, MARBackingStore> qio;
    ModelArrayRef<SharedArray::Q_OW, MARBackingStore> qow;
    // TODO ModelArrayRef to assimilation flux
    ModelArrayRef<SharedArray::DELTA_HICE, MARBackingStore> deltaHice;
    ModelArrayRef<SharedArray::HSNOW_MELT, MARBackingStore> deltaSmelt;

    static const std::string sstSlabName;
    static const std::string sssSlabName;
};

} /* namespace Nextsim */

#endif /* SLABOCEAN_HPP */
