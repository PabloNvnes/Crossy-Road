#include "window.hpp"

void Window::onCreate() {
  // Inicializando shaders e fonte como antes
  auto const filename{abcg::Application::getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  // Inicializar pessoa (jogador) na parte de baixo da tela
  m_personX = 0.0;
  m_personY = -0.9; // Quase na base da tela

  // Inicializar carros em linhas horizontais
  for (int i = 0; i < 5; i++) {
    m_carX[i] = (i % 2 == 0) ? -1.0 : 1.0; // Alterna entre esquerda e direita
    m_carY[i] = -0.8 + i * 0.4; // Cada carro em uma faixa
    m_carSpeeds[i] = 0.05f + 0.02f * i; // Velocidade diferente para cada faixa
    m_gameData.m_carDirections[i] = (i % 2 == 0) ? CarDirection::RightToLeft : CarDirection::LeftToRight;
  }
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
}

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    // Movimentação da pessoa (jogador)
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
      m_gameData.m_personDirection = Direction::Up;
    }
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
      m_gameData.m_personDirection = Direction::Down;
    }
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
      m_gameData.m_personDirection = Direction::Left;
    }
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
      m_gameData.m_personDirection = Direction::Right;
    }
  }
}

void Window::onUpdate() {
  // Atualiza posição da pessoa
  if (m_gameData.m_personDirection == Direction::Up) m_personY += 0.1;
  if (m_gameData.m_personDirection == Direction::Down) m_personY -= 0.1;
  if (m_gameData.m_personDirection == Direction::Left) m_personX -= 0.1;
  if (m_gameData.m_personDirection == Direction::Right) m_personX += 0.1;

  // Limitar a posição da pessoa
  if (m_personX < -1.0) m_personX = -1.0;
  if (m_personX > 1.0) m_personX = 1.0;
  if (m_personY < -1.0) m_personY = -1.0;
  if (m_personY > 1.0) {
    m_gameData.m_state = State::Win; // Vitória se chegar ao topo
  }

  // Atualizar posição dos carros
  for (int i = 0; i < 5; i++) {
    if (m_gameData.m_carDirections[i] == CarDirection::RightToLeft) {
      m_carX[i] -= m_carSpeeds[i];
      if (m_carX[i] < -1.1) m_carX[i] = 1.1; // Reaparece do outro lado
    } else {
      m_carX[i] += m_carSpeeds[i];
      if (m_carX[i] > 1.1) m_carX[i] = -1.1; // Reaparece do outro lado
    }
  }

  // Verificar colisão entre a pessoa e os carros
  for (int i = 0; i < 5; i++) {
    if (std::abs(m_personX - m_carX[i]) < 0.1 && std::abs(m_personY - m_carY[i]) < 0.1) {
      m_gameData.m_state = State::GameOver; // Colisão detectada
    }
  }
}

void Window::onPaint() {
  // Limpar tela
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  // Desenhar pessoa e carros
  drawPerson();
  drawCars();
}

void Window::drawPerson() {
  // Desenhar a pessoa
  m_colors = {{{0.2, 0.8, 0.2, 1}, {0.2, 0.8, 0.2, 1}, {0.2, 0.8, 0.2, 1}, {0.2, 0.8, 0.2, 1}}};
  m_positions = {{{m_personX - 0.05, m_personY - 0.05}, 
                  {m_personX + 0.05, m_personY - 0.05}, 
                  {m_personX - 0.05, m_personY + 0.05}, 
                  {m_personX + 0.05, m_personY + 0.05}}};
  setupModel();
  abcg::glBindVertexArray(m_VAO);
  abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  abcg::glBindVertexArray(0);
}

void Window::drawCars() {
  // Desenhar os carros
  for (int i = 0; i < 5; i++) {
    m_colors = {{{0.8, 0.2, 0.2, 1}, {0.8, 0.2, 0.2, 1}, {0.8, 0.2, 0.2, 1}, {0.8, 0.2, 0.2, 1}}};
    m_positions = {{{m_carX[i] - 0.1, m_carY[i] - 0.05}, 
                    {m_carX[i] + 0.1, m_carY[i] - 0.05}, 
                    {m_carX[i] - 0.1, m_carY[i] + 0.05}, 
                    {m_carX[i] + 0.1, m_carY[i] + 0.05}}};
    setupModel();
    abcg::glBindVertexArray(m_VAO);
    abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    abcg::glBindVertexArray(0);
  }
}

void Window::setupModel() {
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Configurar VBO de cores e posições
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors.data(), GL_STATIC_DRAW);

  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions), m_positions.data(), GL_STATIC_DRAW);

  // Configurar VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(0);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  abcg::glEnableVertexAttribArray(1);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  abcg::glBindVertexArray(0);
}
