#include <Eigen/Dense>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sil/sil.hpp>
#include <vector>

int const WIDTH = 4000 * 3;
int const HEIGHT = 6000 * 3;

glm::vec3 hexToRgb(const std::string &hexColor) {
  // Vérification du format de la chaîne (attendre #RRGGBB ou RRGGBB)
  if (hexColor.length() != 7 && hexColor.length() != 6) {
    throw std::invalid_argument(
        "La couleur hexadécimale doit être sous forme de #RRGGBB ou RRGGBB");
  }

  // Si la chaîne commence par '#', on l'ignore
  std::string color = (hexColor[0] == '#') ? hexColor.substr(1) : hexColor;

  // Conversion de la chaîne hex en valeurs RGB
  int r, g, b;
  std::stringstream ss;

  ss << std::hex
     << color.substr(0, 2); // Extraire les 2 premiers caractères (Rouge)
  ss >> r;
  ss.clear();

  ss << std::hex
     << color.substr(2, 2); // Extraire les 2 caractères suivants (Vert)
  ss >> g;
  ss.clear();

  ss << std::hex
     << color.substr(4, 2); // Extraire les 2 derniers caractères (Bleu)
  ss >> b;

  return glm::vec3{r / 255., g / 255., b / 255.};
}

struct Rectangle {
  glm::vec2 pos;
  glm::vec2 size;
};

Rectangle Respect;

void draw_rectangle(glm::vec2 pos, glm::vec2 size, sil::Image &image,
                    glm::vec3 &color) {
  for (int x{0}; x < size.x; ++x) {
    for (int y{0}; y < size.y; y++) {
      image.pixel(x + pos.x, y + pos.y) = color;
    }
  }
}

bool check_in_greed(glm::vec2 &pos, glm::vec2 &size, int &subdivision,
                    int &thickness, glm::vec2 currentXY, bool damierOn) {
  if (!damierOn) {
    if (currentXY.x <= pos.x + size.x + thickness - 1 && currentXY.x >= pos.x &&
        currentXY.y >= pos.y && currentXY.y <= pos.y + size.y + thickness - 1) {
      if (int(currentXY.y - pos.y) % int(size.y / subdivision) <= thickness ||
          int(currentXY.x - pos.x) % int(size.x / subdivision) <= thickness)
        return true;
    }
    return false;
  } else {
    if (currentXY.x <= pos.x + size.x + thickness - 1 && currentXY.x >= pos.x &&
        currentXY.y >= pos.y && currentXY.y <= pos.y + size.y + thickness - 1) {
      if ((int(currentXY.y - pos.y) / int(size.y / subdivision)) % 2 == 1 &&
          (int(currentXY.x - pos.x) / int(size.x / subdivision)) % 2 == 1)
        return true;
      else if ((int(currentXY.y - pos.y) / int(size.y / subdivision)) % 2 ==
                   0 &&
               (int(currentXY.x - pos.x) / int(size.x / subdivision)) % 2 == 0)
        return true;
      else if (int(currentXY.y - pos.y) % int(size.y / subdivision) <=
                   thickness ||
               int(currentXY.x - pos.x) % int(size.x / subdivision) <=
                   thickness)
        return true;
      return false;
    }
  }
  return false;
}

glm::vec2 apply_rotation(int x, int y, double angle, glm::vec2 size,
                         glm::vec2 pos, int alea1, int alea2) {

  Eigen::Matrix3d RotationM;
  RotationM << std::cos(angle), -std::sin(angle), 0., std::sin(angle),
      std::cos(angle), 0., 0., 0., 1.;
  Eigen::Vector3d ActualPoint;
  ActualPoint << x - size.x / 2 - pos.x + alea1, y - size.y / 2 - pos.y + alea2,
      1.;

  Eigen::Vector3d ResultPoint = RotationM * ActualPoint;
  return glm::vec2{ResultPoint(0) + size.x / 2 + pos.x,
                   ResultPoint(1) + size.y / 2 + pos.y};
}

void draw_grid(glm::vec2 pos, glm::vec2 size, int subdivision, int thickness,
               double angle, sil::Image &image, glm::vec3 &color, bool damier) {

  std::vector<glm::vec2> pointToDraw;

  std::srand(std::time(0));

  int min = 0;
  int max = size.y;

  int alea1 = min + (std::rand() % (max - min + 1));

  alea1 = 0;

  int alea2 = min + (std::rand() % (max - min + 1));

  alea2 = 0;

  for (int x{-1000}; x < image.width() + 1000; x++) {
    for (int y{-1000}; y < image.height() + 1000; y++) {
      if (check_in_greed(pos, size, subdivision, thickness, glm::vec2{x, y},
                         damier)) {

        glm::vec2 newCurrentXY =
            apply_rotation(x, y, angle, size, pos, alea1, alea2);

        if (newCurrentXY.x > Respect.pos.x &&
            newCurrentXY.x < Respect.pos.x + Respect.size.x + thickness &&
            newCurrentXY.y > Respect.pos.y &&
            newCurrentXY.y < Respect.pos.y + Respect.size.y) {

          if (newCurrentXY.x > 0. && newCurrentXY.x < image.width() &&
              newCurrentXY.y > 0 && newCurrentXY.y < image.height()) {
            image.pixel(newCurrentXY.x, newCurrentXY.y) = color;
            if (newCurrentXY.x + 1 < image.width() &&
                newCurrentXY.y + 1 < image.height())
              image.pixel(newCurrentXY.x, newCurrentXY.y + 1) = color;
            if (newCurrentXY.x + 1 < image.width() &&
                newCurrentXY.y + 1 < image.height())
              image.pixel(newCurrentXY.x + 1, newCurrentXY.y) = color;
            if (newCurrentXY.x - 1 > 0. && newCurrentXY.y - 1 > 0.)
              image.pixel(newCurrentXY.x, newCurrentXY.y - 1) = color;
            if (newCurrentXY.x - 1 > 0. && newCurrentXY.y - 1 > 0.)
              image.pixel(newCurrentXY.x - 1, newCurrentXY.y) = color;
          }
        }
      }
    }
  }
  //   image.save("src/KacperMoirage/MiseEnPage/export6.png");
}

void draw_circle(glm::vec2 pos, double rayon, sil::Image &image,
                 glm::vec3 &color) {
  for (int x{int(pos.x - rayon)}; x < pos.x + rayon; x++) {
    for (int y{int(pos.y - rayon)}; y < pos.y + rayon; y++) {
      if ((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y) <=
          rayon * rayon) {
        if (x > 0 && x < image.width() && y > 0 && y < image.height())
          if (x > Respect.pos.x - 1 && x < Respect.pos.x + Respect.size.x + 1 &&
              y > Respect.pos.y - 1 && y < Respect.pos.y + Respect.size.y + 1)
            image.pixel(x, y) = color;
      }
    }
  }
}

void draw_point_grid(glm::vec2 pos, glm::vec2 size, int subdivision,
                     double rayon, double angle, sil::Image &image,
                     glm::vec3 &color, glm::vec2 posGlobal,
                     glm::vec2 sizeGlobal) {
  std::vector<glm::vec2> pointToDraw;

  std::srand(std::time(0));

  int min = 0;
  int max = size.y;

  int alea1 = min + (std::rand() % (max - min + 1));

  alea1 -= size.y / 2;

  int alea2 = min + (std::rand() % (max - min + 1));

  alea2 -= size.y / 4;

  alea1 = 0;
  alea2 = 0;

  for (int x{int(pos.x) - 100}; x < int(pos.x + size.x) + 100; x++) {
    for (int y{int(pos.y) - 100}; y < int(pos.y + size.y) + 100; y++) {

      if (int(y - pos.y) % int(size.y / subdivision) == 0 &&
          int(x - pos.x) % int(size.x / subdivision) == 0) {
        glm::vec2 newCurrentXY =
            apply_rotation(x, y, angle, size, pos, alea1, alea2);

        if (newCurrentXY.x > Respect.pos.x - rayon &&
            newCurrentXY.x < Respect.pos.x + Respect.size.x + rayon &&
            newCurrentXY.y > Respect.pos.y - rayon &&
            newCurrentXY.y < Respect.pos.y + Respect.size.y + rayon)
          // if (pos.x > -rayon - 10 && pos.x < image.width() + rayon + 10 &&
          //     pos.y > -rayon - 10 && pos.y < image.height() + rayon + 10)
          pointToDraw.push_back(newCurrentXY);
      }
    }
  }

  for (glm::vec2 point : pointToDraw) {
    draw_circle(point, rayon, image, color);
  }
}

void change_color_image(std::string path, glm::vec3 background,
                        glm::vec3 color1, glm::vec3 color2) {
  sil::Image image{path};

  glm::vec3 bleuCiel{117 / 255., 139 / 255., 253 / 255.};
  glm::vec3 bleuFoncé{39 / 255., 24 / 255., 126 / 255.};
  glm::vec3 bleuPale{174 / 255., 184 / 255., 254 / 255.};
  glm::vec3 jauneCanard{255 / 255., 134 / 255., 0 / 255.};

  glm::vec3 rose{226 / 255., 116 / 255., 141 / 255.};
  glm::vec3 roseFonce{198 / 255., 77 / 255., 94 / 255.};
  glm::vec3 rouge{170 / 255., 38 / 255., 47 / 255.};

  for (int x{0}; x < image.width(); x++) {
    for (int y{0}; y < image.height(); y++) {
      if (image.pixel(x, y) == rouge)
        image.pixel(x, y) = background;
      if (image.pixel(x, y) == bleuCiel)
        image.pixel(x, y) = color2;
      if (image.pixel(x, y) == bleuPale)
        image.pixel(x, y) = color1;
    }
  }

  image.save("src/KacperMoirage/switch/exportNew1.png");
}

int main() {

  sil::Image imageTest{WIDTH, HEIGHT};

  int nbrDeCase = 5;
  int bigSize = 20000;
  int miniSize = bigSize / nbrDeCase;
  int start = -(1 * miniSize) / 2;

  glm::vec2 pos{start, start};
  glm::vec2 size{bigSize, bigSize};
  int subdivision = 35;
  int thickness = 8;
  double angle = 0.7;
  bool damierOn = true;

  // palette 1

  glm::vec3 bleuCiel{117 / 255., 139 / 255., 253 / 255.};
  glm::vec3 bleuFoncé{39 / 255., 24 / 255., 126 / 255.};
  glm::vec3 bleuPale{174 / 255., 184 / 255., 254 / 255.};
  glm::vec3 jauneCanard{255 / 255., 134 / 255., 0 / 255.};

  // palette 2

  glm::vec3 rose{226 / 255., 116 / 255., 141 / 255.};
  glm::vec3 roseFonce{198 / 255., 77 / 255., 94 / 255.};
  glm::vec3 rouge{170 / 255., 38 / 255., 47 / 255.};

  glm::vec3 violet = hexToRgb("#4F2E8F");

  double rayon = 52;

  // change_color_image("src/KacperMoirage/Final/renduFinal40.png",
  // jauneCanard, bleuFoncé, bleuCiel);

  /*           -------------------------------             */

  glm::vec3 fondFinal =
      glm::vec3(209 / 256., 76 / 256., 160 / 256.); // rgb(209, 76, 160)
  glm::vec3 fonceFinal = hexToRgb("#585FA3");
  glm::vec3 clairFinal = hexToRgb("#A6A9AA");

  for (int x{0}; x < imageTest.width(); x++) {
    for (int y{0}; y < imageTest.height(); y++) {
      imageTest.pixel(x, y) = fondFinal;
    }
  }

  std::vector<double> angleList{
      -0.05, 0.1,   1.55, 1.58,  2.7, 1.178, 3.14, 3.1, 1.51, 3.17, 3.17,
      1.4,   1.565, 2.1,  3.142, 1,   1.5,   2.7,  1.5, 1.55, 1.5,  2.7,
  };

  int marge = 0;

  glm::vec3 backVio = hexToRgb("#4B244A");
  glm::vec3 pointFondVioClair = hexToRgb("#98C1D9");
  glm::vec3 pointDevantVioFon = hexToRgb("#6969B3");
  glm::vec3 VioletMiFonce = hexToRgb("#533A7B");

  // change_color_image("src/KacperMoirage/MiseEnPage/RondFinal1.png", backVio,
  //                    pointFondVioClair, pointDevantVioFon);

  // draw_grid(pos, size, nbrDeCase, thickness, 0,imageTest, white);
  //   for (int t{0}; t < 1; t++) {
  //     double time = t * 3.1415 / 200.;
  for (int X{0}; X < nbrDeCase; ++X) {
    for (int Y{0}; Y < nbrDeCase; Y++) {
      Respect = Rectangle{
          glm::vec2{(start + X * miniSize), (start + Y * miniSize) + marge},
          glm::vec2{miniSize, miniSize}};
      draw_point_grid(
          glm::vec2{start + X * miniSize, start + Y * miniSize + marge},
          glm::vec2{miniSize, miniSize}, subdivision, rayon, 0, imageTest,
          clairFinal, pos, size);
      draw_point_grid(glm::vec2{(start + X * miniSize) - miniSize,
                                (start + Y * miniSize) - miniSize + marge},
                      glm::vec2{3 * miniSize, 3 * miniSize}, 3 * subdivision,
                      rayon, angleList[X * nbrDeCase + Y], imageTest,
                      fonceFinal, pos, size);
      std::cout << "Angle " << X * nbrDeCase + Y << " : "
                << angleList[X * nbrDeCase + Y] << std::endl;
      imageTest.save("src/KacperMoirage/tirage/rendu4.png");

      //   if (X * nbrDeCase + Y == 2) {
      //   draw_grid(glm::vec2{start + X * miniSize, start + Y * miniSize},
      //             glm::vec2{miniSize, miniSize}, subdivision, thickness, 0,
      //             imageTest, roseFonce, false);
      //   draw_grid(glm::vec2{(start + X * miniSize) - miniSize,
      //                       (start + Y * miniSize) - miniSize},
      //             glm::vec2{3 * miniSize, 3 * miniSize}, 3 * subdivision,
      //             thickness, angleList[X * nbrDeCase + Y], imageTest,
      //             rouge, false);
      //   }
    }
  }

  // draw_grid(pos, size, nbrDeCase, thickness, 0,imageTest, jauneCanard,
  // false);

  // draw_grid(pos, size, subdivision, thickness, angle,imageTest);
  // draw_grid(pos, size, subdivision, thickness, 0,imageTest);
  //   std::cout << std::to_string(t) << std::endl;
  // imageTest.save("src/KacperMoirage/MiseEnPage/export9.png");
  //   imageTest = sil::Image{2000, 2000};

  //   draw_point_grid(pos, size, subdivision, rayon, angle, imageTest,
  //   bleuCiel);

  /*           -------------------------------             */

  return 0;
}