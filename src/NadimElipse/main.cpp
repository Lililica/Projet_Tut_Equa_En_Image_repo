#include <iostream>
#include <sil/sil.hpp>
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include <cstdlib> 
#include <ctime>   

void draw_circle(glm::vec2 pos, int rayon, sil::Image& image, glm::vec3& color)
{
    for(int x{0}; x< image.width(); x++)
    {
        for(int y{0}; y< image.height(); y++)
        {
            if((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) <= rayon*rayon)
            {
                image.pixel(x,y) = color;
            }
        }
    }

}


void draw_truc_point(glm::vec2 point1, glm::vec2 point2, sil::Image& imageTest)
{
    int xc = point1.x;
    int yc = point1.y;

    // int rayon = ;


    float crayon = 2;


    for(int it{1}; it<20; it++)
    {
        // double invertIt = 1./it;
        Eigen::MatrixXd A(2, 2);
        A << std::pow(point1.x-xc,it), point1.y-yc,
        std::pow(point2.x-xc,it), point2.y-yc;

        Eigen::MatrixXd B(2, 2);
        B << std::pow(point1.x-point2.x,it), point1.y-point2.y,
        0., 0.;

        // std::cout << B << std::endl;

        Eigen::JacobiSVD<Eigen::MatrixXd> svd (A, Eigen::ComputeThinU|Eigen::ComputeFullV);
        Eigen::VectorXd coef = svd.matrixV().rightCols(1);

        // Eigen::JacobiSVD<Eigen::MatrixXd> svd2 (B, Eigen::ComputeThinU|Eigen::ComputeFullV);
        // Eigen::VectorXd coefInvert = svd2.matrixV().rightCols(1);

        Eigen::VectorXd coefInvert = -1*coef;

        // std::cout << coef << std::endl;

        for(int x{0}; x< imageTest.width()-crayon; x++)
        {
            for(int y{0}; y<imageTest.height()-crayon; y++)
            {
                if(x>=point1.x && y>=point1.y && x<=point2.x && y<=point2.y)
                {
                    if(coef(0)*std::pow(x-xc,it) + coef(1)*(y-yc) >= -crayon && coef(0)*std::pow(x-xc,it) + coef(1)*(y-yc) <= crayon)
                    {     
                        imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                    }
                    if(coefInvert(0)*std::pow(x-point2.x,it) + coefInvert(1)*(y-point2.y) >= -crayon && coefInvert(0)*std::pow(x-point2.x,it) + coefInvert(1)*(y-point2.y) <= crayon)
                    {                
                        imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                    }
                }
            }   
        }
    }
}

glm::vec2 apply_rotation(int x, int y, double angle = 3.14/2.)
{

    Eigen::Matrix3d RotationM;
    RotationM << std::cos(angle), -std::sin(angle), 0.,
                std::sin(angle), std::cos(angle), 0.,
                0. , 0., 1.;
    Eigen::Vector3d ActualPoint;
    ActualPoint << x, y, 1.;

    Eigen::Vector3d ResultPoint = RotationM*ActualPoint;
    return glm::vec2{ResultPoint(0), ResultPoint(1)};
}

void draw_eyes(glm::vec2 point1, glm::vec2 point2, sil::Image& imageTest)
{
    // if(point1.x > point2.x)
    // {
    //     glm::vec2 temp = point1;
    //     point1 = point2;
    //     point2 = temp;
    // }
    glm::vec2 vecteur = glm::vec2{point2.x-point1.x, point2.y-point1.y};
    if((point2.x-point1.x)*(point2.y-point1.y)>=0)
        vecteur = glm::vec2{std::abs(point2.x-point1.x), std::abs(point2.y-point1.y)};
    else if((point2.y-point1.y)<0)
        vecteur = glm::vec2{point1.x-point2.x, std::abs(point2.y-point1.y)};
    glm::vec2 vecteurOrtho = apply_rotation(vecteur.x, vecteur.y);

    glm::vec2 pointMilieu {(point1.x + point2.x)/2, (point1.y + point2.y)/2};

    std::vector<glm::vec2> newCenter;
    std::vector<int> rayonList;

    std::srand(std::time(0)); 

    int min = 20;
    int max = 70;

    int nbrDemiCercle = min + (std::rand() % (max - min + 1));

    for(int i{0}; i<nbrDemiCercle; i++)
    {
        newCenter.push_back(pointMilieu + glm::vec2{(i-nbrDemiCercle/2)/15.,(i-nbrDemiCercle/2)/15.}*vecteurOrtho);
        rayonList.push_back(std::pow(std::pow(newCenter[i].x - point1.x, 2) + std::pow(newCenter[i].y - point1.y, 2),0.5));
    }

    // draw_truc_point(point1, point2, imageTest);
    // draw_truc_point(point3, point2, imageTest);
    // draw_truc_point(point3, point4, imageTest);

    double a = (point2.y - point1.y)/(point2.x - point1.x);
    std::cout << a << std::endl;
    double b = point1.y - a*point1.x;

    for(int i{0}; i<nbrDemiCercle; i++)
    {
        int xc = newCenter[i].x;
        int yc = newCenter[i].y;

        // int xc = 0;
        // int yc = 0;

        int rayon = rayonList[i];
        double offset = 5000;

        // std::cout << rayon << std::endl;
        if(i == 0)
        {
            
            for(int x{0}; x<imageTest.width(); x++)
            {
                for(int y{0}; y<imageTest.height(); y++)
                {
                    if((std::pow(x-newCenter[i + nbrDemiCercle/2].x, 2) + std::pow(y-newCenter[i + nbrDemiCercle/2].y, 2) - rayonList[i + nbrDemiCercle/2]*rayonList[i + nbrDemiCercle/2] <= offset))
                    {   
                        if((std::pow(x-newCenter[i + nbrDemiCercle/2].x, 2) + std::pow(y-newCenter[i + nbrDemiCercle/2].y, 2) - rayonList[i + nbrDemiCercle/2]*rayonList[i + nbrDemiCercle/2] >= -offset))
                            // imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                        // else
                            imageTest.pixel(x,y) = glm::vec3{0.,0.,0.};
                    }

                        
                }
            }
            continue;
        }



        for(int x{0}; x<imageTest.width(); x++)
        {
            for(int y{0}; y<imageTest.height(); y++)
            {
                // if(std::pow(x-xc, 2) + std::pow(y-yc, 2) - rayon*rayon >= -offset && (std::pow(x-xc, 2) + std::pow(y-yc, 2) - rayon*rayon <= offset))
                if(std::pow(x-xc, 2) + std::pow(y-yc, 2) - rayon*rayon >= -offset && (std::pow(x-xc, 2) + std::pow(y-yc, 2) - rayon*rayon <= offset))
                {
                    if(i - nbrDemiCercle/2 > 0){
                        if(a>0 && a*x + b>=y)
                            imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                        if(a<=0 && y>=a*x + b)
                            imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                    }
                    else if(i - nbrDemiCercle/2 < 0){
                        if(a> 0 && y>=a*x + b)
                            imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                        if(a<=0 && a*x + b>=y)
                            imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};
                    }
                    // imageTest.pixel(x,y) = glm::vec3{1.,1.,1.};

                }
                    
            }
        }
    }
}



int main()
{
    sil::Image imageTest{4000, 4000};

    std::srand(std::time(0)); 

    int min = 0;
    int max = imageTest.width();

    // glm::vec2 point1 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};
    // glm::vec2 point2 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};
    // glm::vec2 point3 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};
    // glm::vec2 point4 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};
    // glm::vec2 point5 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};
    // glm::vec2 point6 {min + (std::rand() % (max - min + 1)), min + (std::rand() % (max - min + 1))};

    glm::vec2 point1 {2000, 500};
    glm::vec2 point2 {1000, 1700};
    glm::vec2 point3 {1800, 2000};
    glm::vec2 point4 {2300, 2150};
    glm::vec2 point5 {2100, 2700};
    glm::vec2 point6 {3000, 3500};

    draw_eyes(point1, point6, imageTest);
    draw_eyes(point2, point6, imageTest);
    draw_eyes(point2, point5, imageTest);
    draw_eyes(point1, point2, imageTest);
    draw_eyes(point2, point3, imageTest);
    draw_eyes(point3, point4, imageTest);
    draw_eyes(point4, point5, imageTest);
    draw_eyes(point5, point6, imageTest);


    // std::cout << "("<< point1.x << "," << point1.y << ")" << "\n";
    // std::cout << "("<< point2.x << "," << point2.y << ")" << "\n";

    // draw_eyes(point1,point2,imageTest);
    // draw_eyes(point2,point3,imageTest);
    // draw_eyes(point4,point3,imageTest);
    // draw_eyes(point4,point1,imageTest);
    // draw_eyes(point4,point2,imageTest);
    // draw_eyes(point1, point5,imageTest);
    // draw_eyes(point3, point5, imageTest);
    // draw_eyes(point4, point6,imageTest);
    // draw_eyes(point6, point2, imageTest);






    imageTest.save("src/NadimElipse/output/test14.png");


}