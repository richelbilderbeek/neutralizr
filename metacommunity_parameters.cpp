#include "metacommunity_parameters.h"

#include <cassert>
#include <iostream>

metacommunity_parameters::metacommunity_parameters(
  const int j_m,
  const int rng_seed,
  const double theta
) : m_size{j_m},
    m_rng_seed{rng_seed},
    m_theta{theta}
{
  assert(j_m >= 0);
  assert(theta >= 0.0);
}

std::ostream& operator<<(std::ostream& os, const metacommunity_parameters& p) noexcept
{
  os
    << p.get_size() << ','
    << p.get_rng_seed() << ','
    << p.get_theta()
  ;
  return os;
}

