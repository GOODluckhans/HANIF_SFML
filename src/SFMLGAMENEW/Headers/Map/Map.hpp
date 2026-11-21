#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Sprite/SpriteManager.hpp"
#include "../ECS/ECS.hpp"
struct TileImage {
    int id;
    sf::Texture texture;
};

struct Tileset {
    int firstGid;

    int tileWidth;
    int tileHeight;
    int cols;
    int tileCount;

    bool isImageCollection = false;

    sf::Texture texture;
    std::vector<TileImage> images;
};

struct TileLayer {
    std::string name;

    int width;
    int height; 

    std::vector<unsigned int> data;
};

struct MapObject {
    unsigned int gid;
	ECS::Entity entity;
    sf::Vector2f position;
    sf::Vector2f size;

    float depthY;
};

struct TileMap {
    int width;
    int height;

    int tileWidth;
    int tileHeight;

    std::vector<Tileset> tilesets;
    std::vector<TileLayer> layers;

    std::vector<MapObject> objects;
    std::vector<sf::FloatRect> collider;
};

class Map {
public:
    Map();

    bool loadMap(const std::string& path, TileMap& map);

    sf::VertexArray buildTileLayer(
        const TileMap& map,
        const TileLayer& layer,
        const Tileset& ts
    );
};