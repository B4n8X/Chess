class MCTS_Node {
private:
	MoveGenerator mg;
	int parentAction;
	double score;
	int numberOfVisits = 0;
	MCTS_Node* nodeParent;
	std::list<MCTS_Node> nodeChildren;
	std::list<Move> untried_Actions;
public:
	//~MCTS_Node();
	void move() {
		return;
	}
	int gameResult() {
		return 1;
	}
	bool isGameOver() {
		return false;
	}
	void getLegalActions(board Board) {
		for (int i = 0; i < 64; i++) {
			Piece piece = Board.spaces[i].piece;
			switch (piece.type)
			{
			case 0:
				continue;
			case 1:
			{
				std::list<Move> moves = mg.generatePawnMoves(Board, i, piece);
				break;
			}
			case 2:
			{
				std::list<Move> moves = mg.generateKingMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					untried_Actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 3:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 4, 8);
				for (int i = 0; i < moves.size(); i++) {
					untried_Actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 4:
			{
				std::list<Move> moves = mg.generateKnightMoves(Board, i, piece);
				for (int i = 0; i < moves.size(); i++) {
					untried_Actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 5:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 4);
				for (int i = 0; i < moves.size(); i++) {
					untried_Actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			case 6:
			{
				std::list<Move> moves = mg.generateSlidingMoves(Board, i, piece, 0, 8);
				for (int i = 0; i < moves.size(); i++) {
					untried_Actions.push_back(moves.back());
					moves.pop_back();
				}
				break;
			}
			default:
				break;
			}

		}
		return;
	}
	void bestAction(board Board) {
		getLegalActions(Board);
		return;
	}
	void treePolicy() {
		return;
	}
	void rolloutPolicy() {
		return;
	}
	void bestChild() {
		return;
	}
	int isFullyExpanded(){
		return 1;
	}
	void backpropagate() {
		return;
	}
	int rollout() {
		return 1;
	}
	bool isTerminalNode() {
		return false;
	}
	void expand() {
		return;
	}
	void n() {
		return;
	}
	void q() {
		return;
	}
	void untriedActions() {
		return;
	}
};