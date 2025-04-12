//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>
#include <array>

using namespace My;
using namespace My::MyDRefl;

struct A { float a{ 0.f }; };
struct B : virtual A { float b{ 0.f }; };
struct C : virtual A { float c{ 0.f }; };
struct D : B, C { float d{ 0.f }; };

int main() {
	{ // register
		Mngr.RegisterType<A>();
		Mngr.AddField<&A::a>("a");

		Mngr.RegisterType<B>();
		Mngr.AddBases<B, A>();
		Mngr.AddField<&B::b>("b");

		Mngr.RegisterType<C>();
		Mngr.AddBases<C, A>();
		Mngr.AddField<&C::c>("c");

		Mngr.RegisterType<D>();
		Mngr.AddBases<D, B, C>();
		Mngr.AddField<&D::d>("d");
	}

	auto d = Mngr.MakeShared(TypeID_of<D>);

	d.Var("a") = 1.f;
	d.Var("b") = 3.f;
	d.Var("c") = 4.f;
	d.Var("d") = 5.f;

	for (const auto& [type, field, var] : d.GetTypeFieldVars()) {
		std::cout
			<< Mngr.nregistry.Nameof(field.ID)
			<< ": " << var
			<< std::endl;
	}

	return 0;
}
