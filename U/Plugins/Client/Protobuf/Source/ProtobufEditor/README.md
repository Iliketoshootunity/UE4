# UE4 plugins Protobuf

A protobuf source integration for UE4.

直接在项目中集成 `libprotobuf` 的源码，方便使用

## 测试平台 ##

* 已测试通过
	* `Win32`, `Win64`
	* `Android`
	* `MAC`
	* `iOS`

* 未测试
	* `Linux`

## 如何使用 ##

* 将 `Source\Protobuf` 文件夹复制到自己的项目的`Source`目录下
* 修改自己项目的 `.build.cs` 文件，在 `PublicDependencyModuleNames` 中增加 `Protobuf` 模块，并增加 `bEnableShadowVariableWarnings = false; bEnableUndefinedIdentifierWarnings = false;`，禁掉警告

## 以下记录完整步骤 ##

### 提取 libprotobuf 源码 ###

* 到 [protobuf releases](https://github.com/google/protobuf/releases)下载 `protobuf-cpp` 源码包
* 用 `CMake` 生成 `Visual Studio` 工程

``` bash
cd /D protobuf-3.8.0\cmake

cmake.exe -G "Visual Studio 16 2019 Win64" .
```

* 将工程 `libprotobuf` 中用到的源码文件和对应的头文件提取出来

### 修改 libprotobuf 源码 ###

修改文件：google/protobuf/port_def.inc

1、修改 PROTOBUF_EXPORT
#if defined(_MSC_VER) && defined(PROTOBUF_USE_DLLS)
#ifdef LIBPROTOBUF_EXPORTS
#define PROTOBUF_EXPORT __declspec(dllexport)
#else
#define PROTOBUF_EXPORT __declspec(dllimport)
#endif
#ifdef LIBPROTOC_EXPORTS
#define PROTOC_EXPORT __declspec(dllexport)
#else
#define PROTOC_EXPORT __declspec(dllimport)
#endif
#else
#define PROTOBUF_EXPORT
#define PROTOC_EXPORT
#endif

改为：
#if defined(_MSC_VER) && defined(PROTOBUF_USE_DLLS)
#ifdef LIBPROTOBUF_EXPORTS
#define PROTOBUF_EXPORT __declspec(dllexport)
#else
#define PROTOBUF_EXPORT __declspec(dllimport)
#endif
#ifdef LIBPROTOC_EXPORTS
#define PROTOC_EXPORT __declspec(dllexport)
#else
#define PROTOC_EXPORT __declspec(dllimport)
#endif
#else
#define PROTOBUF_EXPORT PROTOBUF_API
#define PROTOC_EXPORT
#endif

2、修改文件：google/protobuf/arena_impl.h
原代码（行：257）：
#if defined(GOOGLE_PROTOBUF_NO_THREADLOCAL)
  // Android ndk does not support GOOGLE_THREAD_LOCAL keyword so we use a custom thread
  // local storage class we implemented.
  // iOS also does not support the GOOGLE_THREAD_LOCAL keyword.
  static ThreadCache& thread_cache();
#elif defined(PROTOBUF_USE_DLLS)
  // Thread local variables cannot be exposed through DLL interface but we can
  // wrap them in static functions.
  static ThreadCache& thread_cache();
#else
  static GOOGLE_THREAD_LOCAL ThreadCache thread_cache_;
  static ThreadCache& thread_cache() { return thread_cache_; }
#endif
改为：
#if defined(GOOGLE_PROTOBUF_NO_THREADLOCAL)
  // Android ndk does not support GOOGLE_THREAD_LOCAL keyword so we use a custom thread
  // local storage class we implemented.
  // iOS also does not support the GOOGLE_THREAD_LOCAL keyword.
  static ThreadCache& thread_cache();
#elif defined(PROTOBUF_API)
  // Thread local variables cannot be exposed through DLL interface but we can
  // wrap them in static functions.
  static ThreadCache& thread_cache();
#else
  static GOOGLE_THREAD_LOCAL ThreadCache thread_cache_;
  static ThreadCache& thread_cache() { return thread_cache_; }
#endif

3、修改文件：google/protobuf/arena.cc
原代码（行：60）：
#if defined(GOOGLE_PROTOBUF_NO_THREADLOCAL)
ArenaImpl::ThreadCache& ArenaImpl::thread_cache() {
  static internal::ThreadLocalStorage<ThreadCache>* thread_cache_ =
      new internal::ThreadLocalStorage<ThreadCache>();
  return *thread_cache_->Get();
}
#elif defined(PROTOBUF_USE_DLLS)
ArenaImpl::ThreadCache& ArenaImpl::thread_cache() {
  static GOOGLE_THREAD_LOCAL ThreadCache thread_cache_ = {-1, NULL};
  return thread_cache_;
}
#else
GOOGLE_THREAD_LOCAL ArenaImpl::ThreadCache ArenaImpl::thread_cache_ = {-1, NULL};
#endif
改为：
#if defined(GOOGLE_PROTOBUF_NO_THREADLOCAL)
ArenaImpl::ThreadCache& ArenaImpl::thread_cache() {
  static internal::ThreadLocalStorage<ThreadCache>* thread_cache_ =
      new internal::ThreadLocalStorage<ThreadCache>();
  return *thread_cache_->Get();
}
#elif defined(PROTOBUF_API)
ArenaImpl::ThreadCache& ArenaImpl::thread_cache() {
  static GOOGLE_THREAD_LOCAL ThreadCache thread_cache_ = {-1, NULL};
  return thread_cache_;
}
#else
GOOGLE_THREAD_LOCAL ArenaImpl::ThreadCache ArenaImpl::thread_cache_ = {-1, NULL};
#endif

4、port_def.inc 文件末尾添加警告消除
#if PLATFORM_WINDOWS
#pragma warning(disable: 4018 4065 4146 4244 4251 4267 4305 4307 4309 4334 4355 4506 4800 4996)
#pragma warning(disable: 4946 4910 4310 4457 4701 4703 4125 4661)
#endif


### Project.Build.cs ###

bEnableShadowVariableWarnings = false;
bEnableUndefinedIdentifierWarnings = false;

PublicDefinitions.Add("HAVE_PTHREAD");
PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI");
PublicDefinitions.Add("PROTOBUF_DISABLE_LITTLE_ENDIAN_OPT_FOR_TEST");