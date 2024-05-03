class MCTS_Node {
	
private:
	MoveGenerator mg;
	board State;
	int parentAction;
	double score;
	int numberOfVisits = 0;
	MCTS_Node* nodeParent;
	Move nodeParentMove;
	std::list<MCTS_Node> nodeChildren;
	std::list<Move> untried_Actions;
	int results[2];
	
public:
	MCTS_Node() {}
	MCTS_Node(MCTS_Node* parent, board state, Move parentMove);
	//MCTS_Node(MCTS_Node *parent, board state);
	//~MCTS_Node();
	
	int gameResult() {
		return 1;
	}
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

	void bestAction(board Board, int simCount) {
		for (int i = 0; i < simCount; i++) {
			//getLegalActions(Board);
		}
		
		return;
	}
	void treePolicy() {
		return;
	}
	Move rolloutPolicy(std::list<Move> moves) {
		return moves.back();
	}
	void bestChild(float c_param = 0.1f) {
		std::vector<MCTS_Node> choicesWeights;
		for (int i = 0; nodeChildren.size(); i++) {
			break;
		}
		return;
	}
	bool isFullyExpanded(){
		return untried_Actions.size() == 0;
	}
	void backpropagate(int result) {
		numberOfVisits += 1;
		results[result] += 1;
		if (nodeParent) {
			nodeParent->backpropagate(result);
		}
	}
	int rollout(board state) {
		board currentRolloutState = state;
		while (!currentRolloutState.isGameOver()) {
			std::list<Move> possibleMoves = getLegalActions(state);
			Move action = rolloutPolicy(possibleMoves);
			state.move(action);
		}
		return state.gameResult();
	}
	bool isTerminalNode(board state) {
		return state.isGameOver();
	}
	MCTS_Node expand(board Board) {
		Move action = untried_Actions.back();
		untried_Actions.pop_back();
		board state = Board;
		MCTS_Node child = MCTS_Node(this, state, action);
		nodeChildren.push_back(child);
		return child;
		
	}
	int n() {
		return numberOfVisits;
	}
	int q() {
		int wins = results[0];
		int losses = results[1];
		return wins - losses;
	}
	std::list<Move> untriedActions(board state) {
		untried_Actions = getLegalActions(state);
		return untried_Actions;
	}
};