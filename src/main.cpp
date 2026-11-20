#include <sil/sil.hpp>
#include <random.hpp>
#include <cmath>
void keep_green_only(sil::Image& image) 
{
    for (glm::vec3& color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}
void excanghe_channels(sil::Image& image) 
{
     for (glm::vec3& color : image.pixels())
    {
        std::swap(color.b, color.r);
    }
}
void black_and_white(sil::Image& image) 
{
     for (glm::vec3& color : image.pixels())
    {
        float const grey = (0.229 * color.r +  0.587 * color.g + 0.114 * color.b);
        color = glm::vec3{grey};
    }
}
void negative(sil::Image& image) 
{
     for (glm::vec3& color : image.pixels())
    {
        color = 1.f - color;
    }
}
void degrade(sil::Image& image){
    for (int x = 0; x < image.width(); x++)
    {
        float const grey = (float)x / (image.width() -1);
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y) = glm::vec3{grey};
        }
    }
}
void mirror(sil::Image& image){
    sil::Image const copy = image;
    for (int x = 0; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y) = copy.pixel(image.width()- x - 1 , y);
        }
    }
}
void rotate(sil::Image& image)
{
    sil::Image rotated{image.height(), image.width()};

    for (int x = 0; x < rotated.width(); x++)
    {
        for (int y = 0; y < rotated.height(); y++)
        {
            rotated.pixel(x, y) = image.pixel(y, image.height() - 1 - x);
        }
    }

    image = rotated;
}
void noise(sil::Image& image)
{
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        { 
                if (random_int(0,10) < 5)
                {

                image.pixel(x, y).r = random_float(0.f, 1.f);
                image.pixel(x, y).g = random_float(0.f, 1.f);
                image.pixel(x, y).b = random_float(0.f, 1.f);

                }
        }
    }
}

void RGB(sil::Image& image){
    sil::Image copy = image;
    int dif = 30;
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            if (x + dif < image.width())
            {
                copy.pixel(x, y).r = image.pixel(x + dif, y).r;
            }
            if (x - dif >= 0)
            {
                copy.pixel(x, y).b = image.pixel(x - dif, y).b;
            }
        }   
    }
    image = copy;
}

void Brightness(sil::Image& image){
    int power = random_int(2,5);
    for (glm::vec3& color : image.pixels())
    {
        color.r = std::pow(color.r, power);
        color.b = std::pow(color.b, power);
        color.g = std::pow(color.g, power);
    }
}

void whiteCircle(sil::Image& image){
    float const radius = 100;
    float const circle_pixelX = image.width() / 2;
    float const circle_pixelY = image.height() / 2;
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            float const dx = x - circle_pixelX;
            float const dy = y - circle_pixelY;
            float const distance = sqrt(pow(dx,2)+pow(dy,2));
            if (distance < radius){
                image.pixel(x,y) = glm::vec3{1, 1, 1};
            }
        }
    }
}
void whiteCircleMargin(sil::Image& image, float circlex, float circley, float radius){
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            float const dx = x - circlex;
            float const dy = y - circley;
            float const distance = sqrt(pow(dx,2)+pow(dy,2));
            if (distance < radius && distance > radius-5){
                image.pixel(x,y) = glm::vec3{1, 1, 1};
            }
        }
    }
}
void Rosette(sil::Image& image){
    
    float const centerX = image.width()/ 2;
    float const centerY = image.height()/ 2;
    whiteCircleMargin(image, centerX, centerY, 100);

    const double PI = 3.141592653589793;
    for (int i = 0; i < 6; i++)
    {
        float const cx = centerX + 100 * std::cos(i * PI / 3.0);
        float const cy = centerY + 100 * std::sin(i * PI / 3.0);
        whiteCircleMargin(image, cx, cy, 100);
    }
    
}

void Mozaic(sil::Image& image)
{
    int const n = 5;
    sil::Image copy = image;

    for (int x = 0; x < copy.width(); x++)
    {
        for (int y = 0; y < copy.height(); y++)
        {
            copy.pixel(x, y) = image.pixel((x * n) % image.width(), (y * n) % image.height());
        }
    }

    image = copy;
}

void MirorMozaic(sil::Image& image)
{
    int const n = 5;
    sil::Image copy = image;

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            int second_x = (x * n) % image.width();
            int second_y = (y * n) % image.height();

            if ((x * n) / image.width() == 1)
            {
                second_x = image.width() - 1 - second_x;
            }
            if ((y * n) / image.height()% 2 == 1)
            {
                second_y = image.height() - 1 - second_y;
            }

            copy.pixel(x, y) = image.pixel(second_x, second_y);
        }
    }

    image = copy;
}


int main()
{
    {
        sil::Image image{"images/logo.png"};
        keep_green_only(image);
        image.save("output/pouet.png");
    }
    {
        sil::Image image{"images/logo.png"};
        excanghe_channels(image);
        image.save("output/pouet2.png");
    }
    {
        sil::Image image{"images/logo.png"};
        black_and_white(image);
        image.save("output/pouet3.png");
    }
    {
        sil::Image image{300,200};
        degrade(image);
        image.save("output/pouet4.png");
    }
    {
        sil::Image image{"images/logo.png"};
        negative(image);
        image.save("output/pouet5.png");
    }
    {
        sil::Image image{"images/logo.png"};
        mirror(image);
        image.save("output/pouet6.png");
    }
    {
        sil::Image image{"images/logo.png"};
        rotate(image);
        image.save("output/pouet7.png");
    }
    {
        sil::Image image{"images/logo.png"};
        noise(image);
        image.save("output/pouet8.png");
    }
    {
        sil::Image image{"images/logo.png"};
        RGB(image);
        image.save("output/pouet9.png");
    }
    {
        sil::Image image{"images/photo_faible_contraste.jpg"};
        Brightness(image);
        image.save("output/pouet10.png");
    }
    {
        sil::Image image{500,500};
        whiteCircle(image);
        image.save("output/pouet11.png");
    }
    {
        sil::Image image{500,500};
        whiteCircleMargin(image, image.width()/2, image.height()/2, 100);
        image.save("output/pouet12.png");
    }
    {
        sil::Image image{500,500};
        Rosette(image);
        image.save("output/pouet13.png");
    }
    {
        sil::Image image{"images/logo.png"};
        Mozaic(image);
        image.save("output/pouet14.png");
    }
    {
        sil::Image image{"images/logo.png"};
        MirorMozaic(image);
        image.save("output/pouet15.png");
    }
}
