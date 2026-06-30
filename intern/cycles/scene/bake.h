/* SPDX-FileCopyrightText: 2011-2022 Blender Foundation
 *
 * SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "device/device.h"
#include "scene/scene.h"

#include "util/progress.h"

CCL_NAMESPACE_BEGIN

class BakeManager {
 public:
  BakeManager() = default;
  ~BakeManager() = default;

  void set_baking(Scene *scene, const bool use);
  bool get_baking() const;

  void set_use_camera(bool use_camera);

  void set_use_seed(bool use_seed);
  bool get_use_seed() const;

  void set_use_nrc(bool use_nrc);
  bool get_use_nrc() const;

  void set_nrc_max_bounces(int max_bounces);
  int get_nrc_max_bounces() const;

  void set_nrc_training_samples(int training_samples);
  int get_nrc_training_samples() const;

  void device_update(Device *device, DeviceScene *dscene, Scene *scene, Progress &progress);
  void device_free(Device *device, DeviceScene *dscene);

  void tag_update();

  bool need_update() const;

 private:
  bool need_update_ = true;
  bool use_baking_ = false;
  bool use_camera_ = false;
  bool use_seed_ = false;
  bool use_nrc_ = false;
  int nrc_max_bounces_ = 2;
  int nrc_training_samples_ = 16;
};

CCL_NAMESPACE_END
