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



    //Define the vertex buffer outisde the while loop
    //so that it isn't defined every frame

    //we use glGenBuffers to define a vertex buffer
    //the first parameter is how many buffers you would like
    //the second parameter is a pointer to an unsigned int
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //the variable buffer acts as the id to the generated buffer
    //opengl acts as a state machine. Everything you generate in opengl gets
    //assigned a unique identifier. This will be for the objects.

    //If you want to bind/select a buffer, you need to pass in the integer id
    //for the buffer.

    //selecting in Opengl is called binding. We can use glBindBuffer()
    // #define GL_ARRAY_BUFFER 0x8892, a Constant for the hexadecimal value
    // The specific hexadecimal value 0x8892 for GL_ARRAY_BUFFER is just an arbitrarily 
    // chosen unique identifier assigned by the OpenGL standards committee
    //  The value 0x8892 was assigned to GL_ARRAY_BUFFER to uniquely identify 
    // the target for operations related to array buffer objects.

    //the first parameter specifies we are using an array
    //the second parameter is the id we made called buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //While it is obvious to use that there are 2 floats per vertex, openGL doesn't know that
    //But how does OpenGL know that the 6 points make 3 vertices of two points versus 2 vertices of 3 points
    //we need to specify the layout with glVertexAttribPointer()
    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };
    //We will use the function: glBufferData() to set the data in the buffer
    //First parameter is the hexadecimal value to say we are using an Array buffer
    // 
    // second parameter is size. size is in bytes for glBufferData as per documentation
    //a good resource for openGL documentation is: docs.GL for gl4
    // 
    // Third parameter is a pointer to the data, since it's an array, we will
    // just pass that
    // 
    // Fourth paramter is the usage enum
    //static and dynamic are the ones we usually use, but there's also stream
    //These are just hints to tell the GPU on how it will be implemented
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions,GL_STATIC_DRAW);




    //To make the VertexAttribPointer work, you need to use glEnableVertextAttribArray() first
    //this enables a vertex attribute. With index of 0, we are enabling the first attribute
    glEnableVertexAttribArray(0);
    //index 0 since it's the first attribute, count 2 floats, type of data, false for normalize
    //stride is the number of bytes between vertices [NOT ATTRIBUTES] 8bytes for sizeof(float) *2,
    //pointer is for the offset for the attribute, 0. 
    // If you another attribute for -0.5f, -0.5f, we would need to offset by 8bytes to reach it
    // but there is only one attribute which takes a 0byte offset to select
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2, 0 );
    


    //Normally we also add an index buffer, but we will go straight to drawing
    //we also don't have a shader to tell how to draw our triangle

    //To tell openGL what the data is, such as how to interpret the bytes it
    //received. We need to tell how the data is laid outs

    //we can do this with a glVertexAttributePointer, which is closely tied with shaders


    //to trigger a draw pull for our data, there are 2 main ways of doing it
    //glDrawArrays(); for when we have no index buffer
    //glDrawElements(); for when we have an index buffer


    //OpenGL knows to set the data to the buffer we originally set since we did bind to it first
    //we can use glBindBuffer(GL_ARRAY_BUFFER,0); to bind to null
    //like photoshop, when we select a layer and draw, it will draw to that layer
    //it's a state machine
        

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);



        //glBegin(GL_TRIANGLES);
        //glVertex2f( -0.5f, -0.5f );
        //glVertex2f( 0.0f, 0.5f   );
        //glVertex2f( 0.5f, -0.5f  );
        //glEnd();

        //1st: the mode we want to set
        //2nd: starting index in our array, which is i=0
        //3rd: number of indices to render (# of verticies)
        glDrawArrays(GL_TRIANGLES,0,3);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}