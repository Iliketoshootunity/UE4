#pragma once

template <typename T>
struct SIsPointer
{
	enum { Value = false };
};

template <typename T> struct SIsPointer<               T*> { enum { Value = true }; };
template <typename T> struct SIsPointer<const          T*> { enum { Value = true }; };
template <typename T> struct SIsPointer<      volatile T*> { enum { Value = true }; };
template <typename T> struct SIsPointer<const volatile T*> { enum { Value = true }; };

template <typename T> struct SIsPointer<const          T> { enum { Value = SIsPointer<T>::Value }; };
template <typename T> struct SIsPointer<      volatile T> { enum { Value = SIsPointer<T>::Value }; };
template <typename T> struct SIsPointer<const volatile T> { enum { Value = SIsPointer<T>::Value }; };