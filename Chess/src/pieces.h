class Piece {
public:
	/*
	Types:
	0 = None
	1 = Pawn
	2 = King
	3 = Bishop
	4 = Knight
	5 = Rook
	6 = Queen
	*/
	int type = 5;
	/*
	Sides:
	0 = black
	1 = White
	2 = Empty
	*/
	int side = 2;
	/*
	Position:
	(column, row)
	column = 0-8
	row = 0-8
	*/
	int position[2];
	bool isPinned;
};
