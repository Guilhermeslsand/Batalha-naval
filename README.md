# Servidor de Batalha Naval

## 📌 Descrição
Servidor TCP para um jogo de Batalha Naval multijogador, desenvolvido em C++ com Winsock2. Gerencia a comunicação entre dois clientes, atuando como intermediário para troca de mensagens durante o jogo.

## 🚀 Funcionalidades
- Conexão simultânea de dois jogadores
- Encaminhamento de mensagens em tempo real
- Detecção de comando "fim" para encerrar a partida
- Identificação dos jogadores (Cliente 1 e Cliente 2)

## ⚙️ Pré-requisitos
- Windows 10 ou superior
- Compilador C++ (MinGW ou MSVC)
- Conhecimento básico de linha de comando

## 🔧 Configuração
Parâmetro	Valor Padrão	Descrição
Porta do servidor	8080	Porta TCP para conexões
Para alterar a porta, modifique o valor na chamada de vincularSocket() no arquivo principal.

## 📊 Fluxo de Operação
Servidor inicia e aguarda Cliente 1
Cliente 1 conecta e recebe ID "1"
Servidor aguarda Cliente 2
Cliente 2 conecta e recebe ID "2"
Loop principal inicia:
Recebe mensagem de Cliente 1 → Encaminha para Cliente 2
Recebe mensagem de Cliente 2 → Encaminha para Cliente 1
Encerra ao receber "fim" de qualquer cliente

⚠️ Limitações Conhecidas
Buffer fixo de 512 bytes
Suporte apenas para 2 jogadores
Sem mecanismo de reconexão
Não valida formato das mensagens

📝 Notas de Desenvolvimento
Arquivo principal: batalhaNavalServidor.cpp
Dependência: Biblioteca Winsock2 (Ws2_32.lib)
Estrutura:
- Inicialização de sockets
- Loop principal de mensagens
- Limpeza de recursos

## 🛠️ Compilação e Execução
```bash
g++ batalhaNavalServidor.cpp -o servidor -lws2_32
.\servidor
