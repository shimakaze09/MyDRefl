#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

enum class Color { Red, Green, Blue };

int main() {
  Mngr->RegisterType<Color>();
  Mngr->AddField<Color::Red>("Red");
  Mngr->AddField<Color::Green>("Green");
  Mngr->AddField<Color::Blue>("Blue");

  for (const auto& [type, field, var] :
       Mngr->GetTypeFieldVars(ObjectView_of<Color>)) {
    std::cout << field.name.GetView() << ": "
              << static_cast<int>(var.As<Color>()) << std::endl;
  }

  // enumerator -> name
  Color c = Color::Red;
  auto c_field = Mngr->FindField(Type_of<Color>, [c](auto field) {
    return field.info->fieldptr.Var() == c;
  });

  assert(c_field.name.GetView() == "Red");
  std::cout << "name of " << static_cast<int>(c) << " : "
            << c_field.name.GetView() << std::endl;

  // name -> enumerator
  std::string_view name = "Green";
  auto name_field = Mngr->FindField(Type_of<Color>, [name](auto field) {
    return field.name.GetView() == name;
  });
  assert(name_field.info && name_field.info->fieldptr.Var() == Color::Green);
  std::cout << "value of " << name << " : "
            << static_cast<int>(name_field.info->fieldptr.Var().As<Color>())
            << std::endl;
}
