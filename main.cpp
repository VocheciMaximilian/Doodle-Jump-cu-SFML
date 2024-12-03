#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <memory>
using namespace std;
using namespace sf;

class tile {
protected:
    Texture texture;
    Sprite sprite;
    Vector2f position;
    void generateRandomPosition(tile *obj) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> xDistrib(0, 600);
        uniform_real_distribution<> yDistrib(0, 1000);
        float maxJumpHeight = (-20*-20)/(2*10);

        float xOffset=xDistrib(gen);
        float yOffset=yDistrib(gen);
        while(yOffset>maxJumpHeight || xOffset>640) {
            xOffset=xDistrib(gen);
            yOffset=yDistrib(gen);
        }

        position.x = xOffset;
        position.y += yOffset;
    }
public:
    //constructor
    tile(const string& texture_path, const Vector2f& startPoz)
        : position(startPoz) {
        if(!texture.loadFromFile(texture_path)) {
            cout<<"Tile texture didn't load "<<texture_path<<endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }
    //cc
    tile(const tile& other)
        : texture(other.texture), position(other.position) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }
    //op=
    tile& operator=(const tile& other) {
        if(this == &other) return *this;
        texture = other.texture;
        position = other.position;
        sprite.setTexture(texture);
        sprite.setPosition(position);
        return *this;
    }
    //destructor
    virtual ~tile() {}
    //afisare
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
    Sprite& getSprite() {
        return sprite;
    }
};

class solidTile: public tile {
public:
    solidTile(const Vector2f& startPoz)
        : tile("Doodle Jump\\Default_tile.png",startPoz) {}

};

class breakTile: public tile {
public:
    int hp=3;
    breakTile(const Vector2f& startPoz)
        : tile("Doodle Jump\\breakTile.png",startPoz) {}
};

class caracter {
protected:
    Texture texture;
    Sprite sprite;
    Vector2f position; // x, y
    Vector2f velocity; //velocity.x the movement speed on Ox ;  velocity.y the movement speed on Oy
    float gravity=10; // pulls down the caracter
    bool canJump=false;
    bool left=false, right=false;
public:
    //constructor
    caracter(const string& texturePath, const Vector2f& startPoz)
        : position(startPoz), velocity(10, 0) {
        if(!texture.loadFromFile(texturePath)) {
            cout<<"Character texture didn't load "<<texturePath<<endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }
    //cc=
    caracter(const caracter& other)
        : texture(other.texture), position(other.position), velocity(other.velocity) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }
    //op=
    caracter& operator=(const caracter& other) {
        if(this == &other) return *this;
        texture = other.texture;
        position = other.position;
        velocity = other.velocity;
        sprite.setTexture(texture);
        sprite.setPosition(position);
        return *this;
    }
    //destructor
    virtual ~caracter() {}
    //afisare
    Sprite& getSprite() {
        return sprite;
    }
    void draw(RenderWindow& window){
        window.draw(sprite);
    }
    void processEvents(Keyboard::Key key, bool checkPressed) {
        if(checkPressed==true) {
            if(key == Keyboard::A) {
                left=true;
            }
            if(key == Keyboard::D) {
                right=true;
            }
        } else {
            if(key == Keyboard::A) {
                left=false;
            }
            if(key == Keyboard::D) {
                right=false;
            }
        }
    }
    bool checkCollision(const Sprite& tileSprite) {
        if(sprite.getGlobalBounds().intersects(tileSprite.getGlobalBounds()) && velocity.x>0) {
            position.y=tileSprite.getPosition().y-sprite.getGlobalBounds().height;
            velocity.y=0;
            canJump=true;
            return true;
        }
        return false;
    }
    void tileCaracterCollision(tile *obj) {
        checkCollision(obj->getSprite());
    }
    // function that is used to controll the caracter
    void update() {
        if(left) {
            position.x-=velocity.x;
        }
        if(right) {
            position.x+=velocity.x;
        }
        if(position.x<0) { // check if the caracter warps to the other side
            position.x=640;
        } else if(position.x>640) {
            position.x=0;
        }
        if(canJump==true) { // the caracter auto jumps only if he touches a tile
            velocity.y=-20;
            canJump=false;
        }
        velocity.y+=gravity*0.1;
        position.y+=velocity.y;

        sprite.setPosition(position);
    }
};

class enemy : public caracter {
public:
    enemy(const Vector2f& startPoz)
        : caracter("Doodle Jump/blue-lik-left-odskok@2x.png", startPoz) {}
};

class player : public caracter {
public:
    int hp = 3;
    player(const Vector2f& startPoz)
        : caracter("Doodle Jump/blue-lik-left-odskok@2x.png", startPoz){}

    void playerEnemyCollision(enemy *obj) {
        bool collision = checkCollision(obj->getSprite());
        if(collision) {
            hp--;
        }
    }
};

class background {
    protected:
       Texture texture;
       Sprite sprite;
    public:
    //constructor
    background(const string& texture_path) {
        if(!texture.loadFromFile(texture_path)) {
            cout<<"Background texture didn't load "<<texture_path<<endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(0,0);
    }
    //cc
    background(const background& other) {
        texture = other.texture;
        sprite.setTexture(texture);
        sprite.setPosition(0,0);
    }
    //op=
    background& operator=(const background& other) {
        if(this == &other) return *this;
        texture = other.texture;
        sprite.setTexture(texture);
        sprite.setPosition(0,0);
        return *this;
    }
    //destructor
    virtual ~background() {}
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
};

int main() {
    RenderWindow window(VideoMode(640, 1024), "Doodle Jump");
    window.setFramerateLimit(60);

    background background1("Doodle Jump\\ice-bck@2x.png");
    solidTile startTile(Vector2f(320, 800));
    player p(startTile.getSprite().getPosition());

    vector<unique_ptr<tile>> tiles;
    tiles.push_back(make_unique<solidTile>(startTile));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> xDistrib(0, 640-64);
    uniform_real_distribution<> yDistrib(0, 1024-64);

    for (int i = 0; i < 10; ++i) {
        Vector2f randomPos(xDistrib(gen), yDistrib(gen));
        tiles.push_back(make_unique<solidTile>(randomPos));
    }

    enemy e(Vector2f(320, 100));

    View camera;
    camera.setCenter(Vector2f(320, 512));
    camera.setSize(640, 1024);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                p.processEvents(event.key.code, true);
            } else if (event.type == Event::KeyReleased) {
                p.processEvents(event.key.code, false);
            }
        }
        p.update();
        for (const auto& t : tiles) {
            p.tileCaracterCollision(t.get());
        }
        p.playerEnemyCollision(&e);
        if(p.hp<1) {
            window.clear();
            Text text;
            Font font;
            if(!font.loadFromFile("Doodle Jump\\arial.ttf")) {
                cout<<"Font dind t load";
            }
            text.setFont(font);
            text.setString("Game over");
            text.setColor(Color::Red);
            text.setCharacterSize(64);
            text.setStyle(Text::Style::Bold);
            text.setPosition(320, 512);
            window.draw(text);
            window.display();
            window.close();
        }
        if (p.getSprite().getPosition().y < camera.getCenter().y - 200) {
            camera.move(0, p.getSprite().getPosition().y - camera.getCenter().y);
        }
        window.clear();
        window.setView(camera);
        background1.draw(window);
        for (const auto& t : tiles) {
            t->draw(window);
        }
        p.draw(window);
        e.draw(window);
        window.display();
    }
    return 0;
}