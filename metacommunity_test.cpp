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

BOOST_AUTO_TEST_CASE(pick_random_species_from_metacommunity)
{
  const int size{1};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  assert(m.get_species().size() == size);
  const species s = m.get_species()[0];
  std::mt19937 rng_engine{123};
  for (int i=0; i!=1000; ++i)
  {
    const species t = getSpeciesFromMetaCommunity(m, rng_engine);
    BOOST_CHECK_EQUAL(s, t);
  }
}

BOOST_AUTO_TEST_CASE(pick_random_index_from_metacommunity)
{
  const int size{1};
  const int rng_seed{42};
  const double theta{0.5};
  const metacommunity m(
    metacommunity_parameters(size, rng_seed, theta)
  );
  assert(m.get_species().size() == size);
  std::mt19937 rng_engine{123};
  for (int i=0; i!=1000; ++i)
  {
    const int t = getSpeciesIndex(m, rng_engine);
    BOOST_CHECK_EQUAL(0, t);
  }
}



