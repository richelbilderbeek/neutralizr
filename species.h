#ifndef SADO_SPECIES_H
#define SADO_SPECIES_H

#include "species_id.h"

#include <iosfwd>
#include <tuple>
#include <vector>

/// Every species has a unique ID and a color
class species
{
public:
  explicit species();

  auto get_id() const noexcept { return m_id; }
  auto get_r() const noexcept { return m_r; }
  auto get_g() const noexcept { return m_g; }
  auto get_b() const noexcept { return m_b; }

  ///Get the number of individuals this species consists of
  int get_count() const noexcept { return count; }

  double fraction;

  ///Number of individuals this species consists of
  int count;

private:
  species_id m_id;

  int m_r;
  int m_g;
  int m_b;

};

///Count the number of individuals
int count_individuals(const std::vector<species>& s) noexcept;

species create_new_species();

/// Create a color following the rainbow
std::tuple<int, int, int> create_rgb(const int x);

bool operator==(const species &lhs, const species &rhs) noexcept;
bool operator!=(const species &lhs, const species &rhs) noexcept;
bool operator<(const species &lhs, const species &rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const species& spp) noexcept;

#endif // SADO_SPECIES_H
