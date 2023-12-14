# OpenGL Notes

https://www.glfw.org/
https://glew.sourceforge.net/

- Windows uses directX/direct3D for OpenGL graphics
turn modern OpenGL functions into window's OpenGL
access driver dll files and retrive functions

- We will use another library called Glew (openGL extension wrangler)
provide openGL specifications in a header file
the c file looks at drivers and gives you the proper code to work with them

- An alternative to Glew is Glad

# Vertex Buffer
- Vertex buffer can be thought of as a buffer of memory in openGL, or a blob of memory
- to store bytes. The difference is that it's in our GPU, in our VRam (video ram)
- Helps you to define a bunch of data to represent your triangles
- Put it in the GPU Vram, then issues something called a draw call
- a draw command, that reads the buffer and draws it, we also have to tell
- our gpu how to read the data and how to display it on the screen

# Shader
- A program that runs on the GPU


-OpenGL operates like a state machine. You set a series of states (buffers) and draw a your triangle


## select a buffer:
unsigned int buffer;
glGenBuffers(1, &buffer);

## bind the buffer:
glBindBuffer(GL_ARRAY_BUFFER, buffer);

## specify data for buffer
float positions[6] = {
    -0.5f, -0.5f,
    0.0f, 0.5f,
    0.5f, -0.5f
};
glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions,GL_STATIC_DRAW);

## draw call
- glDrawArrays(); for when we have no index buffer
- glDrawElements(); for when we have an index buffer
- 

## vertex attribute pointer
- describes the layout of the data in the buffer
- if you write a float in c++, we know it's a float since it's declared that way
- if you then cast the float as an integer, it will be interpreted as an integer
- That's also how it works in OpenGL with 
- the first x bytes for y and then next n bytes are for m
- The function in openGL is glVertexAttribPointer() does this operation

## What is a vertex
- a vertex is a point on your geometry and isn't always just spatial coordinates
- They usually include position data, but can have more information appended
- (x1,y1) (x1,y1,z1) (x1,y1,z1,u1,v1) are all examples of vertices
- each piece of information in a vertex is referred to as attributes of the vertex






## glVertexAttribPointer()

- Index, the first parameter, specifies the index of the generic vertex attribute to be modified. This is an index to
which attribute you are referring to. Suppose your vertex has position data, texture data, and normal data, you need to
say I want the position to be at index 0, texture at index 1, and normal at index 2

- size, the second parameter, specifies the number of components per generic vertex attribute, must be 1,2,3,4
additionally, the constant GL_BGRA is accepted. the initial value is 4. Has nothing to do with bytes, it's basically
the count. In our Vector2 vertices, we have 2 floats making the count equal to 2 for the size.

- type, the third parameter, specifies the data tpe of each component in the array. the constants, GL_BYTE, GL_UNSIGNED_BYTE,
GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted. There are also a few more such as GL_HALF_FLOAT.
In our Vector2 vertices we are using GL_FLOAT

- normalized, the fourth parameter, a boolean specifies whether the fixed-point data values should be normalized(GL_TRUE) or converted
directedly as a fixed-point value (GL_FALSE) when they are accessed. Our data is already normalized -1<=n<=1, but let's say
you have a rgb color of 0-255, setting this to true will normalize the value for you.

- stride, the fifth parameter, specifies the byte offset between the consecutive genric vertex attributes. if stride is 0, the
generic vertex attribute are understood to be tightly packed in the array. The initial value is 0. The amount of bytes between
each vertex. Let's suppose you have 3 floats for position, 2 floats for texture, and 3 floats for normal.
Then thats 12 bytes for the position, 8 bytes for texture coordinates and 12 bytes for vertex normal.

- pointer, the sixth parameter, specifies the offset of the first comonent of the firt generic vertex attribute in the array in
the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. Initial value is 0. The pointer is the
pointer to the actual attribute and is inside the space of the vertex.

## glEnableVertextAttribArray
To make the VertexAttribPointer work, you need to use glEnableVertextAttribArray() first
this enables a vertex attribute, glEnableVertexAttribArray(0);
The only parameter is an index which specifies the index of the generic vertex attribute to be enabled or disabled
Since we only only after the position attribute, it would be i=0. Can be done before or after the 
pointer code is written due to it being a state machine and it won't affect the code.


# How do Shaders Work in OpenGL
A program that runs on your gpu. We want to utilize the power of the GPU to output graphics. There are some
things you want to do on the CPU and some things you want to do on the GPU.

There are vertex shaders and fragment shaders are the ones you will be using 90% of the time. There are other kinds such
as tessellation shaders, geometry shaders, compute shaders. When you get into more advanced stuff, the other shaders come
in handy to know.

When you issue a draw call, the Vertex shader will get called and then the fragment shader. There are some smaller
steps in between, but this is essentially what happens. The code that is our vertex shader gets called for every vertex
we have. So if there's 3 vertices, the vertex shader gets called 3 times. The vertex shader will tell the GPU where you 
want your vertex to be on your window. Doesn't have much to do with graphics, but it is used to passed data to the next
phase, which is our fragment shader.

The fragments shaders deals with pixels and will run once for each pixel that gets rasterized, or drawn on the screen.
The fragment shader determine the right color for each pixel and get called thousands of times. Doing calculations in
the fragment shader isn't ideal and would be more optimal to do the calculation in the vertex shader instead. Some things
need to be calculated per pixel, such as lighting, due to the texture and material.

Shaders work on the state machine. If you want to use a shader, you enable that shader.
Just like you send data from the CPU to the GPU through a vertex buffer
You can send data to your shader in the form of a uniform that comes from the CPU

Some shaders can get to thousands lines of code, and some game engines create shaders on the fly depending on the
settings the user selects.


## index buffers
Assuming we are trying to draw a square. triangles are the lowest number of vertices needed to make a plane and are
used by GPUs make larger shapes. In order to make a square, we can just use two triangles.

```
   float positions[12] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f,

    0.5f, 0.5f,
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    };
```
We see that some of the same vertices can be seen in both triangles when drawing a square and we are storing the same bytes
twice for two of the vertices. Index buffers let us reuse existing vertices to deal with the overlap.

Some vertices can be very big, such as including info for position, textures, normals, colors, etc. So storing a few thousand
vertices more than once can add up.


```
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

    //use GL_ELEMENT_ARRAY_BUFFER instead of GL_ARRAY_BUFFER for indices
    //index buffers have to be made up of unsigned ints
    unsigned int ibo; //index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
```


To do our draw call:
We do not use ```glDrawArrays(GL_TRIANGLES,0,6);``` anymore, instead we use ```glDrawElements()```
```
    //Drawing with index buffers
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
```

## Dealing with Errors in OpenGL
Two main ways to get errors

glGetError(), is compatible with all versions and sets error flags. Will give you one error at a time

glDebugMessageCallback(), came out in OpenGL 4.3. Much better than getGetError() but can't be used
in earlier versions. Will give you more detailed information on the error itself.


glGetError() is called inside a loop to get all the errors
```
static void GLClearError()
{
    //runs through all the errors to clear it
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
    //as long as the error is not false
    while (GLenum error =glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}
```
We call GLClearError first, then GLCheckError to look for an error for a certain
section of the code. We get an error code back.
We need to convert the error code into hexadecimal to look it up

Example:
error code: 1280 is 0x0500, which is GL_INVALID_ENUM
```
    GLClearError();
    glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
    GLCheckError();
```
You could write code to convert the error message code into the
exact error message if you wanted to. The only thing we don't know
is what line of code the error happened.

If the error is being printed in every frame, the error
is in the render while loop.

We can make our debugger to pause the code execution when an error occurs
using an INSERT. It's similar to a breakpoint, but doing it with code

We make a macro at the top
```
//MSVC specific
#define ASSERT(x) if (!(x)) __debugbreak(); 
```
This only works with MSVC compilers
Then we change our GLCheckError function into this:
```
static bool GLLogCall()
{
    //as long as the error is not false
    while (GLenum error =glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        return false;
    }
    return true;
}
```
It returns false if there's an error, and true if there's no error
Run the code with the debugger to see the breakpoint being
inserted with the macro

Now here's the updated code:
```
    GLClearError();
    glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
    ASSERT(GLLogCall());
```

We can do better and make another macro to do the clearing and checking all in
a single call.
```
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall())
```
The code we want goes in the middle as "x" in between GLCall() and GLLogCall()
Now our code looks like this:
```
GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
```


We can do more with macros to include more information, first let's add more parameters
into GLLogCall:
```
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
```
Then for the macro we have
```
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
```
- Putting a hash in front of x turns it into a string in our macro
- __FILE__ gives the file location, not specific to a compiler
- __LINE__ give the line for the code, not specific to a compiler

Now for every GL call we make, we should wrap it with our macro for debugging
Putting a scope in the macro definition might mess up stack memory variables
that are assigned to the output of a gl function
example:
```
GLCall(unsigned int program = glCreateProgram();)
```
If we used a scope {} instead of \ to create a new line, the program variable
will become undone due to the scope.

## Uniforms
Are a way to send data from the CPU to the shader. In our case, C++ to our shaders to use as a variable.
Let's say we wanted to change the color of our square (2 triangles). We can do two things.
- Use a uniform
- send the data as an attribute with the vertex


Uniforms are set per draw, while attributes are set per vertex. We set the Uniform before the draw call.
One naming convention we can use for uniform variables are writing "u_" in front to signify a uniform
Here is an example of passing a uniform to our Fragment Shader

Our fragment shader before the uniform
```
#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;
        
void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
};
```

Our fragment shader after the uniform
```
#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
        
void main()
{
    color = u_Color;
};
```

Now we set the variable from c++

- since we are using vec4, we need 4 floats, thus we use glUniform4f
once a shader gets created, every shader gets an id so that we can reference it
the way we can look up the id, typically, is by its name.

- glUniform4F's first parameter is the id for the uniform in the shader, which we
can get with glGetUniformLocation passing shader and the name of the uniform as arguments

- We use ASSERT to make use location actually does exist, or create a breakpoint while debugging

```
unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
GLCall(glUseProgram(shader));


GLCall(int location = glGetUniformLocation(shader, "u_Color"));
ASSERT(location != -1);
GLCall(glUniform4f(location,0.2f,0.3f,0.8f,1.0f));
```

To make our uniform value change, we can set a base value and increment value
outside the render loop as such
```
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
    ...
    }
```

Now, within the loop, we set the uniform value again per frame using the variable r.
Once r is greater than 1.0f, the increment decreases, once it reaches zero, it increases.
we use r+=increment to change the value per frame.
```
    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f)
        increment = -0.05f;
    else if (r < 0.0f)
        increment = 0.05f;

    r += increment;
```

To limit our framerate to be less flashy, after the window context is created,
we use the function glfwSwapInterval() function and pass in a 1. This syncs the framerate
with our monitors refresh rate. The animation appears much smoother.

```
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

    .
    .
    .

    return 0;
}
```

One thing to keep in mind is that Uniforms are done on a per draw basis.



## Vertex Arrays
What is the difference between a vertex buffer and a vertex array?
Vertex Array doesn't exist in some other rendering apis.
They are a way to bind vertex buffers with a certain kind of specification/layout

