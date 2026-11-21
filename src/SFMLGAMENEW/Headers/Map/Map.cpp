#include "Map.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <tinyxml2.h>
#include <nlohmann/json.hpp>

#include "../Sprite/SpriteManager.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

Map::Map() {

}

bool Map::loadMap(const std::string& path, TileMap& map) {

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << path << '\n';
        return false;
    }

    json j = json::parse(file);

    map.width = j["width"];
    map.height = j["height"];

    map.tileWidth = j["tilewidth"];
    map.tileHeight = j["tileheight"];

    fs::path mapPath = fs::path(path).parent_path();

    // ==================================
    // LOAD TILESETS
    // ==================================

    for (const auto& ts : j["tilesets"]) {

        Tileset t;

        t.firstGid = ts["firstgid"];

        fs::path tsxPath = mapPath / ts["source"].get<std::string>();

        tinyxml2::XMLDocument doc;

        if (doc.LoadFile(tsxPath.string().c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Failed to load TSX: " << tsxPath << '\n';
            return false;
        }

        auto* tileset = doc.FirstChildElement("tileset");

        if (!tileset) {
            std::cerr << "No <tileset> element found\n";
            return false;
        }

        t.tileWidth = tileset->IntAttribute("tilewidth");
        t.tileHeight = tileset->IntAttribute("tileheight");
        t.cols = tileset->IntAttribute("columns");
        t.tileCount = tileset->IntAttribute("tilecount");

        auto* image = tileset->FirstChildElement("image");

        // ==================================
        // NORMAL ATLAS
        // ==================================

        if (image) {

            const char* source = image->Attribute("source");

            if (!source) return false;

            fs::path imagePath = tsxPath.parent_path() / source;

            if (!t.texture.loadFromFile(imagePath.string())) {
                std::cerr << "Failed to load image: " << imagePath << '\n';
                return false;
            }

            t.isImageCollection = false;

            std::cout << "Loaded Atlas: " << imagePath << '\n';
        }

        // ==================================
        // IMAGE COLLECTION
        // ==================================

        else {

            t.isImageCollection = true;

            std::cout << "Image Collection: "
                << ts["source"] << '\n';

            for (auto* tile = tileset->FirstChildElement("tile");
                tile;
                tile = tile->NextSiblingElement("tile")) {

                TileImage tileImage;

                tileImage.id = tile->IntAttribute("id");

                auto* tileImg = tile->FirstChildElement("image");

                if (!tileImg) continue;

                const char* source = tileImg->Attribute("source");

                if (!source) continue;

                fs::path imagePath = tsxPath.parent_path() / source;

                if (!tileImage.texture.loadFromFile(imagePath.string())) {
                    std::cerr << "Failed to load image: " << imagePath << '\n';
                    return false;
                }

                std::cout << "Loaded Tile ID: "
                    << tileImage.id << '\n';

                t.images.push_back(std::move(tileImage));
            }
        }

        // Simpan KEDUA jenis tileset
        map.tilesets.push_back(std::move(t));
    }

    // ==================================
    // LOAD LAYERS
    // ==================================

    for (auto& layer : j["layers"]) {

        std::string type = layer["type"];
        std::string name = layer["name"];

        // ==================================
        // TILE LAYER
        // ==================================

        if (type == "tilelayer") {

            TileLayer tl;

            tl.name = name;
            tl.width = layer["width"];
            tl.height = layer["height"];

            tl.data = layer["data"].get<std::vector<unsigned int>>();

            map.layers.push_back(std::move(tl));
        }

        // ==================================
        // COLLISION
        // ==================================

        else if (type == "objectgroup" && name == "Collision") {

            for (auto& o : layer["objects"]) {

                float x = o["x"];
                float y = o["y"];

                float width = o["width"];
                float height = o["height"];

                map.collider.push_back(
                    sf::FloatRect(
                        { x, y },
                        { width, height }
                    )
                );
            }
        }

        // ==================================
        // OBJECTS
        // ==================================

        else if (type == "objectgroup" && name == "Objects") {

            for (auto& o : layer["objects"]) {

                MapObject obj;

                obj.gid = o["gid"].get<unsigned int>();

                obj.position = {
                    o["x"].get<float>(),
                    o["y"].get<float>()
                };

                obj.size = {
                    o["width"].get<float>(),
                    o["height"].get<float>()
                };

                // Default Tiled tile-object alignment:
                // bottom-left anchor.
                obj.depthY = obj.position.y;


                // ==================================
                // FIND CORRECT TILESET
                // ==================================

                unsigned int gid = obj.gid & 0x0FFFFFFF;

                Tileset* selectedTileset = nullptr;

                for (auto& ts : map.tilesets) {

                    if (gid >= ts.firstGid) {

                        if (!selectedTileset ||
                            ts.firstGid > selectedTileset->firstGid) {

                            selectedTileset = &ts;
                        }
                    }
                }

                if (!selectedTileset) {
                    std::cerr << "Tileset not found for GID: "
                        << gid << '\n';
                 
                    continue;
                }

                int localID = gid - selectedTileset->firstGid;

                // ==================================
                // GET IMAGE COLLECTION TEXTURE
                // ==================================

                if (selectedTileset->isImageCollection) {

                    for (auto& img : selectedTileset->images) {

					
                        if (img.id == localID) {

                            sf::Vector2f scale = { 
                                obj.size.x / img.texture.getSize().x,
                                obj.size.y / img.texture.getSize().y
                            };

                            sf::Vector2f spritePosition(
                                obj.position.x,
                                obj.position.y - obj.size.y 
                            );

                            auto entity = SpriteManager::CreateSpriteECS(
                                img.texture,
                                spritePosition,
                                scale,
                                false
                            );

                            auto& world = ECS::World::get_instance();

                            auto depth = world.get_component<ECS::Depth>(entity);

                            if (depth) {
                                depth->get().offsetY = -50.f;                            }

                            map.objects.push_back(obj);
                            break;
                        }
                    }
                
                }
            }
        }
    }

    return true;
}

sf::VertexArray Map::buildTileLayer(
    const TileMap& map,
    const TileLayer& layer,
    const Tileset& ts
) {

    sf::VertexArray va(sf::PrimitiveType::Triangles);

    // Image Collection tidak digunakan untuk tile layer atlas.
    if (ts.isImageCollection) return va;

    if (ts.cols <= 0) return va;

    for (int y = 0; y < layer.height; ++y) {

        for (int x = 0; x < layer.width; ++x) {

            unsigned int gid = layer.data[x + y * layer.width];

            if (gid == 0) continue;

            gid &= 0x0FFFFFFF;

            int id = gid - ts.firstGid;

            // Pastikan GID milik tileset ini.
            if (id < 0 || id >= ts.tileCount) continue;

            float tu = (id % ts.cols) * ts.tileWidth;
            float tv = (id / ts.cols) * ts.tileHeight;

            float w = ts.tileWidth;
            float h = ts.tileHeight;

            float px = x * map.tileWidth;
            float py = y * map.tileHeight;

            sf::Vertex v[6] = {

                { { px, py }, sf::Color::White,{ tu, tv } },

                { { px + w, py }, sf::Color::White,{ tu + w, tv } },

                { { px, py + h }, sf::Color::White,{ tu, tv + h } },

                { { px, py + h }, sf::Color::White,{ tu, tv + h } },

                { { px + w, py }, sf::Color::White,{ tu + w, tv } },

                { { px + w, py + h }, sf::Color::White,{ tu + w, tv + h } }
            };

            for (auto& vert : v) {
                va.append(vert);
            }
        }
    }

    return va;
}