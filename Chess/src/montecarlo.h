#include <list>


class Move {
public:
	int StartSquare;
	int TargetSquare;
};

class MCTS_Node {
private:
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
	void generateKingMoves(board Board, int startSquare, Piece piece) {
		return;
	}
	void generateSlidingMoves(board Board, int startSquare, Piece piece) {
		Move move;
		for (int directionIndex = 0; directionIndex < 8; directionIndex++) {
			for (int n = 0; n < Board.numSquaresToEdge[startSquare][directionIndex]; n++) {
				int targetSquare = startSquare + Board.directionOffsets[directionIndex] * (n + 1);
				Piece targetSquarePiece = Board.spaces[targetSquare].piece;
				if (piece.isWhite == targetSquarePiece.isWhite) {
					break;
				}
				bool isCapture = targetSquarePiece.type != 0;
				move.StartSquare = startSquare;
				move.TargetSquare = targetSquare;
				cout << "Pushed";
				untried_Actions.push_back(move);
			}
		}
	}
	void getLegalActions(board Board) {
		for (int i = 0; i < 64; i++) {
			Piece piece = Board.spaces[i].piece;
			switch (piece.type)
			{
			case 0:
				continue;
			case 1:
				break;
			case 2:
				generateKingMoves(Board, i, piece);
				break;
			case 3:
				generateSlidingMoves(Board, i, piece);
				break;
			case 4:
				break;
			case 5:
				generateSlidingMoves(Board, i, piece);
				break;
			case 6:
				generateSlidingMoves(Board, i, piece);
				break;
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