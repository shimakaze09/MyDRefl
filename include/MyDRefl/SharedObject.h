//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "SharedBlock.h"

#include <memory>

namespace My::MyDRefl {
	class SharedObject {
	public:
		//
		// Constructor
		////////////////

		constexpr SharedObject() noexcept = default;
        constexpr SharedObject(std::nullptr_t) noexcept {}
        explicit constexpr SharedObject(TypeID ID) noexcept : ID{ ID } {}
        SharedObject(TypeID ID, SharedBlock& block) noexcept : ID{ ID }, block{ block } {}
        SharedObject(TypeID ID, SharedBlock&& block) noexcept : ID{ ID }, block{ std::move(block) } {}
        SharedObject(const SharedObject& obj) : ID{ obj.ID }, block{ obj.block } {} // only exception
        SharedObject(SharedObject&& obj) noexcept : ID{ obj.ID }, block{ std::move(obj.block) } {}
        SharedObject(ObjectPtr obj, void(*deleter)(void*)) noexcept : ID{ obj.GetID() }, block{ obj.GetPtr(), deleter } {}
        SharedObject(ObjectPtr obj, std::function<void(void*)> deleter) noexcept : ID{ obj.GetID() }, block{ obj.GetPtr(), deleter } {}
        template<typename U, typename Deleter>
        SharedObject(ObjectPtr obj, Deleter&& d) noexcept : ID{ obj.GetID() }, block{ obj.GetPtr(), std::forward<Deleter>(d) } {}
        template<typename U, typename Deleter, typename Alloc>
        SharedObject(ObjectPtr obj, Deleter&& d, Alloc&& alloc) noexcept : ID{ obj.GetID() }, block{ obj.GetPtr(), std::forward<Deleter>(d), std::forward<Alloc>(alloc) } {}

		//
		// Assign
		///////////

		SharedObject& operator=(SharedObject& rhs) noexcept {
            ID = rhs.ID;
            block = rhs.block;
			return *this;
		}

        SharedObject& operator=(SharedObject&& rhs) noexcept {
            ID = rhs.ID;
            block = std::move(rhs.block);
            return *this;
        }

        SharedObject& operator=(std::nullptr_t) noexcept {
            Reset();
            return *this;
        }

        //
        // Cast
        /////////

        ObjectPtr      CastToObjectPtr() & noexcept { return block.AsObjectPtr(ID); }
        ConstObjectPtr CastToObjectPtr() const& noexcept { return block.AsObjectPtr(ID); }

        operator ObjectPtr     () & noexcept { return block.AsObjectPtr(ID); }
        operator ConstObjectPtr() const& noexcept { return block.AsObjectPtr(ID); }

        //
        // Modifiers
        //////////////

        void Reset() noexcept {
            block.Reset();
            ID.Reset();
        }

        void Swap(SharedObject& rhs) noexcept {
            block.Swap(rhs.block);
        }

        //
        // Observers
        //////////////

        TypeID GetID() const noexcept { return ID; }

        void* GetPtr() noexcept { return block.Get(); }
        const void* GetPtr() const noexcept { return block.Get(); }

        template<typename T>
        T* AsPtr() noexcept { return reinterpret_cast<T*>(GetPtr()); }
        template<typename T>
        const T* AsPtr() const noexcept { return const_cast<SharedObject*>(this)->AsPtr<T>(); }

        template<typename T>
        T& As() noexcept { assert(GetPtr()); return *AsPtr<T>(); }
        template<typename T>
        const T& As() const noexcept { assert(GetPtr()); return *AsPtr<T>(); }

        ObjectPtr      AsObjectPtr() & noexcept { return block.AsObjectPtr(ID); }
        ConstObjectPtr AsObjectPtr() const& noexcept { return block.AsObjectPtr(ID); }

        long UseCount() const noexcept { return block.UseCount(); }

        explicit operator bool() const noexcept { return static_cast<bool>(block); }

	private:
        TypeID ID;
		SharedBlock block;
	};
}

//template<>
//struct std::hash<My::MyDRefl::SharedObject> {
//    std::size_t operator()(const My::MyDRefl::SharedObject& obj) noexcept {
//        return std::hash<const void*>()(obj.get());
//    }
//};

inline bool operator==(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() == right.GetID() && left.GetPtr() == right.GetPtr();
}

inline bool operator!=(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() != right.GetID() || left.GetPtr() != right.GetPtr();
}

inline bool operator<(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() < right.GetID() || (left.GetID() == right.GetID() && left.GetPtr() < right.GetPtr());
}

inline bool operator>=(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() > right.GetID() || (left.GetID() == right.GetID() && left.GetPtr() >= right.GetPtr());
}

inline bool operator>(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() > right.GetID() || (left.GetID() == right.GetID() && left.GetPtr() > right.GetPtr());
}

inline bool operator<=(const My::MyDRefl::SharedObject& left, const My::MyDRefl::SharedObject& right) noexcept {
    return left.GetID() < right.GetID() || (left.GetID() == right.GetID() && left.GetPtr() <= right.GetPtr());
}

template <class Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& out, const My::MyDRefl::SharedObject& obj) {
    return out << obj.GetID().GetValue() << obj.GetPtr();
}

namespace std {
    inline void swap(My::MyDRefl::SharedObject& left, My::MyDRefl::SharedObject& right) noexcept {
        left.Swap(right);
    }
}
