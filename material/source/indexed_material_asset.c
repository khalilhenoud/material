/**
 * @file indexed_material_asset.c
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
#include <material/indexed_material_asset.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
indexed_material_asset_def(void *ptr)
{
  assert(ptr);

  {
    indexed_material_asset_t *indexed = (indexed_material_asset_t *)ptr;
    memset(indexed, 0, sizeof(indexed_material_asset_t));
  }
}

uint32_t
indexed_material_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const indexed_material_asset_t *indexed =
      (const indexed_material_asset_t *)ptr;
    indexed_material_asset_t def;
    indexed_material_asset_def(&def);
    return !memcmp(indexed, &def, sizeof(indexed_material_asset_t));
  }
}

void
indexed_material_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const indexed_material_asset_t *indexed =
      (const indexed_material_asset_t *)src;
    asset_ref_serialize(&indexed->bulk_material_ref, stream);
    binary_stream_write(stream, &indexed->index, sizeof(uint32_t));
  }
}

void
indexed_material_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t s_u = sizeof(uint32_t);
    indexed_material_asset_t *indexed = (indexed_material_asset_t *)dst;
    asset_ref_deserialize(&indexed->bulk_material_ref, allocator, stream);
    binary_stream_read(stream, (uint8_t *)&indexed->index, s_u, s_u);
  }
}

size_t
indexed_material_asset_type_size(void)
{
  return sizeof(indexed_material_asset_t);
}

uint32_t
indexed_material_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
indexed_material_asset_get_alloc(const void *ptr)
{
  return NULL;
}

void
indexed_material_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !indexed_material_asset_is_def(ptr));
  assert(allocator);

  {
    indexed_material_asset_t *indexed = (indexed_material_asset_t *)ptr;
    asset_ref_cleanup(&indexed->bulk_material_ref, allocator);
  }
}

const char *
indexed_material_asset_get_dir(void)
{
  static const char *directory = "materials";
  return directory;
}

static
void
indexed_material_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(indexed_material_asset_t));

  {
    indexed_material_asset_t **ptr = (indexed_material_asset_t **)ptr_addr;
    indexed_material_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(indexed_material_asset_t));
    indexed_material_asset_def(asset_ptr);
    indexed_material_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
indexed_material_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(indexed_material_asset_t));

  {
    indexed_material_asset_t **ptr = (indexed_material_asset_t **)ptr_addr;
    indexed_material_asset_t *asset_ptr = *ptr;
    indexed_material_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
indexed_material_asset_get_loader(void)
{
  return indexed_material_asset_loader;
}

deloader_t
indexed_material_asset_get_deloader(void)
{
  return indexed_material_asset_deloader;
}

uint32_t
indexed_material_asset_type_asset_count(const void *src)
{
  assert(src);
  return 1;
}

void
indexed_material_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const indexed_material_asset_t *indexed = src;
    refs[0] = &indexed->bulk_material_ref;
  }
}

uint32_t
indexed_material_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_indexed_material_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = indexed_material_asset_def;
  vtable.fn_is_def = indexed_material_asset_is_def;
  vtable.fn_serialize = indexed_material_asset_serialize;
  vtable.fn_deserialize = indexed_material_asset_deserialize;
  vtable.fn_type_size = indexed_material_asset_type_size;
  vtable.fn_owns_alloc = indexed_material_asset_owns_alloc;
  vtable.fn_get_alloc = indexed_material_asset_get_alloc;
  vtable.fn_cleanup = indexed_material_asset_cleanup;
  vtable.fn_get_dir = indexed_material_asset_get_dir;
  vtable.fn_get_loader = indexed_material_asset_get_loader;
  vtable.fn_get_deloader = indexed_material_asset_get_deloader;
  vtable.fn_type_asset_count = indexed_material_asset_type_asset_count;
  vtable.fn_type_get_assets = indexed_material_asset_type_get_assets;
  vtable.fn_is_asset_type = indexed_material_asset_is_asset_type;
  register_type(get_type_id(indexed_material_asset_t), &vtable);
}