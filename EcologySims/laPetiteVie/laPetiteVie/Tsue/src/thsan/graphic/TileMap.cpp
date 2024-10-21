#include "TileMap.h"
#include "../ressourceManager/TextureManager.h"
#include <iostream>

TileMap::TileMap(const std::string &imageLocation, sf::Vector2u tileSize, sf::Vector2u gridSize, uint8_t *tiles, uint8_t *colTiles)
{
    load(imageLocation, tileSize, gridSize, tiles, colTiles);
}

TileMap::TileMap()
{

}


bool TileMap::load(const std::string &imageLocation, sf::Vector2u tileSize, sf::Vector2u gridSize, uint8_t *tiles, uint8_t *colTiles)
{
    m_tileset = RessourceManager::TextureManager::get(imageLocation);

    //copy tiles dans la variable local m_tiles
    for(unsigned int i = 0; i < gridSize.x * gridSize.y; i++)
        m_tiles[i] = tiles[i];

    m_gridSize = gridSize;
    m_tileSize = tileSize;

    //preparation des vertex
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(gridSize.x * gridSize.y * 4);

    //fabrication du tile map
    for(unsigned int i = 0; i < gridSize.x; i++)
        for(unsigned int j = 0; j < gridSize.y; j++)
        {
            int tileNumber = tiles[i + j*gridSize.x];

            int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

            sf::Vertex *quad = &m_vertices[(i + j*gridSize.x) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }


    setOrigin(0,0);

    return true;
}


/** les les fonction qui modifie une partie (les changes)**/

bool TileMap::changeTile(sf::Vector2u tilePosition, int tileNumber){
    if(tilePosition.x > m_tileset->getSize().x || tilePosition.y > m_tileset->getSize().y)
        return false;

    int curr = (tilePosition.x + tilePosition.y * m_gridSize.x);
    m_tiles[curr] = tileNumber;

    int tu = tileNumber % (m_tileset->getSize().x / m_tileSize.x);
    int tv = tileNumber / (m_tileset->getSize().x / m_tileSize.x);

    sf::Vertex *quad = &m_vertices[curr * 4];

    quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
    quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);

    return true;
}

/*
bool TileMap::changeCollisionTile(sf::Vector2u tilePosition, int tileNumber){
    if(tilePosition.x > m_tileset->getSize().x || tilePosition.y > m_tileset->getSize().y)
        return false;

    int curr = (tilePosition.x + tilePosition.y * m_gridSize.x);
    m_collisionTiles[curr] = tileNumber;

    return true;
}
*/

/** les gets **/
sf::Vector2u TileMap::getTilesSize() const{
    return m_tileSize;
}
sf::Vector2u TileMap::getgridSize() const{
    return m_gridSize;
}
sf::VertexArray TileMap::getVertices() const{
    return m_vertices;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
        states.transform *= getTransform();
        states.texture = m_tileset.get();

        target.draw(m_vertices, states);
}

TileMap::~TileMap()
{
    //dtor
}
