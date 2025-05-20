#include "grille.hpp"



void draw_polynome3(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 pos3, glm::vec2 pos4, sil::Image& image, glm::vec3 color)
{

    double thickness = 0.0002;

    // a*x*x*x + b*x*x + c*x + d + e*y = 0 

    Eigen::MatrixXd A(4,5);
    A << pos1.x*pos1.x*pos1.x, pos1.x*pos1.x, pos1.x, 1, pos1.y,
        pos2.x*pos2.x*pos2.x, pos2.x*pos2.x, pos2.x, 1, pos2.y,
        pos3.x*pos3.x*pos3.x, pos3.x*pos3.x, pos3.x, 1, pos3.y,
        pos4.x*pos4.x*pos4.x, pos4.x*pos4.x, pos4.x, 1, pos4.y;

    // std::cout << A << std::endl;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd (A, Eigen::ComputeThinU|Eigen::ComputeFullV);
    Eigen::VectorXd coef = svd.matrixV().rightCols(1);

    // std::cout << coef << std::endl;

    int crayon = 10;

    for(int x{crayon}; x< image.width()-crayon; x++)
    {
        for(int y{crayon}; y< image.height()-crayon; y++)
        {
            if(coef(0)*x*x*x + coef(1)*x*x + coef(2)*x + coef(3) + coef(4)*y <= thickness && coef(0)*x*x*x + coef(1)*x*x + coef(2)*x + coef(3) + coef(4)*y >= -thickness)
            {
                image.pixel(x,y) = color;
                for(int i{1}; i<crayon; i++)
                {
                    image.pixel(x+i,y) = color;
                    image.pixel(x+i,y+i) = color;
                    image.pixel(x,y+i) = color;
                    image.pixel(x-i,y+i) = color;
                    image.pixel(x-i,y) = color;
                    image.pixel(x+i,y-i) = color;
                    image.pixel(x-i,y-i) = color;
                    image.pixel(x,y-i) = color;
                }
            }
        }
    }

}


void draw_polynome(std::vector<glm::vec2> list_pos, sil::Image& image, glm::vec3 color)
{
    double thickness = 0.0002;

    // a*x*x*x + b*x*x + c*x + d + e*y = 0 

    Eigen::MatrixXd A(list_pos.size(),list_pos.size()+1);
    std::cout << A << std::endl;
    for(int k{0}; k<list_pos.size(); k++)
    {
        for(int i{0}; i<list_pos.size(); i++)
        {
            A(k,i) = std::pow(list_pos[k].x, list_pos.size()-1-i);
        }
        A << list_pos[k].y;
    }

    std::cout << A << std::endl;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd (A, Eigen::ComputeThinU|Eigen::ComputeFullV);
    Eigen::VectorXd coef = svd.matrixV().rightCols(1);

    // std::cout << coef << std::endl;

    int crayon = 10;

    for(int x{crayon}; x< image.width()-crayon; x++)
    {
        for(int y{crayon}; y< image.height()-crayon; y++)
        {
            if(coef(0)*x*x*x + coef(1)*x*x + coef(2)*x + coef(3) + coef(4)*y <= thickness && coef(0)*x*x*x + coef(1)*x*x + coef(2)*x + coef(3) + coef(4)*y >= -thickness)
            {
                image.pixel(x,y) = color;
                for(int i{1}; i<crayon; i++)
                {
                    image.pixel(x+i,y) = color;
                    image.pixel(x+i,y+i) = color;
                    image.pixel(x,y+i) = color;
                    image.pixel(x-i,y+i) = color;
                    image.pixel(x-i,y) = color;
                    image.pixel(x+i,y-i) = color;
                    image.pixel(x-i,y-i) = color;
                    image.pixel(x,y-i) = color;
                }
            }
        }
    }

}


int main()
{
    sil::Image imageTest {5000, 5000};

    glm::vec3 const WHITE{1.,1.,1.};
    glm::vec3 const RED{1.,0.,0.};
    glm::vec3 const GREEN{0.,1.,0.};
    glm::vec3 const BLUE{0.,0.,1.};


    glm::vec2 const pos1 = glm::vec2{100+1000,2000+1000};
    glm::vec2 const pos2 = glm::vec2{700+1000,200+1000};
    glm::vec2 const pos3 = glm::vec2{1300+1000,2000+1000};
    glm::vec2 const pos4 = glm::vec2{1900+1000,200+1000};

    glm::vec2 const size = glm::vec2{400,400};

    double thickness = 0.01;

    draw_grid(pos1, size, 3, 5, 0, imageTest, WHITE);
    draw_grid(pos2, size, 3, 5, 0, imageTest, WHITE);
    draw_grid(pos3, size, 3, 5, 0, imageTest, WHITE);
    draw_grid(pos4, size, 3, 5, 0, imageTest, WHITE);

    glm::vec2 const longueurDemiCase = glm::vec2{size.x/6, size.y/6};
    glm::vec2 const longueurCase = glm::vec2{size.x/3, size.y/3};


    // Second position

        // Case Rouge
    
    glm::vec2 const pos1red = glm::vec2{pos1.x + longueurDemiCase.x, pos1.y + longueurDemiCase.y};
    glm::vec2 const pos2red = glm::vec2{pos2.x + longueurDemiCase.x + longueurCase.x, pos2.y + longueurDemiCase.y + longueurCase.y};
    glm::vec2 const pos3red = glm::vec2{pos3.x + longueurDemiCase.x + 2*longueurCase.x, pos3.y + longueurDemiCase.y};
    glm::vec2 const pos4red = glm::vec2{pos4.x + longueurDemiCase.x + 2*longueurCase.x, pos4.y + longueurDemiCase.y + 2*longueurCase.y};
    std::vector<glm::vec2> posred {pos1red, pos2red, pos3red, pos4red};
        // Case VERT
    
    glm::vec2 const pos1green = glm::vec2{pos1.x + longueurDemiCase.x + longueurCase.x, pos1.y + longueurDemiCase.y + longueurCase.y};
    glm::vec2 const pos2green = glm::vec2{pos2.x + longueurDemiCase.x + 2*longueurCase.x, pos2.y + longueurDemiCase.y + longueurCase.y};
    glm::vec2 const pos3green = glm::vec2{pos3.x + longueurDemiCase.x + longueurCase.x, pos3.y + longueurDemiCase.y + 2*longueurCase.y};
    glm::vec2 const pos4green = glm::vec2{pos4.x + longueurDemiCase.x, pos4.y + longueurDemiCase.y};

        // Case BLEU
    
    glm::vec2 const pos1blue = glm::vec2{pos1.x + longueurDemiCase.x + 2*longueurCase.x, pos1.y + longueurDemiCase.y + 2*longueurCase.y};
    glm::vec2 const pos2blue = glm::vec2{pos2.x + longueurDemiCase.x + 2*longueurCase.x, pos2.y + longueurDemiCase.y};
    glm::vec2 const pos3blue = glm::vec2{pos3.x + longueurDemiCase.x, pos3.y + longueurDemiCase.y };
    glm::vec2 const pos4blue = glm::vec2{pos4.x + longueurDemiCase.x + longueurCase.x, pos4.y + 2*longueurDemiCase.y};


    // draw_polynome3(pos1, pos2, pos3, pos4, thickness, imageTest, WHITE);
    draw_polynome(posred, imageTest, RED);
    draw_polynome3(pos1green, pos2green, pos3green, pos4green, imageTest, GREEN);
    draw_polynome3(pos1blue, pos2blue, pos3blue, pos4blue, imageTest, BLUE);


    imageTest.save("src/KacperRayon/output/test6.png");

}