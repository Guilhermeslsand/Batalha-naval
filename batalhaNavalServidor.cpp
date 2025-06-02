#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")  // Incluir diretiva de linkagem

void inicializarSocketWindows() {
    WSADATA wsaData;
    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (resultado != 0) {
        std::cerr << "Inicialização do Socket Windows falhou: " << resultado << std::endl;
        exit(1);
    }
}

SOCKET criarSocket() {
    SOCKET socketReceptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketReceptor == INVALID_SOCKET) {
        std::cerr << "Error na atribuição do socketReceptor(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }
    return socketReceptor;
}

void vincularSocket(SOCKET socketReceptor, int porta) {
    sockaddr_in informacaoDoServidor;
    informacaoDoServidor.sin_family = AF_INET;
    informacaoDoServidor.sin_addr.s_addr = INADDR_ANY;
    informacaoDoServidor.sin_port = htons(porta);

    int resultado = bind(socketReceptor, (sockaddr*)&informacaoDoServidor, sizeof(informacaoDoServidor));
    if (resultado == SOCKET_ERROR) {
        std::cerr << "Vinculação das informações do servidor ao socket receptor criado falhou: " << WSAGetLastError() << std::endl;
        closesocket(socketReceptor);
        WSACleanup();
        exit(1);
    }
}

void aguardarPorConexoes(SOCKET socketReceptor) {
    int resultado = listen(socketReceptor, SOMAXCONN);
    if (resultado == SOCKET_ERROR) {
        std::cerr << "Houve um erro ao tentar receber alguma conexao: " << WSAGetLastError() << std::endl;
        closesocket(socketReceptor);
        WSACleanup();
        exit(1);
    }
}

SOCKET aceitarConexaoComCliente(SOCKET socketReceptor) {
    SOCKET socketCliente = accept(socketReceptor, NULL, NULL);
    if (socketCliente == INVALID_SOCKET) {
        std::cerr << "Falha ao aceitar a conexao: " << WSAGetLastError() << std::endl;
        closesocket(socketReceptor);
        WSACleanup();
        exit(1);
    }
    return socketCliente;
}

char* receberDado(SOCKET socketCliente) {
    static char buffer[512];
    memset(buffer, '\0', sizeof(buffer));
    int resultado = recv(socketCliente, buffer, sizeof(buffer) - 1, 0);
    if (resultado > 0){
        std::cout << "Dado recebido\n";
	return buffer;
    } else if (resultado == 0) {
        std::cout << "Conexao fechada" << std::endl;
    } else {
        std::cerr << "Falha ao receber dados: " << WSAGetLastError() << std::endl;
    }
}

void enviarDado(SOCKET socketCliente, const char* dado) {
    int resultado = send(socketCliente, dado, strlen(dado), 0);
    if (resultado == SOCKET_ERROR) {
        std::cerr << "Falha ao enviar dado: " << WSAGetLastError() << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        exit(1);
    }
}

void limpar(SOCKET socketReceptor, SOCKET socketCliente,SOCKET socketCliente2) {
    closesocket(socketReceptor);
    closesocket(socketCliente);
    closesocket(socketCliente2);
    WSACleanup();
}

int main() {
    char* buffer; 
    inicializarSocketWindows();
    SOCKET socketReceptor = criarSocket();
    vincularSocket(socketReceptor, 8080);
    
    std::cout << "Aguardando por um cliente\n";
    aguardarPorConexoes(socketReceptor);	    
    SOCKET socketCliente = aceitarConexaoComCliente(socketReceptor);
    std::cout << "Cliente 1, ok" << std::endl;
    enviarDado(socketCliente, "1");
    

    std::cout << "Aguardando por mais um cliente\n";
    SOCKET socketCliente2 = aceitarConexaoComCliente(socketReceptor);
    std::cout << "Cliente 2, ok" << std::endl;
    enviarDado(socketCliente2,"2");
    
    while(true){
    std::cout<<"Aguardando mensagem do cliente 1\n";
    buffer=receberDado(socketCliente);
    std::cout<<"Mensagem do cliente 1 recebida\n";
    if(strcmp(buffer, "fim")==0){
     enviarDado(socketCliente2, "fim");
    break;}
    enviarDado(socketCliente2, buffer);
    std::cout<<"mensagem enviada ao cliente 2\n";
    std::cout<<"Aguardando mensagem do cliente 2\n";
    buffer=receberDado(socketCliente2);
    std::cout<<"Mensagem do cliente 2 recebida\n";
    if(strcmp(buffer, "fim")==0){
     enviarDado(socketCliente, "fim");
    break;}
    enviarDado(socketCliente, buffer);
    std::cout<<"mensagem enviada ao cliente 1\n";
    }
    
    
    std::cout<<"Digite qualquer coisa para fechar o servidor\n";
    getchar();
    

    limpar(socketReceptor, socketCliente, socketCliente2);
    return 0;
}
