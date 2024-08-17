#include <Geometry/Line.h>

core::Line::Line(glm::vec3 from, glm::vec3 to, glm::vec3 color)
{
    // TODO: remove?
    const char* base_path = SDL_GetBasePath();
    std::string vsh_path = base_path;
    std::string fsh_path = base_path;
    vsh_path += "res/shaders/line.vsh";
    fsh_path += "res/shaders/line.fsh";
    line.shader = load_shader(vsh_path.c_str(), fsh_path.c_str());  // TODO: error handling

    line.vertices.insert(line.vertices.end(), {
        from.x, from.y, from.z,
        to.x, to.y, to.z,
        });

    glGenVertexArrays(1, &line.vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(line.vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line.vertices), line.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // TODO
    glUseProgram(line.shader);
    glUniform3fv(glGetUniformLocation(line.shader, "color"), 1, &color[0]);

    return line;
}
