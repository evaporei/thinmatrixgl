#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void process_input(GLFWwindow *const window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

int main(void) {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *const window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D game", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGL() == GL_FALSE)
        return -1;

    glfwSwapInterval(1);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // must be counter clock wise
    const float positions[] = {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        -0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        /* 180.f, 180.f, 0.f, // bottom left */
        /* 540.f, 180.f, 0.f, // bottom right */
        /* 540.f, 540.f, 0.f, // top right */
        /* 180.f, 540.f, 0.f, // top left */
    };
    /* const unsigned int positions_len = sizeof(positions) / sizeof(positions[0]); */
    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 1,
    };
    const unsigned int indices_len = sizeof(indices) / sizeof(indices[0]);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const char *vertex_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main() \n"
        "{\n"
        "   gl_Position = vec4(pos, 1.0);\n"
        "}"
        ;
    const char *fragment_src =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() \n"
        "{\n"
        "   color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}"
        ;

    unsigned int program = glCreateProgram();
    int src_len;

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    src_len = strlen(vertex_src);
    glShaderSource(vs, 1, &vertex_src, &src_len);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    src_len = strlen(fragment_src);
    glShaderSource(fs, 1, &fragment_src, &src_len);
    glCompileShader(fs);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(program);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        glDrawElements(GL_TRIANGLES, indices_len, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
