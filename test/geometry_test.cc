#include "src/geometry/geometry.hpp"

#include <gtest/gtest.h>

#include <vector>

#include "src/geometry/math.hpp"

TEST(QUAD, tangents)
{
  std::vector<std::array<skity::Point, 3>> pts = {
      {skity::Point{10, 20, 0, 1}, skity::Point{10, 20, 0, 1},
       skity::Point{20, 30, 0, 1}},
      {skity::Point{10, 20, 0, 1}, skity::Point{15, 25, 0, 1},
       skity::Point{20, 30, 0, 1}},
      {skity::Point{10, 20, 0, 1}, skity::Point{20, 30, 0, 1},
       skity::Point{20, 30, 0, 1}},
  };

  size_t count = pts.size();
  for (size_t i = 0; i < count; i++) {
    skity::Vector start = skity::QuadCoeff::EvalQuadTangentAt(pts[i], 0);
    skity::Vector mid = skity::QuadCoeff::EvalQuadTangentAt(pts[i], .5f);
    skity::Vector end = skity::QuadCoeff::EvalQuadTangentAt(pts[i], 1.f);

    EXPECT_TRUE(start.x && start.y);
    EXPECT_TRUE(mid.x && mid.y);
    EXPECT_TRUE(end.x && end.y);
    EXPECT_TRUE(skity::FloatNearlyZero(skity::CrossProduct(start, mid)));
    EXPECT_TRUE(skity::FloatNearlyZero(skity::CrossProduct(mid, end)));
  }
}

int main(int argc, const char **argv)
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}