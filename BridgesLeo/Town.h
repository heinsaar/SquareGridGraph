// Dtown.h

#ifndef _DOWNTOWN_MODEL_
#define _DOWNTOWN_MODEL_

#pragma warning (disable : 4786) // Characters truncated
#pragma warning (disable : 4715) // Not all control paths return a value

#include "Grid.h"
#include "HashDot.h"

#include <algorithm>
#include <list>
#include <queue>

using std::list;
using std::find;
using std::queue;

	// CLASS Town
	class Town {
public:
	Town(HashDot);

// INTERFACE
	void BuildBridges();
	void ModelView();
	void ViewStatistics();
	void HashDotView();

private:
	struct _Panel;
	struct _Block;

	enum _Direction  { NONE, UP, RIGHT, DOWN, LEFT };
	enum _BlockShape { LINE_SEGMENT, CORNER, T_SHAPE, CROSS };
	enum _WallShape  { FLAT_WALL, EXTERNAL_CORNER, INTERNAL_CORNER };

	using _BlockPlacer = Grid<_Block*>::walker;
	using _PanelPlacer = Grid<_Block*>::walker;
	using _Buildingptr = Grid<_Block*>::walker;
	using _Scanner     = Grid<_Block*>::walker;	
	using _Directions  = queue<_Direction>;
	using _Symbolptr   = string::iterator;	
	using _BlockSite   = _Block*;
	using _PanelSite   = _Panel*;
	using _ID          = int;

	// STRUCT _Block
	struct _Block {
		_Block(_ID bld_id = 0)
        {
            up = right = down = left = 0;
            b_id_ = bld_id;
        }

		_ID b_id_;
		_PanelSite up, right, down, left;
	};

	struct _Panel {};

// BUILDING BRIDGES
			void _Reset(int&);
	 inline bool _Is_among(const list<_ID>&, _ID);
	 inline	bool _Is_connected(_ID);
	 inline	void _Record_connection(const _ID&);
			void _Set_seek_directions(_Directions&, const _Block&);
			void _Connect_buildings(_Buildingptr, _Buildingptr, const _Direction&);
			void _Place_blocks_between(_Buildingptr, _Buildingptr, const _Direction&);
			void _Build_bridge(_PanelPlacer, vector<_Panel>&, const _Direction&);
			bool _Connect_isolated(_Scanner, int);	
			void _Connect_group(_Scanner); // the connecting algorithm
			void _Build_bridges(); // the main algorithm

// MOVING CLOCKWISE
	  _Direction _First_move_direction(const _Block&);
	  _Direction _First_from_external_corner(const _Block&);
	  _Direction _External_corner_direction(const _Direction&);
	  _Direction _Internal_corner_direction(const _Direction&);
	  _Direction _Flat_wall_direction(const _Direction&);
	  _Direction _Orient(const _Block&, _Direction&);
			void _Move(_Scanner&, _Direction&);
			void _Move_clockwise(_Scanner&, _Direction&);

// SCANNING THE DOWNTOWN
	 inline  int _Count_free_directions(const _Block&);
	 inline	bool _Is_free(const _BlockSite&);
	 inline	bool _Is_free(const _PanelSite&);
	 inline	bool _Is_cross_point(const _Block&);
	 inline	bool _Is_on_building(const _Scanner&);
	 inline	bool _Is_line_segment(const _Block&);
	 inline	bool _Is_on_contour(const _Scanner&);
	 inline	bool _Found_building(const _Scanner&,	const _Scanner&);
	 inline	bool _On_different_buildings(const _Scanner&, const _Scanner&);
			bool _Is_bridge_start(const _Block&, _Direction);
			void _Set_contour_id(const _Scanner&, const _ID&);
			void _Enumerate_buildings();
	_Buildingptr _Building_location(const _ID&);
	_Buildingptr _Seek_up(const _Scanner&, int);
	_Buildingptr _Seek_right(const _Scanner&, int);
	_Buildingptr _Seek_down(const _Scanner&, int);
	_Buildingptr _Seek_left(const _Scanner&, int);
	 _BlockShape _Block_shape(const _Block&);
	  _WallShape _Scanner_view(const _Block&, _Direction);	

// CREATING THE DOWNTOWN MODEL
	 inline	bool _Is_hash(const char&);
	 inline	void _Put_a_block(_BlockSite&);
	 inline	void _Put_panel(_PanelSite&, _Panel&);
	 inline	void _Place_panel_at(_PanelSite&, _Panel&);
	 inline	void _Place_block_at(_BlockSite&);
			void _Process_hash_dot(const HashDot&);	
			void _Set_building_id(_Block&, _ID);	
			void _Place_blocks_clockwise(_BlockPlacer);
			void _Place_panels_clockwise(_PanelPlacer);
			void _Create_downtown_model(const HashDot&);	

// OUTPUT INFO
	int     bridges_;
	int     total_bridges_length_;
	int     disconnected_groups_;
	HashDot h_d_;

// DYNAMIC STATISTICS FOR THE MAIN ALGORITHM
	list<_ID> connected_;
	list<_ID> isolated_;

// IMPLEMENTATION
	Grid<_Block*> town_;
};

#endif /* _DOWNTOWN_MODEL_ */