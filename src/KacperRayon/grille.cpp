#include "grille.hpp"


bool check_in_greed(glm::vec2& pos, glm::vec2& size, int& subdivision, int& thickness, glm::vec2 currentXY)
{
    if(currentXY.x <= pos.x + size.x + thickness-1
     && currentXY.x >= pos.x 
     && currentXY.y >= pos.y 
     && currentXY.y <= pos.y + size.y + thickness-1)
    {
        if(int(currentXY.y-pos.y)%int(size.y/subdivision) <= thickness || int(currentXY.x-pos.x)%int(size.x/subdivision) <= thickness)
            return true;

    }
    return false;
}

glm::vec2 apply_rotation(int x, int y, double angle, glm::vec2 size, glm::vec2 pos)
{
    Eigen::Matrix3d RotationM;
    RotationM << std::cos(angle), -std::sin(angle), 0.,
                std::sin(angle), std::cos(angle), 0.,
                0. , 0., 1.;
    Eigen::Vector3d ActualPoint;
    ActualPoint << x - size.x/2 - pos.x, y - size.y/2 - pos.y, 1.;

    Eigen::Vector3d ResultPoint = RotationM*ActualPoint;
    return glm::vec2{ResultPoint(0) + size.x/2 + pos.x, ResultPoint(1) + size.y/2 + pos.y};
}

void draw_grid(glm::vec2 pos, glm::vec2 size, int subdivision, int thickness, double angle, sil::Image& image, glm::vec3 color)
{
    
    for(int x{0}; x< image.width(); x++)
    {
        for(int y{0}; y< image.height(); y++)
        {
            if(check_in_greed(pos, size, subdivision, thickness, glm::vec2{x,y}))
            {

                glm::vec2 newCurrentXY = apply_rotation(x ,y , angle, size, pos);
                if(newCurrentXY.x > 0. && newCurrentXY.x < image.width() && newCurrentXY.y > 0 && newCurrentXY.y < image.height())
                {
                    image.pixel(newCurrentXY.x, newCurrentXY.y) = color;
                    if(newCurrentXY.x+1 < image.width() && newCurrentXY.y + 1 < image.height())
                        image.pixel(newCurrentXY.x, newCurrentXY.y+1) = color;
                        image.pixel(newCurrentXY.x+1, newCurrentXY.y) = color;
                    if(newCurrentXY.x-1 > 0. && newCurrentXY.y-1 > 0. )
                        image.pixel(newCurrentXY.x, newCurrentXY.y-1) = color;
                        image.pixel(newCurrentXY.x-1, newCurrentXY.y) = color;
                }

            }
        }
    }

}