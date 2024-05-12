/*Includes*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include <corecrt_math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "config.h"
#include "players.h"
#include "move.h"
#include "board.h"
#include "movegenerator.h"
#include "montecarlo.h"
#include "minimax.h"

using namespace std;

MCTS_Node::MCTS_Node(MCTS_Node *parent, board state, Move parentMove) : nodeParent(parent), State(state), nodeParentMove(parentMove)  {
}

int main(void)
{
    MCTS_Node montecarlo;
    Minimax minimax;

    config cfg;
    board chessBoard;
    chessBoard.createBoard();
    chessBoard.createPieces(cfg.boardStartPieces, cfg.boardStartSide);
    chessBoard.PrecomputeMoves();

    /* Initialize the library */
    if (!glfwInit())
        return -1;
 
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(cfg.screenWidth, cfg.screenHeight, "Chess", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        chessBoard.drawBoard();
        chessBoard.drawPieces();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        if (!chessBoard.whitesTurn) {
            /*Move move;
            move = montecarlo.bestAction(chessBoard, cfg.simCount, 0);
            chessBoard.move(move);*/
            Move move = minimax.bestMove(chessBoard, 0, chessBoard.whitePlayer, chessBoard.blackPlayer);
            chessBoard.move(move);
            chessBoard.whitesTurn = true;
        }
        else {
            Move move = minimax.bestMove(chessBoard, 1, chessBoard.whitePlayer, chessBoard.blackPlayer);
            chessBoard.move(move);
            chessBoard.whitesTurn = false;
        }

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}