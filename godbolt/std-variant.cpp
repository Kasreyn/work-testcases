#include <iostream>
#include <variant>

namespace Core {
    enum class EnumTransformationSpace {
        SENSOR,
        ENCODED,
        EXPOSURE,
        VIP1_VCA,
        MASK,
        YUV,
        DEST_OVERLAY,
        EXT_DEVICE_SPHERE
    };

    template <EnumTransformationSpace TSpace, typename CoordinateType>
    struct TransformationSpaceDesc {
        static constexpr EnumTransformationSpace space = TSpace;
        using Coordinate = CoordinateType;
    };
}

namespace Coordinate {
    struct Cartesian2D {};
}

namespace Space {
    using SENSOR = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::SENSOR, Coordinate::Cartesian2D>;
    using ENCODED = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::ENCODED, Coordinate::Cartesian2D>;
    using EXPOSURE = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXPOSURE, Coordinate::Cartesian2D>;
    using VIP1_VCA = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::VIP1_VCA, Coordinate::Cartesian2D>;
    using MASK = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::MASK, Coordinate::Cartesian2D>;
    using YUV = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::YUV, Coordinate::Cartesian2D>;
    using DEST_OVERLAY = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::DEST_OVERLAY, Coordinate::Cartesian2D>;
    using EXT_DEVICE_SPHERE = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXT_DEVICE_SPHERE, Coordinate::Cartesian2D>;
}

using SpaceVariant = std::variant<
    Space::SENSOR, 
    Space::ENCODED, 
    Space::EXPOSURE, 
    Space::VIP1_VCA, 
    Space::MASK, 
    Space::YUV, 
    Space::DEST_OVERLAY, 
    Space::EXT_DEVICE_SPHERE
>;

template <typename TSrcSpace = SpaceVariant, typename TDstSpace = SpaceVariant>
class Path {

public:
    Path() {}

    void print() const {
        std::cout << "Source Space: " << toString(srcSpace) << std::endl;
        std::cout << "Destination Space: " << toString(dstSpace) << std::endl;
    }

private:
    SpaceVariant srcSpace;
    SpaceVariant dstSpace;

    static const char* toString(const SpaceVariant& space) {
        return std::visit([](const auto& val) {
            return toString(val.space);
        }, space);
    }

    static const char* toString(Core::EnumTransformationSpace space) {
        switch (space) {
            case Core::EnumTransformationSpace::SENSOR: return "SENSOR";
            case Core::EnumTransformationSpace::ENCODED: return "ENCODED";
            case Core::EnumTransformationSpace::EXPOSURE: return "EXPOSURE";
            case Core::EnumTransformationSpace::VIP1_VCA: return "VIP1_VCA";
            case Core::EnumTransformationSpace::MASK: return "MASK";
            case Core::EnumTransformationSpace::YUV: return "YUV";
            case Core::EnumTransformationSpace::DEST_OVERLAY: return "DEST_OVERLAY";
            case Core::EnumTransformationSpace::EXT_DEVICE_SPHERE: return "EXT_DEVICE_SPHERE";
            default: return "UNKNOWN";
        }
    }
};

template class Path<SpaceVariant, SpaceVariant>;

int main() {
    // Instantiate the template class with specific SpaceVariant types
    Path<Space::SENSOR, Space::ENCODED> myClass;
    myClass.print();

    return 0;
}
