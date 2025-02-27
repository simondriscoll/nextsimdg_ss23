/*!
 * @file ILateralIceSpread.hpp
 *
 * @date Apr 5, 2022
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#ifndef ILATERALICESPREAD_HPP
#define ILATERALICESPREAD_HPP

#include "include/ModelArrayRef.hpp"
#include "include/ModelComponent.hpp"
#include "include/Time.hpp"

namespace Nextsim {
//! An interface class to calculate the freezing of open water and melting of ice.
class ILateralIceSpread : public ModelComponent {
public:
    virtual ~ILateralIceSpread() = default;

    std::string getName() const override { return "LateralIceSpread"; }
    void setData(const ModelState::DataMap& ms) override { }
    ModelState getState() const override { return ModelState(); }
    ModelState getState(const OutputLevel&) const override { return getState(); }
    ModelState getStateRecursive(const OutputSpec& os) const override
    {
        return os ? getState() : ModelState();
    }
    /*!
     * Updates the freezing of open water for the timestep.
     *
     * @param tStep The object containing the timestep start and duration times.
     * @param hice The ice-average ice thickness.
     * @param hsnow The ice-average snow thickness.
     * @param deltaHi The change in ice thickness this timestep.
     * @param newIce The positive change in ice thickness this timestep.
     * @param cice The ice concentration.
     * @param qow The open water heat flux.
     * @param deltaCFreeze The change in concentration due to freezing.
     */
    virtual void freeze(const TimestepTime& tstep, double hice, double hsnow, double deltaHi,
        double newIce, double& cice, double& qow, double& deltaCfreeze)
        = 0;
    /*!
     * Updates the lateral melting of ice for the timestep.
     *
     * @param tStep The object containing the timestep start and duration times.
     * @param hice The ice-average ice thickness.
     * @param hsnow The ice-average snow thickness.
     * @param deltaHi The change in ice thickness this timestep.
     * @param cice The ice concentration.
     * @param qow The open water heat flux.
     * @param deltaCmelt The change in concentration due to melting.
     */
    virtual void melt(const TimestepTime& tstep, double hice, double hsnow, double deltaHi,
        double& cice, double& qow, double& deltaCmelt)
        = 0;

protected:
    ILateralIceSpread()
        : cice(getSharedArray())
        , qow(getSharedArray())
        , hice(getSharedArray())
        , hsnow(getSharedArray())
        , deltaHi(getSharedArray())
    {
        registerModule();
        ModelComponent::registerSharedArray(SharedArray::DELTA_CICE, &deltaCi);
    }

    ModelArrayRef<SharedArray::C_ICE, MARBackingStore, RW> cice; // From IceGrowth
    ModelArrayRef<SharedArray::Q_OW, MARBackingStore, RW> qow; // From FluxCalculation

    ModelArrayRef<SharedArray::H_ICE, MARBackingStore, RO> hice; // From IceGrowth
    ModelArrayRef<SharedArray::H_SNOW, MARBackingStore, RO> hsnow; // From Ice Growth?
    ModelArrayRef<SharedArray::DELTA_HICE, MARBackingStore, RO> deltaHi; // From Vertical Ice Growth

    // Owned, shared arrays
    HField deltaCi;
};

} /* namespace Nextsim */

#endif /* ILATERALICESPREAD_HPP */
