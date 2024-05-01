class Move {
public:
	int StartSquare;
	int TargetSquare;
};

class MoveGenerator {
private:
	bool SquareIsAttacked(int square) {

	}
public:
	std::list<Move> generateSlidingMoves(board Board, int startSquare, Piece piece, int startDir, int endDir) {
		std::list<Move> moves;
		for (int directionIndex = startDir; directionIndex < endDir; directionIndex++) {
			for (int n = 0; n < Board.numSquaresToEdge[startSquare][directionIndex]; n++) {
				Move move;
				int targetSquare = startSquare + Board.directionOffsets[directionIndex] * (n + 1);
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.isWhite == targetSquarePiece.isWhite) {
					break;
				}
				bool isCapture = targetSquarePiece.type != 0;
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				cout << "Square: " << targetSquare;
				moves.push_back(move);
				
			}
		}
		return moves;
	}
	std::list<Move> generateKingMoves(board Board, int startSquare, Piece piece) {
		std::list<Move> moves;
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.kingMoves[startSquare][n].TargetSquare;
			Piece targetSquarePiece = Board.spaces[targetSquare].piece;
			if (piece.isWhite == targetSquarePiece.isWhite) {
				continue;
			}
			bool isCapture = piece.isWhite != targetSquarePiece.isWhite;
			if (!Board.spaces[targetSquare].underAttack) {
				Move move;
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				moves.push_back(move);
			}

		}
		return moves;
	}
};
