// Maciej Serafin
//komenda do kompilacji\\\\\\
 //g++ -std=c++11 prub.cpp -o zad

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
// implementacja Klasy  Canvas
class Canvas
{
protected:
    int height;
     int width;
public:
    Canvas(int h, int w) : height(h), width(w) {}
    virtual void draw(const std::string& filename) = 0;
};

 class ASCIICanvas : public Canvas 
 {
  private:
    std::vector<std::string> ascii_art;
  public:
    ASCIICanvas(int h, int w) : Canvas(h, w) 
    {
        ascii_art = std::vector<std::string>(height, std::string(width, ' '));
    }

    void draw(const std::string& filename) override 
    {
        std::ofstream out_file(filename);
        for (const auto& line : ascii_art) 
        {
            out_file << line << std::endl;
        }
    }
    void set_pixel(int x, int y, char c) 
    {
        if (x >= 0 && x < width && y >= 0 && y < height) 
        {
            ascii_art[y][x] = c;
        }
    }
};

class Figure
{
protected:
    int x, y;
     char fill_char;
public:
    Figure(int x, int y, char c) : x(x), y(y), fill_char(c) {}
     virtual void draw(ASCIICanvas& canvas) = 0;
      virtual ~Figure() {}//deskuktor
};
class Rectangle : public Figure 
{
protected:
    int height; // Wysokość prostokąta
     int width;  // Szerokość prostokąta
public:
    Rectangle(int x, int y, char c, int h, int w) : Figure(x, y, c), height(h), width(w) {}

    // Implementacja funkcji  draw
    void draw(ASCIICanvas& canvas) override 
    {
        for(int i = x - width/2; i <= x + width/2; i++) 
        {
            for(int j = y - height/2; j <= y + height/2; j++) 
            {
                // Puste w środku
                if (i == x - width/2 || i == x + width/2 || j == y - height/2 || j == y + height/2)
                {
                    canvas.set_pixel(i, j, fill_char);
                }
            }
        }
    }
};

class Square : public Rectangle 
{
public:
    // Konstruktor
    Square(int x, int y, char c, int side) : Rectangle(x, y, c, side, side) {}
};

class Triangle : public Figure 
{
private:
    int height; // Wysokość trójkąta

public:
    // Konstruktor
    Triangle(int x, int y, char c, int h) : Figure(x, y, c), height(h) {}

    // Implementacja metody draw (i tego jak rysuje daną figurę)
    void draw(ASCIICanvas& canvas) override  
    {
        for(int i = 0; i <= height; i++)
        {
            for(int j = x - i; j <= x + i; j++) 
            {
                // Rysowanie tylko linii obrzeżnych trójkąta
                if (j == x - i || j == x + i || i == height)
                {
                    canvas.set_pixel(j, y - i, fill_char);
                }
            }
        }
    }
};

//funkcja do obsługi pliku konfiguracyjnego (czyta jaka figura , i jej parametry)
void load_config(const std::string& filename, ASCIICanvas& canvas, std::vector<Figure*>& figures, std::string& output_file) 
{
    std::ifstream file(filename);
     std::string line;
     //zczytywani z pliku linia  po lini za pomocą while
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
         std::string command;
          iss >> command;
          //sprawdzanie które polecenie(komenda) zosała uytwa do narysowania figury
        if (command == "canvas") 
        {
            int height, width;
             iss >> height >> width;
              canvas = ASCIICanvas(height, width);
        }
        else if (command == "output") 
        {
            iss >> output_file;
        }
        else if (command == "rectangle") 
        {
            int x, y, height, width;
            //zmeinna fill char do przechowywania znaki jaki m będzie rysowana figura
             char fill_char;
              iss >> x >> y >> fill_char >> height >> width;
               figures.push_back(new Rectangle(x, y, fill_char, height, width));
        }
        else if (command == "square") 
        {
            int x, y, side;
             char fill_char;
              iss >> x >> y >> fill_char >> side;
               figures.push_back(new Square(x, y, fill_char, side));
        }
        else if (command == "triangle") 
        {
            int x, y, height;
             char fill_char;
              iss >> x >> y >> fill_char >> height;
               figures.push_back(new Triangle(x, y, fill_char, height));
        }
    }
}

int main(int argc, char** argv) 
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " CONFIG_FILE" << std::endl;
        return 1;
    }

    ASCIICanvas canvas(0, 0);
     std::vector<Figure*> figures;
      std::string output_file;
       load_config(argv[1], canvas, figures, output_file);

    for (auto figure : figures) 
    {
        figure->draw(canvas);
    }

    canvas.draw(output_file);

    for (auto figure : figures) 
    {
        delete figure;
    }

    return 0;
}
