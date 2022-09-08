/*!
 * @file OceanState.cpp
 *
 * @date May 9, 2022
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#include "include/OceanState.hpp"

#include "include/constants.hpp"
#include "include/ModelArrayRef.hpp"

namespace Nextsim {

static const std::map<ModelComponent::ProtectedArray, std::string> fieldNames = {
    { ModelComponent::ProtectedArray::SST, "Sea surface temperature" },
    { ModelComponent::ProtectedArray::SSS, "Sea surface salinity" },
    { ModelComponent::ProtectedArray::MLD, "Mixed layer depth" },
    { ModelComponent::ProtectedArray::TF, "Sea surface freezing temperature" },
    { ModelComponent::ProtectedArray::ML_BULK_CP, "Mixed layer bulk heat capacity" },
};

OceanState::OceanState()
    : tfImpl(nullptr)
    , sst(ModelArray::Type::H)
    , sss(ModelArray::Type::H)
    , mld(ModelArray::Type::H)
    , tf(ModelArray::Type::H)
    , cpml(ModelArray::Type::H)
{
    registerProtectedArray(ProtectedArray::SST, &sst);
    registerProtectedArray(ProtectedArray::SSS, &sss);
    registerProtectedArray(ProtectedArray::MLD, &mld);

    registerProtectedArray(ProtectedArray::TF, &tf);
    registerProtectedArray(ProtectedArray::ML_BULK_CP, &cpml);
}

void OceanState::setData(const ModelState::DataMap&) { }

ModelState OceanState::getState() const
{
    return { {
                 { fieldNames.at(ProtectedArray::SST), sst },
                 { fieldNames.at(ProtectedArray::SSS), sss },
                 { fieldNames.at(ProtectedArray::MLD), mld },
                 { fieldNames.at(ProtectedArray::TF), tf },
                 { fieldNames.at(ProtectedArray::ML_BULK_CP), cpml },
             },
        {} };
}

ModelState OceanState::getState(const OutputLevel& lvl) const { return getState(); }

ModelState OceanState::getStateRecursive(const OutputSpec& os) const
{
    ModelState state(getState());
    return os ? state : ModelState();
}

std::string OceanState::getName() const { return "OceanState"; }

std::unordered_set<std::string> OceanState::hFields() const
{
    std::unordered_set<std::string> fields;
    for (const auto typeName : fieldNames) {
        fields.insert(typeName.second);
    }
    return fields;
}

void OceanState::configure()
{
    tfImpl = &Module::getImplementation<IFreezingPoint>();
    tryConfigure(tfImpl);
}

OceanState::HelpMap& OceanState::getHelpRecursive(HelpMap& map, bool getAll)
{
    return Module::getHelpRecursive<IFreezingPoint>(map, getAll);
}

void OceanState::updateBefore(const TimestepTime& tst)
{
    // Mixed layer heat capacity per unit area
    cpml = mld * Water::rho * Water::cp;
    // Derived class updates
    updateFreezingPoint(tst);

// Zero the nudging fluxes for implementations to update in updateAfter
    }

void OceanState::updateAfter(const TimestepTime& tst)
{
    // Heat flux
    ModelArrayRef<ModelComponent::SharedArray::Q_IO> qio;
    ModelArrayRef<ModelComponent::SharedArray::Q_OW> qow;
    HField heatingRate = qio + qow - qdw;
    sst += tst.step * heatingRate / cpml;

    // Water fluxes
    // Final slab ocean areal mass
    HField arealFinal = mld * Water::rho -

}

void OceanState::updateFreezingPoint(const TimestepTime& tst)
{
    overElements(std::bind(&OceanState::updateFreezingPointI, this, std::placeholders::_1,
                     std::placeholders::_2),
        tst);
}

void OceanState::updateFreezingPointI(size_t i, const TimestepTime&) { tf[i] = (*tfImpl)(sss[i]); }

} /* namespace Nextsim */
