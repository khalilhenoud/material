/**
 * @file bulk_material_asset.c
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
#include <material/bulk_material_asset.h>
#include <library/allocator/allocator.h>
#include <library/asset/asset_ref.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
bulk_material_asset_def(void *ptr)
{
  assert(ptr);

  {
    bulk_material_asset_t *bulk = (bulk_material_asset_t *)ptr;
    memset(bulk, 0, sizeof(bulk_material_asset_t));
  }
}

uint32_t
bulk_material_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const bulk_material_asset_t *bulk = (const bulk_material_asset_t *)ptr;
    bulk_material_asset_t def;
    bulk_material_asset_def(&def);
    return !memcmp(bulk, &def, sizeof(bulk_material_asset_t));
  }
}

void
bulk_material_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const bulk_material_asset_t *bulk = (const bulk_material_asset_t *)src;
    cvector_serialize(&bulk->materials, stream);
  }
}

void
bulk_material_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    bulk_material_asset_t *bulk = (bulk_material_asset_t *)dst;
    cvector_deserialize(&bulk->materials, allocator, stream);
  }
}

size_t
bulk_material_asset_type_size(void)
{
  return sizeof(bulk_material_asset_t);
}

uint32_t
bulk_material_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
bulk_material_asset_get_alloc(const void *ptr)
{
  return NULL;
}

void
bulk_material_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !bulk_material_asset_is_def(ptr));
  assert(allocator);

  {
    bulk_material_asset_t *bulk = (bulk_material_asset_t *)ptr;
    cvector_cleanup2(&bulk->materials);
    bulk_material_asset_def(bulk);
  }
}

const char *
bulk_material_asset_get_dir(void)
{
  static const char *directory = "materials";
  return directory;
}

static
void
bulk_material_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(bulk_material_asset_t));

  {
    bulk_material_asset_t **ptr = (bulk_material_asset_t **)ptr_addr;
    bulk_material_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(bulk_material_asset_t));
    bulk_material_asset_def(asset_ptr);
    bulk_material_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
bulk_material_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(bulk_material_asset_t));

  {
    bulk_material_asset_t **ptr = (bulk_material_asset_t **)ptr_addr;
    bulk_material_asset_t *asset_ptr = *ptr;
    bulk_material_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
bulk_material_asset_get_loader(void)
{
  return bulk_material_asset_loader;
}

deloader_t
bulk_material_asset_get_deloader(void)
{
  return bulk_material_asset_deloader;
}

uint32_t
bulk_material_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const bulk_material_asset_t *bulk = src;
    const material_asset_t *material = NULL;
    uint32_t total = 0;
    for (uint32_t i = 0; i < bulk->materials.size; ++i) {
      material = cvector_as_c(&bulk->materials, i, material_asset_t);
      total += material_asset_type_asset_count(material);
    }
    return total;
  }

  return 0;
}

void
bulk_material_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const bulk_material_asset_t *bulk = src;
    const material_asset_t *material = NULL;
    uint32_t total = 0;
    for (uint32_t i = 0; i < bulk->materials.size; ++i) {
      material = cvector_as_c(&bulk->materials, i, material_asset_t);
      material_asset_type_get_assets(material, refs + total);
      total += material_asset_type_asset_count(material);
    }
  }
}

uint32_t
bulk_material_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_bulk_material_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = bulk_material_asset_def;
  vtable.fn_is_def = bulk_material_asset_is_def;
  vtable.fn_serialize = bulk_material_asset_serialize;
  vtable.fn_deserialize = bulk_material_asset_deserialize;
  vtable.fn_type_size = bulk_material_asset_type_size;
  vtable.fn_owns_alloc = bulk_material_asset_owns_alloc;
  vtable.fn_get_alloc = bulk_material_asset_get_alloc;
  vtable.fn_cleanup = bulk_material_asset_cleanup;
  vtable.fn_get_dir = bulk_material_asset_get_dir;
  vtable.fn_get_loader = bulk_material_asset_get_loader;
  vtable.fn_get_deloader = bulk_material_asset_get_deloader;
  vtable.fn_type_asset_count = bulk_material_asset_type_asset_count;
  vtable.fn_type_get_assets = bulk_material_asset_type_get_assets;
  vtable.fn_is_asset_type = bulk_material_asset_is_asset_type;
  register_type(get_type_id(bulk_material_asset_t), &vtable);
}