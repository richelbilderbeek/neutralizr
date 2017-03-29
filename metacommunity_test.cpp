#include "metacommunity.h"

#include <boost/test/unit_test.hpp>

#include "metacommunity_parameters.h"

BOOST_AUTO_TEST_CASE(metacommunity_use)
{
  const int size{10};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  BOOST_CHECK_EQUAL(size, count_individuals(m));
  BOOST_CHECK_CLOSE(1.0, sum_fractions(m), 0.0001);
}

BOOST_AUTO_TEST_CASE(metacommunity_of_two_should_work)
{
  const int size{2};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  BOOST_CHECK_EQUAL(size, count_individuals(m));
  BOOST_CHECK_CLOSE(1.0, sum_fractions(m), 0.0001);
}


BOOST_AUTO_TEST_CASE(metacommunity_of_one_should_work)
{
  const int size{1};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  BOOST_CHECK_EQUAL(size, count_individuals(m));
  BOOST_CHECK_CLOSE(1.0, sum_fractions(m), 0.0001);
}

BOOST_AUTO_TEST_CASE(empty_metacommunity_should_work)
{
  const int size{0};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  BOOST_CHECK_EQUAL(size, count_individuals(m));
}
