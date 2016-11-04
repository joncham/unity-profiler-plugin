MONO_FUNC (void, mono_profiler_install, (MonoProfiler *prof, MonoProfileFunc shutdown_callback))
MONO_FUNC (void, mono_profiler_set_events, (MonoProfileFlags events))
MONO_FUNC (void, mono_profiler_install_allocation, (MonoProfileAllocFunc callback))

MONO_FUNC (const char*, mono_class_get_name, (MonoClass *klass))
MONO_FUNC (const char*, mono_class_get_namespace, (MonoClass *klass))
MONO_FUNC (int32_t, mono_class_instance_size, (MonoClass *klass))

MONO_FUNC (MonoThread*, mono_thread_current, ())
MONO_FUNC (MonoClass*, mono_object_get_class, (MonoObject *obj))
MONO_FUNC (MonoClassField*, mono_class_get_field_from_name, (MonoClass *klass, const char *name))
MONO_FUNC (uint32_t, mono_field_get_offset, (MonoClassField *field))
