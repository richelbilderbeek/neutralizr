#ifndef METACOMMUNITY_PARAMETERS_H
#define METACOMMUNITY_PARAMETERS_H

#include <iosfwd>

class metacommunity_parameters
{
public:
  /// @param size metacommunity size, Jm, in number of species
  /// @param meta_rng_seed RNG seed for generating the metacommunity
  /// @param meta_theta theta, used in generating the metacommunity
  metacommunity_parameters(
    const int size,
    const int rng_seed,
    const double theta
  );

  ///Get the metacommunity size, Jm, in number of species
  int get_size() const noexcept { return m_size; }
  int get_rng_seed() const noexcept { return m_rng_seed; }
  double get_theta() const noexcept { return m_theta; }
private:
  const int m_size;
  const int m_rng_seed;
  const double m_theta;

  friend std::ostream& operator<<(std::ostream& os, const metacommunity_parameters& p) noexcept;
};

std::ostream& operator<<(std::ostream& os, const metacommunity_parameters& p) noexcept;

#endif // METACOMMUNITY_PARAMETERS_H
