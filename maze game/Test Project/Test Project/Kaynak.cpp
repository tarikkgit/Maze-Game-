//220202015 tarik ertugrul ozkul
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>


const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int SQUARE_SIZE = 50;

const int dx[] = { 0, 0, -1, 1 }; // Y�nler: Yukar�, A�a��, Sol, Sa�
const int dy[] = { -1, 1, 0, 0 };


struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
    // == operat�r�n�n tan�m�
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class StaticObstacleType {
    Tree,
    Tree2,
    Mountain,
    Mountain2,
    Rock,
    Rock2,
    Wall,
    Wall2,
    Bird,
    Bee
};

enum class MovingObstacleType {
    Bird,
    Bee
};

enum class TreasureType {
    Gold,
    Silver,
    Emerald,
    Copper
};

enum class Theme {
    Winter,
    Summer
};





class StaticObject : public sf::Sprite {
public:
    sf::Texture texture;
    StaticObject(float scale = 0.5f) { // �l�ek fakt�r� varsay�lan olarak 0.5
        setScale(scale, scale);
    }
};

class MovingObject : public sf::Sprite {
public:
    sf::Texture texture;
};

class Treasure : public sf::Sprite {
public:
    sf::Texture texture;
    Treasure(float scale = 0.5f) { // �l�ek fakt�r� varsay�lan olarak 0.5
        setScale(scale, scale);
    }
};

// Ekran� sis efekti ile kapatma fonksiyonu
void fogScreen(sf::RenderWindow& window) {
    sf::RectangleShape fog(sf::Vector2f(window.getSize().x, window.getSize().y));
    fog.setFillColor(sf::Color(0, 0, 0, 150)); // Sis efektini ayarla
    window.draw(fog);
    window.display();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sis efekti i�in bekleme s�resi
}
// En k�sa yolu �izdirme fonksiyonu
void drawShortestPath(sf::RenderWindow& window, const std::vector<Point>& shortestPath) {
    sf::VertexArray lines(sf::LinesStrip, shortestPath.size());
    for (size_t i = 0; i < shortestPath.size(); ++i) {
        lines[i].position = sf::Vector2f(shortestPath[i].y * SQUARE_SIZE + SQUARE_SIZE / 2, shortestPath[i].x * SQUARE_SIZE + SQUARE_SIZE / 2);
        lines[i].color = sf::Color::Green; // Yolu ye�il renkle �iz
    }
    window.draw(lines);
    window.display();
}


enum class RockSize {
    Size_1x1,
    Size_2x2,
    Size_3x3
};

enum class MountainSize {
    Size_1x1,
    Size_15x15
};

enum class TreeSize {
    Size_1x1,
    Size_2x2,
    Size_3x3,
    Size_4x4,
    Size_5x5
};

enum class WallSize {
    Size_1x1,
    Size_10x1
};



class StaticObstacleFactory {
public:
    static StaticObject* createStaticObstacle(StaticObstacleType type, Theme theme) {
        StaticObject* staticObstacle = new StaticObject;
        std::string filePath;
        if (theme == Theme::Winter) {
            switch (type) {
            case StaticObstacleType::Tree:
                filePath = "C:\\Resimler\\Winter Tree.jpeg";
                break;
            case StaticObstacleType::Tree2:
                filePath = "C:\\Resimler\\Summer Tree.jpeg";
                break;
            case StaticObstacleType::Mountain:
                filePath = "C:\\Resimler\\Winter Mountain.jpeg";
                break;
            case StaticObstacleType::Mountain2:
                filePath = "C:\\Resimler\\Summer Mountain.jpeg";
                break;
            case StaticObstacleType::Rock:
                filePath = "C:\\Resimler\\Winter Rock.jpeg";
                break;
            case StaticObstacleType::Rock2:
                filePath = "C:\\Resimler\\Summer Rock.jpeg";
                break;
            case StaticObstacleType::Wall:
                filePath = "C:\\Resimler\\Winter Wall.jpeg";
                break;
            case StaticObstacleType::Wall2:
                filePath = "C:\\Resimler\\indir.jpeg";
                break;
            }
        }
        else if (theme == Theme::Summer) {
            switch (type) {
            case StaticObstacleType::Tree:
                filePath = "C:\\Resimler\\Summer Tree.jpeg";
                break;
            case StaticObstacleType::Mountain:
                filePath = "C:\\Resimler\\Summer Mountain.jpeg";
                break;
            case StaticObstacleType::Rock:
                filePath = "C:\\Resimler\\Summer Rock.jpeg";
                break;
            case StaticObstacleType::Wall:
                filePath = "C:\\Resimler\\Summer Wall.jpeg";
                break;
            }
        }

        if (!staticObstacle->texture.loadFromFile(filePath)) {
            std::cerr << "Error loading texture: " << filePath << std::endl;
            delete staticObstacle;
            staticObstacle = nullptr;
        }
        else {
            staticObstacle->setTexture(staticObstacle->texture);
            staticObstacle->setScale(0.5f, 0.5f); // �l�eklendirme
        }

        return staticObstacle;
    }

};
/*
StaticObject* StaticObstacleFactory::createStaticObstacle(StaticObstacleType type, Theme theme, int size) {
    StaticObject* staticObstacle = new StaticObject;
    std::string filePath;
    sf::Texture texture;

    // Engelin tipine g�re dosya yolu belirle
    switch (type) {
    case StaticObstacleType::Tree:
        filePath = "C:\\Resimler\\Tree.jpeg"; // �rnek dosya yolu, ger�ek yolunuza g�re g�ncelleyin
        break;
        // Di�er engel t�rleri i�in ayn� i�lemi yap
    }

    // Resimleri y�kle
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
        delete staticObstacle;
        staticObstacle = nullptr;
    }
    else {
        // Belirtilen boyuta g�re resmi �l�eklendir
        staticObstacle->setTexture(texture);
        staticObstacle->setScale(static_cast<float>(size) / static_cast<float>(texture.getSize().x),
            static_cast<float>(size) / static_cast<float>(texture.getSize().y));
    }

    return staticObstacle;
}
*/


class MovingObstacleFactory {
public:
    static MovingObject* createMovingObstacle(MovingObstacleType type) {
        std::string filePath;
        switch (type) {
        case MovingObstacleType::Bird:
            filePath = "C:\\Resimler\\Bird.jpeg";
            break;
        case MovingObstacleType::Bee:
            filePath = "C:\\Resimler\\Bee.jpeg";
            break;
        default:
            std::cerr << "Invalid moving obstacle type!" << std::endl;
            return nullptr;
        }

        MovingObject* movingObstacle = new MovingObject;
        if (!movingObstacle->texture.loadFromFile(filePath)) {
            std::cerr << "Error loading texture: " << filePath << std::endl;
            delete movingObstacle;
            return nullptr;
        }

        movingObstacle->setTexture(movingObstacle->texture);
        movingObstacle->setScale(0.5f, 0.5f); // �l�eklendirme
        return movingObstacle;
    }
};

class Bird : public MovingObject {
private:
    int verticalRange; // Yatay olarak hareket edebilece�i maksimum mesafe
    int direction; // Hareket y�n� (UP veya DOWN)
    Point initialPosition; // Ku�un ba�lang�� konumu
public:
    Bird(int range) : verticalRange(range) {
        direction = UP;
    }
    void setInitialPosition(int x, int y, int mapSize) {
        // Ku�un ba�lang�� pozisyonunu belirle
        initialPosition.x = x;
        initialPosition.y = y;

        // Harita boyutunu kontrol et ve pozisyonlar� ayarla
        if (initialPosition.x < 0) {
            initialPosition.x = 0;
        }
        else if (initialPosition.x >= mapSize * SQUARE_SIZE) {
            initialPosition.x = mapSize * SQUARE_SIZE - 1;
        }

        if (initialPosition.y < 0) {
            initialPosition.y = 0;
        }
        else if (initialPosition.y >= mapSize * SQUARE_SIZE) {
            initialPosition.y = mapSize * SQUARE_SIZE - 1;
        }
    }
    void update() {
        // Hareket y�n�ne g�re konumu g�ncelle
        if (direction == UP) {
            move(0, -1); // Yukar� y�nde hareket
            if (getPosition().y <= (initialPosition.y - verticalRange)) {
                direction = DOWN; // Maksimum y�ksekli�e ula�t���nda y�n� de�i�tir
            }
        }
        else {
            move(0, 1); // A�a�� y�nde hareket
            if (getPosition().y >= initialPosition.y) {
                direction = UP; // Maksimum a�a�� inildi�inde y�n� de�i�tir
            }
        }
    }
};


class BirdFactory {
public:
    static Bird* createBird(int range) {
        Bird* bird = new Bird(range);
        // Ku�un resmini y�kleme i�lemi burada yap�labilir
        return bird;
    }
};



class TreasureFactory {
private:
    static std::vector<sf::Texture> textures;

public:
    static void initializeTextures() {
        // Resim dosyalar�n� y�kle
        std::string fileNames[] = {
            "C:\\Resimler\\Golden Chest.jpeg",
            "C:\\Resimler\\Silver Chest.jpeg",
            "C:\\Resimler\\Emerald Chest.jpeg",
            "C:\\Resimler\\Copper Chest.jpeg"
        };

        for (const auto& fileName : fileNames) {
            sf::Texture texture;
            if (texture.loadFromFile(fileName)) {
                textures.push_back(texture);
            }
            else {
                std::cerr << "Error loading texture: " << fileName << std::endl;
            }
        }
    }

    static Treasure* createTreasure() {
        if (textures.empty()) {
            std::cerr << "Textures are not initialized!" << std::endl;
            return nullptr;
        }

        Treasure* treasure = new Treasure;
        int randomIndex = rand() % textures.size();
        treasure->setTexture(textures[randomIndex]);
        treasure->setScale(0.5f, 0.5f); // �l�eklendirme
        // Hazineye isim atama
        /*
        sf::Texture& texture = textures[randomIndex];
        std::string fileName = texture.copyToImage().getPixel(0, 0).toString(); // Dosya ad�n� al
        std::size_t found = fileName.find_last_of("/\\"); // Dosya ad�ndaki son klas�r ay�r�c�s�n� bul
        if (found != std::string::npos) {
            fileName = fileName.substr(found + 1); // Dosya ad�ndan klas�r� ay�r
        }
        found = fileName.find_last_of('.'); // Dosya ad�ndaki uzant� ay�r�c�s�n� bul
        if (found != std::string::npos) {
            fileName = fileName.substr(0, found); // Uzant�y� kald�r
        }
        treasure->setName(fileName); // Hazine ismini ata
        */

        return treasure;
    }
};

std::vector<sf::Texture> TreasureFactory::textures; // S�n�f�n static �ye de�i�kenini ba�lat


class Map {
private:
    int size;
    std::vector<std::vector<int>> grid;
    Point start;
    Theme theme;
    Theme leftTheme;
    Theme rightTheme;
    std::vector<Point> collectedTreasures;
    std::map<int, std::string> treasureNames; // Hazine konumlar�n� ve isimlerini tutacak
    static int nextTreasureID; // Sonraki benzersiz hazine kimli�i
   


public:
    std::vector<std::string> treasureTypes = { "Golden","Silver","Emerald","Cooper" };
    std::vector<Bird*> birds;
    int getSize() { return size; }
    Map(int _size, Theme _leftTheme, Theme _rightTheme) : size(_size), leftTheme(_leftTheme), rightTheme(_rightTheme) {
        grid.resize(size, std::vector<int>(size, 0));
        srand(time(0));

        int startX = rand() % size;
        int startY = rand() % size;
        start = Point(startX, startY);
        generateBirds(1); // Ku�lar� olu�tur ve haritaya yerle�tir

    }

    void generateBirds(int numBirds) {
        for (int i = 0; i < numBirds; ++i) {
            int x = rand() % size;
            int y = rand() % size;
            while (grid[x][y] != 0) { // Ku�lar� duvarlar�n ve di�er engellerin �zerine yerle�tirmeyin
                x = rand() % size;
                y = rand() % size;
            }
            // Ku�lar� olu�tur ve ba�lang�� pozisyonunu belirle
            Bird* bird = BirdFactory::createBird(1); // 5, ku�un dikey hareket aral���d�r (�rne�in)
            bird->setInitialPosition(y * SQUARE_SIZE, x * SQUARE_SIZE, size); // Pozisyonu piksel cinsinden belirt
            birds.push_back(bird);
        }
    }




    // Map s�n�f�ndaki generateMap fonksiyonu
    void generateMap(sf::RenderWindow& window) {

        // Haritay� olu�tur
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (j < size / 2) {
                    // Statik engelleri yerle�tir
                    StaticObstacleType staticObstacleType;
                    if (theme == Theme::Winter) {
                        staticObstacleType = static_cast<StaticObstacleType>(rand() % 4);
                    }
                    else if (theme == Theme::Summer) {
                        staticObstacleType = static_cast<StaticObstacleType>((rand() % 3) + 1); // Yaz i�in engellerin se�im aral���n� de�i�tir
                    }

                    if (staticObstacleType == StaticObstacleType::Wall || staticObstacleType == StaticObstacleType::Mountain) {
                        grid[i][j] = 1; // Engelli alan
                        StaticObject* staticObstacle = StaticObstacleFactory::createStaticObstacle(staticObstacleType, theme);
                        if (staticObstacle) {
                            staticObstacle->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                            window.draw(*staticObstacle);
                        }
                        else {
                            std::cerr << "Error creating static obstacle!" << std::endl;
                        }
                    }
                    else {
                        grid[i][j] = 0; // Bo� alan
                    }
                }
                else {
                    grid[i][j] = 0; // Bo� alan
                }
            }

        }

        // Ku�lar� ekrana �iz
        for (auto& bird : birds) {
            window.draw(*bird);
        }


        // Hazineyi yerle�tir
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (rand() % 10 == 0 && grid[i][j] == 0) { // Her 10 h�crede bir hazine yerle�tirme olas�l���
                    grid[i][j] = 2; // Hazineyi i�aretle
                    Treasure* treasure = TreasureFactory::createTreasure();
                    if (treasure) {
                        treasure->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                        window.draw(*treasure);
                    }
                }
            }
        }


        // Ku�lar y�klenemediyse hata mesaj� g�ster
        if (birds.empty()) {
            std::cerr << "Error loading birds!" << std::endl;
        }
    }


    void drawMap(sf::RenderWindow& window) {
        sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        // Ku�lar� �iz
        for (auto& bird : birds) {
            window.draw(*bird);
        }

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                square.setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                square.setOutlineColor(sf::Color::Black); // Karelerin kenar rengini siyah yap
                square.setOutlineThickness(1); // Karelerin kenar kal�nl���n� belirle

                // Temaya g�re renk ayarla
                if (j < size / 2) {
                    if (leftTheme == Theme::Winter) {
                        square.setFillColor(sf::Color::Cyan); // K�� temas� i�in mavi renk
                    }
                    else if (leftTheme == Theme::Summer) {
                        square.setFillColor(sf::Color::Yellow); // Yaz temas� i�in sar� renk
                    }
                }
                else {
                    if (rightTheme == Theme::Winter) {
                        square.setFillColor(sf::Color::Cyan); // K�� temas� i�in mavi renk
                    }
                    else if (rightTheme == Theme::Summer) {
                        square.setFillColor(sf::Color::Yellow); // Yaz temas� i�in sar� renk
                    }
                }

                // Duvarlar� ve hazineleri �iz
                if (grid[i][j] == 1) {
                    // Engelleri �iz
                    StaticObstacleType obstacleType;
                    if (j < size / 2) {
                        if (leftTheme == Theme::Winter) {
                            obstacleType = StaticObstacleType::Rock; // K�� temas� i�in da�lar� �izdir
                        }
                        else if (leftTheme == Theme::Summer) {
                            obstacleType = StaticObstacleType::Mountain2; // Yaz temas� i�in a�a�lar� �izdir
                        }
                    }
                    else {
                        if (rightTheme == Theme::Winter) {
                            obstacleType = StaticObstacleType::Mountain; // K�� temas� i�in da�lar� �izdir
                        }
                        else if (rightTheme == Theme::Summer) {
                            obstacleType = StaticObstacleType::Mountain2; // Yaz temas� i�in a�a�lar� �izdir
                        }
                    }

                    StaticObject* staticObstacle = StaticObstacleFactory::createStaticObstacle(obstacleType, theme);
                    if (staticObstacle) {
                        staticObstacle->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                        window.draw(*staticObstacle);
                    }
                    else {
                        std::cerr << "Error creating static obstacle!" << std::endl;
                    }
                }
                else if (grid[i][j] == 2) {
                    // Hazineleri �iz
                    Treasure* treasure = TreasureFactory::createTreasure();
                    if (treasure) {
                        treasure->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                        window.draw(*treasure);
                    }
                }
                else {
                    // Engelsiz alanlara da engel yerle�tir
                    if (rand() % 4 == 0) {
                        grid[i][j] = 1; // Engelli alan
                        StaticObstacleType obstacleType;
                        if (j < size / 2) {
                            if (leftTheme == Theme::Winter) {
                                obstacleType = StaticObstacleType::Mountain; // K�� temas� i�in da�lar� �izdir
                            }
                            else if (leftTheme == Theme::Summer) {
                                obstacleType = StaticObstacleType::Tree; // Yaz temas� i�in a�a�lar� �izdir
                            }
                        }
                        else {
                            if (rightTheme == Theme::Winter) {
                                obstacleType = StaticObstacleType::Mountain; // K�� temas� i�in da�lar� �izdir
                            }
                            else if (rightTheme == Theme::Summer) {
                                // Yaz temas� i�in farkl� engel t�rleri �izdir
                                int obstacleIndex = rand() % 4;
                                switch (obstacleIndex) {
                                case 0:
                                    obstacleType = StaticObstacleType::Mountain2;
                                    break;
                                case 1:
                                    obstacleType = StaticObstacleType::Rock2;
                                    break;
                                case 2:
                                    obstacleType = StaticObstacleType::Wall2;
                                    break;
                                case 3:
                                    obstacleType = StaticObstacleType::Tree2; // Yaz temas� i�in a�a�lar� �izdir
                                    break;
                                }
                            }
                        }

                        StaticObject* staticObstacle = StaticObstacleFactory::createStaticObstacle(obstacleType, theme);
                        if (staticObstacle) {
                            staticObstacle->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                            window.draw(*staticObstacle);
                        }
                        else {
                            std::cerr << "Error creating static obstacle!" << std::endl;
                        }
                    }
                    else {
                        window.draw(square); // Bo� alanlar� �iz
                    }
                }
            }
        }
    }



    Point getStartPoint() {
        return start;
    }

    bool isObstacle(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size) return true;
        return grid[x][y] == 1;
    }

    bool isTreasure(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size) return false;
        return grid[x][y] == 2 && !isTreasureCollected(Point(x, y)); // Hazinenin toplan�p toplanmad���n� kontrol et
    }
    void collectTreasure(const Point& treasurePosition, const std::string& treasureName) {
        collectedTreasures.push_back(treasurePosition); // Hazineyi topland� olarak i�aretler
        int treasureID = nextTreasureID++;
       // Hazine t�r�n� rastgele se�
    std::string treasureType = treasureTypes[std::rand() %4];
    treasureNames[treasureID] = treasureType; // Hazine t�r�n� ekler
    }

    bool isTreasureCollected(const Point& treasurePosition) const {
        // E�er hazine zaten topland�ysa true, aksi halde false d�nd�r�r
        return std::find(collectedTreasures.begin(), collectedTreasures.end(), treasurePosition) != collectedTreasures.end();
    }
    void printTreasureTypes() {
        for (const auto& entry : treasureNames) {
            int treasureID = entry.first;
            const std::string& treasureType = entry.second;
            std::cout << "Treasure ID: " << treasureID << ", Type: " << treasureType << std::endl;
        }
    }

};


int Map::nextTreasureID = 1; // Statik veriyi ba�latma




class AutonomousCharacter {
private:
    Point position;
    Map map;
public:
    AutonomousCharacter(Point start, Map _map) : position(start), map(_map) {}

    void move(int direction) {
        // Harita boyutunu al
        int mapSize = map.getSize();
        // �nceki konumu saklamak i�in oldPosition tan�mlan�r
        Point oldPosition = position;
        switch (direction) {
        case UP:
            if (position.x > 0) { // Yukar� hareket etmeden �nce s�n�rlar� kontrol et
                position.x--;
                std::cout << "Karakter yukar� hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case DOWN:
            if (position.x < mapSize - 1) { // A�a�� hareket etmeden �nce s�n�rlar� kontrol et
                position.x++;
                std::cout << "Karakter a�a�� hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case LEFT:
            if (position.y > 0) { // Sola hareket etmeden �nce s�n�rlar� kontrol et
                position.y--;
                std::cout << "Karakter sola hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case RIGHT:
            if (position.y < mapSize - 1) { // Sa�a hareket etmeden �nce s�n�rlar� kontrol et
                position.y++;
                std::cout << "Karakter sa�a hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        }
    }

    void moveWithoutObstacles(int direction) {
        // Bu yeni metod, engelleri kontrol ederek hareket etmeyi sa�lar.

        // Harita boyutunu al
        int mapSize = map.getSize();
        // �nceki konumu saklamak i�in oldPosition tan�mlan�r
        Point oldPosition = position;
        switch (direction) {
        case UP:
            if (position.x > 0 && !map.isObstacle(position.x - 1, position.y)) { // Yukar� hareket etmeden �nce s�n�rlar� ve engelleri kontrol et
                position.x--;
                std::cout << "Karakter yukar� hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case DOWN:
            if (position.x < mapSize - 1 && !map.isObstacle(position.x + 1, position.y)) { // A�a�� hareket etmeden �nce s�n�rlar� ve engelleri kontrol et
                position.x++;
                std::cout << "Karakter a�a�� hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case LEFT:
            if (position.y > 0 && !map.isObstacle(position.x, position.y - 1)) { // Sola hareket etmeden �nce s�n�rlar� ve engelleri kontrol et
                position.y--;
                std::cout << "Karakter sola hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case RIGHT:
            if (position.y < mapSize - 1 && !map.isObstacle(position.x, position.y + 1)) { // Sa�a hareket etmeden �nce s�n�rlar� ve engelleri kontrol et
                position.y++;
                std::cout << "Karakter sa�a hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        }


    }

    Point getPosition() {
        return position;
    }
};

Point shortestPath(Map& map, Point start) {
    std::queue<Point> q;
    std::vector<std::vector<bool>> visited(map.getSize(), std::vector<bool>(map.getSize(), false));
    std::vector<std::vector<int>> distance(map.getSize(), std::vector<int>(map.getSize(), 0));

    q.push(start);
    visited[start.x][start.y] = true;
    distance[start.x][start.y] = 0;

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        if (map.isTreasure(current.x, current.y)) {
            return current; // Hazine bulundu�unda hazine noktas�n� d�nd�r
        }

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];
            if (newX >= 0 && newX < map.getSize() && newY >= 0 && newY < map.getSize() &&
                !map.isObstacle(newX, newY) && !visited[newX][newY]) {
                q.push(Point(newX, newY));
                visited[newX][newY] = true;
                distance[newX][newY] = distance[current.x][current.y] + 1;
            }
        }
    }

    return Point(-1, -1); // Hazine bulunamad���nda (-1, -1) d�nd�r
}

std::string getName() {
    std::string name;
    std::string surname;

    std::cout << "Lutfen isminizi girin: ";
    std::cin >> name;
    std::cout << "Lutfen soyisminizi girin: ";
    std::cin >> surname;

    return name + " " + surname;
}



std::vector<Point> findAllTreasures(Map& map) {
    std::vector<Point> allTreasures;

    // Haritadaki her noktay� kontrol edin
    for (int i = 0; i < map.getSize(); ++i) {
        for (int j = 0; j < map.getSize(); ++j) {
            if (map.isTreasure(i, j)) {
                // E�er bu nokta bir hazine ise, en k�sa yolu bulun
                Point start(i, j);
                Point treasurePosition = shortestPath(map, start);

                // Hazine bulundu�unda listeye ekleyin
                allTreasures.push_back(treasurePosition);
            }
        }
    }

    return allTreasures;
}



Point treasurePosition;
int main() {
    std::string fullName = getName();
    std::cout << "Hos geldiniz, " << fullName << "!" << std::endl;

    // Harita boyutunu kullan�c�dan al
    int size;
    std::cout << "Harita boyutunu girin: ";
    std::cin >> size;

    // Pencereyi olu�tur
    sf::RenderWindow window(sf::VideoMode(size * SQUARE_SIZE, size * SQUARE_SIZE), "Harita");
    // Pencere boyutunu al
    sf::Vector2u windowSize = window.getSize();

    // Haritay� olu�tur
    Map map(size, Theme::Winter, Theme::Summer);
    map.generateBirds(1);

    // Ba�lang�� noktas�n� al
    Point start = map.getStartPoint();

    // Otonom karakteri olu�tur ve ba�lang�� noktas�na yerle�tir
    AutonomousCharacter character(start, map);

    // Texture'lar� ba�lat
    TreasureFactory treasureFactory; // TreasureFactory s�n�f�ndan bir �rnek olu�tur
    treasureFactory.initializeTextures(); // �rne�in �zerinden initializeTextures fonksiyonunu �a��r

    // Engelleri ve hazineleri yerle�tir
    map.generateMap(window);

    // Haritay� �iz
    window.clear();
    map.drawMap(window);
    // T�m hazineleri bul
    std::vector<Point> allTreasures = findAllTreasures(map);

    // Yeni Harita Olu�tur Butonu
    sf::RectangleShape newMapButton(sf::Vector2f(150, 50)); // D��me boyutlar�
    newMapButton.setFillColor(sf::Color::Blue); // D��menin rengi
    newMapButton.setPosition((windowSize.x - 310) / 2, windowSize.y - 70); // D��menin konumu

    // D��me metni
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) { // Windows i�in
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    sf::Text buttonText("New Map", font, 20);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect newMapTextBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(newMapTextBounds.width / 2, newMapTextBounds.height / 2);
    buttonText.setPosition(newMapButton.getPosition().x + newMapButton.getSize().x / 2, newMapButton.getPosition().y + newMapButton.getSize().y / 2);


    // Start Butonu
    sf::RectangleShape startButton(sf::Vector2f(150, 50)); // D��me boyutlar�
    startButton.setFillColor(sf::Color::Red); // D��menin varsay�lan rengi
    startButton.setPosition((windowSize.x + 10) / 2, windowSize.y - 70); // D��menin konumu

    // D��me metni
    sf::Text startButtonText("Start", font, 20); // D��me metni
    startButtonText.setFillColor(sf::Color::White);
    sf::FloatRect startTextBounds = startButtonText.getLocalBounds();
    startButtonText.setOrigin(startTextBounds.width / 2, startTextBounds.height / 2);
    startButtonText.setPosition(startButton.getPosition().x + startButton.getSize().x / 2, startButton.getPosition().y + startButton.getSize().y / 2);

    // Mouse t�klama ve �zerine gelme durumunu kontrol et
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool newMapHovered = newMapButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    bool startHovered = startButton.getGlobalBounds().contains(mousePos.x, mousePos.y);

    if (newMapHovered) {
        newMapButton.setFillColor(sf::Color(100, 100, 255)); // D��menin �zerine gelindi�inde rengini de�i�tir
    }
    else {
        newMapButton.setFillColor(sf::Color::Blue); // D��menin varsay�lan rengi
    }

    if (startHovered) {
        startButton.setFillColor(sf::Color(255, 100, 100)); // D��menin �zerine gelindi�inde rengini de�i�tir
    }
    else {
        startButton.setFillColor(sf::Color::Red); // D��menin varsay�lan rengi
    }

    window.draw(newMapButton);
    window.draw(buttonText);
    window.draw(startButton);
    window.draw(startButtonText);
    window.display();

    Point characterPosition = start;
    bool startClicked = false;
    // Pencere d�ng�s�
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Fare hareket etti�inde d��melerin �zerinde olup olmad���n� kontrol et
                if (newMapButton.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
                    // Yeni Harita Olu�tur d��mesinin �zerine gelindi�inde rengi de�i�tir
                    newMapButton.setFillColor(sf::Color::Green);
                }
                else {
                    newMapButton.setFillColor(sf::Color::Blue);
                }

                if (startButton.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
                    // Start d��mesinin �zerine gelindi�inde rengi de�i�tir
                    startButton.setFillColor(sf::Color::White);
                }
                else {
                    startButton.setFillColor(sf::Color::Red);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Yeni Harita Olu�tur Butonu'na t�klan�rsa
                    if (newMapButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        // Yeni harita olu�tur
                        map.generateMap(window);
                        // Ba�lang�� noktas�n� al
                        start = map.getStartPoint();
                        // Hazineyi bul
                        treasurePosition = shortestPath(map, start);
                        // Karakterin ba�lang�� konumu
                        characterPosition = start;
                        // Haritay� temizle ve yeniden �iz
                        window.clear();
                        map.drawMap(window);
                        window.draw(newMapButton);
                        window.draw(buttonText);
                        window.draw(startButton);
                        window.draw(startButtonText);
                        window.display();
                        std::this_thread::sleep_for(std::chrono::seconds(5));



                    }
                    // Start butonuna t�klan�rsa
                    else if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        // Start butonuna t�kland���nda karakterin ad�mlar�n� yazd�rmak i�in bayra�� true yap
                        startClicked = true;
                    }

                }
            }
        }

        // Start butonuna t�kland�ysa karakterin ad�mlar�n� yazd�r
        if (startClicked) {
            // Haritadaki b�t�n hazineleri bul
            std::vector<Point> allTreasures = findAllTreasures(map);

            // Her bir hazineyi ziyaret et
            for (const auto& treasurePosition : allTreasures) {
                // Hazine t�r�n� rastgele se�
                std::string treasureType = map.treasureTypes[std::rand() %4];
                // Hazineyi topla ve t�r�n� belirle
                map.collectTreasure(treasurePosition, treasureType);
                // Hazineye ula�ana kadar karakteri hareket ettir
                while (characterPosition.x != treasurePosition.x || characterPosition.y != treasurePosition.y) {
                    // Hazineye giden y�n� bul
                    int direction;
                    if (characterPosition.x < treasurePosition.x) {
                        direction = DOWN;
                    }
                    else if (characterPosition.x > treasurePosition.x) {
                        direction = UP;
                    }
                    else if (characterPosition.y < treasurePosition.y) {
                        direction = RIGHT;
                    }
                    else {
                        direction = LEFT;
                    }

                    // Karakteri hareket ettir
                    character.move(direction);

                    // Karakterin yeni konumunu g�ncelle
                    characterPosition = character.getPosition();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }

                // Hazine bulundu�unda mesaj yazd�r
                std::cout << "Hazine bulundu! Konum: (" << treasurePosition.x << ", " << treasurePosition.y << ")" << std::endl;

                // Haritay� temizle ve yeniden �iz

                window.display();
                window.draw(newMapButton);
                window.draw(buttonText);
                window.draw(startButton);
                window.draw(startButtonText);
                window.display();
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            // T�m hazineler topland�ktan sonra hazinelerin t�rlerini yazd�r
                map.printTreasureTypes();
            // Start butonuna t�kland�ktan sonra karakterin ad�mlar�n� yazd�rd�ktan sonra bayra�� s�f�rla
            startClicked = false;
        }

        // D��meleri ve metinleri �iz
        window.draw(newMapButton);
        window.draw(buttonText);
        window.draw(startButton);
        window.draw(startButtonText);
        window.display();

    }
    return 0;
}