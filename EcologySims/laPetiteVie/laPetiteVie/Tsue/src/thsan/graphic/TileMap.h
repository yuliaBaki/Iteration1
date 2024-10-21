#ifndef TILEMAP_H
#define TILEMAP_H

#include <string.h>
#include <memory>
#include <SFML/Graphics.hpp>


class  TileMap: public sf::Drawable, public sf::Transformable
{
    public:
        /** constructeur **/
        TileMap(const std::string &imageLocation, sf::Vector2u tileSize, sf::Vector2u gridSize, uint8_t *tiles, uint8_t *colTiles);
        TileMap();

        /** fonctions d'initialisation(les loads) **/
        bool load(const std::string &imageLocation, sf::Vector2u tileSize, sf::Vector2u gridSize, uint8_t *tiles, uint8_t *colTiles);

        /** les fonction qui modifie une partie (les changes)**/
        bool changeTile(sf::Vector2u tilePosition, int tileNumber);
        //bool changeCollisionTile(sf::Vector2u tilePosition, int tileNumber);

        /** les gets **/
        sf::Vector2u getTilesSize() const;
        sf::Vector2u getgridSize() const;
        sf::VertexArray getVertices() const;

        /** deconstructeur **/
        virtual ~TileMap();

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        //variable membre
        sf::VertexArray m_vertices;
        std::shared_ptr<sf::Texture> m_tileset;
        sf::Vector2u m_gridSize;
        sf::Vector2u m_tileSize;

        //tableau membre
        int m_tiles[1600];
};

#endif // TILEMAP_H
