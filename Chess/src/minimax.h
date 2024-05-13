class Minimax {
private:
	MoveGenerator mg;
	
public:
	std::vector<Move> getLegalActions(board Board, int side) {
		std::vector<Move> actions;
		for (int i = 0; i < 64; i++) {
			Piece piece = Board.spaces[i].piece;
			switch (piece.type)
			{
			case 0:
				continue;
			case 1:
			{
				std::vector<Move> moves = mg.generatePawnMoves(Board, i, piece, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 2:
			{
				std::vector<Move> moves = mg.generateKingMoves(Board, i, piece, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 3:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 4, 8, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 4:
			{
				std::vector<Move> moves = mg.generateKnightMoves(Board, i, piece, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 5:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 4, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 6:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 8, side);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			default:
				break;
			}
		}
		return actions;
	}
	Move bestMove(board Board, int side, Player white, Player black) {
		board state = Board;
		mg.blackPlayer = black;
		mg.whitePlayer = white;
		std::vector<Move> bestMoves;
		std::vector<Move> legalMoves;
		legalMoves = getLegalActions(state, side);
		if (side == 1) {
			Board.checkThreatenedSquares(legalMoves, getLegalActions(state, 0));
		}
		else if (side == 0) {
			Board.checkThreatenedSquares(legalMoves, getLegalActions(state, 1));
		}
		
		
		int bestUtil = -999;
		for (int i = 0; i < legalMoves.size(); i++) {
			
			Move move = legalMoves.back();
			legalMoves.pop_back();
			state.move(move);
			
			int util = -Search(state, -999, 999, 5, side);

			state.undoMove(move);
			if (util > bestUtil) {
				bestUtil = util;
				bestMoves.clear();
				bestMoves.push_back(move);
			}
			else if (util == bestUtil) {
				bestMoves.push_back(move);
			}
		}
		Move best = bestMoves.at(rand() % bestMoves.size());
		/*cout << "From: " << best.StartSquare << endl;
		cout << "To: " << best.TargetSquare << endl;*/
		return best;
	}
	int Search(board state, int alpha, int beta, int level, int side) {
		std::vector<Move> legalMoves;
		std::vector<Move> opponentMoves;
		legalMoves = getLegalActions(state, side);
		if (side == 0) {
			opponentMoves = getLegalActions(state, 1);
		}
		else if (side == 1) {
			opponentMoves = getLegalActions(state, 0);
		}
		if (level == 0) {
			return state.evaluatePosition(legalMoves, opponentMoves);
		}
		int bestUtil = -999;
		for (int i = 0; i < legalMoves.size(); i++) {
			if (bestUtil >= beta) {
				break;
			}
			if (bestUtil > alpha) {
				alpha = bestUtil;
			}
			Move move = legalMoves.back();
			legalMoves.pop_back();
			if (side == 1) {
				if (state.checkForMate(0, opponentMoves)) {
					return 900 + level;
				}
			}
			else if (side == 0) {
				if (state.checkForMate(1, opponentMoves)) {
					return 900 + level;
				}
			}
			
			
			state.move(move);
			int util = -Search(state, -beta, -alpha, level - 1, side);
			state.undoMove(move);
			if (util > bestUtil) {
				bestUtil = util;
			}
		}
		return bestUtil;
	}
};