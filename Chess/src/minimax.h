class Minimax {
private:
	MoveGenerator mg;
	
public:
	std::list<Move> getLegalActions(board Board) {
		std::list<Move> actions;
		for (int i = 0; i < 64; i++) {
			Piece piece = Board.spaces[i].piece;
			switch (piece.type)
			{
			case 0:
				continue;
			case 1:
			{
				std::list<Move> moves = mg.generatePawnMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 2:
			{
				std::list<Move> moves = mg.generateKingMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 3:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 4, 8);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 4:
			{
				std::list<Move> moves = mg.generateKnightMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 5:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 4);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 6:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 8);
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
	Move bestMove(board Board) {
		board state = Board;
		std::list<Move> bestMoves;
		std::list<Move> legalMoves;
		legalMoves = getLegalActions(state);
		int bestUtil = -999;
		for (int i = 0; i < legalMoves.size(); i++) {
			Move move = legalMoves.back();
			legalMoves.pop_back();
			state.move(move);
			int util = Search(state, -999, 999, 4);
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
	}
	int Search(board state, int alpha, int beta, int level) {
		std::list<Move> legalMoves;
		legalMoves = getLegalActions(state);

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
		}
	}
};