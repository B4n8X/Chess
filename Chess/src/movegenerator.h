class MoveGenerator {
public:
	Player whitePlayer;
	Player blackPlayer;
	std::vector<Move> generatePawnMoves(board Board, int startSquare, Piece piece, int side) {
		std::vector<Move> m;
		if (piece.side != side) {
			return m;
		}
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
				//Corner moves (Attacking moves)
				else if((targetSquare-startSquare) % 8 != 0) {
					if (targetSquarePiece.side != piece.side) {
						if (targetSquarePiece.side == 2) {
							continue;
						}
						//cout << "Corner: " << targetSquare << " \n";
						if (piece.side == 1) {
							if (targetSquare > 55) {
								for (int j = 0; j < 4; j++) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 3 + j;
									m.push_back(move);
								}
								continue;
							}

						}
						else if (piece.side == 0) {
							if (targetSquare < 8) {
								for (int j = 0; j < 4; j++) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 3 + j;
									m.push_back(move);
								}
								continue;
							}
						}
						Move move;
						move.StartSquare = startSquare;
						move.TargetSquare = targetSquare;
						m.push_back(move);
					}
					continue;
				}

				
				if(targetSquarePiece.type == 0){
					//Promotion moves
					if (piece.side == 1) {
						if (targetSquare > 55) {
							for (int j = 0; j < 4; j++) {
								Move move;
								move.StartSquare = startSquare;
								move.TargetSquare = targetSquare;
								move.flag = 3 + j;
								m.push_back(move);
							}
							continue;
						}

					}
					else if (piece.side == 0) {
						if (targetSquare < 8) {
							for (int j = 0; j < 4; j++) {
								Move move;
								move.StartSquare = startSquare;
								move.TargetSquare = targetSquare;
								move.flag = 3 + j;
								m.push_back(move);
							}
							continue;
						}
					}
					//Standard moves
					if (targetSquare - startSquare == 16 || targetSquare - startSquare == -16) {
						if (piece.flags.canPawnDoubleMove) {
							if (side == 1 && startSquare < 56) {
								if (Board.spaces[startSquare + 8].piece.type == 0) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 7;
									m.push_back(move);
								}
								else {
									continue;
								}
							}
							else if (side == 0 && startSquare > 7) {
								if (Board.spaces[startSquare - 8].piece.type == 8) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 7;
									m.push_back(move);
								}
								else {
									continue;
								}
							}
							
						}
						else {
							continue;
						}
					}
					else {
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
	std::vector<Move> generateSlidingMoves(board Board, int startSquare, Piece piece, int startDir, int endDir, int side) {
		std::vector<Move> m;
		if (piece.side != side) {
			return m;
		}
		if (Board.inCheck(side) && Board.isPinned(startSquare, side)) {
			return m;
		}
		for (int directionIndex = startDir; directionIndex < endDir; directionIndex++) {
			if (side == 1) {
				if (Board.isPinned(startSquare, side) && !Board.movingAlongDirection(Board.directionOffsets[directionIndex], startSquare, whitePlayer.kingSquare)) {
					continue;
				}
			}
			else if (side == 0) {
				if (Board.isPinned(startSquare, side) && !Board.movingAlongDirection(Board.directionOffsets[directionIndex], startSquare, blackPlayer.kingSquare)) {
					continue;
				}
			}
			
			for (int n = 0; n < Board.numSquaresToEdge[startSquare][directionIndex]; n++) {
				Move move;
				int targetSquare = startSquare + Board.directionOffsets[directionIndex] * (n + 1);
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					break;
				}
				if (targetSquarePiece.type != 0) {
					move.StartSquare = startSquare;
					move.TargetSquare = targetSquare;
					m.push_back(move);
					break;
				}
				
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				//cout << "Square: " << targetSquare;
				m.push_back(move);
			}
		}
		return m;
	}
	std::vector<Move> generateKingMoves(board Board, int startSquare, Piece piece, int side) {
		std::vector<Move> m;
		if (piece.side != side) {
			return m;
		}
		
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.kingMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					continue;
				}
				if (side == 1) {
					if (Board.spaces[targetSquare].underBlackAttack) {
						continue;
					}
				}
				else if (side == 0) {
					if (Board.spaces[targetSquare].underWhiteAttack) {
						continue;
					}
				}
				
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
	std::vector<Move> generateKnightMoves(board Board, int startSquare, Piece piece, int side) {
		std::vector<Move> m;
		if (piece.side != side) {
			return m;
		}
		if (Board.isPinned(startSquare, side)) {
			return m;
		}
		//cout << "From: " << startSquare << " \n";
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.knightMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side || Board.inCheck(side)){ /*OR IF IN CHECK AND KNIGHT IS NOT BLOCKING CHECK OR CAPTURING CHECKING PIECE(ADD PLAYER.H WITH PLAYER STATUS OR UNDER BOARD ADD PLAYERTURNS AND STATUSES, LIKE A STRUCT)*/
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
