#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>

//https://www.glfw.org/
//https://glew.sourceforge.net/

//Windows uses directX/direct3D for OpenGL graphics
//turn modern OpenGL functions into window's OpenGL
//access driver dll files and retrive functions

//we will use another library called Glew (openGL extension wrangler)
//provide openGL specifications in a header file
//the c file looks at drivers and gives you the proper code to work with them

//an alternative to Glew is Glad

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

 


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);



        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();



        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}