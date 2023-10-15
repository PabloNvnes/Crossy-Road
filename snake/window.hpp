#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Window : public abcg::OpenGLWindow {
  protected:
    void onEvent(SDL_Event const &event) override;
    void onCreate() override;
    void onUpdate() override;
    void onPaint() override;
    void onPaintUI() override;
    void onResize(glm::ivec2 const &size) override;
    void onDestroy() override;

  private:
    glm::ivec2 m_viewportSize{};

    GLuint m_VAO{};
    GLuint m_VBOColors{};
    GLuint m_VBOPositions{};
    GLuint m_program{};

    GameData m_gameData;

    abcg::Timer m_timer;

    ImFont *m_font{};

    double m_unit = 0.1;

    //Definindo variável booleana correspondente a geração da maça
    bool generateApple;

    //Definindo variáveis referentes ao tamanho da snake
    double mx_apple;
    double my_apple;

    int ml_snake;

    //Definindo tamanho que a snake terá em x/y (máx 30)
    std::array<double, 30> mx_snake;
    std::array<double, 30> my_snake;

    std::array<glm::vec4, 20> m_colors;
    std::array<glm::vec2, 20> m_positions;

    std::default_random_engine m_randomEngine;

    void drawGrid();
    void drawApple();
    void drawSnake();
    void setupModel();
    void play();
    void validate();
};

#endif