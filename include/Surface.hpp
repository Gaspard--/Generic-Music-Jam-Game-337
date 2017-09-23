#ifdef SURFACE_HPP
# define SURFACE_HPP

# include "Vect.hpp"

class SurfaceData
{
public:
  Vect<2u, double> pos;
  double size;
};

template<size_t direction>
class Surface : public SurfaceData
{
public:
  Surface(Vect<2u, double>, double);
  ~Surface();
};

#endif // !SURFACE_HPP
