// Dtown.h

#ifndef _DOWNTOWN_MODEL_
#define _DOWNTOWN_MODEL_

#pragma warning (disable : 4786) // Characters truncated
#pragma warning (disable : 4715) // Not all control paths return a value

#include "rich_containers.h"
#include "hash_dot.h"
#include "grid.h"

#include <algorithm>
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

    enum Direction  { NONE, UP, RIGHT, DOWN, LEFT };
    enum BlockShape { LINE_SEGMENT, CORNER, T_SHAPE, CROSS };
    enum WallShape  { FLAT_WALL, EXTERNAL_CORNER, INTERNAL_CORNER };

    using Walker      = Grid<Block*>::walker;
    using BlockPlacer = Grid<Block*>::walker;
    using PanelPlacer = Grid<Block*>::walker;
    using Buildingpos = Grid<Block*>::walker;
    using Scanner     = Grid<Block*>::walker;    
    using Directions  = std::queue<Direction>;
    using BlockSite   = Block*;
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
         void record_connection(const ID&);
   Directions get_seek_directions(const Block&);
         void connect_buildings(   Buildingpos, Buildingpos, const Direction);
         void place_blocks_between(Buildingpos, Buildingpos, const Direction);
         void build_bridge(PanelPlacer, const int length,    const Direction);
         bool connect_isolated(Scanner, int);    
         void connect_group(Scanner); // the connecting algorithm
         void connect();              // the main algorithm

// MOVING CLOCKWISE
    Direction first_move_direction(      const Block&);
    Direction first_from_external_corner(const Block&);
    Direction external_corner_direction(const Direction);
    Direction internal_corner_direction(const Direction);
    Direction flat_wall_direction(      const Direction);
    Direction orient(    const Block&, Direction&);
         void move(          Scanner&, Direction&);
         void move_clockwise(Scanner&, Direction&);

// SCANNING THE DOWNTOWN
         void enumerate_buildings();
         bool is_free(const BlockSite&);
         bool is_on_building( const Scanner&);
         bool is_on_contour(  const Scanner&);
         bool is_cross_point( const Block&);
         bool is_line_segment(const Block&);
         bool is_bridge_start(const Block&, Direction);
         bool found_building(        const Scanner&, const Scanner&);
         bool on_different_buildings(const Scanner&, const Scanner&);
         void set_contour_id(        const Scanner&, const ID&);
          int count_free_directions(const Block&);
  Buildingpos get_building_location(const ID&);
  Buildingpos seek_up(   const Scanner&, int);
  Buildingpos seek_right(const Scanner&, int);
  Buildingpos seek_down( const Scanner&, int);
  Buildingpos seek_left( const Scanner&, int);
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
    Grid<Block*> grid_;
};

#endif // _DOWNTOWN_MODEL_