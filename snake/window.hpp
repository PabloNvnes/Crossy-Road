#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <array>
#include <random>

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Window : public abcg::OpenGLWindow {
  protected:
    void onCreate() override;
    void onUpdate() override;
    void onPaint() override;
    void onEvent(SDL_Event const &event) override;
    void onDestroy() override;

  private:
    GLuint m_VAO{};
    GLuint m_VBOColors{};
    GLuint m_VBOPositions{};
    ImFont *m_font{};

    GameData m_gameData;

    std::array<glm::vec4, 4> m_colors{};
    std::array<glm::vec2, 4> m_positions{};

    // Posição da pessoa (jogador)
    float m_personX{};
    float m_personY{};

    // Carros: posições e velocidades
    std::array<float, 5> m_carX{};
    std::array<float, 5> m_carY{};
    std::array<float, 5> m_carSpeeds{};

    void drawPerson();
    void drawCars();
    void setupModel();
};

#endif
