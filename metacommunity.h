#ifndef METACOMMUNITY_H
#define METACOMMUNITY_H

#include <iosfwd>
#include <vector>
#include "species.h"

struct metacommunity_parameters;

/// The mainland community
/// consisting out of species and their abundances
class metacommunity
{
public:
  /// @param n_cols number of columns the world has
  /// @param n_rows number of rows the world has
  metacommunity(
    const metacommunity_parameters& p
  );
  const auto& get_species() const noexcept { return m_v; }

private:
  std::vector<species> m_v;
};

///Count the number of individuals
int count_individuals(const metacommunity& m) noexcept;

///Create the initial species abundances
std::vector<int> create_meta_abundances(const metacommunity_parameters& p);

std::vector<species> create_meta_species(const metacommunity_parameters& p);

std::ostream& operator<<(std::ostream& os, const metacommunity& c) noexcept;

#endif // METACOMMUNITY_H
