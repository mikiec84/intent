#include "core/units.h"

#include "gtest/gtest.h"

namespace boost {
namespace units {

typedef length_base_dimension::dimension_type    length_dimension;
typedef mass_base_dimension::dimension_type      mass_dimension;
typedef time_base_dimension::dimension_type      time_dimension;

typedef derived_dimension<length_base_dimension,2>::type  area_dimension;
typedef derived_dimension<mass_base_dimension,1,
                          length_base_dimension,2,
                          time_base_dimension,-2>::type   energy_dimension;

namespace test {

struct meter_base_unit : base_unit<meter_base_unit, length_dimension, 1> { };
struct kilogram_base_unit : base_unit<kilogram_base_unit, mass_dimension, 2> { };
struct second_base_unit : base_unit<second_base_unit, time_dimension, 3> { };

typedef make_system<
    meter_base_unit,
    kilogram_base_unit,
    second_base_unit>::type mks_system;

/// unit typedefs
typedef unit<dimensionless_type,mks_system>      dimensionless;

typedef unit<length_dimension,mks_system>        length;
typedef unit<mass_dimension,mks_system>          mass;
typedef unit<time_dimension,mks_system>          time;

typedef unit<area_dimension,mks_system>          area;
typedef unit<energy_dimension,mks_system>        energy;

/// unit constants
BOOST_UNITS_STATIC_CONSTANT(meters,length);
BOOST_UNITS_STATIC_CONSTANT(kilograms,mass);
BOOST_UNITS_STATIC_CONSTANT(seconds,time);

} // namespace test
} // namespace units
} // namespace boost

namespace intent {
namespace core {
    namespace units = boost::units;
}
}


// This test is not attempting to exercise any particular functionality; we
// assume the boost libraries have adequate testing already, and that they work
// as advertised. Instead, we just prove that we can use a boost library within
// its intent namespace.
TEST(units_test, available)
{
    using namespace intent::core::units;
    using namespace intent::core::units::test;

    quantity<length> L = 2.0*meters;                     // quantity of length
    quantity<energy> E = kilograms*pow<2>(L/seconds);    // quantity of energy
    ASSERT_TRUE(E.value() > 0.0);
}
