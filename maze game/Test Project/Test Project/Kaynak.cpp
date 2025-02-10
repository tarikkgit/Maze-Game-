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

const int dx[] = { 0, 0, -1, 1 }; // Yönler: Yukarý, Aþaðý, Sol, Sað
const int dy[] = { -1, 1, 0, 0 };


struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
    // == operatörünün tanýmý
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
    StaticObject(float scale = 0.5f) { // Ölçek faktörü varsayýlan olarak 0.5
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
    Treasure(float scale = 0.5f) { // Ölçek faktörü varsayýlan olarak 0.5
        setScale(scale, scale);
    }
};

// Ekraný sis efekti ile kapatma fonksiyonu
void fogScreen(sf::RenderWindow& window) {
    sf::RectangleShape fog(sf::Vector2f(window.getSize().x, window.getSize().y));
    fog.setFillColor(sf::Color(0, 0, 0, 150)); // Sis efektini ayarla
    window.draw(fog);
    window.display();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sis efekti için bekleme süresi
}
// En kýsa yolu çizdirme fonksiyonu
void drawShortestPath(sf::RenderWindow& window, const std::vector<Point>& shortestPath) {
    sf::VertexArray lines(sf::LinesStrip, shortestPath.size());
    for (size_t i = 0; i < shortestPath.size(); ++i) {
        lines[i].position = sf::Vector2f(shortestPath[i].y * SQUARE_SIZE + SQUARE_SIZE / 2, shortestPath[i].x * SQUARE_SIZE + SQUARE_SIZE / 2);
        lines[i].color = sf::Color::Green; // Yolu yeþil renkle çiz
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
            staticObstacle->setScale(0.5f, 0.5f); // Ölçeklendirme
        }

        return staticObstacle;
    }

};
/*
StaticObject* StaticObstacleFactory::createStaticObstacle(StaticObstacleType type, Theme theme, int size) {
    StaticObject* staticObstacle = new StaticObject;
    std::string filePath;
    sf::Texture texture;

    // Engelin tipine göre dosya yolu belirle
    switch (type) {
    case StaticObstacleType::Tree:
        filePath = "C:\\Resimler\\Tree.jpeg"; // Örnek dosya yolu, gerçek yolunuza göre güncelleyin
        break;
        // Diðer engel türleri için ayný iþlemi yap
    }

    // Resimleri yükle
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
        delete staticObstacle;
        staticObstacle = nullptr;
    }
    else {
        // Belirtilen boyuta göre resmi ölçeklendir
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
        movingObstacle->setScale(0.5f, 0.5f); // Ölçeklendirme
        return movingObstacle;
    }
};

class Bird : public MovingObject {
private:
    int verticalRange; // Yatay olarak hareket edebileceði maksimum mesafe
    int direction; // Hareket yönü (UP veya DOWN)
    Point initialPosition; // Kuþun baþlangýç konumu
public:
    Bird(int range) : verticalRange(range) {
        direction = UP;
    }
    void setInitialPosition(int x, int y, int mapSize) {
        // Kuþun baþlangýç pozisyonunu belirle
        initialPosition.x = x;
        initialPosition.y = y;

        // Harita boyutunu kontrol et ve pozisyonlarý ayarla
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
        // Hareket yönüne göre konumu güncelle
        if (direction == UP) {
            move(0, -1); // Yukarý yönde hareket
            if (getPosition().y <= (initialPosition.y - verticalRange)) {
                direction = DOWN; // Maksimum yüksekliðe ulaþtýðýnda yönü deðiþtir
            }
        }
        else {
            move(0, 1); // Aþaðý yönde hareket
            if (getPosition().y >= initialPosition.y) {
                direction = UP; // Maksimum aþaðý inildiðinde yönü deðiþtir
            }
        }
    }
};


class BirdFactory {
public:
    static Bird* createBird(int range) {
        Bird* bird = new Bird(range);
        // Kuþun resmini yükleme iþlemi burada yapýlabilir
        return bird;
    }
};



class TreasureFactory {
private:
    static std::vector<sf::Texture> textures;

public:
    static void initializeTextures() {
        // Resim dosyalarýný yükle
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
        treasure->setScale(0.5f, 0.5f); // Ölçeklendirme
        // Hazineye isim atama
        /*
        sf::Texture& texture = textures[randomIndex];
        std::string fileName = texture.copyToImage().getPixel(0, 0).toString(); // Dosya adýný al
        std::size_t found = fileName.find_last_of("/\\"); // Dosya adýndaki son klasör ayýrýcýsýný bul
        if (found != std::string::npos) {
            fileName = fileName.substr(found + 1); // Dosya adýndan klasörü ayýr
        }
        found = fileName.find_last_of('.'); // Dosya adýndaki uzantý ayýrýcýsýný bul
        if (found != std::string::npos) {
            fileName = fileName.substr(0, found); // Uzantýyý kaldýr
        }
        treasure->setName(fileName); // Hazine ismini ata
        */

        return treasure;
    }
};

std::vector<sf::Texture> TreasureFactory::textures; // Sýnýfýn static üye deðiþkenini baþlat


class Map {
private:
    int size;
    std::vector<std::vector<int>> grid;
    Point start;
    Theme theme;
    Theme leftTheme;
    Theme rightTheme;
    std::vector<Point> collectedTreasures;
    std::map<int, std::string> treasureNames; // Hazine konumlarýný ve isimlerini tutacak
    static int nextTreasureID; // Sonraki benzersiz hazine kimliði
   


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
        generateBirds(1); // Kuþlarý oluþtur ve haritaya yerleþtir

    }

    void generateBirds(int numBirds) {
        for (int i = 0; i < numBirds; ++i) {
            int x = rand() % size;
            int y = rand() % size;
            while (grid[x][y] != 0) { // Kuþlarý duvarlarýn ve diðer engellerin üzerine yerleþtirmeyin
                x = rand() % size;
                y = rand() % size;
            }
            // Kuþlarý oluþtur ve baþlangýç pozisyonunu belirle
            Bird* bird = BirdFactory::createBird(1); // 5, kuþun dikey hareket aralýðýdýr (örneðin)
            bird->setInitialPosition(y * SQUARE_SIZE, x * SQUARE_SIZE, size); // Pozisyonu piksel cinsinden belirt
            birds.push_back(bird);
        }
    }




    // Map sýnýfýndaki generateMap fonksiyonu
    void generateMap(sf::RenderWindow& window) {

        // Haritayý oluþtur
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (j < size / 2) {
                    // Statik engelleri yerleþtir
                    StaticObstacleType staticObstacleType;
                    if (theme == Theme::Winter) {
                        staticObstacleType = static_cast<StaticObstacleType>(rand() % 4);
                    }
                    else if (theme == Theme::Summer) {
                        staticObstacleType = static_cast<StaticObstacleType>((rand() % 3) + 1); // Yaz için engellerin seçim aralýðýný deðiþtir
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
                        grid[i][j] = 0; // Boþ alan
                    }
                }
                else {
                    grid[i][j] = 0; // Boþ alan
                }
            }

        }

        // Kuþlarý ekrana çiz
        for (auto& bird : birds) {
            window.draw(*bird);
        }


        // Hazineyi yerleþtir
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (rand() % 10 == 0 && grid[i][j] == 0) { // Her 10 hücrede bir hazine yerleþtirme olasýlýðý
                    grid[i][j] = 2; // Hazineyi iþaretle
                    Treasure* treasure = TreasureFactory::createTreasure();
                    if (treasure) {
                        treasure->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                        window.draw(*treasure);
                    }
                }
            }
        }


        // Kuþlar yüklenemediyse hata mesajý göster
        if (birds.empty()) {
            std::cerr << "Error loading birds!" << std::endl;
        }
    }


    void drawMap(sf::RenderWindow& window) {
        sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        // Kuþlarý çiz
        for (auto& bird : birds) {
            window.draw(*bird);
        }

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                square.setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                square.setOutlineColor(sf::Color::Black); // Karelerin kenar rengini siyah yap
                square.setOutlineThickness(1); // Karelerin kenar kalýnlýðýný belirle

                // Temaya göre renk ayarla
                if (j < size / 2) {
                    if (leftTheme == Theme::Winter) {
                        square.setFillColor(sf::Color::Cyan); // Kýþ temasý için mavi renk
                    }
                    else if (leftTheme == Theme::Summer) {
                        square.setFillColor(sf::Color::Yellow); // Yaz temasý için sarý renk
                    }
                }
                else {
                    if (rightTheme == Theme::Winter) {
                        square.setFillColor(sf::Color::Cyan); // Kýþ temasý için mavi renk
                    }
                    else if (rightTheme == Theme::Summer) {
                        square.setFillColor(sf::Color::Yellow); // Yaz temasý için sarý renk
                    }
                }

                // Duvarlarý ve hazineleri çiz
                if (grid[i][j] == 1) {
                    // Engelleri çiz
                    StaticObstacleType obstacleType;
                    if (j < size / 2) {
                        if (leftTheme == Theme::Winter) {
                            obstacleType = StaticObstacleType::Rock; // Kýþ temasý için daðlarý çizdir
                        }
                        else if (leftTheme == Theme::Summer) {
                            obstacleType = StaticObstacleType::Mountain2; // Yaz temasý için aðaçlarý çizdir
                        }
                    }
                    else {
                        if (rightTheme == Theme::Winter) {
                            obstacleType = StaticObstacleType::Mountain; // Kýþ temasý için daðlarý çizdir
                        }
                        else if (rightTheme == Theme::Summer) {
                            obstacleType = StaticObstacleType::Mountain2; // Yaz temasý için aðaçlarý çizdir
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
                    // Hazineleri çiz
                    Treasure* treasure = TreasureFactory::createTreasure();
                    if (treasure) {
                        treasure->setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
                        window.draw(*treasure);
                    }
                }
                else {
                    // Engelsiz alanlara da engel yerleþtir
                    if (rand() % 4 == 0) {
                        grid[i][j] = 1; // Engelli alan
                        StaticObstacleType obstacleType;
                        if (j < size / 2) {
                            if (leftTheme == Theme::Winter) {
                                obstacleType = StaticObstacleType::Mountain; // Kýþ temasý için daðlarý çizdir
                            }
                            else if (leftTheme == Theme::Summer) {
                                obstacleType = StaticObstacleType::Tree; // Yaz temasý için aðaçlarý çizdir
                            }
                        }
                        else {
                            if (rightTheme == Theme::Winter) {
                                obstacleType = StaticObstacleType::Mountain; // Kýþ temasý için daðlarý çizdir
                            }
                            else if (rightTheme == Theme::Summer) {
                                // Yaz temasý için farklý engel türleri çizdir
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
                                    obstacleType = StaticObstacleType::Tree2; // Yaz temasý için aðaçlarý çizdir
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
                        window.draw(square); // Boþ alanlarý çiz
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
        return grid[x][y] == 2 && !isTreasureCollected(Point(x, y)); // Hazinenin toplanýp toplanmadýðýný kontrol et
    }
    void collectTreasure(const Point& treasurePosition, const std::string& treasureName) {
        collectedTreasures.push_back(treasurePosition); // Hazineyi toplandý olarak iþaretler
        int treasureID = nextTreasureID++;
       // Hazine türünü rastgele seç
    std::string treasureType = treasureTypes[std::rand() %4];
    treasureNames[treasureID] = treasureType; // Hazine türünü ekler
    }

    bool isTreasureCollected(const Point& treasurePosition) const {
        // Eðer hazine zaten toplandýysa true, aksi halde false döndürür
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


int Map::nextTreasureID = 1; // Statik veriyi baþlatma




class AutonomousCharacter {
private:
    Point position;
    Map map;
public:
    AutonomousCharacter(Point start, Map _map) : position(start), map(_map) {}

    void move(int direction) {
        // Harita boyutunu al
        int mapSize = map.getSize();
        // Önceki konumu saklamak için oldPosition tanýmlanýr
        Point oldPosition = position;
        switch (direction) {
        case UP:
            if (position.x > 0) { // Yukarý hareket etmeden önce sýnýrlarý kontrol et
                position.x--;
                std::cout << "Karakter yukarý hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case DOWN:
            if (position.x < mapSize - 1) { // Aþaðý hareket etmeden önce sýnýrlarý kontrol et
                position.x++;
                std::cout << "Karakter aþaðý hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case LEFT:
            if (position.y > 0) { // Sola hareket etmeden önce sýnýrlarý kontrol et
                position.y--;
                std::cout << "Karakter sola hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case RIGHT:
            if (position.y < mapSize - 1) { // Saða hareket etmeden önce sýnýrlarý kontrol et
                position.y++;
                std::cout << "Karakter saða hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        }
    }

    void moveWithoutObstacles(int direction) {
        // Bu yeni metod, engelleri kontrol ederek hareket etmeyi saðlar.

        // Harita boyutunu al
        int mapSize = map.getSize();
        // Önceki konumu saklamak için oldPosition tanýmlanýr
        Point oldPosition = position;
        switch (direction) {
        case UP:
            if (position.x > 0 && !map.isObstacle(position.x - 1, position.y)) { // Yukarý hareket etmeden önce sýnýrlarý ve engelleri kontrol et
                position.x--;
                std::cout << "Karakter yukarý hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case DOWN:
            if (position.x < mapSize - 1 && !map.isObstacle(position.x + 1, position.y)) { // Aþaðý hareket etmeden önce sýnýrlarý ve engelleri kontrol et
                position.x++;
                std::cout << "Karakter aþaðý hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case LEFT:
            if (position.y > 0 && !map.isObstacle(position.x, position.y - 1)) { // Sola hareket etmeden önce sýnýrlarý ve engelleri kontrol et
                position.y--;
                std::cout << "Karakter sola hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
            }
            break;
        case RIGHT:
            if (position.y < mapSize - 1 && !map.isObstacle(position.x, position.y + 1)) { // Saða hareket etmeden önce sýnýrlarý ve engelleri kontrol et
                position.y++;
                std::cout << "Karakter saða hareket etti. Konum: (" << position.x << ", " << position.y << ")" << std::endl;
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
            return current; // Hazine bulunduðunda hazine noktasýný döndür
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

    return Point(-1, -1); // Hazine bulunamadýðýnda (-1, -1) döndür
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

    // Haritadaki her noktayý kontrol edin
    for (int i = 0; i < map.getSize(); ++i) {
        for (int j = 0; j < map.getSize(); ++j) {
            if (map.isTreasure(i, j)) {
                // Eðer bu nokta bir hazine ise, en kýsa yolu bulun
                Point start(i, j);
                Point treasurePosition = shortestPath(map, start);

                // Hazine bulunduðunda listeye ekleyin
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

    // Harita boyutunu kullanýcýdan al
    int size;
    std::cout << "Harita boyutunu girin: ";
    std::cin >> size;

    // Pencereyi oluþtur
    sf::RenderWindow window(sf::VideoMode(size * SQUARE_SIZE, size * SQUARE_SIZE), "Harita");
    // Pencere boyutunu al
    sf::Vector2u windowSize = window.getSize();

    // Haritayý oluþtur
    Map map(size, Theme::Winter, Theme::Summer);
    map.generateBirds(1);

    // Baþlangýç noktasýný al
    Point start = map.getStartPoint();

    // Otonom karakteri oluþtur ve baþlangýç noktasýna yerleþtir
    AutonomousCharacter character(start, map);

    // Texture'larý baþlat
    TreasureFactory treasureFactory; // TreasureFactory sýnýfýndan bir örnek oluþtur
    treasureFactory.initializeTextures(); // Örneðin üzerinden initializeTextures fonksiyonunu çaðýr

    // Engelleri ve hazineleri yerleþtir
    map.generateMap(window);

    // Haritayý çiz
    window.clear();
    map.drawMap(window);
    // Tüm hazineleri bul
    std::vector<Point> allTreasures = findAllTreasures(map);

    // Yeni Harita Oluþtur Butonu
    sf::RectangleShape newMapButton(sf::Vector2f(150, 50)); // Düðme boyutlarý
    newMapButton.setFillColor(sf::Color::Blue); // Düðmenin rengi
    newMapButton.setPosition((windowSize.x - 310) / 2, windowSize.y - 70); // Düðmenin konumu

    // Düðme metni
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) { // Windows için
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    sf::Text buttonText("New Map", font, 20);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect newMapTextBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(newMapTextBounds.width / 2, newMapTextBounds.height / 2);
    buttonText.setPosition(newMapButton.getPosition().x + newMapButton.getSize().x / 2, newMapButton.getPosition().y + newMapButton.getSize().y / 2);


    // Start Butonu
    sf::RectangleShape startButton(sf::Vector2f(150, 50)); // Düðme boyutlarý
    startButton.setFillColor(sf::Color::Red); // Düðmenin varsayýlan rengi
    startButton.setPosition((windowSize.x + 10) / 2, windowSize.y - 70); // Düðmenin konumu

    // Düðme metni
    sf::Text startButtonText("Start", font, 20); // Düðme metni
    startButtonText.setFillColor(sf::Color::White);
    sf::FloatRect startTextBounds = startButtonText.getLocalBounds();
    startButtonText.setOrigin(startTextBounds.width / 2, startTextBounds.height / 2);
    startButtonText.setPosition(startButton.getPosition().x + startButton.getSize().x / 2, startButton.getPosition().y + startButton.getSize().y / 2);

    // Mouse týklama ve üzerine gelme durumunu kontrol et
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool newMapHovered = newMapButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    bool startHovered = startButton.getGlobalBounds().contains(mousePos.x, mousePos.y);

    if (newMapHovered) {
        newMapButton.setFillColor(sf::Color(100, 100, 255)); // Düðmenin üzerine gelindiðinde rengini deðiþtir
    }
    else {
        newMapButton.setFillColor(sf::Color::Blue); // Düðmenin varsayýlan rengi
    }

    if (startHovered) {
        startButton.setFillColor(sf::Color(255, 100, 100)); // Düðmenin üzerine gelindiðinde rengini deðiþtir
    }
    else {
        startButton.setFillColor(sf::Color::Red); // Düðmenin varsayýlan rengi
    }

    window.draw(newMapButton);
    window.draw(buttonText);
    window.draw(startButton);
    window.draw(startButtonText);
    window.display();

    Point characterPosition = start;
    bool startClicked = false;
    // Pencere döngüsü
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Fare hareket ettiðinde düðmelerin üzerinde olup olmadýðýný kontrol et
                if (newMapButton.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
                    // Yeni Harita Oluþtur düðmesinin üzerine gelindiðinde rengi deðiþtir
                    newMapButton.setFillColor(sf::Color::Green);
                }
                else {
                    newMapButton.setFillColor(sf::Color::Blue);
                }

                if (startButton.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
                    // Start düðmesinin üzerine gelindiðinde rengi deðiþtir
                    startButton.setFillColor(sf::Color::White);
                }
                else {
                    startButton.setFillColor(sf::Color::Red);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Yeni Harita Oluþtur Butonu'na týklanýrsa
                    if (newMapButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        // Yeni harita oluþtur
                        map.generateMap(window);
                        // Baþlangýç noktasýný al
                        start = map.getStartPoint();
                        // Hazineyi bul
                        treasurePosition = shortestPath(map, start);
                        // Karakterin baþlangýç konumu
                        characterPosition = start;
                        // Haritayý temizle ve yeniden çiz
                        window.clear();
                        map.drawMap(window);
                        window.draw(newMapButton);
                        window.draw(buttonText);
                        window.draw(startButton);
                        window.draw(startButtonText);
                        window.display();
                        std::this_thread::sleep_for(std::chrono::seconds(5));



                    }
                    // Start butonuna týklanýrsa
                    else if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        // Start butonuna týklandýðýnda karakterin adýmlarýný yazdýrmak için bayraðý true yap
                        startClicked = true;
                    }

                }
            }
        }

        // Start butonuna týklandýysa karakterin adýmlarýný yazdýr
        if (startClicked) {
            // Haritadaki bütün hazineleri bul
            std::vector<Point> allTreasures = findAllTreasures(map);

            // Her bir hazineyi ziyaret et
            for (const auto& treasurePosition : allTreasures) {
                // Hazine türünü rastgele seç
                std::string treasureType = map.treasureTypes[std::rand() %4];
                // Hazineyi topla ve türünü belirle
                map.collectTreasure(treasurePosition, treasureType);
                // Hazineye ulaþana kadar karakteri hareket ettir
                while (characterPosition.x != treasurePosition.x || characterPosition.y != treasurePosition.y) {
                    // Hazineye giden yönü bul
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

                    // Karakterin yeni konumunu güncelle
                    characterPosition = character.getPosition();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }

                // Hazine bulunduðunda mesaj yazdýr
                std::cout << "Hazine bulundu! Konum: (" << treasurePosition.x << ", " << treasurePosition.y << ")" << std::endl;

                // Haritayý temizle ve yeniden çiz

                window.display();
                window.draw(newMapButton);
                window.draw(buttonText);
                window.draw(startButton);
                window.draw(startButtonText);
                window.display();
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            // Tüm hazineler toplandýktan sonra hazinelerin türlerini yazdýr
                map.printTreasureTypes();
            // Start butonuna týklandýktan sonra karakterin adýmlarýný yazdýrdýktan sonra bayraðý sýfýrla
            startClicked = false;
        }

        // Düðmeleri ve metinleri çiz
        window.draw(newMapButton);
        window.draw(buttonText);
        window.draw(startButton);
        window.draw(startButtonText);
        window.display();

    }
    return 0;
}