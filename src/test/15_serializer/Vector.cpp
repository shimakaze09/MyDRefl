#include "Vector.hpp"

#include <MyDRefl/MyDRefl.hpp>

void RegisterVector() {
  My::MyDRefl::Mngr.RegisterType<Vector>();
  My::MyDRefl::Mngr.AddField<&Vector::data>("data");
}
