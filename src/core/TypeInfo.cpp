//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/TypeInfo.h>

#include <MyDRefl/ReflMngr.h>

using namespace My::MyDRefl;

ObjectPtr TypeInfo::RWVar(StrID fieldID) noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (!target->second.fieldptr.IsObject())
    return nullptr;

  return target->second.fieldptr.Map();
}

ConstObjectPtr TypeInfo::RVar(StrID fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (!target->second.fieldptr.IsObject())
    return nullptr;

  return target->second.fieldptr.Map();
}

ObjectPtr TypeInfo::RWVar(void* obj, StrID fieldID) noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (target->second.fieldptr.IsConst())
    return nullptr;

  return target->second.fieldptr.Map(obj);
}

ConstObjectPtr TypeInfo::RVar(const void* obj, StrID fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  return target->second.fieldptr.Map(obj);
}

bool TypeInfo::IsStaticInvocable(StrID methodID,
                                 Span<TypeID> argTypeIDs) const noexcept {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.methodptr.IsStatic() &&
        target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs))
      return true;
  }
  return false;
}

bool TypeInfo::IsConstInvocable(StrID methodID,
                                Span<TypeID> argTypeIDs) const noexcept {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (!target->second.methodptr.IsMemberVariable() &&
        target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs))
      return true;
  }
  return false;
}

bool TypeInfo::IsInvocable(StrID methodID,
                           Span<TypeID> argTypeIDs) const noexcept {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs))
      return true;
  }
  return false;
}

InvokeResult TypeInfo::Invoke(StrID methodID, Span<TypeID> argTypeIDs,
                              void* args_buffer, void* result_buffer) const {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.methodptr.IsStatic() &&
        target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs)) {
      return {
          true, target->second.methodptr.GetResultDesc().typeID,
          target->second.methodptr.Invoke_Static(args_buffer, result_buffer)};
    }
  }
  return {};
}

InvokeResult TypeInfo::Invoke(const void* obj, StrID methodID,
                              Span<TypeID> argTypeIDs, void* args_buffer,
                              void* result_buffer) const {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (!target->second.methodptr.IsMemberVariable() &&
        target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs)) {
      return {true, target->second.methodptr.GetResultDesc().typeID,
              target->second.methodptr.Invoke(obj, args_buffer, result_buffer)};
    }
  }
  return {};
}

InvokeResult TypeInfo::Invoke(void* obj, StrID methodID,
                              Span<TypeID> argTypeIDs, void* args_buffer,
                              void* result_buffer) const {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);

  {  // first: object variable and static
    auto iter = target;
    for (size_t i = 0; i < num; ++i, ++iter) {
      if (!iter->second.methodptr.IsMemberConst() &&
          iter->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs)) {
        return {true, iter->second.methodptr.GetResultDesc().typeID,
                iter->second.methodptr.Invoke(obj, args_buffer, result_buffer)};
      }
    }
  }

  {  // second: object const
    auto iter = target;
    for (size_t i = 0; i < num; ++i, ++iter) {
      if (iter->second.methodptr.IsMemberConst() &&
          iter->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs)) {
        return {true, iter->second.methodptr.GetResultDesc().typeID,
                iter->second.methodptr.Invoke(obj, args_buffer, result_buffer)};
      }
    }
  }

  return {};
}

//SharedObject TypeInfo::Invoke(StrID methodID, Span<TypeID> argTypeIDs, void* args_buffer,
//	std::shared_ptr<std::pmr::memory_resource> result_rsrc) const {
//	auto target = methodinfos.find(methodID);
//	size_t num = methodinfos.count(methodID);
//	for (size_t i = 0; i < num; ++i, ++target) {
//		if (target->second.methodptr.IsStatic()
//			&& target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs))
//		{
//			const auto& rst_desc = target->second.methodptr.GetResultDesc();
//			void* result_buffer = result_rsrc->allocate(rst_desc.size, rst_desc.alignment);
//			auto dtor = target->second.methodptr.Invoke_Static(args_buffer, result_buffer);
//			if (dtor) {
//				return {
//					{rst_desc.typeID, result_buffer},
//					[dtor = std::move(dtor), result_rsrc = std::move(result_rsrc), size = rst_desc.size, alignment = rst_desc.alignment]
//					(void* result_buffer) {
//						dtor(result_buffer);
//						result_rsrc->deallocate(result_buffer, size, alignment);
//					}
//				};
//			}
//			else {
//				return {
//					{rst_desc.typeID, result_buffer},
//					[result_rsrc = std::move(result_rsrc), size = rst_desc.size, alignment = rst_desc.alignment]
//					(void* result_buffer) {
//						result_rsrc->deallocate(result_buffer, size, alignment);
//					}
//				};
//			}
//		}
//	}
//	return {};
//}
//
//SharedObject TypeInfo::Invoke(const void* obj, StrID methodID, Span<TypeID> argTypeIDs, void* args_buffer,
//	std::shared_ptr<std::pmr::memory_resource> result_rsrc) const {
//	auto target = methodinfos.find(methodID);
//	size_t num = methodinfos.count(methodID);
//	for (size_t i = 0; i < num; ++i, ++target) {
//		if (!target->second.methodptr.IsMemberVariable()
//			&& target->second.methodptr.GetParamList().IsConpatibleWith(argTypeIDs))
//		{
//			const auto& rst_desc = target->second.methodptr.GetResultDesc();
//			void* result_buffer = result_rsrc->allocate(rst_desc.size, rst_desc.alignment);
//			auto dtor = target->second.methodptr.Invoke(obj, args_buffer, result_buffer);
//			if (dtor) {
//				return {
//					{rst_desc.typeID, result_buffer},
//					[dtor = std::move(dtor), result_rsrc = std::move(result_rsrc), size = rst_desc.size, alignment = rst_desc.alignment]
//					(void* result_buffer) {
//						dtor(result_buffer);
//						result_rsrc->deallocate(result_buffer, size, alignment);
//					}
//				};
//			}
//			else {
//				return {
//					{rst_desc.typeID, result_buffer},
//					[result_rsrc = std::move(result_rsrc), size = rst_desc.size, alignment = rst_desc.alignment]
//					(void* result_buffer) {
//						result_rsrc->deallocate(result_buffer, size, alignment);
//					}
//				};
//			}
//		}
//	}
//	return {};
//}
//
//// without bases, non-const + const + static, non-const && static > const
//SharedObject TypeInfo::Invoke(void* obj, StrID methodID, Span<TypeID> argTypeIDs, void* args_buffer,
//	std::shared_ptr<std::pmr::memory_resource> result_rsrc) const {
//
//}
