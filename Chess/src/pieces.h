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
	struct Flags {
		bool canPawnDoubleMove = false;
		
	};
	Flags flags;
	void resetFlags() {
		flags.canPawnDoubleMove = false;
		return;
	}
};
