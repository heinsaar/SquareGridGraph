#ifndef TOWN_MODEL_H
#define TOWN_MODEL_H

#pragma warning (disable : 4786) // Characters truncated
#pragma warning (disable : 4715) // Not all control paths return a value

#include "rich_containers.h"
#include "hash_dot.h"
#include "grid.h"

#include <algorithm>
#include <memory>
#include <queue>

    class Town {
public:
     Town(const HashDot&);

// INTERFACE
    void connect_all(bool say = true, bool viz = false);
    void display_model();
    void display_statistics();
    void display_hash_dot();
    void write_to(std::string fileName, bool withStats = true);

private:
    struct Panel;
    struct Block;

    enum class Direction  { NONE, UP, RIGHT, DOWN, LEFT };
    enum class BlockShape { NONE, LINE_SEGMENT, CORNER, T_SHAPE, CROSS };
    enum class WallShape  { NONE, FLAT_WALL, EXTERNAL_CORNER, INTERNAL_CORNER };

    using Walker      = Grid<std::unique_ptr<Block>>::walker;
    using BlockPlacer = Grid<std::unique_ptr<Block>>::walker;
    using PanelPlacer = Grid<std::unique_ptr<Block>>::walker;
    using BuildingPos = Grid<std::unique_ptr<Block>>::walker;
    using Directions  = std::vector<Direction>;
    using BlockSite   = std::unique_ptr<Block>;
    using PanelSite   = bool;
    using ID          = int;

    struct Block {
        Block(ID id = 0) { b_id_ = id; }

        ID b_id_;
        bool up    = false;
        bool right = false;
        bool down  = false;
        bool left  = false;
    };

// BUILDING BRIDGES
         void reset(int&);
         bool is_connected(ID);
         void record_connection(const ID);
   Directions seek_directions(const Block&);
         void connect_buildings(   BuildingPos, BuildingPos, const Direction);
         void place_blocks_between(BuildingPos, BuildingPos, const Direction);
         void build_bridge(PanelPlacer, const int length,    const Direction);
         bool connect_isolated(const Walker&, int);    
         void connect_group(Walker); // the connecting algorithm
         void connect();             // the main algorithm

// MOVING CLOCKWISE
    Direction first_move_direction(      const Block&);
    Direction first_from_external_corner(const Block&);
    Direction external_corner_direction(const Direction);
    Direction internal_corner_direction(const Direction);
    Direction flat_wall_direction(      const Direction);
    Direction orient(   const Block&, Direction&);
         void move(          Walker&, Direction&);
         void move_clockwise(Walker&, Direction&);

// SCANNING THE DOWNTOWN
         void enumerate_buildings();
         bool is_free(const BlockSite&);
         bool is_on_building( const Walker&);
         bool is_on_contour(  const Walker&);
         bool is_cross_point( const Block&);
         bool is_line_segment(const Block&);
         bool is_bridge_start(const Block&, Direction);
         bool found_building(        const Walker&, const Walker&);
         bool on_different_buildings(const Walker&, const Walker&);
         void set_contour_id(        const Walker&, const ID&);
          int count_free_directions(const Block&);
  BuildingPos get_building_location(const ID&);
  BuildingPos seek_up(   const Walker&, int);
  BuildingPos seek_right(const Walker&, int);
  BuildingPos seek_down( const Walker&, int);
  BuildingPos seek_left( const Walker&, int);
   BlockShape block_shape( const Block&);
    WallShape scanner_view(const Block&, Direction);    

// CREATING THE DOWNTOWN MODEL
         bool is_hash(const char);
         void put_panel_at(PanelSite&);
         void put_block(   BlockSite&);
         void put_block_at(BlockSite&);
         void set_building_id(Block&, ID);    
         void place_blocks_clockwise(Walker);
         void place_panels_clockwise(Walker);
         void create_model(const HashDot&);    

// OUTPUT INFO
    int bridges_              = 0;
    int bridges_total_length_ = 0;
    int disconnected_groups_  = 0;
    const HashDot& h_d_;

// DYNAMIC STATISTICS FOR THE MAIN ALGORITHM
    List<ID> connected_;
    List<ID> unvisited_;

// IMPLEMENTATION
    Grid<std::unique_ptr<Block>> grid_;
};

#endif // TOWN_MODEL_H