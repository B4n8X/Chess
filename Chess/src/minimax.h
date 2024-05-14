class Minimax {
private:
	MoveGenerator mg;
	
public:
	std::vector<Move> getLegalActions(board Board) {
		std::vector<Move> actions;
		for (int i = 0; i < 64; i++) {
			Piece piece = Board.spaces[i].piece;
			switch (piece.type)
			{
			case 0:
				continue;
			case 1:
			{
				std::vector<Move> moves = mg.generatePawnMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 2:
			{
				std::vector<Move> moves = mg.generateKingMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 3:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 4, 8);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 4:
			{
				std::vector<Move> moves = mg.generateKnightMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 5:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 4);
				for (int i = 0; i < moves.size(); i++) {
					actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 6:
			{
				std::vector<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 8);
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
		Board.checkThreatenedSquares(actions);
		return actions;
	}
	Move bestMove(board Board, int depth, Player white, Player black, bool isMaximizingPlayer, int side, int alpha, int beta) {
		mg.blackPlayer = black;
		mg.whitePlayer = white;
		Move best;
		std::vector<Move> legalMoves;
		int util = NULL;
		int bestUtil = -999999;
		legalMoves = getLegalActions(Board);
		cout << legalMoves.size() << endl;
		for (int i = 0; i < legalMoves.size(); i++) {
			Move move = legalMoves.at(i);
			if(Board.spaces[move.StartSquare].piece.side == side){
				cout << move.StartSquare << endl;
			}

			
			Board.move(move);
			util = Search(Board, depth-1, black, white, !isMaximizingPlayer, side, alpha, beta);
			if (isMaximizingPlayer) {
				if (util > bestUtil) {
					bestUtil = util;
					best = move;
				}
				alpha = max(alpha, util);
			}
			else {
				if (util < bestUtil) {
					bestUtil = util;
					best = move;
				}
				beta = max(beta, util);
			}
			Board.undoMove(move);
			
			if (beta <= alpha) {
				break;
			}

		}
		return best;
	}
	int Search(board Board, int depth, Player white, Player black, bool isMaximizingPlayer, int side, int alpha, int beta) {
		std::vector<Move>legalMoves = getLegalActions(Board);
		depth -= 1;
		int bestUtil = -999999;
		if (depth == 0) {
			int eval = Board.evaluatePosition(side);
			return eval;
		}
		for (int i = 0; i < legalMoves.size(); i++) {
			int util;
			Move move = legalMoves.at(i);
			Board.move(move);
			util = Search(Board, depth, black, white, !isMaximizingPlayer, side, alpha, beta);
			if (isMaximizingPlayer) {
				if (util >= bestUtil) {
					bestUtil = util;
				}
				alpha = max(alpha, util);
			}
			else {
				if (util <= bestUtil) {
					bestUtil = util;
				}
				beta = max(beta, util);
			}
			
			
			Board.undoMove(move);
			if (beta <= alpha) {
				break;
			}
			
		}
		return bestUtil;
		
	}
		
};