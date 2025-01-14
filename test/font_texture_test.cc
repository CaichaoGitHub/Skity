#include "src/render/text/font_texture.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <skity/text/utf.hpp>
#include <string>
#include <vector>

#include "test_config.hpp"

template <class T>
void print_region(T const& region) {
  std::cout << "region = {" << region.x << ", " << region.y << " ," << region.z
            << ", " << region.w << "}" << std::endl;
}

int main(int argc, const char** argv) {
  auto typeface = skity::Typeface::MakeFromFile(TEST_BUILD_IN_FONT);

  skity::FontTexture font_texture{typeface.get()};

  std::vector<skity::GlyphID> glyphs{};

  std::string str = "Hello World";

  skity::UTF::UTF8ToCodePoint(str.c_str(), str.size(), glyphs);

  std::vector<glm::ivec4> regions{};

  float font_size = 14.f;

  for (auto id : glyphs) {
    regions.emplace_back(font_texture.GetGlyphRegion(id, font_size));
  }

  for (auto const& region : regions) {
    print_region(region);
  }

  std::vector<glm::vec4> regions_uv{};
  for (auto const& region : regions) {
    glm::vec4 uv = {
        font_texture.CalculateUV(region.x, region.y),
        font_texture.CalculateUV(region.x + region.z, region.y + region.w),
    };

    regions_uv.emplace_back(uv);
  }

  std::cout << "------- UV -------" << std::endl;

  for (auto const& uv : regions_uv) {
    print_region(uv);
  }

  std::vector<glm::ivec4> regions2 = {};
  for (auto id : glyphs) {
    regions2.emplace_back(font_texture.GetGlyphRegion(id, 30.f));
  }

  std::cout << "--------- big size ------------" << std::endl;

  for (auto const& reg : regions2) {
    print_region(reg);
  }

  return 0;
}