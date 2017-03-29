#ifndef PARAMETERS_H
#define PARAMETERS_H


class parameters
{
public:
  /// @param n_cols number of columns the world has
  /// @param n_rows number of rows the world has
  parameters(
    const int n_cols,
    const int n_rows
  );


private:
  const int m_n_cols;
  const int m_n_rows;
};

#endif // PARAMETERS_H
