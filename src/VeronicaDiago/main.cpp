#include <iostream>
#include <sil/sil.hpp>
#include <Eigen/Dense>
#include <cstdlib> 
#include <ctime>   


void make_diago(sil::Image& imageTest)
{
    int nbrIter = 10000;



    std::srand(std::time(0)); 

    int min = 0;
    int max = 100;

    int minb = imageTest.height() - 200;
    int maxb = imageTest.height() + 200;


    // int minb = 0;
    // int maxb = 100;

    int mina = 0;
    int maxa = 100;

    int minThick = 0;
    int maxThick = 100;

    for(int i{0}; i<nbrIter; i++)
    {
        double a = (((mina) + (std::rand() % (maxa - mina + 1)))/50.) - 1.;
        a *= 2;
        double b = minb + (std::rand() % (maxb - minb + 1));

        double ajout = (min + (std::rand() % (max - min + 1)))/500.;
        // double red = (min + (std::rand() % (max - min + 1)))/100.;
        double red = 0.8 + ajout;
        // double green = (min + (std::rand() % (max - min + 1)))/100.;
        double blue = 0.7 + ajout;
        // double blue = (min + (std::rand() % (max - min + 1)))/100.;
        double green = 0.2 + ajout;

        // std::cout << a << std::endl;

        if(a <= 1.3 && a >= 0.7)
        {
            double thickness = (min + (std::rand() % (max - min + 1)))/10.;
            for(int x{1}; x<imageTest.width()-1; x++)
            {
                for(int y{1}; y<imageTest.height()-1; y++)
                {
                    if(-a*x + b - y <= thickness && -a*x + b - y >= -thickness)
                    {
                        imageTest.pixel(x,y) = glm::vec3{red,green,blue};
                        imageTest.pixel(x+1,y) = glm::vec3{red,green,blue};
                        imageTest.pixel(x,y+1) = glm::vec3{red,green,blue};
                        imageTest.pixel(x-1,y) = glm::vec3{red,green,blue};
                        imageTest.pixel(x,y-1) = glm::vec3{red,green,blue};
                    }
                        
                }
            }
        }


    }
}

int main()
{

    sil::Image imageTest{1000, 1000};

    std::srand(std::time(0)); 

    int min = 0;
    int max = 100;



    int xc = 0;
    int yc = imageTest.height();
    int rayon = 50;
    int offset = 200;

    for(int x{1}; x<imageTest.width()-1; x++)
    {
        for(int y{1}; y<imageTest.height()-1; y++)
        {
            double ajout;
            double red;
            double blue;
            double green;
            if((x + y - imageTest.height()) >0)
            {
                ajout = 0.7*std::pow(2. - (float(x)/(imageTest.width()-1) + float(y)/(imageTest.width()-1)),5);
                red =  0.4 + ajout;
                blue =  0.2 + ajout;
                green = 0.1 + ajout;
            }else{
                ajout = 0.7*std::pow((float(x)/(imageTest.width()-1) + float(y)/(imageTest.width()-1)),5);
                red =  0.4 + ajout;
                blue =  0.2 + ajout;
                green = 0.1 + ajout;
            }
            
            if((((x-xc)*(x-xc) + (y-yc)*(y-yc))%(rayon*rayon + offset) <= rayon*rayon + offset && ((x-xc)*(x-xc) + (y-yc)*(y-yc))%(rayon*rayon + offset) >= rayon*rayon - offset))
            {
                imageTest.pixel(x,y) = glm::vec3{red,green,blue};
                imageTest.pixel(x+1,y) = glm::vec3{red,green,blue};
                imageTest.pixel(x,y+1) = glm::vec3{red,green,blue};
                imageTest.pixel(x-1,y) = glm::vec3{red,green,blue};
                imageTest.pixel(x,y-1) = glm::vec3{red,green,blue};
            }
        }
    }


    imageTest.save("src/VeronicaDiago/output/onde8.png");

    return 0;
}