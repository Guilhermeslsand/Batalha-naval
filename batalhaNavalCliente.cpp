 #include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
#pragma comment(lib, "Ws2_32.lib") // Incluir diretiva de linkagem



void limparTela() {
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}
/*
    Nesse jogo um campo tem dimensão 10 x 10
    Por padrão, quando o campo está vazio ele é preenchido com '-'

    '-' representa que não tem nada no seu campo

    Os navios aqui são representados por uma letra

    Pporta aviões -> p
    Navio de guerra -> n
    Submarino -> s
    Cruzador -> c
    Destrudor -> d

    Quando esiver 0 no campo significa que o local foi escolhido como alvo pelo
   adversário mas não tinha nada lá, loco não houve acerto

    Quando estiver x no campo significa que o ataque foi um sucesso e atingiu
   alguma posição de algum návio.

    Ganha quem conseguir acertar todas as posições dos náivos adversário.

*/

// Transforma uma dois número como 5 e 5 em coordenada para o campo como F5
// Lembrar que no campo naval F5 -> F é a coluna e 5 é a linha.
string posicao_letra(int col, int lin) {
  string sCol;
  string sLin = to_string(lin);
  switch (col) {
  case 0:
    sCol = "A";
    break;

  case 1:
    sCol = "B";
    break;

  case 2:
    sCol = "C";
    break;

  case 3:
    sCol = "D";
    break;

  case 4:
    sCol = "E";
    break;

  case 5:
    sCol = "F";
    break;

  case 6:
    sCol = "G";
    break;

  case 7:
    sCol = "H";
    break;

  case 8:
    sCol = "I";
    break;

  case 9:
    sCol = "J";
    break;

  default:
    break;
  }
  // Concatenando a coluna com a linha em uma string sozinha.
  string pos = sCol + sLin;
  return pos;
}

// Converter uma coordena como A5, em inteiro, onde só estou pegando a coluna
int posicao_coluna(string pos) {
  int nCol = -1;
  stringstream ss(pos);
  char sCol;
  int nLin;

  ss >> sCol >> nLin;

  switch (sCol) {
  case 'A':
    nCol = 0;
    break;

  case 'B':
    nCol = 1;
    break;

  case 'C':
    nCol = 2;
    break;

  case 'D':
    nCol = 3;
    break;

  case 'E':
    nCol = 4;
    break;

  case 'F':
    nCol = 5;
    break;

  case 'G':
    nCol = 6;
    break;

  case 'H':
    nCol = 7;
    break;

  case 'I':
    nCol = 8;
    break;

  case 'J':
    nCol = 9;
    break;

  default:
    break;
  }
  int a = nCol + 1;
  if (false) {
    cout << a;
  }
  return nCol;
}

// Converter uma coordena como A5, em inteiro, onde só estou pegando a linha
int posicao_linha(string pos) {

  stringstream ss(pos);
  char sCol;
  int nLin;

  ss >> sCol >> nLin;

  return nLin - 1;
}

// Criação da classe campo que vai conter o meu campo e o campo do adversário.
class campo {
public:
  char meuCampo[10][10];
  char rivalCampo[10][10];
  int perdas = 0;
  char * getMeuCampo(){return meuCampo[0];}
  void setMeuCampo(char* campo){
	 for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            meuCampo[i][j] = campo[i * 10 + j];
        }
    }}

  // Função para printar um campo.
  void mostrar_meu_campo(string nome) {
    cout << nome << "\n";
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        cout << meuCampo[i][j] << " ";
      }
      cout << "\n";
    }
  }

  // Função para printar um campo.
  void mostrar_rival_campo(string nome) {
    cout << nome << "\n";
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        cout << rivalCampo[i][j] << " ";
      }
      cout << "\n";
    }
  }

  // Função para inicializar os campos com nada neles.
  void iniciar_campos() {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        meuCampo[i][j] = '-';
        rivalCampo[i][j] = '-';
      }
    }
  }

  // Função para checar o que tem no coordenada indicada
  int checar_local(int coluna, int linha, char matriz[10][10]) {
    int checagem;
    if (matriz[linha][coluna] == 'p' || matriz[linha][coluna] == 'n' ||
        matriz[linha][coluna] == 's' || matriz[linha][coluna] == 'd' ||
        matriz[linha][coluna] == 'c') {
      // tem um navio no local
      checagem = 1;
    } else if (matriz[linha][coluna] == '0') {
      // O local já foi escolhido como alvo, mas não acertou em nada
      checagem = 2;
    } else if (matriz[linha][coluna] == 'x') {
      // Local escolhido como alvo, atingiu um posição de um navio
      checagem = 3;
    } else if (matriz[linha][coluna] == '-') {
      // Não tem nada, pode colocar návio
      checagem = 0;
    } else {
      // Possível erro
      checagem = 4;
    }
    return checagem;
  }

  // Função para verificar se vai haver sobreposição de navios quando for
  // colocar no campo
  bool superposicao(int tamanho, int coluna, int linha, int decisao) {
    bool desocupado = true;
    for (int i = 0; i < tamanho; i++) {

      if (decisao == 0) {
        if (checar_local(coluna, linha + i, meuCampo) == 1) {
          desocupado = false;
          // cout<<"Está ocupado\n";
        }
      } else if (decisao == 1) {
        if (checar_local(coluna + i, linha, meuCampo) == 1) {
          desocupado = false;
          // cout<<"Está ocupado\n";
        }
      } else {
        cout << "Possível erro";
        desocupado = false;
      }
    }
    return desocupado;
  }

  // Função para verifcar se algum návio vai escapar da borda caso seja colocado
  // na posição indicada
  bool verificar_bordar(int coluna, int linha, int tamanho, int decisao) {
    bool verificar = true;
    if (decisao == 0 && linha + tamanho > 9) {
      // cout<<"Passou da borda pela vertical\n";
      verificar = false;
    } else if (decisao == 1 && coluna + tamanho > 9) {
      // cout<<"Passou da borda pela horizontal\n";
      verificar = false;
    }
    return verificar;
  }

  vector<int> colocar_manualmente() {
    string stringPos;
    int decisao;
    int coluna;
    int linha;
    vector<int> vetor;
    cout << "Digite a posição do Porta Aviões (5 casas)\n";
    cout << "Exemplo A9\n";
    cin >> stringPos;
    cout << "Digite 0 se quer colocar na vertical\n";
    cout << "Digite 1 se quer colocar na horizontal\n";
    cin >> decisao;
    coluna = posicao_coluna(stringPos);
    linha = posicao_linha(stringPos);
    vetor.push_back(linha);
    vetor.push_back(coluna);
    vetor.push_back(decisao);
    return vetor;
  }

  vector<int> colocar_automaticamente() {
    int coluna = rand() % 10;
    int linha = rand() % 10;
    int decisao = rand() % 2;
    vector<int> vetor;
    vetor.push_back(linha);
    vetor.push_back(coluna);
    vetor.push_back(decisao);
    return vetor;
  }

  // Função para preencher o campo e colocar os návios nos locais
  void preencher_campo() {
    vector<int> vetor;
    int decisao;
    int coluna;
    int linha;
    srand(time(0));
    do {
      vetor = colocar_automaticamente();
      linha = vetor.at(0);
      coluna = vetor.at(1);
      decisao = vetor.at(2);

    } while (verificar_bordar(coluna, linha, 5, decisao) == false ||
             superposicao(5, coluna, linha, decisao) == false);
    if (decisao == 0) {
      for (int i = 0; i < 5; i++) {
        meuCampo[linha + i][coluna] = 'p';
      }
    } else if (decisao == 1) {
      for (int i = 0; i < 5; i++) {
        meuCampo[linha][coluna + i] = 'p';
      }
    }
    // mostrar_campo("Meu Campo", meuCampo);
    do {
      vetor = colocar_automaticamente();
      linha = vetor.at(0);
      coluna = vetor.at(1);
      decisao = vetor.at(2);
    } while (verificar_bordar(coluna, linha, 4, decisao) == false ||
             superposicao(4, coluna, linha, decisao) == false);
    if (decisao == 0) {
      for (int i = 0; i < 4; i++) {
        meuCampo[linha + i][coluna] = 'n';
      }
    } else if (decisao == 1) {
      for (int i = 0; i < 4; i++) {
        meuCampo[linha][coluna + i] = 'n';
      }
    }
    // mostrar_campo("Meu CAmpo", meuCampo);
    do {
      vetor = colocar_automaticamente();
      linha = vetor.at(0);
      coluna = vetor.at(1);
      decisao = vetor.at(2);
    } while (verificar_bordar(coluna, linha, 3, decisao) == false ||
             superposicao(3, coluna, linha, decisao) == false);
    if (decisao == 0) {
      for (int i = 0; i < 3; i++) {
        meuCampo[linha + i][coluna] = 'c';
      }
    } else if (decisao == 1) {
      for (int i = 0; i < 3; i++) {
        meuCampo[linha][coluna + i] = 'c';
      }
    }
    // mostrar_campo("Meu CAmpo", meuCampo);
    do {
      vetor = colocar_automaticamente();
      linha = vetor.at(0);
      coluna = vetor.at(1);
      decisao = vetor.at(2);
    } while (verificar_bordar(coluna, linha, 3, decisao) == false ||
             superposicao(3, coluna, linha, decisao) == false);
    if (decisao == 0) {
      for (int i = 0; i < 3; i++) {
        meuCampo[linha + i][coluna] = 's';
      }
    } else if (decisao == 1) {
      for (int i = 0; i < 3; i++) {
        meuCampo[linha][coluna + i] = 's';
      }
    }
    // mostrar_campo("Meu CAmpo", meuCampo);
    do {
      vetor = colocar_automaticamente();
      linha = vetor.at(0);
      coluna = vetor.at(1);
      decisao = vetor.at(2);
    } while (verificar_bordar(coluna, linha, 2, decisao) == false ||
             superposicao(2, coluna, linha, decisao) == false);
    if (decisao == 0) {
      for (int i = 0; i < 2; i++) {
        meuCampo[linha + i][coluna] = 'd';
      }
    } else if (decisao == 1) {
      for (int i = 0; i < 2; i++) {
        meuCampo[linha][coluna + i] = 'd';
      }
    }
    // mostrar_campo("Meu Campo", meuCampo);
  }

  void modificar_matriz(char matriz[10][10], int linha, int coluna,
                        char letra) {
    matriz[linha][coluna] = letra;
  }

  int getDano() { return perdas; }
};

bool validar_coordenada(string posicao) {
  // Verifica se o tamanho da string é 2 ou 3 (ex.: A1 ou A10)
  if (posicao.length() < 2 || posicao.length() > 3)
    return false;

  // Verifica se o primeiro caractere é uma letra de 'A' a 'J'
  char coluna = posicao[0];
  if (coluna < 'A' || coluna > 'J')
    return false;

  // Verifica se o restante da string é um número de 1 a 10
  string linha = posicao.substr(1); // Pega a parte numérica da coordenada
  int numeroLinha = stoi(linha);
  if (numeroLinha < 1 || numeroLinha > 10)
    return false;

  // Se passar por todas as verificações, a coordenada é válida
  return true;
}

string solicitar_coordenada() {
  string posicao;
  do {
    cout << "Digite uma coordenada valida (ex.: A1, B10): ";
    cin >> posicao;
    if (!validar_coordenada(posicao)) {
      cout << "Voce digitou uma coordenada de forma errada\n";
    }
  } while (!validar_coordenada(posicao));
  return posicao;
}

void atacar(campo &campRival, campo &campMeu, string posicao) {

  int resultado;
  do {
    int coluna = posicao_coluna(posicao);
    int linha = posicao_linha(posicao);
    resultado = campRival.checar_local(coluna, linha, campRival.meuCampo);

    if (resultado == 0) {
      cout << "Voce nao acertou nada\n";
      campRival.modificar_matriz(campRival.meuCampo, linha, coluna, '0');
      campMeu.modificar_matriz(campMeu.rivalCampo, linha, coluna, '0');
    } else if (resultado == 1) {
      cout << "Voce acertou um navio\n";
      campRival.modificar_matriz(campRival.meuCampo, linha, coluna, 'x');
      campMeu.modificar_matriz(campMeu.rivalCampo, linha, coluna, 'x');
      campRival.perdas += 1;
    } else if (resultado == 2 || resultado == 3) {
      cout << "Voce ja escolheu esse alvo\n";
    } else {
      cout << "Possível erro\n";
    }
  } while (resultado >= 2 && !validar_coordenada(posicao));
}

void inicializarSocketWindows() {
  WSADATA wsaData;
  int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (resultado != 0) {
    std::cerr << "Falha ao inicializar o socket windows: " << resultado
              << std::endl;
    exit(1);
  }
}

SOCKET criarSocket() {
  SOCKET socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socketCliente == INVALID_SOCKET) {
    std::cerr << "Erro ao criar o socket" << WSAGetLastError() << std::endl;
    WSACleanup();
    exit(1);
  }
  return socketCliente;
}

void conectarAoServidor(SOCKET socketCliente, const char *enderecoDoServidor,
                        int porta) {
  sockaddr_in informacaoDoServidor;
  informacaoDoServidor.sin_family = AF_INET;
  informacaoDoServidor.sin_addr.s_addr = inet_addr(enderecoDoServidor);
  informacaoDoServidor.sin_port = htons(porta);

  int resultado = connect(socketCliente, (sockaddr *)&informacaoDoServidor,
                          sizeof(informacaoDoServidor));
  if (resultado == SOCKET_ERROR) {
    std::cerr << "Não foi possível se conectar ao servidor : "
              << WSAGetLastError() << std::endl;
    closesocket(socketCliente);
    WSACleanup();
    exit(1);
  }
}

void enviarDado(SOCKET socketCliente, const char *dado) {
  int resultado = send(socketCliente, dado, strlen(dado), 0);
  if (resultado == SOCKET_ERROR) {
    std::cerr << "Falha ao enviar dado: " << WSAGetLastError() << std::endl;
    closesocket(socketCliente);
    WSACleanup();
    exit(1);
  }
}

char *receberDado(SOCKET socketCliente) {
  static char buffer[512];
  memset(buffer, '\0', sizeof(buffer));
  int resultado = recv(socketCliente, buffer, sizeof(buffer) - 1, 0);
  if (resultado > 0) {
    // std::cout << "Dado recebido\n";
    return buffer;
  } else if (resultado == 0) {
    std::cout << "Conexao fechada\n";
  } else {
    std::cerr << "Falha ao receber dado: " << WSAGetLastError() << "\n";
  }
}

void limpar(SOCKET socketCliente) {
  closesocket(socketCliente);
  WSACleanup();
}

int main() {
  
  char *buffer;
  char mensagem[512];
  int jogador = 0;
  inicializarSocketWindows();
  SOCKET socketCliente = criarSocket();
  conectarAoServidor(socketCliente, "10.0.0.133", 8080);
  buffer = receberDado(socketCliente);
  std::cout << buffer[0] << "\n";
  if (buffer[0] == '1') {
    std::cout << "voce eh o 1 jogador\n";
    jogador = 1;
  } else if (buffer[0] == '2') {
    std::cout << "voce eh o 2 jogador\n";
    jogador = 2;
  }

  campo eu;
  campo rival;
  srand(time(0));
  string posicao;

  eu.iniciar_campos();
  eu.preencher_campo();
  rival.iniciar_campos();
  rival.preencher_campo();
    
    if(jogador==1){    
    enviarDado(socketCliente, eu.getMeuCampo());
    rival.setMeuCampo(receberDado(socketCliente));}
    
    else if(jogador==2){    
    rival.setMeuCampo(receberDado(socketCliente));
    enviarDado(socketCliente, eu.getMeuCampo());}

  
  
 



  while (jogador == 1  && eu.getDano()<3 && rival.getDano()<3) {

    limparTela();
    eu.mostrar_meu_campo("Meu Campo");
    eu.mostrar_rival_campo("Campo do Rival");
    posicao = solicitar_coordenada();
    atacar(rival, eu, posicao);
    enviarDado(socketCliente, posicao.c_str());
    limparTela();
    eu.mostrar_meu_campo("Meu Campo");
    eu.mostrar_rival_campo("Campo do Rival");
    buffer = receberDado(socketCliente);
    atacar(eu, rival, buffer);
    
   
  }
  
  while (jogador == 2 && eu.getDano()<3 && rival.getDano()<3) {
    
    limparTela();
    eu.mostrar_meu_campo("Meu Campo");
    eu.mostrar_rival_campo("Campo do Rival");
    buffer = receberDado(socketCliente);
    atacar(eu, rival, buffer);
    limparTela();
    eu.mostrar_meu_campo("Meu Campo");
    eu.mostrar_rival_campo("Campo do Rival");
    posicao = solicitar_coordenada();
    atacar(rival, eu, posicao);
    enviarDado(socketCliente, posicao.c_str());

  }

if (eu.getDano() >= 3) {
        cout << "Rival Venceu\n";
    } else if (rival.getDano() >= 3) {
        cout << "Voce Venceu\n";
    }


  std::cout << "CONEXAO ENCERRADA\n";
  limpar(socketCliente);
  return 0;
}
