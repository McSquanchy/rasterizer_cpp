//
// Created by kevin on 21/10/2021.
//

#include <fstream>
#include <iostream>
#include <exponential.hpp>
#include "bitmap_texture.h"

void BitmapTexture::Read(const char *fp) {
  m_colors.clear();
  std::ifstream f;
  f.open(fp, std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    std::cerr << "File could not be opened!\n";
    return;
  }

  const int file_header_size = 14;
  const int info_header_size = 40;

  unsigned char fileHeader[file_header_size];
  f.read(reinterpret_cast<char *>(fileHeader), file_header_size);

  if (fileHeader[0]!='B' || fileHeader[1]!='M') {
    std::cerr << "wrong input format\n";
    f.close();
    return;
  }

  unsigned char information_header[info_header_size];
  f.read(reinterpret_cast<char *>(information_header), info_header_size);

  m_width =
      information_header[4] + (information_header[5] << 8) + (information_header[6] << 16)
          + (information_header[7] << 24);

  m_height =
      information_header[8] + (information_header[9] << 8) + (information_header[10] << 16)
          + (information_header[11] << 24);

  m_colors.resize(m_width*std::abs(m_height));

  const int paddingAmount = ((4 - (m_width*3)%4)%4);

  if (m_height < 0) { // reverse order if m_height is negative
    m_height *= -1;
    for (auto y = m_height - 1; y >= 0; --y) {
      for (auto x = 0; x < m_width; ++x) {
        unsigned char color[3];
        f.read(reinterpret_cast<char *>(color), 3);
        auto r = static_cast<float>(glm::pow(color[2]/255.0f, m_gamma));
        auto g = static_cast<float>(glm::pow(color[1]/255.0f, m_gamma));
        auto b = static_cast<float>(glm::pow(color[0]/255.0f, m_gamma));
        m_colors[y*m_width + x] = glm::vec3{r, g, b};
      }
      f.ignore(paddingAmount);
    }
  } else {
    for (auto y = 0; y < m_height; ++y) {
      for (auto x = 0; x < m_width; ++x) {
        unsigned char color[3];
        f.read(reinterpret_cast<char *>(color), 3);
        auto r = static_cast<float>(glm::pow(color[2]/255.0f, m_gamma));
        auto g = static_cast<float>(glm::pow(color[1]/255.0f, m_gamma));
        auto b = static_cast<float>(glm::pow(color[0]/255.0f, m_gamma));
        m_colors[y*m_width + x] = glm::vec3{r, g, b};
      }
      f.ignore(paddingAmount);
    }
  }

  f.close();
  std::cout << "Read texture " << fp << " successfully.\n";
}

glm::vec3 &BitmapTexture::GetColor(float x, float y) {
  int x_coordinate = static_cast<int>(std::clamp(x, 0.f, 1.0f)*((float)m_width - 1));
  int y_coordinate = static_cast<int>(std::clamp(y, 0.f, 1.0f)*((float)m_height - 1));
  return m_colors[y_coordinate*m_width + x_coordinate];
}

glm::vec3 &BitmapTexture::GetColorInt(int x, int y) {
  return m_colors[y*m_width + x];
}