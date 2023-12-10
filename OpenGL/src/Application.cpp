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


//Code to create a shader
//the strings are meant to be the actual source code
//you can bring in shaders as a string or pull them from another file
static unsigned int CompileShader(unsigned int type,const std::string& source)
{
    unsigned int id = glCreateShader(type);
    //c_str returns a pointer the string
    //you can also do &source[0] to get the mem address of the first char
    //source needs to exist at this point when you compile this code
    //or it can point to random memory and cause an error
    const char* src = source.c_str();
    //takes a pointer to the pointer and a length to the string
    //by putting nullptr, it assumes that the null terminator ends
    //the string so you don't have to put the length
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);// doesn't return anything, but can be queried

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //some people use heap memory to get the message and delete it
        //but this little hack lets you use stack memory to get the message
        //alloca is a function that C gives you that allocate on the stack dynamically
        char* message= (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        //There are more than two types of shaders, but for this demo, it's enough
        std::cout << "Failed to compile "<<(type==GL_VERTEX_SHADER?"vertex":"fragment") << " shader"<< std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    //TODO: Error handling
    return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //returns an unsigned int
    //you can use GLuint but if you use something besides opengl, your code won't be as compatible
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //Now we attach the shaders to the program and link it
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //After the shaders have been linked, we can delete the "intermediates"
    //technically we should be called glDetachShader, to get rid of the source code
    //but it can be useful for debuggin with the downside of taking up a trival amount
    //of memory. A lot of game engines don't bother detaching.
    //that topic will discussed later on
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}






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



    //In order to use core profile, you need VAO defined
    //Will be covered in the Vertex Arrays video
   
    //we use glGenBuffers to define a vertex buffer
    //the first parameter is how many buffers you would like
    //the second parameter is a pointer to an unsigned int
    unsigned int buffer;
    unsigned int vao;
    glGenVertexArrays(1, &vao);
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
    glBindVertexArray(vao);
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


    //in c++ you don't need to write plus for strings, they will be concatenated
    //we can cast our vec2 postion into vec4
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0 ) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "}\n";
    //colors are just floats from 0-1 with transparency being the last input rgba()
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader,fragmentShader);
    glUseProgram(shader);


       
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
    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}