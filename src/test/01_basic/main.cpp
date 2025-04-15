#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Point {
  float x, y;
};

int main() {
  Mngr.RegisterType<Point>();
  Mngr.AddField<&Point::x>("x");
  Mngr.AddField<&Point::y>("y");

  SharedObject p = Mngr.MakeShared(Type_of<Point>);

  Mngr.Var(p, "x") = 1.f;
  Mngr.Var(p, "y") = 2.f;

  for (const auto& [type, field, var] : p.GetTypeFieldVars()) {
    std::cout << field.name.GetView() << ": " << var << std::endl;
  }

  const Point q{3.f, 4.f};

  for (const auto& [type, field, var] : ObjectView{q}.GetTypeFieldVars()) {
    std::cout << field.name.GetView() << ": " << var << std::endl;
  }
}
