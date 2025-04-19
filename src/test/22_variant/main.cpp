#include <MyDRefl/MyDRefl.hpp>
#include <iostream>
#include <variant>

using namespace Smkz;
using namespace Smkz::MyDRefl;

int main() {
  Mngr.RegisterType<std::variant<int, float, double>>();

  for (const auto& [name, method] :
       MethodRange_of<std::variant<int, float, double>>) {
    std::cout << name.GetView() << ": ";

    std::cout << "[";
    switch (method.methodptr.GetMethodFlag()) {
      case MethodFlag::Variable:
        std::cout << "Variable";
        break;
      case MethodFlag::Const:
        std::cout << "Const";
        break;
      case MethodFlag::Static:
        std::cout << "Static";
        break;
      default:
        continue;
        break;
    }
    std::cout << "]";

    std::cout << " " << method.methodptr.GetResultType().GetName() << "(";

    for (const auto& param : method.methodptr.GetParamList())
      std::cout << param.GetName() << ", ";

    std::cout << ")" << std::endl;
  }

  {
    SharedObject v = Mngr.MakeShared(Type_of<std::variant<int, float, double>>,
                                     TempArgsView{3.});
    std::cout << v.GetType().GetName() << std::endl;
    std::cout << v.variant_visit_get().GetType().GetName() << std::endl;

    std::cout << v.index() << std::endl;
    std::cout << v.holds_alternative(Type_of<double>) << std::endl;
    std::cout << v.holds_alternative(Type_of<float>) << std::endl;

    for (size_t i = 0; i < v.variant_size(); ++i)
      std::cout << v.variant_alternative(i).GetName() << std::endl;
  }

  return 0;
}
