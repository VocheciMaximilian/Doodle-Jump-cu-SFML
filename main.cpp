#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class caracter {
protected:
    Texture texture;
    Sprite sprite;
    Vector2f position; // x, y
    Vector2f velocity; //velocity.x vitez de miscare pe Ox ;  velocity.y viteza de miscare pe Oy
    float gravity=10;
    bool canJump=false;
    bool left=false, right=false;
public:
    //constructor
    caracter(const string& texture_path, const Vector2f& startPoz)
        : position(startPoz), velocity(10, 0) {
        if(!texture.loadFromFile(texture_path)) {
            cout<<"Character texture didn't load "<<texture_path<<endl;
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
    void checkCollision(const Sprite& tileSprite) {
        if(sprite.getGlobalBounds().intersects(tileSprite.getGlobalBounds()) && velocity.x>0) {
            position.y=tileSprite.getPosition().y-sprite.getGlobalBounds().height;
            velocity.y=0;
            canJump=true;
        }
    }
    // functie pentru controlare caracterului
    void update() {
        if(left) {
            position.x-=velocity.x;
        }
        if(right) {
            position.x+=velocity.x;
        }
        if(position.x<0) { // testez daca trece dintr-o parte in alta
            position.x=640;
        } else if(position.x>640) {
            position.x=0;
        }
        if(canJump==true) { // poate sari doar daca a atins o platforma
            velocity.y=-20;
            canJump=false;
        }
        velocity.y+=gravity*0.1;
        position.y+=velocity.y;

        sprite.setPosition(position);
    }
};

class tile {
    protected:
       Texture texture;
       Sprite sprite;
       Vector2f position;
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
    tile tile1("C:\\Users\\maxvo\\OneDrive\\Desktop\\Doodle Jump\\Default_tile.png", Vector2f(320, 600));
    tile tile2("C:\\Users\\maxvo\\OneDrive\\Desktop\\Doodle Jump\\Default_tile.png", Vector2f(100, 800));

    background background1("C:\\Users\\maxvo\\OneDrive\\Desktop\\Doodle Jump\\bck@2x.png");
    caracter player("C:\\Users\\maxvo\\OneDrive\\Desktop\\Doodle Jump\\blue-lik-right@2x.png", Vector2f(320, 600));
    while(window.isOpen()) {
        Event event;
        while(window.pollEvent(event)) {
            if(event.type == Event::Closed) {
                window.close();
            }
            if(event.type == Event::KeyPressed) {
                player.processEvents(event.key.code, true);
            } else if(event.type == Event::KeyReleased) {
                player.processEvents(event.key.code, false);
            }
        }
        player.update();
        player.checkCollision(tile1.getSprite());
        window.clear();
        background1.draw(window);
        tile1.draw(window);
        tile2.draw(window);
        player.update();
        player.checkCollision(tile2.getSprite());
        player.draw(window);
        window.display();
    }
    return 0;
}
