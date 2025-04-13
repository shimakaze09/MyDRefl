#include "InvokeUtil.h"

using namespace My::MyDRefl;

bool details::IsPriorityCompatible(std::span<const Type> params,
                                   std::span<const Type> argTypes) {
  if (params.size() != argTypes.size())
    return false;

  for (size_t i = 0; i < params.size(); i++) {
    if (params[i] == argTypes[i])
      continue;

    const auto& lhs = params[i];
    const auto& rhs = argTypes[i];

    if (lhs.IsRValueReference()) {  // &&{T} | &&{const{T}}
      const auto unref_lhs = lhs.Name_RemoveRValueReference();  // T | const{T}
      assert(!type_name_is_volatile(unref_lhs));
      if (!type_name_is_const(unref_lhs)) {
        if (rhs.Is(unref_lhs))
          continue;  // &&{T} <- T
      }
    } else {
      if (!lhs.IsLValueReference()) {  // T
        if (lhs.Is(rhs.Name_RemoveRValueReference()))
          continue;  // T <- &&{T}
      }
    }

    return false;
  }

  return true;
}

bool details::IsNonCopiedArgConstructCompatible(
    std::span<const Type> params, std::span<const Type> argTypes) {
  if (params.size() != argTypes.size())
    return false;

  for (size_t i = 0; i < params.size(); i++) {
    if (params[i] == argTypes[i])
      continue;

    const auto& lhs = params[i];
    const auto& rhs = argTypes[i];

    if (lhs.IsLValueReference()) {  // &{T} | &{const{T}}
      const auto unref_lhs = lhs.Name_RemoveLValueReference();  // T | const{T}
      if (type_name_is_const(unref_lhs)) {                      // &{const{T}}
        if (unref_lhs == rhs.Name_RemoveRValueReference())
          continue;  // &{const{T}} <- &&{const{T}}

        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (rhs.Is(raw_lhs) || raw_lhs == rhs.Name_RemoveReference())
          continue;  // &{const{T}} <- T | &{T} | &&{T}
      }
    } else if (lhs.IsRValueReference()) {  // &&{T} | &&{const{T}}
      const auto unref_lhs = lhs.Name_RemoveRValueReference();  // T | const{T}
      assert(!type_name_is_volatile(unref_lhs));

      if (type_name_is_const(unref_lhs)) {                       // &&{const{T}}
        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (rhs.Is(raw_lhs))
          continue;  // &&{const{T}} <- T

        if (raw_lhs == rhs.Name_RemoveRValueReference())  // &&{const{T}}
          continue;  // &&{const{T}} <- &&{T}
      } else {
        if (rhs.Is(unref_lhs))
          continue;  // &&{T} <- T
      }
    } else {  // T
      if (lhs.Is(rhs.Name_RemoveRValueReference()))
        continue;  // T <- &&{T}
    }

    return false;
  }

  return true;
}

bool details::IsNonCopiedArgConstructCompatible(
    std::span<const Type> params, std::span<const TypeID> argTypeIDs) {
  if (params.size() != argTypeIDs.size())
    return false;

  for (size_t i = 0; i < params.size(); i++) {
    if (params[i] == argTypeIDs[i])
      continue;

    const auto& lhs = params[i];
#ifndef NDEBUG
    // because rhs(arg)'s ID maybe have no name in the registry
    // so we use type_name_add_*_hash(...) to avoid it
    auto rhs = Mngr.tregistry.Nameof(argTypeIDs[i]);
#endif  // !NDEBUG
    const std::size_t rhs_hash = argTypeIDs[i].GetValue();

    if (lhs.IsLValueReference()) {                        // &{T} | &{const{T}}
      auto unref_lhs = lhs.Name_RemoveLValueReference();  // T | const{T}
      if (type_name_is_const(unref_lhs)) {                // &{const{T}}
        if (type_name_add_rvalue_reference_hash(unref_lhs) == rhs_hash)
          continue;  // &{const{T}} <- &&{const{T}}

        auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (TypeID{raw_lhs}.GetValue() == rhs_hash ||
            type_name_add_lvalue_reference_hash(raw_lhs) == rhs_hash ||
            type_name_add_rvalue_reference_hash(raw_lhs) == rhs_hash)
          continue;  // &{const{T}} <- T | &{T} | &&{T}
      }
    } else if (lhs.IsRValueReference()) {  // &&{T} | &&{const{T}}
      auto unref_lhs = lhs.Name_RemoveRValueReference();  // T | const{T}
      assert(!type_name_is_volatile(unref_lhs));

      if (type_name_is_const(unref_lhs)) {                 // &&{const{T}}
        auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (TypeID{raw_lhs}.GetValue() == rhs_hash)
          continue;  // &&{const{T}} <- T

        if (type_name_add_rvalue_reference_hash(raw_lhs) ==
            rhs_hash)  // &&{const{T}}
          continue;    // &&{const{T}} <- &&{T}
      } else {
        if (TypeID{unref_lhs}.GetValue() == rhs_hash)
          continue;  // &&{T} <- T
      }
    } else {  // T
      if (type_name_add_rvalue_reference_hash(lhs) == rhs_hash)
        continue;  // T <- &&{T}
    }

    return false;
  }

  return true;
}

details::NewArgsGuard::NewArgsGuard(bool is_priority,
                                    std::pmr::memory_resource* rsrc,
                                    std::span<const Type> paramTypeIDs,
                                    std::span<const Type> argTypes,
                                    ArgPtrBuffer orig_argptr_buffer)
    : rsrc{rsrc} {
  if (argTypes.size() != paramTypeIDs.size())
    return;

  if (is_priority) {
    is_compatible = IsPriorityCompatible(paramTypeIDs, argTypes);
    argptr_buffer = orig_argptr_buffer;
    return;
  }

  // 1. is compatible ? (collect infos)

  const std::uint8_t num_args = static_cast<std::uint8_t>(argTypes.size());

  ArgInfo info_copiedargs[MaxArgNum + 1];
  std::uint8_t num_copiedargs = 0;
  std::uint32_t size_copiedargs = 0;

  for (std::uint8_t i = 0; i < argTypes.size(); i++) {
    if (paramTypeIDs[i] == argTypes[i])
      continue;

    const auto& lhs = paramTypeIDs[i];
    const auto& rhs = argTypes[i];

    if (lhs.IsLValueReference()) {  // &{T} | &{const{T}}
      const auto unref_lhs = lhs.Name_RemoveLValueReference();  // T | const{T}
      if (type_name_is_const(unref_lhs)) {                      // &{const{T}}
        if (unref_lhs == rhs.Name_RemoveRValueReference())
          continue;  // &{const{T}} <- &&{const{T}}

        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T
        if (rhs.Is(raw_lhs) || raw_lhs == rhs.Name_RemoveReference())
          continue;  // &{const{T}} <- T | &{T} | &&{T}

        assert(!type_name_is_pointer(raw_lhs));

        Type raw_lhs_type{raw_lhs};
        if (Mngr.IsNonCopiedArgConstructible(raw_lhs_type,
                                             std::span<const Type>{&rhs, 1})) {
          auto& info = info_copiedargs[num_copiedargs++];
          assert(num_copiedargs <= MaxArgNum);

          info.idx = i;
          info.is_ptr = false;
          info.name = raw_lhs_type.GetName().data();
          info.name_size =
              static_cast<std::uint16_t>(raw_lhs_type.GetName().size());
          info.name_hash = raw_lhs_type.GetID().GetValue();

          continue;  // &{const{T}} <- T{arg}
        }
      }
    } else if (lhs.IsRValueReference()) {  // &&{T} | &&{const{T}}
      const auto unref_lhs = lhs.Name_RemoveRValueReference();   // T | const{T}
      if (type_name_is_const(unref_lhs)) {                       // &&{const{T}}
        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (rhs.Is(raw_lhs))
          continue;  // &&{const{T}} <- T

        if (raw_lhs == rhs.Name_RemoveRValueReference())  // &&{const{T}}
          continue;  // &&{const{T}} <- &&{T}

        assert(!type_name_is_pointer(raw_lhs));

        Type raw_lhs_type{raw_lhs};
        if (Mngr.IsNonCopiedArgConstructible(raw_lhs_type,
                                             std::span<const Type>{&rhs, 1})) {
          auto& info = info_copiedargs[num_copiedargs++];
          assert(num_copiedargs <= MaxArgNum);

          info.idx = i;
          info.is_ptr = false;
          info.name = raw_lhs_type.GetName().data();
          info.name_size =
              static_cast<std::uint16_t>(raw_lhs_type.GetName().size());
          info.name_hash = raw_lhs_type.GetID().GetValue();

          continue;  // &&{const{T}} <- T{arg}
        }
      } else {  // &&{T}
        if (rhs.Is(unref_lhs))
          continue;
      }
    } else {  // T
      if (lhs.Is(rhs.Name_RemoveRValueReference()))
        continue;  // T <- &&{T}

      if ((lhs.IsPointer() || Mngr.IsCopyConstructible(lhs)) &&
          (lhs.Is(rhs.Name_RemoveLValueReference()) ||
           lhs.Is(rhs.Name_RemoveCVRef()))) {
        auto& info = info_copiedargs[num_copiedargs++];
        assert(num_copiedargs <= MaxArgNum);

        info.idx = i;
        info.is_ptr = type_name_is_pointer(lhs);
        info.name = lhs.GetName().data();
        info.name_size = static_cast<std::uint16_t>(lhs.GetName().size());
        info.name_hash = lhs.GetID().GetValue();

        continue;  // T <- T{arg} [copy]
      }

      if (Mngr.IsNonCopiedArgConstructible(lhs,
                                           std::span<const Type>{&rhs, 1})) {
        auto& info = info_copiedargs[num_copiedargs++];
        assert(num_copiedargs <= MaxArgNum);

        info.idx = i;
        info.is_ptr = false;
        info.name = lhs.GetName().data();
        info.name_size = static_cast<std::uint16_t>(lhs.GetName().size());
        info.name_hash = lhs.GetID().GetValue();

        continue;  // T <- T{arg}
      }
    }

    return;  // not compatible
  }

  is_compatible = true;

  if (num_copiedargs == 0) {
    argptr_buffer = orig_argptr_buffer;
    return;
  }

  // 2. compute offset and alignment

  for (std::uint8_t k = 0; k < num_copiedargs; ++k) {
    std::uint32_t size, alignment;
    if (info_copiedargs[k].is_ptr) {
      size = static_cast<std::uint32_t>(sizeof(void*));
      alignment = static_cast<std::uint32_t>(alignof(void*));
    } else {
      ++num_copied_nonptr_args;
      const auto& typeinfo = Mngr.typeinfos.at(info_copiedargs[k].GetType());
      size = static_cast<std::uint32_t>(typeinfo.size);
      alignment = static_cast<std::uint32_t>(typeinfo.alignment);
    }

    std::uint32_t offset =
        (size_copiedargs + (alignment - 1)) / alignment * alignment;
    info_copiedargs[k].offset = offset;
    size_copiedargs = offset + size;

    if (alignment > max_alignment)
      max_alignment = alignment;
  }

  // 3. fill buffer

  // buffer = copied args buffer + argptr buffer + non-ptr arg info buffer

  std::uint32_t offset_new_arg_buffer = 0;
  std::uint32_t offset_new_argptr_buffer =
      (size_copiedargs + alignof(void*) - 1) / alignof(void*) * alignof(void*);
  std::uint32_t offset_new_nonptr_arg_info_buffer =
      offset_new_argptr_buffer + num_args * sizeof(void*);

  buffer_size = offset_new_nonptr_arg_info_buffer +
                num_copied_nonptr_args * sizeof(ArgInfo);

  buffer = rsrc->allocate(buffer_size, max_alignment);

  auto new_arg_buffer = forward_offset(buffer, offset_new_arg_buffer);
  auto new_argptr_buffer = reinterpret_cast<void**>(
      forward_offset(buffer, offset_new_argptr_buffer));
  new_nonptr_arg_info_buffer = reinterpret_cast<ArgInfo*>(
      forward_offset(buffer, offset_new_nonptr_arg_info_buffer));

  info_copiedargs[num_copiedargs].idx = static_cast<std::uint8_t>(-1);  // guard
  std::uint8_t idx_copiedargs = 0, idx_nonptr_args = 0;
  for (std::uint8_t i = 0; i < num_args; i++) {
    const auto& info = info_copiedargs[idx_copiedargs];
    if (i < info.idx) {
      new_argptr_buffer[i] = orig_argptr_buffer[i];
      continue;
    }
    assert(i == info.idx);

    void* arg_buffer = forward_offset(new_arg_buffer, info.offset);
    new_argptr_buffer[i] = arg_buffer;

    // copy
    if (info.is_ptr)
      buffer_as<void*>(arg_buffer) = orig_argptr_buffer[i];
    else {
      bool success = Mngr.NonCopiedArgConstruct(
          ObjectView{info.GetType(), arg_buffer},
          std::span<const Type>{&argTypes[i], 1},
          static_cast<ArgPtrBuffer>(&orig_argptr_buffer[i]));
      assert(success);
      new_nonptr_arg_info_buffer[idx_nonptr_args++] = info;
    }

    ++idx_copiedargs;
  }
  assert(idx_copiedargs == num_copiedargs);
  assert(idx_nonptr_args == num_copied_nonptr_args);

  argptr_buffer = new_argptr_buffer;
}

details::NewArgsGuard::~NewArgsGuard() {
  if (buffer) {
    for (const auto& info : std::span<const ArgInfo>{new_nonptr_arg_info_buffer,
                                                     num_copied_nonptr_args}) {
      bool success = Mngr.Destruct({info.GetType(), argptr_buffer[info.idx]});
      assert(success);
    }
    rsrc->deallocate(buffer, buffer_size, max_alignment);
  }
}