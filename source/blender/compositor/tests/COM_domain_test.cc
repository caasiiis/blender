/* SPDX-FileCopyrightText: 2025 Blender Authors
 *
 * SPDX-License-Identifier: Apache-2.0 */

#include "testing/testing.h"

#include "COM_domain.hh"

namespace blender::compositor::tests {

/* -------------------------------------------------------------------- */
/** \name Domain Construction Tests
 * \{ */

TEST(compositor_domain, SizeOnlyConstructor)
{
  Domain domain(int2(800, 600));
  EXPECT_EQ(domain.data_size, int2(800, 600));
  EXPECT_EQ(domain.display_size, int2(800, 600));
  EXPECT_EQ(domain.data_offset, int2(0, 0));
  EXPECT_EQ(domain.transformation, float3x3::identity());
}

TEST(compositor_domain, SizeAndTransformConstructor)
{
  float3x3 transform = float3x3::identity();
  Domain domain(int2(1024, 768), transform);
  EXPECT_EQ(domain.data_size, int2(1024, 768));
  EXPECT_EQ(domain.display_size, int2(1024, 768));
  EXPECT_EQ(domain.data_offset, int2(0, 0));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Domain Identity Tests
 * \{ */

TEST(compositor_domain, Identity)
{
  Domain domain = Domain::identity();
  EXPECT_EQ(domain.data_size, int2(1, 1));
  EXPECT_EQ(domain.transformation, float3x3::identity());
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Domain Transpose Tests
 * \{ */

TEST(compositor_domain, Transposed)
{
  Domain domain(int2(800, 600));
  Domain transposed = domain.transposed();
  EXPECT_EQ(transposed.data_size, int2(600, 800));
  EXPECT_EQ(transposed.display_size, int2(600, 800));
}

TEST(compositor_domain, TransposedSquare)
{
  Domain domain(int2(512, 512));
  Domain transposed = domain.transposed();
  EXPECT_EQ(transposed.data_size, int2(512, 512));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Domain Equality Tests
 * \{ */

TEST(compositor_domain, EqualityIdentical)
{
  Domain a(int2(800, 600));
  Domain b(int2(800, 600));
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
}

TEST(compositor_domain, EqualityDifferentSize)
{
  Domain a(int2(800, 600));
  Domain b(int2(1024, 768));
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}

TEST(compositor_domain, IsEqualWithEpsilon)
{
  Domain a(int2(800, 600));
  Domain b(int2(800, 600));
  EXPECT_TRUE(Domain::is_equal(a, b, 10e-6f));
}

TEST(compositor_domain, IsEqualDifferentOffsets)
{
  Domain a(int2(800, 600));
  Domain b(int2(800, 600));
  b.data_offset = int2(10, 20);
  EXPECT_FALSE(Domain::is_equal(a, b));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Domain Transform Tests
 * \{ */

TEST(compositor_domain, TransformIdentity)
{
  Domain domain(int2(800, 600));
  domain.transform(float3x3::identity());
  EXPECT_EQ(domain.transformation, float3x3::identity());
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Interpolation/Extension String Conversion Tests
 * \{ */

TEST(compositor_domain, InterpolationToString)
{
  EXPECT_STREQ(to_string(Interpolation::Nearest).c_str(), "Nearest");
  EXPECT_STREQ(to_string(Interpolation::Bilinear).c_str(), "Bilinear");
  EXPECT_STREQ(to_string(Interpolation::Bicubic).c_str(), "Bicubic");
  EXPECT_STREQ(to_string(Interpolation::Anisotropic).c_str(), "Anisotropic");
}

TEST(compositor_domain, ExtensionToString)
{
  EXPECT_STREQ(to_string(Extension::Extend).c_str(), "Extend");
  EXPECT_STREQ(to_string(Extension::Repeat).c_str(), "Repeat");
  EXPECT_STREQ(to_string(Extension::Clip).c_str(), "Clip");
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Realize Transformation Tests
 * \{ */

TEST(compositor_domain, RealizeIdentityTransformation)
{
  Domain domain(int2(800, 600));
  Domain realized = domain.realize_transformation(true);
  EXPECT_EQ(realized.data_size, int2(800, 600));
  EXPECT_EQ(realized.transformation, float3x3::identity());
}

TEST(compositor_domain, RealizeTransformationNoTranslation)
{
  Domain domain(int2(800, 600));
  Domain realized = domain.realize_transformation(false);
  EXPECT_EQ(realized.data_size, int2(800, 600));
}

/** \} */

}  // namespace blender::compositor::tests
