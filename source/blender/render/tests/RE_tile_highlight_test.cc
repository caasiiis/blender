/* SPDX-FileCopyrightText: 2025 Blender Authors
 *
 * SPDX-License-Identifier: Apache-2.0 */

#include "testing/testing.h"

#include "tile_highlight.h"

namespace blender::render::tests {

TEST(render_tile_highlight, InitiallyEmpty)
{
  TilesHighlight highlight;
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 0);
}

TEST(render_tile_highlight, HighlightSingleTile)
{
  TilesHighlight highlight;
  highlight.highlight_tile(10, 20, 100, 50);
  Span<rcti> tiles = highlight.get_all_highlighted_tiles();
  EXPECT_EQ(tiles.size(), 1);
  EXPECT_EQ(tiles[0].xmin, 10);
  EXPECT_EQ(tiles[0].xmax, 110);
  EXPECT_EQ(tiles[0].ymin, 20);
  EXPECT_EQ(tiles[0].ymax, 70);
}

TEST(render_tile_highlight, HighlightMultipleTiles)
{
  TilesHighlight highlight;
  highlight.highlight_tile(0, 0, 64, 64);
  highlight.highlight_tile(64, 0, 64, 64);
  highlight.highlight_tile(0, 64, 64, 64);
  Span<rcti> tiles = highlight.get_all_highlighted_tiles();
  EXPECT_EQ(tiles.size(), 3);
}

TEST(render_tile_highlight, UnhighlightTile)
{
  TilesHighlight highlight;
  highlight.highlight_tile(10, 20, 100, 50);
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 1);

  highlight.unhighlight_tile(10, 20, 100, 50);
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 0);
}

TEST(render_tile_highlight, ClearAll)
{
  TilesHighlight highlight;
  highlight.highlight_tile(0, 0, 64, 64);
  highlight.highlight_tile(64, 0, 64, 64);
  highlight.highlight_tile(0, 64, 64, 64);
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 3);

  highlight.clear();
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 0);
}

TEST(render_tile_highlight, DuplicateHighlight)
{
  TilesHighlight highlight;
  highlight.highlight_tile(10, 20, 100, 50);
  highlight.highlight_tile(10, 20, 100, 50);
  /* Duplicate tiles should not increase the count (it's a Set). */
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 1);
}

TEST(render_tile_highlight, UnhighlightNonExistent)
{
  TilesHighlight highlight;
  highlight.highlight_tile(10, 20, 100, 50);
  /* Unhighlight a different tile that was never added. */
  highlight.unhighlight_tile(0, 0, 64, 64);
  EXPECT_EQ(highlight.get_all_highlighted_tiles().size(), 1);
}

}  // namespace blender::render::tests
