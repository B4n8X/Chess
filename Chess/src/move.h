class Move {
public:
	int StartSquare;
	int TargetSquare;
	/*
	* 0 = None
	* 1 = En Passant
	* 2 = Castling
	* 3 = PromotionToQueen
	* 4 = PromotionToKnight
	* 5 = PromotionToRook
	* 6 = PromotionToBishop
	* 7 = PawnTwoForward
	*/
	int flag = 0;
	bool rightCastling;
	
};