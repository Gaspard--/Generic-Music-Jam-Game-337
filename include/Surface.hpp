#ifdef SURFACE_HPP
# define SURFACE_HPP

# include "Vect.hpp"

struct SurfaceData
{
  Vect<2u, double> pos;
  double size;
};

template<std::size_t DIR>
struct Surface : public SurfaceData
{
  Surface(Vect<2u, double>, double);
  ~Surface();

  template<std::size_t DIR2>
  constexpr Surface<DIR2> rotate() const
  {
    return {complexProduct(getDir<(DIR2 + 4 - DIR) & 3>(), pos), size}
  }
};

#endif // !SURFACE_HPP
