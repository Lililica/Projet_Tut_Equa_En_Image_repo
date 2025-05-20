#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <random>
#include <sil/sil.hpp>
#include <string>

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

void trace_tresse(sil::Image &image, std::string combinaison, int Posy,
                  int offsetScreen) {

  glm::vec3 bleuCiel{117 / 255., 139 / 255., 253 / 255.};
  glm::vec3 bleuFoncé{39 / 255., 24 / 255., 126 / 255.};
  glm::vec3 bleuPale{174 / 255., 184 / 255., 254 / 255.};
  glm::vec3 jauneCanard{255 / 255., 134 / 255., 0 / 255.};

  // palette 2

  glm::vec3 rose{226 / 255., 116 / 255., 141 / 255.};
  glm::vec3 roseFonce{198 / 255., 77 / 255., 94 / 255.};
  glm::vec3 rouge{170 / 255., 38 / 255., 47 / 255.};

  // palette 3
  glm::vec3 VioletMur = hexToRgb("#774C60");
  glm::vec3 RougeBrick = hexToRgb("#B75D69");
  glm::vec3 BlancPale = hexToRgb("#EACDC2");

  // palette 4
  glm::vec3 Vert2 = hexToRgb("#6BA368");
  glm::vec3 Vert3 = hexToRgb("#515B3A");
  glm::vec3 VertFonce = hexToRgb("#353D2F");

  // Palette 3
  glm::vec3 Rag1 = hexToRgb("#0E79B2");
  glm::vec3 Rag2 = hexToRgb("#BF1363");
  glm::vec3 Rag3 = hexToRgb("#F39237");

  // Platte Final 1

  glm::vec3 color1 = hexToRgb("#7EA956");
  glm::vec3 color2 = hexToRgb("#462748");
  glm::vec3 color3 = hexToRgb("#9D4A87");

  // Palette final 2

  glm::vec3 color4 = hexToRgb("#48578D");
  glm::vec3 color5 = hexToRgb("#F2CA63");
  glm::vec3 color6 = hexToRgb("#95B4D4");

  // Palette Final 3

  glm::vec3 color7 = hexToRgb("#7870A7");
  glm::vec3 color8 = hexToRgb("#BDB974"); // rgb(189, 185, 116) #BDB974
  glm::vec3 color9 = hexToRgb("#5C94C1");

  glm::vec3 ligne1 = color7;
  glm::vec3 ligne2 = color8;
  glm::vec3 ligne3 = color9;

  int offset = 80;

  int thickness = 40;

  int sizeSegment =
      ((image.width() + 100 - 2 * offsetScreen) / (combinaison.size()));
  int offsetSegment = sizeSegment / 4;

  glm::vec3 pivot;

  combinaison += "aba";

  for (int i{0}; i < combinaison.size() + 1; i++) {
    char lettre = combinaison[i];
    switch (lettre) {
    case 'a':
      pivot = ligne1;
      ligne1 = ligne2;
      ligne2 = pivot;
      break;

    case 'b':
      pivot = ligne2;
      ligne2 = ligne3;
      ligne3 = pivot;
      break;

    default:
      break;
    }

    for (int x{0}; x < image.width(); ++x) {
      for (int y(0); y < image.height(); ++y) {
        if ((x >= offsetScreen +
                      i * (sizeSegment) /*- offsetSegment + sizeSegment/2*/
             &&
             x <= offsetScreen + i * (sizeSegment) +
                      2 * offsetSegment /*+ offsetSegment + sizeSegment/2*/) &&
            i < combinaison.size() - 2) {
          // if(x >= offsetScreen + i*(sizeSegment) && x <=
          // image.width()-offsetScreen - ((combinaison.size()-1-i)))
          // {
          if (y <= Posy + thickness && y >= Posy - thickness)
            image.pixel(x, y) = ligne1;
          if (y <= Posy + offset + thickness && y >= Posy + offset - thickness)
            image.pixel(x, y) = ligne2;
          if (y <= Posy + 2 * offset + thickness &&
              y >= Posy + 2 * offset - thickness)
            image.pixel(x, y) = ligne3;
          // }
        } else if (x <=
                       offsetScreen +
                           (i) *
                               (sizeSegment) /*- offsetSegment + sizeSegment/2*/
                   &&
                   x >=
                       offsetScreen + (i - 1) * (sizeSegment) +
                           2 * offsetSegment /*+ offsetSegment + sizeSegment/2*/
                   && i != 0 && i < combinaison.size() - 1) {

          int pointA = offsetScreen + i * (sizeSegment) + offsetSegment +
                       sizeSegment / 2;
          int pointB = offsetScreen + (i + 1) * (sizeSegment)-offsetSegment +
                       sizeSegment / 2;
          double a = double(offset) / (pointB - pointA);

          if (lettre == 'a') {
            if (y <= Posy + 2 * offset + thickness &&
                y >= Posy + 2 * offset - thickness)
              image.pixel(x, y) = ligne3;
            double b = Posy + a * pointA - 1.5 * offset;
            if (y <= -a * x + b + thickness && y >= -a * x + b - thickness)
              image.pixel(x, y) = ligne1;
            b = Posy + 2.5 * offset - a * pointA;
            if (y <= a * x + b + thickness && y >= a * x + b - thickness)
              image.pixel(x, y) = ligne2;
          }

          if (lettre == 'b') {
            if (y <= Posy + thickness && y >= Posy - thickness)
              image.pixel(x, y) = ligne1;
            double b = Posy + a * pointA - 0.5 * offset;
            if (y <= -a * x + b + thickness && y >= -a * x + b - thickness)
              image.pixel(x, y) = ligne2;
            b = Posy + 3.5 * offset - a * pointA;
            if (y <= a * x + b + thickness && y >= a * x + b - thickness)
              image.pixel(x, y) = ligne3;
          }
        }
      }
    }
  }
}

int main() {

  sil::Image image{8000, 12000};

  glm::vec3 bleuCiel{117 / 255., 139 / 255., 253 / 255.};

  // Palette 1
  glm::vec3 VioletFonce = hexToRgb("#372549");
  glm::vec3 VioletMur = hexToRgb("#774C60");
  glm::vec3 RougeBrick = hexToRgb("#B75D69");
  glm::vec3 BlancPale = hexToRgb("#EACDC2");

  // Palette 2
  glm::vec3 Vert1 = hexToRgb("#9CFC97");
  glm::vec3 Vert2 = hexToRgb("#6BA368");
  glm::vec3 Vert3 = hexToRgb("#515B3A");
  glm::vec3 VertFonce = hexToRgb("#353D2F");

  // Palette 3
  glm::vec3 Rag1 = hexToRgb("#0E79B2");
  glm::vec3 Rag2 = hexToRgb("#BF1363");
  glm::vec3 Rag3 = hexToRgb("#F39237");

  // Platte Final 1

  glm::vec3 color1 = hexToRgb("#7EA956");
  glm::vec3 color2 = hexToRgb("#462748");
  glm::vec3 color3 = hexToRgb("#9D4A87");

  glm::vec3 background = bleuCiel;

  for (int x{0}; x < image.width(); ++x) {
    for (int y(0); y < image.height(); ++y) {
      image.pixel(x, y) = hexToRgb("#423683");
    }
  }

  std::random_device rd;
  std::mt19937 g(rd());

  int nbrPyramide = 50;
  std::string contener =
      "ababbababbbababaaabababaabbabbbbbaaabababaabababaaabbab";

  int pos1 = image.width() / nbrPyramide;
  int pos2 = image.width() / 3;
  int incrementPos = (pos2 - pos1) / (nbrPyramide - 3);

  for (int i{3}; i < nbrPyramide + 3; i++) {
    // std::cout << pos1 + (i-3)*incrementPos << std::endl;
    // std::cout << pos1 << std::endl;
    std::shuffle(contener.begin(), contener.end(), g);
    trace_tresse(image, contener,
                 ((i - 3) * (image.height() + 300) / (nbrPyramide - 3)) - 300,
                 -600);
    contener.pop_back();
    image.save("src/MeresseLigne/bande/im19.png");
  }

  // std::shuffle(contener.begin(), contener.end(), g);
  // trace_tresse(image, contener, 450,0);

  image.save("src/MeresseLigne/bande/im19.png");

  return 0;
}

//  A8642C
//  C6D4B3
//  E2883C

// 38	84	46	 #26552E
// 179	186	190	 #B3BABA
//  211	112	61	 #D3703D
// 66, 54, 131 #423683