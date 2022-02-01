#pragma once



typedef unsigned uint32;
typedef __int32 int32;

#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof(*Array))
#define ARRAY_MATCH(RefArray, OtherArray) static_assert(ARRAY_SIZE(RefArray) == ARRAY_SIZE(OtherArray), "Array size mismatch!")

#define DECLARE_NOCOPY(ClassName) \
ClassName& operator= (const ClassName&) = delete; \
ClassName(const ClassName&) = delete
#define DECLARE_NOMOVE(ClassName) \
ClassName& operator= (ClassName&&) = delete; \
ClassName(ClassName&&) = delete

#define DECLARE_COPY(ClassName) \
ClassName& operator= (const ClassName&); \
ClassName(const ClassName&)
#define DECLARE_MOVE(ClassName) \
ClassName& operator= (ClassName&&) noexcept; \
ClassName(ClassName&&) noexcept

#define IMPLEMENT_COPY(ClassName) \
ClassName& ClassName::operator= (const ClassName&) = default; \
ClassName::ClassName(const ClassName&) = default
#define IMPLEMENT_MOVE(ClassName) \
ClassName& ClassName::operator= (ClassName&&) noexcept = default; \
ClassName::ClassName(ClassName&&) noexcept = default

#define FunctionPointer(Type, VarName, ...) Type (*VarName) (__VA_ARGS__)
#define FloatConsumer(Name) FunctionPointer(float, Name, float)

template <class Type>
inline Type&& Move(Type& Moved) noexcept
{
    return static_cast<Type&&>(Moved);
}



