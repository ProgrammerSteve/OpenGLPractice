# OpenGL Notes

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