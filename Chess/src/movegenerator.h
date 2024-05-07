class MoveGenerator {
private:
	bool isPinned(board Board, int startSquare) {
		return false;
	}
public:
	std::list<Move> generatePawnMoves(board Board, int startSquare, Piece piece) {
		std::list<Move> m;

		//cout << "From: " << startSquare << " \n";

		for (int n = 0; n < 4; n++) {
			int targetSquare;
			if (piece.side == 0) {
				targetSquare = Board.blackPawnMoves[startSquare][n].TargetSquare;
				
			}
			else if (piece.side == 1) {
				targetSquare = Board.whitePawnMoves[startSquare][n].TargetSquare;
			}
			//cout << targetSquare << endl;
			
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					continue;
				}
				else if((targetSquare-startSquare) % 8 != 0) {
					if (targetSquarePiece.side != piece.side) {
						if (targetSquarePiece.side == 2) {
							continue;
						}
						//cout << "Corner: " << targetSquare << " \n";
						Move move;
						move.StartSquare = startSquare;
						move.TargetSquare = targetSquare;
						m.push_back(move);
					}
					continue;
				}
				if(targetSquarePiece.type == 0){
					
					//cout << "Not corner: " << targetSquare << " \n";
					Move move;
					move.StartSquare = startSquare;
					move.TargetSquare = targetSquare;
					m.push_back(move);
				}
				
			}
			
		}
		return m;
	}
	std::list<Move> generateSlidingMoves(board Board, int startSquare, Piece piece, int startDir, int endDir) {
		std::list<Move> m;
		for (int directionIndex = startDir; directionIndex < endDir; directionIndex++) {
			for (int n = 0; n < Board.numSquaresToEdge[startSquare][directionIndex]; n++) {
				Move move;
				int targetSquare = startSquare + Board.directionOffsets[directionIndex] * (n + 1);
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					break;
				}
				bool isCapture = targetSquarePiece.type != 0;
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				//cout << "Square: " << targetSquare;
				m.push_back(move);
			}
		}
		return m;
	}
	std::list<Move> generateKingMoves(board Board, int startSquare, Piece piece) {
		std::list<Move> m;
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.kingMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					continue;
				}
				
				bool isCapture = piece.side != targetSquarePiece.side;
				if (piece.side == 1) {
					if (!Board.spaces[targetSquare].underBlackAttack) {
						Move move;
						move.StartSquare = startSquare;
						move.TargetSquare = targetSquare;
						m.push_back(move);
					}
				}
				else if(piece.side == 0){
					if (!Board.spaces[targetSquare].underWhiteAttack) {
						Move move;
						move.StartSquare = startSquare;
						move.TargetSquare = targetSquare;
						m.push_back(move);
					}
				}
			}
			
		}
		return m;
	}
	std::list<Move> generateKnightMoves(board Board, int startSquare, Piece piece) {
		std::list<Move> m;
		if (isPinned(Board, startSquare)) {
			return m;
		}
		//cout << "From: " << startSquare << " \n";
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.knightMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) /*OR IF IN CHECK AND KNIGHT IS NOT BLOCKING CHECK OR CAPTURING CHECKING PIECE(ADD PLAYER.H WITH PLAYER STATUS OR UNDER BOARD ADD PLAYERTURNS AND STATUSES, LIKE A STRUCT)*/ {
					continue;
				}
				//cout << "to: " << targetSquare << " \n";
				Move move;
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				m.push_back(move);
			}
		}
		return m;
	}
};
