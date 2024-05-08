class MCTS_Node {
	
private:
	MoveGenerator mg;
	board State;
	Move parentAction;
	int numberOfVisits = 0;
	MCTS_Node* nodeParent;
	Move nodeParentMove;
	std::vector<MCTS_Node> nodeChildren;
	std::vector<Move> untried_Actions;
	/*
	0 - Wins
	1 - Losses
	*/
	int results[2] = { 0, 0 };
	
public:
	MCTS_Node() {}
	MCTS_Node(MCTS_Node* parent, board state, Move parentMove);
	//MCTS_Node(MCTS_Node *parent, board state);
	//~MCTS_Node();
	bool isGameOver(board state) {

		return false;
	}
	int gameResult(board state) {
		return 1;
	}
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
		return actions;
	}

	Move bestAction(board Board, int simCount) {
		/*Move best;
		for (int i = 0; i < simCount; i++) {
			cout << "ching";
			MCTS_Node v = treePolicy(Board);
			
			int reward = v.rollout(Board);
			v.backpropagate(reward);
		}
		MCTS_Node best = bestChild();
		return best;*/
	}
	MCTS_Node treePolicy(board state) {
		MCTS_Node currentNode;
		while (!isTerminalNode(state)) {
			if (!isFullyExpanded()) {
				return expand(state);
			}
			else {
				cout << "chong";
				currentNode = bestChild();
			}
		}
		return currentNode;
	}
	Move rolloutPolicy(std::vector<Move> moves) {
		return moves.back();
	}
	MCTS_Node bestChild(float c_param = 0.1f) {
		
		if (nodeChildren.empty()) return MCTS_Node();
		double uct, max = -1;
		MCTS_Node argmax;
		for (auto child : nodeChildren) {
			double winrate = child.q() / child.n();
			uct = winrate + c_param * sqrt(2 * log(child.n())) / child.n();
			if (uct > max) {
				max = uct;
				argmax = child;
			}
		}
		return argmax;
	}
	bool isFullyExpanded(){
		return untried_Actions.size() == 0;
	}
	void backpropagate(int result) {
		numberOfVisits += 1;
		//results[result] += 1;
		if (nodeParent) {
			nodeParent->backpropagate(result);
		}
	}
	int rollout(board state) {
		while (!isGameOver(state)) {
			std::vector<Move> possibleMoves = getLegalActions(state);
			Move action = rolloutPolicy(possibleMoves);
			state.move(action);
		}
		return gameResult(state);
	}
	bool isTerminalNode(board state) {
		return isGameOver(state);
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
	std::vector<Move> untriedActions(board state) {
		untried_Actions = getLegalActions(state);
		return untried_Actions;
	}
};