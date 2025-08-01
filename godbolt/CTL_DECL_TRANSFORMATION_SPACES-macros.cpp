#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <variant>
#include <type_traits>
#include <utility>

namespace CTL {
namespace Coordinate {
	struct Cartesian2D {
		float x;
		float y;
	};

	struct Cartesian3D {
		float x;
		float y;
		float z;
	};

	struct PTZ {
		float pan;
		float tilt;
	};

	struct Geographic {
		double latitude;
		double longitude;
		double altitude;
	};

	using CoordinateVariant = std::variant<Cartesian2D, Cartesian3D, PTZ, Geographic>;
} // namespace CTL::Coordinate
}

namespace Core {
	enum class EnumTransformationSpace {
		ANONYMOUS_SPACE,
		SENSOR,
		EXPOSURE,
		VIP1_VCA,
		MASK,
		YUV,
		DEST_OVERLAY,
		ENCODED,
		EXT_DEVICE_SPHERE,
		EXT_DEVICE_3D,
		EXT_GEOGRAPHIC,
	};

    template<typename TCoordinate, std::size_t... INDEX>
    constexpr bool IsVariantAlternative(std::index_sequence<INDEX...>) {
        return (
            std::is_same_v<TCoordinate, std::variant_alternative_t<INDEX, CTL::Coordinate::CoordinateVariant>> ||
            ...);
    }

    template<typename TCoordinate>
    concept ConceptCoordinate = IsVariantAlternative<TCoordinate>(
        std::make_index_sequence<std::variant_size_v<CTL::Coordinate::CoordinateVariant>>{});

	template<EnumTransformationSpace transformationSpace, ConceptCoordinate TCoordinateType>
	class TransformationSpaceDesc {
	  public:
		using CoordinateType = TCoordinateType;

	  private:
		static constexpr EnumTransformationSpace TRANSFORMATION_SPACE = transformationSpace;

	  public:
		static constexpr EnumTransformationSpace GetTransformationSpace() {
			return TRANSFORMATION_SPACE;
		}
	};
} // namespace Core

namespace Space {
	using SENSOR =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::SENSOR, CTL::Coordinate::Cartesian2D>;
	using EXPOSURE =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXPOSURE, CTL::Coordinate::Cartesian2D>;
	using VIP1_VCA =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::VIP1_VCA, CTL::Coordinate::Cartesian2D>;
	using MASK =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::MASK, CTL::Coordinate::Cartesian2D>;
	using YUV =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::YUV, CTL::Coordinate::Cartesian2D>;
	using DEST_OVERLAY = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::DEST_OVERLAY,
													   CTL::Coordinate::Cartesian2D>;
	using ENCODED =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::ENCODED, CTL::Coordinate::Cartesian2D>;
	using EXT_DEVICE_SPHERE =
		Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXT_DEVICE_SPHERE,
									  CTL::Coordinate::Cartesian3D>;
	using EXT_DEVICE_3D	 = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXT_DEVICE_3D,
														 CTL::Coordinate::Cartesian3D>;
	using EXT_GEOGRAPHIC = Core::TransformationSpaceDesc<Core::EnumTransformationSpace::EXT_GEOGRAPHIC,
														 CTL::Coordinate::Geographic>;
	using SpaceVariant = std::variant<Space::SENSOR, Space::EXPOSURE, Space::VIP1_VCA, Space::MASK, Space::YUV, Space::DEST_OVERLAY, Space::ENCODED, Space::EXT_DEVICE_SPHERE, Space::EXT_DEVICE_3D, Space::EXT_GEOGRAPHIC >;
}

#define MY_SEQUENCE ((SENSOR, Coordinate::Cartesian2D))((EXPOSURE, Coordinate::Cartesian2D))
#define MY_SEQUENCE2 (SENSOR, Coordinate::Cartesian2D)(EXPOSURE, Coordinate::Cartesian2D)
#define CTL_FIRST_ELEM(s, data, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem)

#define MAP_LINE(s, data, elem) map[Core::EnumTransformationSpace::BOOST_PP_TUPLE_ELEM(2, 0, elem)] = Space::BOOST_PP_TUPLE_ELEM(2, 0, elem){};

#define GENERATE_MAP_LINES(seq) BOOST_PP_SEQ_FOR_EACH(MAP_LINE, _, seq)

#define CTL_DECL_TRANSFORMATION_SPACES(ENUM_ONLY_SPACE, ...)                   \
  void createMap() {                                                           \
                                                                               \
    static const std::unordered_map<Core::EnumTransformationSpace,             \
                                    Space::SpaceVariant>                       \
        s_EnumToSpaceMap = [] {                                                \
          std::unordered_map<Core::EnumTransformationSpace,                    \
                             Space::SpaceVariant>                              \
              map;                                                             \
          GENERATE_MAP_LINES(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))   \
          return map;                                                          \
        }();                                                                   \
  }

namespace Space {
CTL_DECL_TRANSFORMATION_SPACES(
	ANONYMOUS_SPACE,
	(SENSOR, Coordinate::Cartesian2D),
	(EXPOSURE, Coordinate::Cartesian2D),
	(VIP1_VCA, Coordinate::Cartesian2D),
	(MASK, Coordinate::Cartesian2D),
	(YUV, Coordinate::Cartesian2D),
	(DEST_OVERLAY, Coordinate::Cartesian2D),
	(ENCODED, Coordinate::Cartesian2D),
	(EXT_DEVICE_SPHERE, Coordinate::Cartesian3D),
	(EXT_DEVICE_3D, Coordinate::Cartesian3D),
	(EXT_GEOGRAPHIC, Coordinate::Geographic)
)
}

int main(int argc, char* argv[]) {
	Space::createMap();

	return 0;
}
