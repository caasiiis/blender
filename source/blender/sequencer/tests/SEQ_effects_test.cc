/* SPDX-FileCopyrightText: 2025 Blender Authors
 *
 * SPDX-License-Identifier: Apache-2.0 */

#include "testing/testing.h"

#include "BLI_array.hh"
#include "BLI_math_vector_types.hh"

#include "SEQ_effects.hh"

#include "effects/effects.hh"

namespace blender::seq::tests {

/* -------------------------------------------------------------------- */
/** \name Gaussian Blur Kernel Tests
 * \{ */

TEST(sequencer_effects, GaussianKernelSumsToOne)
{
  Array<float> kernel = make_gaussian_blur_kernel(5.0f, 5);
  float sum = 0.0f;
  for (float v : kernel) {
    sum += v;
  }
  EXPECT_NEAR(sum, 1.0f, 1e-6f);
}

TEST(sequencer_effects, GaussianKernelSize)
{
  Array<float> kernel = make_gaussian_blur_kernel(3.0f, 3);
  EXPECT_EQ(kernel.size(), 7); /* 2 * size + 1 */
}

TEST(sequencer_effects, GaussianKernelSymmetric)
{
  Array<float> kernel = make_gaussian_blur_kernel(4.0f, 4);
  int n = kernel.size();
  for (int i = 0; i < n / 2; i++) {
    EXPECT_FLOAT_EQ(kernel[i], kernel[n - 1 - i]);
  }
}

TEST(sequencer_effects, GaussianKernelCenterIsLargest)
{
  Array<float> kernel = make_gaussian_blur_kernel(5.0f, 5);
  int center = kernel.size() / 2;
  for (int i = 0; i < int(kernel.size()); i++) {
    EXPECT_LE(kernel[i], kernel[center]);
  }
}

TEST(sequencer_effects, GaussianKernelSmallRadius)
{
  Array<float> kernel = make_gaussian_blur_kernel(1.0f, 1);
  EXPECT_EQ(kernel.size(), 3);
  float sum = 0.0f;
  for (float v : kernel) {
    sum += v;
  }
  EXPECT_NEAR(sum, 1.0f, 1e-6f);
}

TEST(sequencer_effects, GaussianKernelAllPositive)
{
  Array<float> kernel = make_gaussian_blur_kernel(5.0f, 5);
  for (float v : kernel) {
    EXPECT_GT(v, 0.0f);
  }
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Early Out Function Tests
 * \{ */

TEST(sequencer_effects, EarlyOutFadeZero)
{
  EXPECT_EQ(early_out_fade(nullptr, 0.0f), StripEarlyOut::UseInput1);
}

TEST(sequencer_effects, EarlyOutFadeOne)
{
  EXPECT_EQ(early_out_fade(nullptr, 1.0f), StripEarlyOut::UseInput2);
}

TEST(sequencer_effects, EarlyOutFadeMid)
{
  EXPECT_EQ(early_out_fade(nullptr, 0.5f), StripEarlyOut::DoEffect);
}

TEST(sequencer_effects, EarlyOutMulInput2Zero)
{
  EXPECT_EQ(early_out_mul_input2(nullptr, 0.0f), StripEarlyOut::UseInput1);
}

TEST(sequencer_effects, EarlyOutMulInput2NonZero)
{
  EXPECT_EQ(early_out_mul_input2(nullptr, 0.5f), StripEarlyOut::DoEffect);
}

TEST(sequencer_effects, EarlyOutMulInput1Zero)
{
  EXPECT_EQ(early_out_mul_input1(nullptr, 0.0f), StripEarlyOut::UseInput2);
}

TEST(sequencer_effects, EarlyOutMulInput1NonZero)
{
  EXPECT_EQ(early_out_mul_input1(nullptr, 1.0f), StripEarlyOut::DoEffect);
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Pixel Load/Store Tests (float)
 * \{ */

TEST(sequencer_effects, LoadPremulPixelFloat)
{
  float pixel[4] = {0.1f, 0.2f, 0.3f, 0.4f};
  float4 result = load_premul_pixel(pixel);
  EXPECT_FLOAT_EQ(result.x, 0.1f);
  EXPECT_FLOAT_EQ(result.y, 0.2f);
  EXPECT_FLOAT_EQ(result.z, 0.3f);
  EXPECT_FLOAT_EQ(result.w, 0.4f);
}

TEST(sequencer_effects, StorePremulPixelFloat)
{
  float4 pixel(0.5f, 0.6f, 0.7f, 0.8f);
  float dst[4] = {0};
  store_premul_pixel(pixel, dst);
  EXPECT_FLOAT_EQ(dst[0], 0.5f);
  EXPECT_FLOAT_EQ(dst[1], 0.6f);
  EXPECT_FLOAT_EQ(dst[2], 0.7f);
  EXPECT_FLOAT_EQ(dst[3], 0.8f);
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Strip Type Classification Tests
 * \{ */

TEST(sequencer_effects, StripTypeIsEffect)
{
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_CROSS));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_ADD));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_MUL));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_ALPHAOVER));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_WIPE));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_GLOW));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_GAUSSIAN_BLUR));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_TEXT));
  EXPECT_TRUE(strip_type_is_effect(STRIP_TYPE_COMPOSITOR));
}

TEST(sequencer_effects, StripTypeIsNotEffect)
{
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_IMAGE));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_META));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_SCENE));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_MOVIE));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_SOUND));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_MOVIECLIP));
  EXPECT_FALSE(strip_type_is_effect(STRIP_TYPE_MASK));
}

TEST(sequencer_effects, EffectIsTransition)
{
  EXPECT_TRUE(effect_is_transition(STRIP_TYPE_CROSS));
  EXPECT_TRUE(effect_is_transition(STRIP_TYPE_GAMCROSS));
  EXPECT_TRUE(effect_is_transition(STRIP_TYPE_WIPE));
  EXPECT_TRUE(effect_is_transition(STRIP_TYPE_COMPOSITOR));
}

TEST(sequencer_effects, EffectIsNotTransition)
{
  EXPECT_FALSE(effect_is_transition(STRIP_TYPE_ADD));
  EXPECT_FALSE(effect_is_transition(STRIP_TYPE_MUL));
  EXPECT_FALSE(effect_is_transition(STRIP_TYPE_GAUSSIAN_BLUR));
  EXPECT_FALSE(effect_is_transition(STRIP_TYPE_TEXT));
  EXPECT_FALSE(effect_is_transition(STRIP_TYPE_IMAGE));
}

TEST(sequencer_effects, EffectMinInputsZero)
{
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_ADJUSTMENT), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_MULTICAM), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_COLOR), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_TEXT), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_COMPOSITOR), 0);
}

TEST(sequencer_effects, EffectMinInputsOne)
{
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_GAUSSIAN_BLUR), 1);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_GLOW), 1);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_SPEED), 1);
}

TEST(sequencer_effects, EffectMinInputsTwo)
{
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_CROSS), 2);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_ADD), 2);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_MUL), 2);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_ALPHAOVER), 2);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_WIPE), 2);
}

TEST(sequencer_effects, NonEffectMinInputsZero)
{
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_IMAGE), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_MOVIE), 0);
  EXPECT_EQ(effect_type_get_min_num_inputs(STRIP_TYPE_SCENE), 0);
}

/** \} */

}  // namespace blender::seq::tests
