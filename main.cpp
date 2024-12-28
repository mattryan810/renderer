#include <iostream>
#include <Eigen/Dense> 
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

const double pi = 3.14159265358979323846;

// Convert cartesian to polar
Eigen::MatrixXd polConvert(const Eigen::MatrixXd& input) {
        Eigen::VectorXd x,y,z;
        x = input.row(0);
        y = input.row(1);
        z = input.row(2);
        Eigen::VectorXd r(x.size());
        Eigen::VectorXd p(x.size());
        Eigen::VectorXd t(x.size());
        Eigen::MatrixXd pol(3,x.size());

        for (int i = 0; i < x.size(); ++i) {
            r(i) = std::sqrt(x(i) * x(i) + y(i) * y(i) + z(i) * z(i));
            // atan2 handles div by zero cases but is weird cus its two arguments: first / second
            t(i) = std::atan2(y(i),x(i));
            p(i) = std::atan2(std::sqrt(x(i)*x(i) + y(i)*y(i)) , z(i));
            
        }

        pol.row(0) = r;
        pol.row(1) = t;
        pol.row(2) = p;

        return pol;
}


// define camera
class Camera {
    public:

    Eigen::Vector3d direction;
    double fov; // IN RADIANS

    Camera(const Eigen::Vector3d& direction_vec, const double view_fov):
    direction(direction_vec),fov(view_fov) {

        // Ensures direction is in polar coordinates
        direction = polConvert(direction);

    }
};

//define shape
class Shape {

    public :

    Eigen::MatrixXd orVerts;
    

    Shape(int vertexNum) : orVerts(3,vertexNum) {}

    // get polar vertices
    virtual const Eigen::MatrixXd pVerts() const {
        Eigen::MatrixXd pV;
        pV << 0;
        return pV;
    };

};

// define cube
class Cube : public Shape {

    public:

    double sideLength;
    Eigen::Vector3d center;

    // Vertices pre-transformation, centered at true origin
    Eigen::Matrix<double, 3, 8> orVerts;

    // Constructor
    Cube(double side_length, const Eigen::Vector3d& centerPoint)
    : sideLength(side_length), center(centerPoint), Shape(8)
        {
           double h = sideLength / 2;

            orVerts <<  h,  h,  h,  h, -h, -h, -h, -h,
                        h,  h, -h, -h,  h,  h, -h, -h,
                        h, -h,  h, -h,  h, -h,  h, -h;
    
        }

    // Member Function

    const Eigen::MatrixXd pVerts() const {
        Eigen::MatrixXd polVerts;
        polVerts = orVerts.colwise() + center;
        polVerts = polConvert(polVerts);
        return polVerts;
    };


};

// define render option setter
class RenderOptions {
public:
    int width;
    int height;
    float framerate;


    RenderOptions(int window_width, int window_height, float window_framerate):
    width(window_width), height(window_height), framerate(window_framerate) {}

    const int getWidth() const {
        return width;
    };

    public:
    const int getHeight() const {
        return height;
    };

    public:
    const int getFramerate() const {
        return framerate;
    };


};


// Project polar to 2D coordinates:
Eigen::MatrixXd project(const Eigen::MatrixXd& polarCoords, const Camera& cam, const RenderOptions& rend) {
    
    // separation variable
    assert(polarCoords.rows() == 3);
    Eigen::MatrixXd sep = polarCoords.colwise() - cam.direction;
    double scale = rend.getWidth() / cam.fov;

    // convert to screen coordinates
    sep.row(1) = (sep.row(1).array() * scale).round() + (rend.getWidth() / 2);
    sep.row(2) = (sep.row(2).array() * scale).round() + (rend.getHeight() / 2);
    
    return sep;
    
};

std::vector<sf::CircleShape> show(const Eigen::MatrixXd& points) {
    std::vector<sf::CircleShape> verts;
    int ptCount = points.cols();
    for (int i = 0; i < ptCount; ++i) {
        sf::CircleShape vertex(50 / (points(0,i) * points(0,i)));
        vertex.setPosition(points(1,i),points(2,i));
        vertex.setFillColor(sf::Color::Cyan);
        verts.push_back(vertex);
    }
    return verts;
}

int main() {
    Eigen::Vector3d displace(4,0,0);
    Cube cube(2,displace);
    Eigen::Vector3d camDir(1,0,0);
    Camera cam(camDir,2);
    RenderOptions rend(800,600,75);
    Eigen::Matrix3d zRotL,zRotR,yRotL,yRotR;
        zRotL << 0.99863,    -0.052336, 0,
                 0.052336,   0.99863,   0,
                 0,          0,         1;
               
        zRotR << 0.99863,    0.052336,  0,
                 -0.052336,  0.99863,   0,
                 0,          0,         1;
        yRotR << 0.99863,   0, 0.052336,
                 0,         1, 0,
                 -0.052336, 0, 0.99863;
        yRotL << 0.99863,   0, -0.052336,
                 0,         1, 0,
                0.052336,   0, 0.99863;
    bool right = false;
    bool left = false;
    bool a = false;
    bool d = false;
    bool w = false;
    bool s = false;
    bool up = false;
    bool down = false;


    sf::RenderWindow window(sf::VideoMode(rend.getWidth(), rend.getHeight()), "Renderer");
        window.setFramerateLimit(rend.getFramerate());


        while (window.isOpen()) {

            // Closes window
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
               
            // motion controls

                //key pressed
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.scancode == sf::Keyboard::Scan::Right)
                    {
                        right = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Up)
                    {
                        up = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        down = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Left)
                    {
                        left = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::D)
                    {
                        d = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::A)
                    {
                        a = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::W)
                    {
                        w = true;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::S)
                    {
                        s = true;
                    }

                }
                // key released
                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.scancode == sf::Keyboard::Scan::Right)
                    {
                        right = false;

                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Left)
                    {
                        left = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Up)
                    {
                        up = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        down = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::D)
                    {
                        d = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::A)
                    {
                        a = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::W)
                    {
                        w = false;
                    }
                    if (event.key.scancode == sf::Keyboard::Scan::S)
                    {
                        s = false;
                    }
                }
                // results
                if (right == true) {
                    cam.direction = zRotR *  cam.direction;
                }
                if (left == true) {
                    cam.direction = zRotL * cam.direction;
                }
                if (d == true) {
                    cube.orVerts = zRotR * cube.orVerts;
                }
                if (a == true) {
                    cube.orVerts = zRotL * cube.orVerts;
                }
                if (w == true) {
                    cube.orVerts = yRotR * cube.orVerts;
                }
                if (s == true) {
                    cube.orVerts = yRotL * cube.orVerts;
                }
                if (up == true) {
                    cube.center = cube.center + 0.1 * (cam.direction / cam.direction.norm());
                }
                if (down == true) {
                    cube.center = cube.center - 0.1 * (cam.direction / cam.direction.norm());
                }


            // Starts new frame
            window.clear(sf::Color::Black);

            std::vector<sf::CircleShape> circles = show(project(cube.pVerts(), cam, rend));
            for (auto& circle : circles) {
                window.draw(circle);
            }

            
            // Shows drawn frame
            window.display();


            }

        }
    
return 1;
}