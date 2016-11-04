// AllocationProfiler.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ProfilerPlugin.h"
#include <string>

/* Mono APIs */

typedef enum {
	MONO_PROFILE_NONE = 0,
	MONO_PROFILE_APPDOMAIN_EVENTS = 1 << 0,
	MONO_PROFILE_ASSEMBLY_EVENTS = 1 << 1,
	MONO_PROFILE_MODULE_EVENTS = 1 << 2,
	MONO_PROFILE_CLASS_EVENTS = 1 << 3,
	MONO_PROFILE_JIT_COMPILATION = 1 << 4,
	MONO_PROFILE_INLINING = 1 << 5,
	MONO_PROFILE_EXCEPTIONS = 1 << 6,
	MONO_PROFILE_ALLOCATIONS = 1 << 7,
	MONO_PROFILE_GC = 1 << 8,
	MONO_PROFILE_THREADS = 1 << 9,
	MONO_PROFILE_REMOTING = 1 << 10,
	MONO_PROFILE_TRANSITIONS = 1 << 11,
	MONO_PROFILE_ENTER_LEAVE = 1 << 12,
	MONO_PROFILE_COVERAGE = 1 << 13,
	MONO_PROFILE_INS_COVERAGE = 1 << 14,
	MONO_PROFILE_STATISTICAL = 1 << 15,
	MONO_PROFILE_METHOD_EVENTS = 1 << 16,
	MONO_PROFILE_MONITOR_EVENTS = 1 << 17,
	MONO_PROFILE_IOMAP_EVENTS = 1 << 18, /* this should likely be removed, too */
	MONO_PROFILE_GC_MOVES = 1 << 19
} MonoProfileFlags;


struct MonoProfiler;
struct MonoObject;
struct MonoClass;
typedef void (*MonoProfileFunc) (MonoProfiler *prof);
typedef void (*MonoProfileAllocFunc) (MonoProfiler *prof, MonoObject *obj, MonoClass *klass);

struct MonoThread;
struct MonoClassField;

#define MONO_FUNC(ret, name, sig) \
	typedef ret (*name##_func) sig; \
	static name##_func name;
#include "MonoFuncs.h"
#undef MONO_FUNC

static bool initialized;

static void AllocationCallback (MonoProfiler *prof, MonoObject *obj, MonoClass *klass);

PROFILERPLUGIN_API int Initialize (void)
{
	if (initialized)
		return 0;

	HMODULE hMono = LoadLibrary (L"mono.dll");

#define MONO_FUNC(ret, name, sig) \
	name = (name##_func)GetProcAddress(hMono, #name);
#include "MonoFuncs.h"
#undef MONO_FUNC

	MonoThread* thread = mono_thread_current ();
	MonoClass* threadClass = mono_object_get_class ((MonoObject*)thread);
	MonoClassField* managedIdField = mono_class_get_field_from_name (threadClass, "managed_id");
	uint32_t managedIdOffset = mono_field_get_offset (managedIdField);

	int32_t managedId = *(int32_t*)((char*)thread + managedIdOffset);

	mono_profiler_install (NULL, NULL);
	mono_profiler_install_allocation (AllocationCallback);
	mono_profiler_set_events (MONO_PROFILE_ALLOCATIONS);

	::MessageBoxA (NULL, "Hello World!", "Allocation Profilers", MB_OK);
	return 1;
}

static void AllocationCallback (MonoProfiler *prof, MonoObject *obj, MonoClass *klass)
{
	char buffer[1024];

	const char* ns = mono_class_get_namespace (klass);
	if (ns == NULL)
		ns = "<global>";
	const char* n = mono_class_get_name (klass);
	int32_t size = mono_class_instance_size (klass);
	snprintf (buffer, sizeof (buffer), "Thread: %d Allocating: %s.%s Size: %d\n", GetCurrentThreadId (), ns, n, size);

	OutputDebugStringA (buffer);
}
