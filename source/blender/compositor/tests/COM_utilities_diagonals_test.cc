/* SPDX-FileCopyrightText: 2025 Blender Authors
 *
 * SPDX-License-Identifier: Apache-2.0 */

#include "testing/testing.h"

#include "COM_utilities_diagonals.hh"

namespace blender::compositor::tests {

/* -------------------------------------------------------------------- */
/** \name Diagonal Count Tests
 * \{ */

TEST(compositor_diagonals, NumberOfDiagonals)
{
  EXPECT_EQ(compute_number_of_diagonals(int2(6, 3)), 8);
  EXPECT_EQ(compute_number_of_diagonals(int2(1, 1)), 1);
  EXPECT_EQ(compute_number_of_diagonals(int2(3, 3)), 5);
  EXPECT_EQ(compute_number_of_diagonals(int2(4, 1)), 4);
  EXPECT_EQ(compute_number_of_diagonals(int2(1, 4)), 4);
  EXPECT_EQ(compute_number_of_diagonals(int2(10, 10)), 19);
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Diagonal Length Tests
 * \{ */

TEST(compositor_diagonals, DiagonalLengthSquareMatrix)
{
  /* For a 3x3 matrix, diagonals have lengths: 1, 2, 3, 2, 1. */
  int2 size(3, 3);
  EXPECT_EQ(compute_diagonal_length(size, 0), 1);
  EXPECT_EQ(compute_diagonal_length(size, 1), 2);
  EXPECT_EQ(compute_diagonal_length(size, 2), 3);
  EXPECT_EQ(compute_diagonal_length(size, 3), 2);
  EXPECT_EQ(compute_diagonal_length(size, 4), 1);
}

TEST(compositor_diagonals, DiagonalLengthWideMatrix)
{
  /* For a 6x3 matrix, diagonals have lengths: 1, 2, 3, 3, 3, 3, 2, 1. */
  int2 size(6, 3);
  EXPECT_EQ(compute_diagonal_length(size, 0), 1);
  EXPECT_EQ(compute_diagonal_length(size, 1), 2);
  EXPECT_EQ(compute_diagonal_length(size, 2), 3);
  EXPECT_EQ(compute_diagonal_length(size, 3), 3);
  EXPECT_EQ(compute_diagonal_length(size, 4), 3);
  EXPECT_EQ(compute_diagonal_length(size, 5), 3);
  EXPECT_EQ(compute_diagonal_length(size, 6), 2);
  EXPECT_EQ(compute_diagonal_length(size, 7), 1);
}

TEST(compositor_diagonals, DiagonalLengthTallMatrix)
{
  /* For a 2x4 matrix, diagonals have lengths: 1, 2, 2, 2, 1. */
  int2 size(2, 4);
  EXPECT_EQ(compute_diagonal_length(size, 0), 1);
  EXPECT_EQ(compute_diagonal_length(size, 1), 2);
  EXPECT_EQ(compute_diagonal_length(size, 2), 2);
  EXPECT_EQ(compute_diagonal_length(size, 3), 2);
  EXPECT_EQ(compute_diagonal_length(size, 4), 1);
}

TEST(compositor_diagonals, DiagonalLengthSingleElement)
{
  int2 size(1, 1);
  EXPECT_EQ(compute_diagonal_length(size, 0), 1);
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Diagonal Start Position Tests
 * \{ */

TEST(compositor_diagonals, DiagonalStartWideMatrix)
{
  /* 6x3 matrix: starts at (0,2), (0,1), (0,0), (1,0), (2,0), (3,0), (4,0), (5,0). */
  int2 size(6, 3);
  EXPECT_EQ(compute_diagonal_start(size, 0), int2(0, 2));
  EXPECT_EQ(compute_diagonal_start(size, 1), int2(0, 1));
  EXPECT_EQ(compute_diagonal_start(size, 2), int2(0, 0));
  EXPECT_EQ(compute_diagonal_start(size, 3), int2(1, 0));
  EXPECT_EQ(compute_diagonal_start(size, 4), int2(2, 0));
  EXPECT_EQ(compute_diagonal_start(size, 5), int2(3, 0));
  EXPECT_EQ(compute_diagonal_start(size, 6), int2(4, 0));
  EXPECT_EQ(compute_diagonal_start(size, 7), int2(5, 0));
}

TEST(compositor_diagonals, DiagonalStartSquareMatrix)
{
  int2 size(3, 3);
  EXPECT_EQ(compute_diagonal_start(size, 0), int2(0, 2));
  EXPECT_EQ(compute_diagonal_start(size, 2), int2(0, 0));
  EXPECT_EQ(compute_diagonal_start(size, 4), int2(2, 0));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Diagonal Direction Tests
 * \{ */

TEST(compositor_diagonals, DiagonalDirection)
{
  EXPECT_EQ(get_diagonal_direction(), int2(1, 1));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Anti-Diagonal Tests
 * \{ */

TEST(compositor_diagonals, AntiDiagonalLength)
{
  int2 size(6, 3);
  EXPECT_EQ(compute_anti_diagonal_length(size, 0), 1);
  EXPECT_EQ(compute_anti_diagonal_length(size, 2), 3);
  EXPECT_EQ(compute_anti_diagonal_length(size, 5), 3);
  EXPECT_EQ(compute_anti_diagonal_length(size, 7), 1);
}

TEST(compositor_diagonals, AntiDiagonalStartWideMatrix)
{
  /* 6x3 matrix: starts at (0,0), (1,0), (2,0), (3,0), (4,0), (5,0), (5,1), (5,2). */
  int2 size(6, 3);
  EXPECT_EQ(compute_anti_diagonal_start(size, 0), int2(0, 0));
  EXPECT_EQ(compute_anti_diagonal_start(size, 1), int2(1, 0));
  EXPECT_EQ(compute_anti_diagonal_start(size, 5), int2(5, 0));
  EXPECT_EQ(compute_anti_diagonal_start(size, 6), int2(5, 1));
  EXPECT_EQ(compute_anti_diagonal_start(size, 7), int2(5, 2));
}

TEST(compositor_diagonals, AntiDiagonalDirection)
{
  EXPECT_EQ(get_anti_diagonal_direction(), int2(-1, 1));
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Jump Flooding Value Encoding Tests
 * \{ */

TEST(compositor_diagonals, JumpFloodingEncodeNonFlooded)
{
  int2 result = encode_jump_flooding_value(int2(5, 10), false);
  EXPECT_EQ(result, JUMP_FLOODING_NON_FLOODED_VALUE);
}

TEST(compositor_diagonals, JumpFloodingEncodeFlooded)
{
  int2 texel(5, 10);
  int2 result = encode_jump_flooding_value(texel, true);
  EXPECT_EQ(result, texel);
}

TEST(compositor_diagonals, JumpFloodingInitializeSeed)
{
  int2 texel(3, 7);
  int2 result = initialize_jump_flooding_value(texel, true);
  EXPECT_EQ(result, texel);
}

TEST(compositor_diagonals, JumpFloodingInitializeNonSeed)
{
  int2 texel(3, 7);
  int2 result = initialize_jump_flooding_value(texel, false);
  EXPECT_EQ(result, JUMP_FLOODING_NON_FLOODED_VALUE);
}

/** \} */

}  // namespace blender::compositor::tests
