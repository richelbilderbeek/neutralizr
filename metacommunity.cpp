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
  const int size{p.get_size()};

  std::vector<species> v;

  const std::vector<int> abund = create_meta_abundances(p);

  assert(size > 1);
  const double multiply = 1.0 / static_cast<double>(size - 1.0);


  for(std::size_t i = 0; i < abund.size() ;++i)
    {
      species newS;
      newS.fraction = abund[i] * multiply;
      newS.count = abund[i];

      v.push_back(newS);
    }

  //remove all empty species
  std::vector<species> temp;
  for(std::vector<species>::iterator m = v.begin(); m != v.end(); ++m)
  {
    if((*m).count > 0) temp.push_back((*m));
  }
  v = temp;

  std::sort(
    v.begin(),
    v.end(),
    [](const auto& lhs, const auto rhs)
    {
      return lhs.count < rhs.count;
    }
  );
  //update fractions
  double cumsum = 0.0;
  for(std::vector<species>::iterator it = v.begin(); it != v.end(); ++it)
  {
    double add = 1.0 * (*it).count / size;
    cumsum += add;
    (*it).fraction = cumsum;
  }
  return v;
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
