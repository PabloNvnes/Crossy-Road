#include "window.hpp"

int main(int argc, char **argv) {

  /*A main permaneceu semelhante aos projetos apresentados em aula,
    somente desativamos o contador de FPS e o botão de tela cheia.
  */

  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 2, .doubleBuffering = true});
    window.setWindowSettings({
      .width = 600,
      .height = 600,
      .showFPS = false,
      .showFullscreenButton = false,
      .title = "Snake",
    });

    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());

    return -1;
  }

  return 0;
}