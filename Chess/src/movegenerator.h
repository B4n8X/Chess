class MoveGenerator {
public:
	Player whitePlayer;
	Player blackPlayer;
	std::vector<Move> generatePawnMoves(board Board, int startSquare, Piece piece) {
		std::vector<Move> m;
		//cout << "From: " << startSquare << " \n";
		for (int n = 0; n < 4; n++) {
			int targetSquare;
			if (piece.side == 0) {
				
				targetSquare = Board.blackPawnMoves[startSquare][n].TargetSquare;
				
			}
			else if (piece.side == 1) {
				targetSquare = Board.whitePawnMoves[startSquare][n].TargetSquare;
			}
			//cout << "To: " << targetSquare << endl;
			
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
						if (targetSquare - startSquare == 16) {
							if (!piece.flags.canPawnDoubleMove || Board.spaces[startSquare + 8].piece.type != 0) {
								continue;
							}
						}
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
						if (targetSquare - startSquare == -16) {
							if (!piece.flags.canPawnDoubleMove || Board.spaces[startSquare - 8].piece.type != 0) {
								
								continue;
							}
						}
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
							if (piece.side == 1 && startSquare < 56) {
								if (Board.spaces[startSquare + 8].piece.type == 0) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 7;
									m.push_back(move);
								}
							}
							else if (piece.side == 0 && startSquare > 7) {
								if (Board.spaces[startSquare - 8].piece.type == 0) {
									Move move;
									move.StartSquare = startSquare;
									move.TargetSquare = targetSquare;
									move.flag = 7;
									m.push_back(move);
								}
							}
							
						}
						else {
							
							continue;
						}
					}
					Move move;
					move.StartSquare = startSquare;
					move.TargetSquare = targetSquare;
					m.push_back(move);
				}
			}
		}
		return m;
	}
	std::vector<Move> generateSlidingMoves(board Board, int startSquare, Piece piece, int startDir, int endDir) {
		std::vector<Move> m;
		if (Board.inCheck(piece.side) && Board.isPinned(startSquare, piece.side)) {
			return m;
		}
		for (int directionIndex = startDir; directionIndex < endDir; directionIndex++) {
			if (piece.side == 1) {
				if (Board.isPinned(startSquare, piece.side) && !Board.movingAlongDirection(Board.directionOffsets[directionIndex], startSquare, whitePlayer.kingSquare)) {
					continue;
				}
			}
			else if (piece.side == 0) {
				if (Board.isPinned(startSquare, piece.side) && !Board.movingAlongDirection(Board.directionOffsets[directionIndex], startSquare, blackPlayer.kingSquare)) {
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
				
				if (Board.squareInCheckRay(targetSquare, piece.side) || !Board.inCheck(piece.side)){
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
				else {
				}
			}
		}
		return m;
	}
	std::vector<Move> generateKingMoves(board Board, int startSquare, Piece piece) {
		std::vector<Move> m;
		
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.kingMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side) {
					continue;
				}
				if (piece.side == 1) {
					if (Board.spaces[targetSquare].underBlackAttack) {
						continue;
					}
				}
				else if (piece.side == 0) {
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
	std::vector<Move> generateKnightMoves(board Board, int startSquare, Piece piece) {
		std::vector<Move> m;
		if (Board.isPinned(startSquare, piece.side)) {
			return m;
		}
		//cout << "From: " << startSquare << " \n";
		for (int n = 0; n < 8; n++) {
			int targetSquare = Board.knightMoves[startSquare][n].TargetSquare;
			if (targetSquare >= 0 && targetSquare < 64) {
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.side == targetSquarePiece.side || Board.inCheck(piece.side) && Board.squareInCheckRay(targetSquare, piece.side)){
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
