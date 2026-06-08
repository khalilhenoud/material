/**
 * @file bulk_material_asset.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef BULK_MATERIAL_ASSET_H
#define BULK_MATERIAL_ASSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <material/internal/module.h>
#include <material/material_asset.h>
#include <library/asset/types.h>
#include <library/containers/cvector.h>


////////////////////////////////////////////////////////////////////////////////
//| bulk_material_asset_t, '*' = bulk_material_asset
//|=============================================================================
//| OPERATION                   | SUPPORTED
//|=============================================================================
//|    *_def                    | YES
//|    *_is_def                 | YES
//|    *_replicate              |
//|    *_fullswap               |
//|    *_serialize              | YES
//|    *_deserialize            | YES
//|    *_hash                   |
//|    *_is_equal               |
//|    *_type_size              | YES
//|    *_type_alignment         |
//|    *_type_id_count          |
//|    *_type_ids               |
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
//|    *_get_dir                | YES
//|    *_get_loader             | YES
//|    *_get_deloader           | YES
//|    *_type_asset_count       | YES
//|    *_type_get_assets        | YES
//|    *_is_asset_type          | YES
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct bulk_material_asset_t {
  cvector_t materials;
} bulk_material_asset_t;

MATERIAL_API
void
bulk_material_asset_def(void *ptr);

MATERIAL_API
uint32_t
bulk_material_asset_is_def(const void *ptr);

MATERIAL_API
void
bulk_material_asset_serialize(
  const void *src,
  binary_stream_t *stream);

MATERIAL_API
void
bulk_material_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

MATERIAL_API
size_t
bulk_material_asset_type_size(void);

MATERIAL_API
uint32_t
bulk_material_asset_owns_alloc(void);

MATERIAL_API
const allocator_t *
bulk_material_asset_get_alloc(const void *ptr);

MATERIAL_API
void
bulk_material_asset_cleanup(
  void *ptr,
  const allocator_t *allocator);

MATERIAL_API
const char *
bulk_material_asset_get_dir(void);

MATERIAL_API
loader_t
bulk_material_asset_get_loader(void);

MATERIAL_API
deloader_t
bulk_material_asset_get_deloader(void);

MATERIAL_API
uint32_t
bulk_material_asset_type_asset_count(const void *src);

MATERIAL_API
void
bulk_material_asset_type_get_assets(const void *src, const asset_ref_t *refs[]);

MATERIAL_API
uint32_t
bulk_material_asset_is_asset_type(void);

#ifdef __cplusplus
}
#endif

#endif