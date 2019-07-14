#ifndef _ba8bcb26a8134e92a64e1d05f3a2c24a
#define _ba8bcb26a8134e92a64e1d05f3a2c24a

#include <boost/units/absolute.hpp>
#include <boost/units/base_dimension.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/config.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/dim.hpp>
#include <boost/units/dimension.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/dimensionless_type.hpp>
#include <boost/units/dimensionless_unit.hpp>
#include <boost/units/get_dimension.hpp>
#include <boost/units/get_system.hpp>
#include <boost/units/heterogeneous_system.hpp>
#include <boost/units/homogeneous_system.hpp>
#include <boost/units/io.hpp>
#include <boost/units/is_dim.hpp>
#include <boost/units/is_dimension_list.hpp>
#include <boost/units/is_dimensionless.hpp>
#include <boost/units/is_dimensionless_quantity.hpp>
#include <boost/units/is_dimensionless_unit.hpp>
#include <boost/units/is_quantity.hpp>
#include <boost/units/is_quantity_of_dimension.hpp>
#include <boost/units/is_quantity_of_system.hpp>
#include <boost/units/is_unit.hpp>
#include <boost/units/is_unit_of_dimension.hpp>
#include <boost/units/is_unit_of_system.hpp>
#include <boost/units/lambda.hpp>
#include <boost/units/limits.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/operators.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/reduce_unit.hpp>
#include <boost/units/scale.hpp>
#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/units_fwd.hpp>

#include <boost/units/physical_dimensions.hpp>

#include <boost/units/systems/si.hpp>
#include <boost/units/systems/cgs.hpp>
#include <boost/units/systems/abstract.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/angle/gradians.hpp>
#include <boost/units/systems/angle/revolutions.hpp>
#include <boost/units/systems/temperature/celsius.hpp>
#include <boost/units/systems/temperature/fahrenheit.hpp>

#include <boost/units/base_units/angle/arcminute.hpp>
#include <boost/units/base_units/angle/arcsecond.hpp>
#include <boost/units/base_units/angle/degree.hpp>
#include <boost/units/base_units/angle/gradian.hpp>
#include <boost/units/base_units/angle/radian.hpp>
#include <boost/units/base_units/angle/revolution.hpp>
#include <boost/units/base_units/angle/steradian.hpp>

#include <boost/units/base_units/imperial/drachm.hpp>
#include <boost/units/base_units/imperial/fluid_ounce.hpp>
#include <boost/units/base_units/imperial/foot.hpp>
#include <boost/units/base_units/imperial/furlong.hpp>
#include <boost/units/base_units/imperial/gallon.hpp>
#include <boost/units/base_units/imperial/gill.hpp>
#include <boost/units/base_units/imperial/grain.hpp>
#include <boost/units/base_units/imperial/hundredweight.hpp>
#include <boost/units/base_units/imperial/inch.hpp>
#include <boost/units/base_units/imperial/league.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include <boost/units/base_units/imperial/ounce.hpp>
#include <boost/units/base_units/imperial/pint.hpp>
#include <boost/units/base_units/imperial/pound.hpp>
#include <boost/units/base_units/imperial/quart.hpp>
#include <boost/units/base_units/imperial/quarter.hpp>
#include <boost/units/base_units/imperial/stone.hpp>
#include <boost/units/base_units/imperial/thou.hpp>
#include <boost/units/base_units/imperial/ton.hpp>
#include <boost/units/base_units/imperial/yard.hpp>

#include <boost/units/base_units/us/cup.hpp>
#include <boost/units/base_units/us/dram.hpp>
#include <boost/units/base_units/us/fluid_dram.hpp>
#include <boost/units/base_units/us/fluid_ounce.hpp>
#include <boost/units/base_units/us/foot.hpp>
#include <boost/units/base_units/us/gallon.hpp>
#include <boost/units/base_units/us/gill.hpp>
#include <boost/units/base_units/us/grain.hpp>
#include <boost/units/base_units/us/hundredweight.hpp>
#include <boost/units/base_units/us/inch.hpp>
#include <boost/units/base_units/us/mil.hpp>
#include <boost/units/base_units/us/mile.hpp>
#include <boost/units/base_units/us/minim.hpp>
#include <boost/units/base_units/us/ounce.hpp>
#include <boost/units/base_units/us/pint.hpp>
#include <boost/units/base_units/us/pound.hpp>
#include <boost/units/base_units/us/pound_force.hpp>
#include <boost/units/base_units/us/quart.hpp>
#include <boost/units/base_units/us/tablespoon.hpp>
#include <boost/units/base_units/us/teaspoon.hpp>
#include <boost/units/base_units/us/ton.hpp>
#include <boost/units/base_units/us/yard.hpp>

#include <boost/units/base_units/astronomical/astronomical_unit.hpp>
#include <boost/units/base_units/astronomical/light_day.hpp>
#include <boost/units/base_units/astronomical/light_hour.hpp>
#include <boost/units/base_units/astronomical/light_minute.hpp>
#include <boost/units/base_units/astronomical/light_second.hpp>
#include <boost/units/base_units/astronomical/light_year.hpp>
#include <boost/units/base_units/astronomical/parsec.hpp>

#include <boost/units/base_units/metric/angstrom.hpp>
#include <boost/units/base_units/metric/are.hpp>
#include <boost/units/base_units/metric/atmosphere.hpp>
#include <boost/units/base_units/metric/bar.hpp>
#include <boost/units/base_units/metric/barn.hpp>
#include <boost/units/base_units/metric/day.hpp>
#include <boost/units/base_units/metric/fermi.hpp>
#include <boost/units/base_units/metric/hectare.hpp>
#include <boost/units/base_units/metric/hour.hpp>
#include <boost/units/base_units/metric/knot.hpp>
#include <boost/units/base_units/metric/liter.hpp>
#include <boost/units/base_units/metric/micron.hpp>
#include <boost/units/base_units/metric/minute.hpp>
#include <boost/units/base_units/metric/mmHg.hpp>
#include <boost/units/base_units/metric/nautical_mile.hpp>
#include <boost/units/base_units/metric/ton.hpp>
#include <boost/units/base_units/metric/torr.hpp>
#include <boost/units/base_units/metric/year.hpp>

namespace intent {
namespace core {

namespace units = boost::units;

}} // end namespace

#endif // sentry
