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