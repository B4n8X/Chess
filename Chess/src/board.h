#include "pieces.h"

using namespace std;
class boardSpace {
public:
	Piece piece;
	
	int index;
	int row;
	int column;

	bool underWhiteAttack = false;
	bool underBlackAttack = false;

	bool isWhite = false;
	float vertex1[2];
	float vertex2[2];
	float vertex3[2];
	float vertex4[2];
	void calculateVertexes(){
		vertex1[0] = -1.0f + (column * 0.25f);
		vertex1[1] = -1.0f + (row * 0.25f);
		vertex2[0] = -1.0f + ((column + 1) * 0.25f);
		vertex2[1] = -1.0f + (row * 0.25f);
		vertex3[0] = -1.0f + ((column + 1) * 0.25f);
		vertex3[1] = -1.0f + ((row + 1) * 0.25f);
		vertex4[0] = -1.0f + (column * 0.25f);
		vertex4[1] = -1.0f + ((row + 1) * 0.25f);
	}
};

class board {
public:
	Player whitePlayer;
	Player blackPlayer;
	boardSpace spaces[64];
	int numSquaresToEdge[64][8];
	Move kingMoves[64][8];
	Move knightMoves[64][8];
	Move whitePawnMoves[64][4];
	Move blackPawnMoves[64][4];
	int directionLookup[127];
	int directionOffsets[8] = { //Direction offsets for sliding pieces (N, S, W, E, NW, SW, NE, SE)
		8,
		-8,
		-1,
		1,
		7,
		-7,
		9,
		-9,
	};
	bool whitesTurn = true;
	bool movingAlongDirection(int offset, int startSquare, int targetSquare) {
		int moveDir = directionLookup[targetSquare - startSquare + 63];
		if (offset == moveDir || -offset == moveDir) {
			return true;
		}
		else {
			return false;
		}
	}
	bool squareInCheckRay(int startSquare, int side) {
		return false;
	}
	bool isPinned(int startSquare, int side) {
		int kingSquare = NULL;
		int pinnerSquare = NULL;
		for (int i = 0; i < 8; i++) {
			for (int n = 0; n < numSquaresToEdge[startSquare][i]; n++) {
				int targetSquare = startSquare + directionOffsets[i] * (n + 1);
				Piece targetSquarePiece = spaces[targetSquare].piece;
				if (targetSquarePiece.type == 2 && targetSquarePiece.side == side) {
					kingSquare = targetSquare;
				}
				if (targetSquarePiece.type == 3 || targetSquarePiece.type == 5 || targetSquarePiece.type == 6) {
					pinnerSquare = targetSquare;
					if (kingSquare != NULL) {
						if (movingAlongDirection(directionOffsets[i], kingSquare, pinnerSquare)) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	bool inCheck(int side) {
		if (side == 1) {
			if (spaces[whitePlayer.kingSquare].underBlackAttack) {
				return true;
			}
			return false;
		}
		else if (side == 0) {
			if (spaces[blackPlayer.kingSquare].underWhiteAttack) {
				return true;
			}
			return false;
		}
	}
	bool checkForMate(int side, std::vector<Move> moves) {
		if (inCheck(side) && moves.size() == 0) {
			return true;
		}
		return false;
	}
	int sumMaterial(int side) {
		int sum = NULL;
		for (int i = 0; i < 64; i++) {
			if (spaces[i].piece.type != 0) {
				if (spaces[i].piece.side == side) {
					switch (spaces[i].piece.type) {
					case 1:
						sum += 1;
						break;
					case 2:
						sum += 999;
						break;
					case 3:
						sum += 3;
						break;
					case 4:
						sum += 3;
						break;
					case 5:
						sum += 5;
						break;
					case 6:
						sum += 8;
						break;
					}
				}
			}
		}
		return sum;
	}
	int materialBalance() {
		return sumMaterial(1) - sumMaterial(0);
	}
	int evaluatePosition(std::vector<Move> playerMoves, std::vector<Move> opponentMoves) {
		return ((playerMoves.size() - opponentMoves.size()) + 100 * materialBalance());
	}
	void checkThreatenedSquares(std::vector<Move> whiteMoves, std::vector<Move> blackMoves) {
		for (int i = 0; i < whiteMoves.size(); i++) {
			int targetSquareWhite = whiteMoves.at(i).TargetSquare;
			for (int j = 0; j < blackMoves.size(); j++) {
				int targetSquareBlack = blackMoves.at(j).TargetSquare;
				spaces[targetSquareBlack].underBlackAttack = true;
			}
			spaces[targetSquareWhite].underWhiteAttack = true;
		}
	}
	void move(Move move) {
		if (spaces[move.StartSquare].piece.type == 1) {
			spaces[move.StartSquare].piece.flags.canPawnDoubleMove = false;
		}
		spaces[move.TargetSquare].piece = spaces[move.StartSquare].piece;
		if (spaces[move.StartSquare].piece.type == 2) {
			if (spaces[move.StartSquare].piece.side == 0) {
				blackPlayer.kingSquare = move.TargetSquare;
			}
			else if (spaces[move.StartSquare].piece.side == 1) {
				whitePlayer.kingSquare = move.TargetSquare;
			}
		}
		spaces[move.StartSquare].piece.type = 0;
		spaces[move.StartSquare].piece.side = 2;
		
		//cout << "New type: " << spaces[move.TargetSquare].piece.type << endl;
		switch (move.flag) {
		case 3:
			spaces[move.TargetSquare].piece.type = 6;
			break;
		case 4:
			spaces[move.TargetSquare].piece.type = 4;
			break;
		case 5:
			spaces[move.TargetSquare].piece.type = 5;
			break;
		case 6:
			spaces[move.TargetSquare].piece.type = 3;
			break;
		}
		return;
	}
	void undoMove(Move move) {
		spaces[move.StartSquare].piece = spaces[move.TargetSquare].piece;
		spaces[move.TargetSquare].piece.type = 0;
		spaces[move.TargetSquare].piece.side = 2;
		spaces[move.TargetSquare].piece.resetFlags();
		if (move.flag == 3
			|| move.flag == 4
			|| move.flag == 5
			|| move.flag == 6)
		{
			spaces[move.StartSquare].piece.type = 1;
		}
		return;
	}
	
	void PrecomputeMoves() {
		for (int i = 0; i < 64; i++) {
			int y = i / 8;
			int x = i - y * 8;

			//First precompute how many squares to the edge from each square
			int north = 7 - spaces[i].row;
			int south = spaces[i].row;
			int west = spaces[i].column;
			int east = 7 - spaces[i].column;
			
			numSquaresToEdge[i][0] = north;
			numSquaresToEdge[i][1] = south;
			numSquaresToEdge[i][2] = west;
			numSquaresToEdge[i][3] = east;
			numSquaresToEdge[i][4] = min(north, west);
			numSquaresToEdge[i][5] = min(south, east);
			numSquaresToEdge[i][6] = min(north, east);
			numSquaresToEdge[i][7] = min(south, west);

			for (int i = 0; i < 127; i++) {
				int offset = i - 63;
				int absOffset = abs(offset);
				int absDir = 1;
				if (absOffset % 9 == 0) {
					absDir = 9;
				}
				else if (absOffset % 8 == 0) {
					absDir = 8;
				}
				else if (absOffset % 7 == 0) {
					absDir = 7;
				}
				if (offset == 0) {
					directionLookup[i] = absDir * 0;
				}
				else if (offset > 0) {
					directionLookup[i] = absDir * 1;
				}
				else if (offset < 0) {
					directionLookup[i] = absDir * -1;
				}
				
			}


			int knightMoveOffset[] = {15, 17, -17, -15, 10, 6, -6, -10};
			//Precompute legal knight moves
			for (int j = 0; j < 8; j++) {
				int knightMoveSquare = i + knightMoveOffset[j];
				
				if (knightMoveSquare >= 0 && knightMoveSquare < 64) {
					//cout << knightMoveSquare + " \n";
					int knightSquareY = knightMoveSquare / 8;
					int knightSquareX = knightMoveSquare - knightSquareY * 8;
					int maxCoordMoveDst = max(abs(x - knightSquareX), abs(y - knightSquareY));
					if (maxCoordMoveDst == 2) {
						Move move;
						move.StartSquare = i;
						move.TargetSquare = knightMoveSquare;
						knightMoves[i][j] = move;
					}
				}
			}

			//Precompute legal pawn moves
			int whitePawnMoveOffsets[] = { 7,8,9,16 };
			int blackPawnMoveOffsets[] = { -7,-8,-9,-16 };
			for (int j = 0; j < 4; j++) {
				int pawnMoveSquareWhite = i + whitePawnMoveOffsets[j];
				
				int pawnMoveSquareBlack = i + blackPawnMoveOffsets[j];
				if (pawnMoveSquareWhite >= 0 && pawnMoveSquareWhite < 64) {
					int pawnMoveSquareY = pawnMoveSquareWhite / 8;
					int pawnMoveSquareX = pawnMoveSquareWhite - pawnMoveSquareY * 8;
					int maxCoordMoveDst = max(abs(x - pawnMoveSquareX), abs(y - pawnMoveSquareY));
					if(j < 3){
						if (maxCoordMoveDst == 1) {
							Move move;
							move.StartSquare = i;
							//cout << pawnMoveSquareWhite << endl;
							move.TargetSquare = pawnMoveSquareWhite;
							whitePawnMoves[i][j] = move;
						}
					}
					else {
						if (maxCoordMoveDst == 2) {
							Move move;
							move.StartSquare = i;
							move.TargetSquare = pawnMoveSquareWhite;
							whitePawnMoves[i][j] = move;
						}
					}
				}
				if (pawnMoveSquareBlack >= 0 && pawnMoveSquareBlack < 64) {
					int pawnMoveSquareY = pawnMoveSquareBlack / 8;
					int pawnMoveSquareX = pawnMoveSquareBlack - pawnMoveSquareY * 8;
					int maxCoordMoveDst = max(abs(x - pawnMoveSquareX), abs(y - pawnMoveSquareY));
					if (j < 3) {
						if (maxCoordMoveDst == 1) {
							Move move;
							move.StartSquare = i;
							//cout << pawnMoveSquareWhite << endl;
							move.TargetSquare = pawnMoveSquareBlack;
							blackPawnMoves[i][j] = move;
						}
					}
					else {
						if (maxCoordMoveDst == 2) {
							Move move;
							move.StartSquare = i;
							move.TargetSquare = pawnMoveSquareBlack;
							blackPawnMoves[i][j] = move;
						}
					}
				}
			}

			//Precompute legal king moves (Exluding castling)
			for (int j = 0; j < 8; j++) {
				int kingMoveSquare = i + directionOffsets[j];
				if (kingMoveSquare >= 0 && kingMoveSquare < 64) {
					int kingSquareY = kingMoveSquare / 8;
					int kingSquareX = kingMoveSquare - kingSquareY * 8;
					int maxCoordMoveDst = max(abs(x - kingSquareX), abs(y - kingSquareY));
					if (maxCoordMoveDst == 1) {
						Move move;
						move.StartSquare = i;
						move.TargetSquare = kingMoveSquare;
						kingMoves[i][j] = move;
					}
				}
			}
		}
	}
	void createPieces(const int *types, const int *side) {
		for (int i = 0; i < 64; i++) {
			spaces[i].piece.type = types[i];
			spaces[i].piece.side = side[i];
			if (spaces[i].piece.type == 1) {
				spaces[i].piece.flags.canPawnDoubleMove = true;
			}
			if (spaces[i].piece.type == 2) {
				if (spaces[i].piece.side == 0) {
					blackPlayer.kingSquare = i;
				}
				else if (spaces[i].piece.side == 1) {
					whitePlayer.kingSquare = i;
				}
			}
		}
	}
	void createBoard() {
		int index = 0;
		for (int rows = 0; rows < 8; rows++) {
			for (int columns = 0; columns < 8; columns++) {
				spaces[index].column = columns;
				spaces[index].row = rows;
				spaces[index].calculateVertexes();
				spaces[index].index = index;
				if ((columns + rows) % 2 != 0) {
					spaces[index].isWhite = true;
				}
				index++;
			}
		}
	}
	void drawBoard() {
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < 64; i++) {
			glBegin(GL_QUADS);
			if (spaces[i].isWhite) {
				glColor3f(0.81f, 0.725f, 0.592f);
			}
			else {
				glColor3f(0.13f, 0.545f, 0.13f);
			}
			glVertex2f(spaces[i].vertex1[0], spaces[i].vertex1[1]); //TOP LEFT CORNER
			glVertex2f(spaces[i].vertex2[0], spaces[i].vertex2[1]); //TOP RIGHT CORNER
			glVertex2f(spaces[i].vertex3[0], spaces[i].vertex3[1]); //BOTTOM RIGHT CORNER
			glVertex2f(spaces[i].vertex4[0], spaces[i].vertex4[1]); //BOTTOM LEFT CORNER
			glEnd();
			glFlush();
		}
		return;
	}

	void drawPieces() {
		glColor3f(255.0f, 255.0f, 255.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		for (int i = 0; i < 64; i++) {
			int width, height;
			unsigned char* data;
			switch (spaces[i].piece.type) {
			case 1:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackpawn.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whitepawn.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 2:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackking.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whiteking.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 3:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackbishop.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whitebishop.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 4:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackknight.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whiteknight.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 5:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackrook.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whiterook.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 6:
				if (spaces[i].piece.side == 0) {
					data = stbi_load("../Resources/textures/blackqueen.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("../Resources/textures/whitequeen.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			default:
				continue;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(spaces[i].vertex1[0], spaces[i].vertex1[1]); //TOP LEFT CORNER
			glTexCoord2f(1.0f, 1.0f); glVertex2f(spaces[i].vertex2[0], spaces[i].vertex2[1]); //TOP RIGHT CORNER
			glTexCoord2f(1.0f, 0.0f); glVertex2f(spaces[i].vertex3[0], spaces[i].vertex3[1]); //BOTTOM RIGHT CORNER
			glTexCoord2f(0.0f, 0.0f); glVertex2f(spaces[i].vertex4[0], spaces[i].vertex4[1]); //BOTTOM LEFT CORNER
			glEnd();
			glFlush();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
	}
};