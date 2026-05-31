/**
 * @file material_asset.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <string.h>
#include <acsu/material/material_asset.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
material_asset_def(void *ptr)
{
  assert(ptr);

  {
    material_asset_t *material = (material_asset_t *)ptr;
    memset(material, 0, sizeof(material_asset_t));
  }
}

uint32_t
material_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const material_asset_t *material = (const material_asset_t *)ptr;
    material_asset_t def;
    material_asset_def(&def);
    return !memcmp(material, &def, sizeof(material_asset_t));
  }
}

static
void
texture_asset_properties_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const texture_properties_t *tex_prop = (const texture_properties_t *)src;
    asset_ref_serialize(&tex_prop->texture_ref, stream);
    binary_stream_write(stream, &tex_prop->angle, sizeof(float));
    binary_stream_write(stream, &tex_prop->u, sizeof(float));
    binary_stream_write(stream, &tex_prop->u_scale, sizeof(float));
    binary_stream_write(stream, &tex_prop->v, sizeof(float));
    binary_stream_write(stream, &tex_prop->v_scale, sizeof(float));
  }
}

void
material_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const material_asset_t *material = (const material_asset_t *)src;
    cstring_serialize(&material->name, stream);
    binary_stream_write(stream, &material->ambient, sizeof(mat_rgba_t));
    binary_stream_write(stream, &material->diffuse, sizeof(mat_rgba_t));
    binary_stream_write(stream, &material->specular, sizeof(mat_rgba_t));
    binary_stream_write(stream, &material->shininess, sizeof(float));
    binary_stream_write(stream, &material->opacity, sizeof(float));
    cvector_serialize_func(
      &material->textures, stream, texture_asset_properties_serialize);
  }
}

static
void
texture_asset_properties_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t s_f = sizeof(float);
    texture_properties_t *tex_prop = (texture_properties_t *)dst;
    asset_ref_deserialize(&tex_prop->texture_ref, allocator, stream);
    binary_stream_read(stream, (uint8_t *)&tex_prop->angle, s_f, s_f);
    binary_stream_read(stream, (uint8_t *)&tex_prop->u, s_f, s_f);
    binary_stream_read(stream, (uint8_t *)&tex_prop->u_scale, s_f, s_f);
    binary_stream_read(stream, (uint8_t *)&tex_prop->v, s_f, s_f);
    binary_stream_read(stream, (uint8_t *)&tex_prop->v_scale, s_f, s_f);
  }
}

void
material_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t s_c = sizeof(mat_rgba_t);
    size_t s_f = sizeof(float);
    material_asset_t *material = (material_asset_t *)dst;
    cstring_def(&material->name);
    cstring_deserialize(&material->name, allocator, stream);
    binary_stream_read(stream, (uint8_t *)&material->ambient, s_c, s_c);
    binary_stream_read(stream, (uint8_t *)&material->diffuse, s_c, s_c);
    binary_stream_read(stream, (uint8_t *)&material->specular, s_c, s_c);
    binary_stream_read(stream, (uint8_t *)&material->shininess, s_f, s_f);
    binary_stream_read(stream, (uint8_t *)&material->opacity, s_f, s_f);
    cvector_deserialize_func(
      &material->textures,
      allocator, stream, texture_asset_properties_deserialize);
  }
}

size_t
material_asset_type_size(void)
{
  return sizeof(material_asset_t);
}

uint32_t
material_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
material_asset_get_alloc(const void *ptr)
{
  return NULL;
}

static
void
texture_asset_properties_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && allocator);

  {
    texture_properties_t *tex_prop = (texture_properties_t *)ptr;
    asset_ref_cleanup(&tex_prop->texture_ref, allocator);
  }
}

void
material_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !material_asset_is_def(ptr));
  assert(allocator);

  {
    material_asset_t *material = (material_asset_t *)ptr;
    cstring_cleanup2(&material->name);
    cvector_cleanup_func(
      &material->textures,
      material->textures.allocator,
      texture_asset_properties_cleanup);
    material_asset_def(material);
  }
}

const char *
material_asset_get_dir(void)
{
  static const char *directory = "materials";
  return directory;
}

static
void
material_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(material_asset_t));

  {
    material_asset_t **ptr = (material_asset_t **)ptr_addr;
    material_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(material_asset_t));
    material_asset_def(asset_ptr);
    material_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
material_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(material_asset_t));

  {
    material_asset_t **ptr = (material_asset_t **)ptr_addr;
    material_asset_t *asset_ptr = *ptr;
    material_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
material_asset_get_loader(void)
{
  return material_asset_loader;
}

deloader_t
material_asset_get_deloader(void)
{
  return material_asset_deloader;
}

uint32_t
material_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const material_asset_t *material = src;
    return (uint32_t)material->textures.size;
  }

  return 0;
}

void
material_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const material_asset_t *material = src;
    for (uint32_t i = 0; i < material->textures.size; ++i) {
      const texture_properties_t *tex_prop = cvector_as_c(
        &material->textures, i, texture_properties_t);
      refs[i] = &tex_prop->texture_ref;
    }
  }
}

uint32_t
material_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_material_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = material_asset_def;
  vtable.fn_is_def = material_asset_is_def;
  vtable.fn_serialize = material_asset_serialize;
  vtable.fn_deserialize = material_asset_deserialize;
  vtable.fn_type_size = material_asset_type_size;
  vtable.fn_owns_alloc = material_asset_owns_alloc;
  vtable.fn_get_alloc = material_asset_get_alloc;
  vtable.fn_cleanup = material_asset_cleanup;
  vtable.fn_get_dir = material_asset_get_dir;
  vtable.fn_get_loader = material_asset_get_loader;
  vtable.fn_get_deloader = material_asset_get_deloader;
  vtable.fn_type_asset_count = material_asset_type_asset_count;
  vtable.fn_type_get_assets = material_asset_type_get_assets;
  vtable.fn_is_asset_type = material_asset_is_asset_type;
  register_type(get_type_id(material_asset_t), &vtable);
}