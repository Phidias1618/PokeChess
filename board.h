﻿#pragma once

class Board;
class File;
class Square;
class BoardIterator;

#define self (*this)

#include <forward_list>

#include "SDL+.h"

#include "game.h"
#include "piece.h"

class Square {
private:
	Board* board;
public:

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const short x; // never change the postion of x and y in Square, they must remain at 8 and 10 bytes offset respectively
	const short y; // never change the postion of x and y in Square, they must remain at 8 and 10 bytes offset respectively
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool is_accessible : 1;

	Piece* piece;

	Square();

	Square(Board* b);

	Square(Board* b, int x_, int y_);
	Square(Board* b, Piece* p, int x_, int y_);

	// return wether a piece from the <color> side can attack this square
	auto is_controlled_by(piece_color color) -> bool; 

	// move the piece within this square to the graveyard
	auto to_graveyard() -> void;

	void draw();

	// clear the space while freeing memory
	auto clear() -> void; 

	// clear the space without freeing memory
	auto remove() -> void; 

	friend class File;
};

class File {
private:
	Board* board;
	//static Piece *trash;
	Square data[8];
	File();
	File(Board* b, int y_position);
	auto init(Board* b, int y) -> void;

public:

	Square& operator[](int i);

	void clear();

	~File();

	friend class Board;
};


class Board {
private:

	File grid[8];

public:
	PieceClass layout[8];

	std::forward_list<King*> king_list[2];
	short nb_of_kings[2];
	
	short white_death;
	short black_death;
	
	Piece* white_graveyard[16];
	Piece* black_graveyard[16];

	Color light_square_color;
	Color dark_square_color;
	Color selected_piece_color;
	Color possible_move_color;
	Color last_move_color;

	Square* last_move_begin_square;
	Square* last_move_end_square;
	move_data last_move_data;

	piece_color active_player;
	double crit_rate;
	double miss_rate;

	bool in_bonus_move;

	int turn_number;
	std::list<move_data> move_historic;

	void resize_surface();

	auto begin() -> BoardIterator;

	auto end() -> BoardIterator;

	Surface surface;

	Board();

	bool in_stalemate(piece_color color);

	void clear();

	void reset();

	File& operator[](int i);

	~Board();

	friend class BoardIterator;
};

class BoardIterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = signed int;
	using value_type = Square&;
	using pointer = Square*;
	using reference = Square&;

	BoardIterator(Board& board, int x, int y);

	BoardIterator(const BoardIterator& other);

	auto operator*()->reference;
	auto operator->()->pointer;
	
	auto operator++()->BoardIterator&;
	auto operator++(int)->BoardIterator;
	
	auto operator--()->BoardIterator&;
	auto operator--(int)->BoardIterator;
	
	auto operator+=(difference_type step)->BoardIterator&;
	auto operator+(difference_type step)->BoardIterator;
	
	auto operator-=(difference_type step)->BoardIterator&;
	auto operator-(difference_type step)->BoardIterator;


	
	friend auto operator==(const BoardIterator& lhs, const BoardIterator& rhs) -> bool;
	friend auto operator !=(const BoardIterator& lhs, const BoardIterator& rhs) -> bool;
	friend class Board;
private:
	Board& iterated_board;
	int index;

	BoardIterator(Board& board, int index_);
};
