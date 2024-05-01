#include "pieces.h"



using namespace std;
class boardSpace {
public:
	Piece piece;
	int index;
	int row;
	int column;

	bool underAttack;

	
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
	boardSpace spaces[64];
	int numSquaresToEdge[64][8];
	Move kingMoves[64][8];
	int directionOffsets[8] = {
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
	void createPieces(int *types, int *side) {
		for (int i = 0; i < 64; i++) {
			spaces[i].piece.type = types[i];
			if (side[i] == 1) {
				spaces[i].piece.isWhite = true;
			}
			else {
				spaces[i].piece.isWhite = false;
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
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackpawn.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whitepawn.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 2:
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackking.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whiteking.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 3:
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackbishop.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whitebishop.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 4:
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackknight.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whiteknight.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 5:
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackrook.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whiterook.png", &width, &height, 0, STBI_rgb_alpha);
				}
				break;
			case 6:
				if (spaces[i].piece.isWhite == false) {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/blackqueen.png", &width, &height, 0, STBI_rgb_alpha);
				}
				else {
					data = stbi_load("C:/Users/zagiz/Desktop/Programming/Chess/Resources/textures/whitequeen.png", &width, &height, 0, STBI_rgb_alpha);
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