#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
//__debugbreak() is MSVC compiler specific
#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


//https://www.glfw.org/
//https://glew.sourceforge.net/

//Windows uses directX/direct3D for OpenGL graphics
//turn modern OpenGL functions into window's OpenGL
//access driver dll files and retrive functions

//we will use another library called Glew (openGL extension wrangler)
//provide openGL specifications in a header file
//the c file looks at drivers and gives you the proper code to work with them

//an alternative to Glew is Glad


static void GLClearError()
{
    //runs through all the errors to clear it
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    //as long as the error is not false
    while (GLenum error =glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " 
            <<function<<" "<<file<<":"<<line << std::endl;
        return false;
    }
    return true;
}





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
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //After the shaders have been linked, we can delete the "intermediates"
    //technically we should be called glDetachShader, to get rid of the source code
    //but it can be useful for debuggin with the downside of taking up a trival amount
    //of memory. A lot of game engines don't bother detaching.
    //that topic will discussed later on
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE=-1, VERTEX=0, FRAGMENT=1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type=ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX; //set mode to vertex
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT; //set mode to fragment
            }
           
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
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
    glfwSwapInterval(1);

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
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glGenBuffers(1, &buffer));
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
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

    //While it is obvious to use that there are 2 floats per vertex, openGL doesn't know that
    //But how does OpenGL know that the 6 points make 3 vertices of two points versus 2 vertices of 3 points
    //we need to specify the layout with glVertexAttribPointer()
    float positions[] = {
        -0.5f, -0.5f,//0
        0.5f, -0.5f,//1
        0.5f, 0.5f,//2
        -0.5f, 0.5f//3
    };

    //using the indices of the position array,
    //we can draw the triangles as such
    unsigned int indices[] = {
        0,1,2,  //drawing the first triangle
        2,3,0   //drawing the second triangle
    };

    //without using index buffers
    float positionsOld[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,

        0.5f, 0.5f,//duplicate
        -0.5f, 0.5f,
        -0.5f, -0.5f,//duplicate
    };


    //use GL_ELEMENT_ARRAY_BUFFER instead of GL_ARRAY_BUFFER for indices
    //index buffers have to be made up of unsigned ints
    unsigned int ibo; //index buffer object
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));




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
    GLCall(glBufferData(GL_ARRAY_BUFFER, 2*6*sizeof(float), positions,GL_STATIC_DRAW));




    //To make the VertexAttribPointer work, you need to use glEnableVertextAttribArray() first
    //this enables a vertex attribute. With index of 0, we are enabling the first attribute
    GLCall(glEnableVertexAttribArray(0));
    //index 0 since it's the first attribute, count 2 floats, type of data, false for normalize
    //stride is the number of bytes between vertices [NOT ATTRIBUTES] 8bytes for sizeof(float) *2,
    //pointer is for the offset for the attribute, 0. 
    // If you another attribute for -0.5f, -0.5f, we would need to offset by 8bytes to reach it
    // but there is only one attribute which takes a 0byte offset to select
    GLCall(glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2, 0 ));





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

    //We now parse the shaders from the resource folder
    //in c++ you don't need to write plus for strings, they will be concatenated
    //we can cast our vec2 postion into vec4
    //std::string vertexShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0 ) in vec4 position;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "gl_Position = position;\n"
    //    "}\n";
    ////colors are just floats from 0-1 with transparency being the last input rgba()
    //std::string fragmentShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0) out vec4 color;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    //    "}\n";



    ShaderProgramSource source = ParseShader("./res/shaders/Basic.shader");
    //std::cout << "Vertex\n";
    //std::cout << source.VertexSource << std::endl;
    //std::cout << "Fragment\n";
    //std::cout << source.FragmentSource << std::endl;


    unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
    GLCall(glUseProgram(shader));

    //since we are using vec4, we need 4 floats, thus we use glUniform4f
    //once a shader gets created, every shader gets an id so that we can reference it
    //the way we can look up the id, typically, is by its name.
    //glUniform4F's first parameter is the id for the uniform in the shader, which we
    //can get wtih glGetUniformLocation passing shader and the name of the uniform as arguments

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location,0.2f,0.3f,0.8f,1.0f));


    float r = 0.0f;
    float increment = 0.05f;
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
        //glDrawArrays(GL_TRIANGLES,0,6);

        //GLClearError();
        //Drawing with index buffers
        //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
        //ASSERT(GLLogCall());
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    //glDeleteShader(shader);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}