#include "metacommunity.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <iostream>

#include "metacommunity_parameters.h"

metacommunity::metacommunity(const metacommunity_parameters& p)
  : m_v{create_meta_species(p)}
{

}

int count_individuals(const metacommunity& m) noexcept
{
  return count_individuals(m.get_species());
}

std::vector<int> create_meta_abundances(const metacommunity_parameters& p)
{
  const int size{p.get_size()};
  if (size == 0) return {};

  const int rng_seed{p.get_rng_seed()};
  const double theta{p.get_theta()};
  std::mt19937 rng_engine{rng_seed};
  std::uniform_real_distribution<double> d(0.0, 1.0);

  std::vector<int> abund(1,0);
  std::size_t nsp = 1;
  abund.push_back(1);
  for(int j = 1; j < size; ++j)
  {
    const double x = d(rng_engine);
    const double val = theta / (theta + j -1);
    if(x < val)
    {
      nsp++;
      if(nsp > (abund.size()-1))
      {
        int dif = 1+nsp - abund.size();
        for(int k = 0; k < dif; ++k) abund.push_back(0);

      }

      abund[nsp] = 1;
    }
    else
    {
      int translate_to_abund = (int)((x * j)-1);
      //now find corresponding species
      std::size_t index = 0;
      while(index < abund.size())
      {
        translate_to_abund -= abund[index];
        if(translate_to_abund <= 0) break;

        index++;
      }

      abund[index] = abund[index] + 1;
    }
  }
  assert(std::accumulate(std::begin(abund), std::end(abund), 0) == size);
  return abund;
}

std::vector<species> create_meta_species(const metacommunity_parameters& p)
{
  const std::vector<int> abundances = create_meta_abundances(p);

  //Create species with desired abundances
  std::vector<species> v;
  std::transform(
    std::begin(abundances),
    std::end(abundances),
    std::back_inserter(v),
    [](const int n_individuals)
    {
      species s;
      s.count = n_individuals;
      return s;
    }
  );

  //remove all empty species
  const auto new_end =
    std::remove_if(
      std::begin(v),
      std::end(v),
      [](const species& s)
      {
        return s.count == 0;
      }
    );
  v.erase(new_end, std::end(v));

  //sort species by anbundance
  std::sort(
    v.begin(),
    v.end(),
    [](const auto& lhs, const auto rhs)
    {
      return lhs.count < rhs.count;
    }
  );
  //set fractions
  double sum_f{0.0};
  for (species& s: v)
  {
    const double sz{static_cast<double>(p.get_size())};
    const double f{static_cast<double>(s.count) / sz};
    sum_f += f;
    s.fraction = sum_f;
  }
  return v;
}

species getSpeciesFromMetaCommunity(
  const metacommunity& m,
  std::mt19937& rng_engine
)
{
  std::uniform_real_distribution<double> d(0.0, 1.0);
  const double rand{d(rng_engine)};

  int min = 0;
  int max = m.get_species().size() - 1;
  int med = (int)((max+min)*0.5);
  while((max-min) > 1)
    {
      if(m.get_species()[med].fraction > rand) max = med;
      else min = med;

      med = (int)((max+min)*0.5);
    }
  if(m.get_species()[med].fraction < rand) return m.get_species()[max];
  else return m.get_species()[med];
}

int getSpeciesIndex(
  const metacommunity& m,
  std::mt19937& rng_engine
)
{
  std::uniform_real_distribution<double> d(0.0, 1.0);
  const double rand{d(rng_engine)};
  int min = 0;
  int max = m.get_species().size() - 1;
  int med = (int)((max+min)*0.5);
  while((max-min) > 1)
    {
      if(m.get_species()[med].fraction > rand) max = med;
      else min = med;

      med = (int)((max+min)*0.5);
    }
  if(m.get_species()[med].fraction < rand) return max;
  else return med;

}

double sum_fractions(const metacommunity& c) noexcept
{
  return sum_fractions(c.get_species());
}

std::ostream& operator<<(std::ostream& os, const metacommunity& c) noexcept
{
  int i{0};
  for (const auto& s: c.get_species())
  {
    os << i << ": " << s << '\n';
    ++i;
  }
  return os;

}
