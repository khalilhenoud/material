/**
 * @file material_asset.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef MATERIAL_ASSET_H
#define MATERIAL_ASSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <acsu/internal/module.h>
#include <library/asset/asset_ref.h>
#include <library/asset/types.h>
#include <library/containers/cvector.h>
#include <library/string/cstring.h>


////////////////////////////////////////////////////////////////////////////////
//| material_asset_t, '*' = material_asset
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
struct texture_properties_t {
  asset_ref_t texture_ref;
  float u, v;
  float u_scale, v_scale;
  float angle;
} texture_properties_t;

typedef
struct mat_rgba_t {
  float data[4];
} mat_rgba_t;

typedef
struct material_asset_t {
  cstring_t name;
  mat_rgba_t ambient;
  mat_rgba_t diffuse;
  mat_rgba_t specular;
  float shininess;
  float opacity;
  cvector_t textures;
} material_asset_t;

ACSU_API
void
material_asset_def(void *ptr);

ACSU_API
uint32_t
material_asset_is_def(const void *ptr);

ACSU_API
void
material_asset_serialize(
  const void *src,
  binary_stream_t *stream);

ACSU_API
void
material_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

ACSU_API
size_t
material_asset_type_size(void);

ACSU_API
uint32_t
material_asset_owns_alloc(void);

ACSU_API
const allocator_t *
material_asset_get_alloc(const void *ptr);

ACSU_API
void
material_asset_cleanup(
  void *ptr,
  const allocator_t *allocator);

ACSU_API
const char *
material_asset_get_dir(void);

ACSU_API
loader_t
material_asset_get_loader(void);

ACSU_API
deloader_t
material_asset_get_deloader(void);

ACSU_API
uint32_t
material_asset_type_asset_count(const void *src);

ACSU_API
void
material_asset_type_get_assets(const void *src, const asset_ref_t *refs[]);

ACSU_API
uint32_t
material_asset_is_asset_type(void);

#ifdef __cplusplus
}
#endif

#endif