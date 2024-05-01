#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <math.h>
#include <corecrt_math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "config.h"
#include "move.h"
#include "board.h"
#include "movegenerator.h"
#include "montecarlo.h"

using namespace std;

int main(void)
{
    MCTS_Node montecarlo;
   
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
        montecarlo.bestAction(chessBoard);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);



        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}