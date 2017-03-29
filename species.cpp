#include "species.h"
#include <cassert>
#include <iostream>
#include <numeric>

species::species()
  : m_id{create_new_species_id()},
    m_r{},
    m_g{},
    m_b{}
{
  const auto c = create_rgb(m_id.get_id());
  m_r = std::get<0>(c);
  m_g = std::get<1>(c);
  m_b = std::get<2>(c);
  assert(m_r >= 0);
  assert(m_g >= 0);
  assert(m_b >= 0);
  assert(m_r < 256);
  assert(m_g < 256);
  assert(m_b < 256);
}

int count_individuals(const std::vector<species>& v) noexcept
{
  return std::accumulate(
    std::begin(v),
    std::end(v),
    0,
    [](const int sum, const auto& s)
    {
      return sum + s.get_count();
    }
  );

}

std::tuple<int, int, int> create_rgb(const int x)
{
  const int a = x % (256 * 6);
  const int phase = a / 256;
  const int b = a % 256;
  switch (phase)
  {
    case 0: return std::make_tuple(    255,      b,       0);
    case 1: return std::make_tuple(255 - b,    255,       0);
    case 2: return std::make_tuple(      0,    255,       b);
    case 3: return std::make_tuple(      0, 255 -b,     255);
    case 4: return std::make_tuple(      b,      0,     255);
    case 5: return std::make_tuple(    255,      0, 255 - b);
  }
  assert(!"Should not get here");
  return {0,0,0};
}

double sum_fractions(const std::vector<species>& v) noexcept
{
  return std::accumulate(
    std::begin(v),
    std::end(v),
    0,
    [](const int sum, const auto& s)
    {
      return sum + s.get_fraction();
    }
  );
}

bool operator==(const species& lhs, const species& rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}
bool operator!=(const species& lhs, const species& rhs)  noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const species& s) noexcept
{
  os << "ID:" << s.get_id() << ", "
    << "color: (" << s.get_r() << ", "
    << s.get_g() << ", " << s.get_b() << "), "
    << "count: " << s.count << ", "
    << "f: " << s.fraction
  ;
  return os;
}


