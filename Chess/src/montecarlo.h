class MCTS_Node {
	
private:
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
			std::list<Move> possibleMoves = currentRolloutState.getLegalActions(state);
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
		untried_Actions = state.getLegalActions(state);
		return untried_Actions;
	}
};