#ifndef ENTITY_HPP
# define ENTITY_HPP

# include "Surface.hpp"

constexpr Vect<2u, double> complexProduct(Vect<2u, double> a, Vect<2u, double> b)
{
  return {a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0]};
}

constexpr Vect<2u, double> getDir(std::size_t index)
{
  constexpr Vect<2u, double> up{0, 1};
  Vect<2u, double> result{1, 0};

  for (auto i(0u); i != index; ++i)
    result = complexProduct(result, up);
  return (result);
}

class Entity
{
public:
  Entity();
  ~Entity();

  template<std::size_t DIR>
  Surface<DIR> get() const
  {
    return Surface<DIR>{pos + complexProduct(size * 0.5, getDir(DIR))), size[!(DIR & 1)]};
  }

#define ENTITY_DIRECTION_GETTER(DIR, NAME) \
  Surface<DIR> get##NAME() const \
  { \
    return get<DIR>(); \
  }

  template<std::size_t DIR>
  bool hasCollision(const Surface<DIR> &surface) const
  {
    auto rotatedSurface(surface.rotate<2>());
    auto rotatedSelf(get<(DIR + 2) & 3>().rotate<0>());
    auto rotatedSpeed(complexProduct(getDir((4 + 2 - DIR) & 3), speed));

    if (rotatedSpeed.x <= 0.0)
      return false;
    rotatedSelf.pos[1] += (rotatedSpeed.y * (rotatedSurface.x - rotatedSelf.x) / rotatedSpeed.x);
    return ((rotatedSelf.pos[0] + rotatedSelf.size - rotatedSurface.pos[0]) *
      (rotatedSurface.pos[0] + rotatedSurface.size - rotatedSelf.pos[0]) < 0);
  }

  ENTITY_DIRECTION_GETTER(0u, Right);
  ENTITY_DIRECTION_GETTER(1u, Up);
  ENTITY_DIRECTION_GETTER(2u, Left);
  ENTITY_DIRECTION_GETTER(3u, Down);

public:
  bool isUseless;
  Vect<2, double> pos;
  Vect<2, double> size;
  Vect<2, double> speed;
};

#endif // !ENTITY_HPP
