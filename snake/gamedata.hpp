#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

/* Neste arquivo definirmos a estrutura que descreve o estado atual do jogo e 
   dos dispositivos de entrada.
*/

/*Definindo estados possíveis do jogo:
    State::Playing > Aplicação no modo jogo com a snake respondendo aos comandos.
    State::GameOver > Quando o jogador perdeu.
    State::Win > Jogador ganhou.
*/
enum class State { Playing, GameOver, Win }; 

//Definindo dispositivos de entrada/direções
enum class Direction { Up, Down, Left, Right }; 

// Definindo movimento dos carros
enum class CarDirection { LeftToRight, RightToLeft }; 

struct GameData {
  State m_state{State::Playing};
  Direction m_personDirection{Direction::Up}; // Direção do jogador (de baixo para cima)
  std::array<CarDirection, 5> m_carDirections; // Array para representar a direção dos carros em 5 faixas (5 linhas de carros)
};


#endif
