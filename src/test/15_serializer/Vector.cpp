#include "Vector.hpp"

#include <MyDRefl/MyDRefl.hpp>

void RegisterVector() {
  Smkz::MyDRefl::Mngr.RegisterType<Vector>();
  Smkz::MyDRefl::Mngr.AddField<&Vector::data>("data");
}
