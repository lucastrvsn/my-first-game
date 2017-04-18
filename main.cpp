#include <iostream>
#include <conio2.h>
#include <windows.h>
#include <time.h>
using namespace std;

const int LARGURA_MAX = 100;
const int ALTURA_MAX = 100;

struct Configuracao
{
    bool running;
    bool loadscreen;
    int posicao_menu[2];
    bool ingame;
};

struct Labirinto
{
    int altura;
    int largura;
    char mapa[LARGURA_MAX][ALTURA_MAX];
    char caminho[LARGURA_MAX][ALTURA_MAX];
    bool visitado[LARGURA_MAX][ALTURA_MAX];
    bool desenhado;
    int inicialx;
    int inicialy;
    bool redraw;
    int premios;
};

struct Player
{
    int x;
    int y;
    int xanterior;
    int yanterior;
    int score;
    int ganhaVida;
    int lives;
    int marretas;
    bool gerado;
    bool ganhouVida;
    char style;
};

struct Inimigo
{
    int x;
    int y;
    int xanterior;
    int yanterior;
    int caminho[LARGURA_MAX][ALTURA_MAX];
    bool gerado;
    char style;
};

char PegaTecla(char &tecla); // retorna a tecla pressionada pelo usuario
void Init(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo); // seta os valores das variaveis iniciais
void LoadScreen(Configuracao &cfg); // tela inicial do jogo
void Menu(Configuracao &cfg, Player player, Inimigo inimigo, Labirinto labirinto); // menu para a escolha inicial
bool Update(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo); // realiza as verificações de funcionamento do jogo e atualiza
void GerarLogo(); // gera logo inicial ######## precisa terminar de criar ########
void EscolherDificuldade(Configuracao cfg, Player player, Inimigo inimigo, Labirinto labirinto); // responsavel pela escolha da dificuldade do jogo e inicio do mesmo
void OpcoesMenu(Configuracao cfg, int qnt); // opcoes mostradas na tela das teclas de acao
void NovoJogo(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo, int dificuldade); // realiza a introducao de um novo jogo e define os parametros
void HideCursor(); // esconde o cursor do DOS
void GerarFrescura(char *nome); // gera a parte superior dos menus
int Random(int dificuldade); // funcao que randomiza os numeros
void GerarLabirinto(Labirinto &labirinto, Player &player, Inimigo &inimigo, int dificuldade); // gera o labirinto com seus parametros definidos pelo usuario (dificuldade escolhida) e atualiza o mapa
void Verifica(Labirinto &labirinto, Inimigo &inimigo, int valor, int dificuldade, int i, int j); // verifica os campos e preenche o labirinto/mapa[][]
void DefineLab(Labirinto &labirinto, Inimigo &inimigo, int dificuldade); // define o tamanho de cada tipo de labirinto
void YouWin(Player player); // funcao chamada quando o jogo é ganhado
void Help(Configuracao cfg); // funcao que abre o menu de ajuda ao jogador
void GameOver(Player player); // funcao chamada quando o jogo é perdido
void Enemy(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo); // gera o inimigo e verifica suas posicoes e o move
void modoRusso(Player &player, Labirinto &labirinto); // funcao responsavel pela marreta do jogador (quebrar as paredes)
void Credits(Configuracao cfg); // creditos do jogo e informacoes

int main()
{
    system("title Jogo do Labirinto BETA 0.1"); // define o titulo da janela
    srand(time(NULL));
    system("MODE con cols=80 lines=50"); // define o tamanho da janela
    HideCursor();
    // declaração das structs
    Configuracao cfg;
    Player player;
    Labirinto labirinto;
    Inimigo inimigo;

    Init(cfg, player, labirinto, inimigo);

    while(cfg.running)
    {
        if(!cfg.loadscreen)
            LoadScreen(cfg);

        Menu(cfg, player, inimigo, labirinto);
    }

    return 0;
}

void HideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void Init(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo)
{
    // configura o jogo
    cfg.loadscreen = true; // precisa terminar de criar essa logo
    cfg.running = true;
    cfg.posicao_menu[0] = 7;
    cfg.posicao_menu[1] = 8;
    cfg.ingame = false;

    // inicia jogador
    player.x = 1;
    player.y = 1;
    player.score = 0;
    player.ganhaVida = 0;
    player.lives = 4;
    player.style = 002;
    player.marretas = 1;
    player.ganhaVida = false;

    // inicia inimigo
    inimigo.x = 1;
    inimigo.y = 1;
    inimigo.style = 15;

    labirinto.premios = 0;
}

void LoadScreen(Configuracao &cfg)
{
    GerarLogo();

    Sleep(2000);
    clrscr();
    cfg.loadscreen = true;
}

void GameOver(Player player)
{
    clrscr();
    gotoxy(5, 4);
    textcolor(YELLOW);
    cout << "VOCE PERDEU!";
    Sleep(1000);
    gotoxy(6, 5);
    textcolor(YELLOW);
    cout << "QUE PENA :(";
    Sleep(1000);
    gotoxy(7, 6);
    textcolor(YELLOW);
    if(player.score > 0)
        cout << "mas... veja pelo lado bom, voce fez " << player.score << " pontos! UHUL!";
    else
        cout << "voce nao fez nada de pontos? poxa, nem e tao dificil assim :(";
    Sleep(3000);
    gotoxy(7, 8);
    textcolor(YELLOW);
    cout << "VEJO VOCE EM UMA PROXIMA!";
    Sleep(3000);
}

void GerarLogo()
{
    char letra;
    int color;
    int colorb;

    for(int i = 0; i < 5; i++)
    {
        color = rand() % 7 + 2;
        colorb = rand() % 7 + 2;
        textcolor(color);
        textbackground(colorb);

        // letra L
        for(int i = 3; i < 10; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(3, i);
            cout << letra << letra;
        }
        for(int i = 3; i < 8; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(i, 10);
            cout << letra << letra;
        }

        colorb = rand() % 7 + 2;
        // letra A
        for(int i = 10; i > 5; i--)
        {
            letra = rand() % 254 + 8;
            gotoxy(10, i);
            cout << letra << letra;
        }
        letra = rand() % 254 + 8;
        gotoxy(11, 5);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(12, 4);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(13, 3);
        cout << letra << letra;

        letra = rand() % 254 + 8;
        gotoxy(15, 4);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(16, 5);
        cout << letra << letra;
        for(int i = 5; i < 11; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(17, i);
            cout << letra << letra;
        }

        letra = rand() % 254 + 8;
        gotoxy(12, 8);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(13, 8);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(14, 8);
        cout << letra << letra;

        colorb = rand() % 7 + 2;
        //letra B
        for(int i = 3; i < 11; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(20, i);
            cout << letra << letra;
        }

        letra = rand() % 254 + 8;
        gotoxy(21, 10);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(23, 10);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(24, 9);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(24, 8);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(23, 7);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(22, 7);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(24, 6);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(24, 5);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(24, 4);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(23, 3);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(22, 3);
        cout << letra << letra;

        colorb = rand() % 7 + 2;
        //letra T
        for(int i = 3; i < 11; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(31, i);
            cout << letra << letra;
        }
        for(int i = 27; i < 34; i++)
        {
            letra = rand() % 254 + 8;
            gotoxy(i, 3);
            cout << letra << letra;
        }

        colorb = rand() % 7 + 2;
        //letra O
        letra = rand() % 254 + 8;
        gotoxy(38, 3);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(40, 3);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 4);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 5);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 6);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 7);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 8);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(37, 9);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(38, 10);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(39, 10);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(40, 10);
        cout << letra << letra;

        letra = rand() % 254 + 8;
        gotoxy(41, 4);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(41, 5);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(41, 6);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(41, 7);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(41, 8);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        gotoxy(41, 9);
        cout << letra << letra;
        letra = rand() % 254 + 8;
        Sleep(320);
    }

    textbackground(1);
    textcolor(3);
    gotoxy(5, 12);
    cout << "Criado pelo aluno:";
    gotoxy(5, 13);
    cout << "Lucas de Almeida Trevisan ^-^";
    gotoxy(5, 15);
    cout << "Trabalho destinado a disciplina de";
    gotoxy(5, 16);
    cout << "Algoritmos e tecnicas de programacao II.";

    Sleep(1500);

    textbackground(0);
}

bool Update(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo)
{
    enum {Y1, Y2, X1, X2}; // y1 = -- ///// y2 = ++ //// x1 = -- //// x2 = ++

    char tecla;

    PegaTecla(tecla);

    player.xanterior = player.x;
    player.yanterior = player.y;

    int lastEdit;

    if(tecla == 'w' && player.y > 1)
    {
        player.y--;
        lastEdit = Y1;
    }
    else if(tecla == 's' && player.y < labirinto.altura - 1)
    {
        player.y++;
        lastEdit = Y2;
    }
    else if(tecla == 'a' && player.x > 1)
    {
        player.x--;
        lastEdit = X1;
    }
    else if(tecla == 'd' && player.x < labirinto.largura - 1)
    {
        player.x++;
        lastEdit = X2;
    }
    else if(tecla == 'q' && player.marretas > 0)
    {
        modoRusso(player, labirinto);
    }
    else if(tecla == 27)
    {
        main();
    }

    if(labirinto.caminho[player.x][player.y] == 1)
    {
        if(lastEdit == Y1)
            player.y++;
        else if(lastEdit == Y2)
            player.y--;
        else if(lastEdit == X1)
            player.x++;
        else if(lastEdit == X2)
            player.x--;
    }
    else if(labirinto.caminho[player.x][player.y] == '.' && player.y >= 1 && player.y < labirinto.altura && player.x >= 1 && player.x < labirinto.largura)
    {
        player.lives--;
    }
    else if(labirinto.caminho[player.x][player.y] == 2)
    {
        player.score += 10;
        player.ganhaVida++;
    }
    else if(labirinto.caminho[player.x][player.y] == 3)
    {
        player.lives = 0;
    }

    if(player.x == inimigo.x && player.y == inimigo.y)
    {
        player.lives = 0;
    }

    Enemy(cfg, player, labirinto, inimigo); /////////////////////////////////////////////////////////////////////////////

    /*if(labirinto.mapa[player.x][player.y] == 2)
    {
        labirinto.premios--;
    }*/

    if(player.ganhaVida == 6 && !player.ganhouVida)
    {
        player.lives++;
        flashbackground(2, 60);
        player.ganhouVida = true;
    }

    if(player.ganhaVida == 12)
    {
        player.marretas++;
        player.ganhaVida = 0;
        flashbackground(2, 60);
        player.ganhouVida = false;
    }

    if(player.lives == 0)
    {
        cfg.ingame = false;
        GameOver(player);
    }

    if(player.score > labirinto.premios / 2 && player.x == labirinto.inicialx && player.y == labirinto.inicialy)
    {
        cfg.ingame = false;
        YouWin(player);
    }

    labirinto.visitado[player.x][player.y] = true;
    labirinto.desenhado = false;
    labirinto.redraw = true;

    return true;
}

void modoRusso(Player &player, Labirinto &labirinto)
{
    char tecla;
    PegaTecla(tecla);

    if(labirinto.caminho[player.x + 1][player.y] == 1 || labirinto.caminho[player.x - 1][player.y] == 1 || labirinto.caminho[player.x][player.y + 1] == 1 || labirinto.caminho[player.x][player.y - 1] == 1)
    {
        if(tecla == 'd')
        {
            labirinto.caminho[player.x + 1][player.y] = 0;
            labirinto.mapa[player.x + 1][player.y] = 255;
            gotoxy(player.x + 1, player.y);
            cout << " ";
        }
        else if(tecla == 'a')
        {
            labirinto.caminho[player.x - 1][player.y] = 0;
            labirinto.mapa[player.x - 1][player.y] = 255;
            gotoxy(player.x - 1, player.y);
            cout << " ";
        }
        else if(tecla == 's')
        {
            labirinto.caminho[player.x][player.y + 1] = 0;
            labirinto.mapa[player.x][player.y + 1] = 255;
            gotoxy(player.x, player.y + 1);
            cout << " ";
        }
        else if(tecla == 'w')
        {
            labirinto.caminho[player.x][player.y - 1] = 0;
            labirinto.mapa[player.x][player.y - 1] = 255;
            gotoxy(player.x, player.y - 1);
            cout << " ";
        }

        player.marretas--;
    }
    else
        return;

    flashbackground(4, 100);
}

void Enemy(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo)
{
    int valor;
    bool gerado = false;
    inimigo.xanterior = inimigo.x;
    inimigo.yanterior = inimigo.y;

    while(!gerado)
    {
        valor = rand() % 4 + 1; // 1 = /\ | 2 = > | 3 = \/ | 4 = <

        if(valor == 1)
        {
            if(inimigo.caminho[inimigo.x][inimigo.y - 1] != 1 && inimigo.y >= 1)
            {
                inimigo.y--;
                gerado = true;
            }
        }
        else if(valor == 2)
        {
            if(inimigo.caminho[inimigo.x + 1][inimigo.y] != 1 && inimigo.x < labirinto.largura - 1)
            {
                inimigo.x++;
                gerado = true;
            }
        }
        else if(valor == 3)
        {
            if(inimigo.caminho[inimigo.x][inimigo.y + 1] != 1 && inimigo.y < labirinto.altura - 1)
            {
                inimigo.y++;
                gerado = true;
            }
        }
        else if(valor == 4)
        {
            if(inimigo.caminho[inimigo.x - 1][inimigo.y] != 1 && inimigo.x >= 1)
            {
                inimigo.x--;
                gerado = true;
            }
        }

        if(inimigo.x < 1 || inimigo.x > labirinto.largura || inimigo.y < 1 || inimigo.y > labirinto.altura)
        {
            inimigo.x = labirinto.inicialx;
            inimigo.y = labirinto.inicialy;
        }
    }

    gotoxy(inimigo.xanterior, inimigo.yanterior);
    cout << " ";
}

void YouWin(Player player)
{
    clrscr();

    gotoxy(5, 4);
    textcolor(YELLOW);
    cout << "VOCE GANHOU!";
    Sleep(1000);
    gotoxy(6, 5);
    textcolor(YELLOW);
    cout << "TU E FODA! :)";
    Sleep(1000);
    gotoxy(7, 6);
    textcolor(YELLOW);
    cout << "Terminou o jogo com " << player.score << " pontos!";
    Sleep(3000);
    gotoxy(7, 7);
    textcolor(YELLOW);
    cout << "E ainda com " << player.lives << " vidas sobrando! FODA!";
    Sleep(3000);
    gotoxy(7, 9);
    textcolor(YELLOW);
    cout << "VAMOS VER SE CONSEGUE VENCER DE NOVO!";
    Sleep(3000);
}

void Menu(Configuracao &cfg, Player player, Inimigo inimigo, Labirinto labirinto)
{
    char tecla;
    bool escolhido = false;
    int posicaoNoMenu = 0;

    clrscr();

    GerarFrescura(" Bem vindo ao Labirinto! ");

    while(!escolhido)
    {
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1]);
        cout << "[ ] Novo jogo";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 1);
        cout << "[ ] Ajuda";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 2);
        cout << "[ ] Creditos";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 3);
        cout << "[ ] Sair";

        gotoxy(cfg.posicao_menu[0] + 1, cfg.posicao_menu[1] + posicaoNoMenu);
        cout << "#";

        OpcoesMenu(cfg, 5);

        PegaTecla(tecla);

        if(tecla == 'w' && posicaoNoMenu > 0)
        {
            posicaoNoMenu--;
        }
        else if(tecla == 's' && posicaoNoMenu < 3)
        {
            posicaoNoMenu++;
        }
        else if(tecla == 32 || tecla == 13)
        {
            escolhido = true;
        }

        if(tecla == 27)
        {
            posicaoNoMenu = 1;
            cfg.running = false;
            escolhido = true;
        }
    }

    if(posicaoNoMenu == 0)
        EscolherDificuldade(cfg, player, inimigo, labirinto);
    else if(posicaoNoMenu == 1)
        Help(cfg);
    else if(posicaoNoMenu == 2)
        Credits(cfg);
    else if(posicaoNoMenu == 3)
        cfg.running = false;
}

void Help(Configuracao cfg)
{
    clrscr();
    GerarFrescura(" Menu de ajuda ao jogador ");

    gotoxy(5, cfg.posicao_menu[1]);
    cout << "Para movimentar o personagem, use as teclas:";

    textcolor(RED);
    gotoxy(14, cfg.posicao_menu[1] + 2);
    cout << "W";
    gotoxy(12, cfg.posicao_menu[1] + 3);
    cout << "A";
    gotoxy(14, cfg.posicao_menu[1] + 3);
    cout << "S";
    gotoxy(16, cfg.posicao_menu[1] + 3);
    cout << "D";

    textcolor(DARKGRAY);
    gotoxy(5, cfg.posicao_menu[1] + 5);
    cout << "O objetivo do jogo e coletar metade ou mais dos premios";
    gotoxy(5, cfg.posicao_menu[1] + 6);
    cout << "e voltar ao ponto inicial. Os premios possuem esse simbolo: ";
    gotoxy(14, cfg.posicao_menu[1] + 8);
    textcolor(2);
    cout << (char)005;

    textcolor(DARKGRAY);
    gotoxy(5, cfg.posicao_menu[1] + 10);
    cout << "Ao pegar seis premios, voce ganha uma vida.";
    gotoxy(5, cfg.posicao_menu[1] + 11);
    cout << "E ao pegar doze premios, voce ganha uma marreta. Use-a para quebrar parede!";
    gotoxy(5, cfg.posicao_menu[1] + 12);
    cout << "Para usar a marreta pressione a tecla ";
    textcolor(RED);
    cout << "[Q] ";
    textcolor(DARKGRAY);
    cout << "+ uma tecla de direcao.";

    textcolor(DARKGRAY);
    gotoxy(5, cfg.posicao_menu[1] + 14);
    cout << "Cuidado para nao cair em buracos, eles sao assim:";
    gotoxy(14, cfg.posicao_menu[1] + 16);
    cout << (char)254;
    gotoxy(5, cfg.posicao_menu[1] + 18);
    cout << "E nao chegue perto do inimigo:";
    gotoxy(14, cfg.posicao_menu[1] + 20);
    textcolor(3);
    cout << (char)15;
    textcolor(DARKGRAY);
    gotoxy(5, cfg.posicao_menu[1] + 22);
    cout << "Voce morre se encostar em um desses!";
    gotoxy(5, cfg.posicao_menu[1] + 23);
    cout << "Agora voce esta pronto para jogar!";

    textcolor(YELLOW);
    gotoxy(60, cfg.posicao_menu[1] + 25);
    cout << "BOM JOGO!";

    _getch();
}

void Credits(Configuracao cfg)
{
    clrscr();
    GerarFrescura(" Creditos do jogo Labirinto ");

    gotoxy(5, cfg.posicao_menu[1]);
    cout << "Desenvolvido por Lucas de Almeida Trevisan";
    gotoxy(5, cfg.posicao_menu[1] + 2);
    cout << "Esse projeto faz parte da nota para o primeiro bimestre";
    gotoxy(5, cfg.posicao_menu[1] + 3);
    cout << "da disciplina de Algoritmos e Tecnicas de Programacao II.";

    _getch();
}

void EscolherDificuldade(Configuracao cfg, Player player, Inimigo inimigo, Labirinto labirinto)
{
    char tecla;
    bool escolhido = false;
    int posicaoNoMenu = 0;

    clrscr();

    GerarFrescura(" Escolha uma dificuldade ");

    while(!escolhido)
    {
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1]);
        cout << "[ ] Facil";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 1);
        cout << "[ ] Medio";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 2);
        cout << "[ ] Dificil";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 3);
        cout << "[ ] Leandro BOLADISSIMO!";
        gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + 4);
        cout << "[ ] Joao Lucas HARDCORE BITCH!";

        gotoxy(cfg.posicao_menu[0] + 1, cfg.posicao_menu[1] + posicaoNoMenu);
        cout << "#";

        OpcoesMenu(cfg, 6);

        PegaTecla(tecla);

        if(tecla == 'w' && posicaoNoMenu > 0)
        {
            posicaoNoMenu--;
        }
        else if(tecla == 's' && posicaoNoMenu < 4)
        {
            posicaoNoMenu++;
        }
        else if(tecla == 32 || tecla == 13)
        {
            escolhido = true;
        }
        else if(tecla == 27)
        {
            Menu(cfg, player, inimigo, labirinto);
        }
    }

    for(int i = 0; i < 5; i++)
        if(posicaoNoMenu == i)
            NovoJogo(cfg, player, labirinto, inimigo, posicaoNoMenu);
}

void NovoJogo(Configuracao &cfg, Player &player, Labirinto &labirinto, Inimigo &inimigo, int dificuldade)
{
    Init(cfg, player, labirinto, inimigo);
    cfg.ingame = true;
    labirinto.desenhado = false;
    labirinto.redraw = false;

    clrscr();

    while(cfg.ingame)
    {
        if(!labirinto.desenhado)
            GerarLabirinto(labirinto, player, inimigo, dificuldade);
        else
        {
            Update(cfg, player, labirinto, inimigo);
        }
    }

    //void GameOver(int pontuacao, int dificuldade);
}

void GerarLabirinto(Labirinto &labirinto, Player &player, Inimigo &inimigo, int dificuldade)
{
    if(!labirinto.redraw)
    {
        int valor;
        DefineLab(labirinto, inimigo, dificuldade);

        for(int i = 1; i < labirinto.altura; i++)
            for(int j = 1; j < labirinto.largura; j++)
            {
                valor = Random(dificuldade);

                Verifica(labirinto, inimigo, valor, dificuldade, i, j);

                gotoxy(j, i);
                cout << labirinto.mapa[j][i];
            }

        if(!player.gerado && !inimigo.gerado)
        {
            int valor1 = rand() % (labirinto.largura - 2) + 2;
            int valor2 = rand() % (labirinto.altura - 2) + 2;
            player.x = valor1;
            player.y = valor2;
            labirinto.inicialx = valor1;
            labirinto.inicialy = valor2;
            labirinto.mapa[labirinto.inicialx][labirinto.inicialy] = 'o';
            gotoxy(labirinto.inicialx, labirinto.inicialy);
            cout << labirinto.mapa[labirinto.inicialx][labirinto.inicialy];

            valor1 = rand() % (labirinto.largura - 2) + 2;
            valor2 = rand() % (labirinto.altura - 2) + 2;
            inimigo.x = valor1;
            inimigo.y = valor2;
            gotoxy(inimigo.x, inimigo.y);
            cout << (char)inimigo.style;

            player.gerado = true;
            inimigo.gerado = true;
        }
    }
    else
    {
        gotoxy(player.xanterior, player.yanterior);
        textcolor(DARKGRAY);
        labirinto.caminho[player.xanterior][player.yanterior] = '.';
        cout << labirinto.caminho[player.xanterior][player.yanterior];
        gotoxy(player.x, player.y);
        textcolor(YELLOW);
        cout << (char)player.style;
        gotoxy(labirinto.inicialx, labirinto.inicialy);
        textcolor(3);
        cout << labirinto.mapa[labirinto.inicialx][labirinto.inicialy];
        gotoxy(inimigo.x, inimigo.y);
        textcolor(3);
        cout << (char)inimigo.style;

        if(labirinto.caminho[inimigo.xanterior][inimigo.yanterior] == 2)
        {
            textcolor(2);
            gotoxy(inimigo.xanterior, inimigo.yanterior);
            cout << (char)005;
        }
        else if(labirinto.caminho[inimigo.xanterior][inimigo.yanterior] == 3)
        {
            gotoxy(inimigo.xanterior, inimigo.yanterior);
            textcolor(8);
            cout << (char)254;
        }
        else if(labirinto.caminho[inimigo.xanterior][inimigo.yanterior] == '.')
        {
            gotoxy(inimigo.xanterior, inimigo.yanterior);
            textcolor(DARKGRAY);
            cout << ".";
        }

        textcolor(WHITE);
        gotoxy(5, labirinto.altura + 2);
        cout << "Vidas restantes: ";
        for(int i = 1; i <= player.lives; i++)
        {
            gotoxy(21 + i, labirinto.altura + 2);
            cout << "  ";
        }
        for(int i = 1; i <= player.lives; i++)
        {
            gotoxy(21 + i, labirinto.altura + 2);
            textcolor(LIGHTRED);
            cout << (char)3;
        }
        textcolor(WHITE);
        gotoxy(5, labirinto.altura + 3);
        cout << "Pontos: " << player.score << " / " << labirinto.premios;
        textcolor(WHITE);
        gotoxy(5, labirinto.altura + 4);
        cout << "Objetivo: " << labirinto.premios / 2;
        gotoxy(5, labirinto.altura + 5);
        cout << "Marretas: " << player.marretas;
    }

    labirinto.desenhado = true;
    labirinto.redraw = false;
}

void DefineLab(Labirinto &labirinto, Inimigo &inimigo, int dificuldade)
{
    if(dificuldade == 0)
    {
        labirinto.largura = 78;
        labirinto.altura = 15;
    }
    else if(dificuldade == 1)
    {
        labirinto.largura = 78;
        labirinto.altura = 20;
    }
    else if(dificuldade == 2)
    {
        labirinto.largura = 78;
        labirinto.altura = 25;
    }
    else if(dificuldade == 3)
    {
        labirinto.largura = 78;
        labirinto.altura = 30;
    }
    else if(dificuldade == 4)
    {
        labirinto.largura = 78;
        labirinto.altura = 30;
    }

    labirinto.mapa[labirinto.largura][labirinto.altura] = {0};
    labirinto.caminho[labirinto.largura][labirinto.altura] = {0};
    labirinto.visitado[labirinto.largura][labirinto.altura] = {false};
}

void Verifica(Labirinto &labirinto, Inimigo &inimigo, int valor, int dificuldade, int i, int j)
{
    char vazio = 255;
    char parede = 178;
    char buraco = 254;
    char premio = 005;

    int corParede = 4;
    int corBuraco = 8;
    int corPremio = 2;

    int aux;

    if(dificuldade == 0)
    {
        if(valor >= 0 && valor <= 4 && labirinto.caminho[j][i] == 0)
        {
            labirinto.mapa[j][i] = vazio;
            labirinto.caminho[j][i] = 0;
        }
        else if(valor > 4 && valor <= 6 && labirinto.caminho[j][i] == 0)
        {
            textcolor(corParede);
            labirinto.mapa[j][i] = parede;
            labirinto.caminho[j][i] = 1;
        }
        else if(valor > 6 && valor <= 8 && labirinto.caminho[j][i] == 0)
        {
            textcolor(corPremio);
            labirinto.mapa[j][i] = premio;
            labirinto.caminho[j][i] = 2;
            labirinto.premios += 10;
        }
    }
    else if(dificuldade == 1)
    {
        aux = rand() % 2 + 1;
        if(valor >= 0 && valor < 2)
        {
            labirinto.mapa[j][i] = vazio;
            labirinto.caminho[j][i] = 0;
        }
        else if(valor >= 2 && valor < 4)
        {
            textcolor(corParede);
            labirinto.mapa[j][i] = parede;
            labirinto.caminho[j][i] = 1;
        }
        else if(valor >= 4 && valor < 7)
        {
            textcolor(corPremio);
            labirinto.mapa[j][i] = premio;
            labirinto.caminho[j][i] = 2;
            labirinto.premios += 10;
        }
        else if(valor == 7 && aux == 1)
        {
            textcolor(corBuraco);
            labirinto.mapa[j][i] = buraco;
            labirinto.caminho[j][i] = 3;
        }
    }
    else if(dificuldade == 2)
    {
        aux = rand() % 4;
        if(valor >= 0 && valor < 4)
        {
            labirinto.mapa[j][i] = vazio;
            labirinto.caminho[j][i] = 0;
        }
        else if(valor >= 4 && valor < 8)
        {
            textcolor(corParede);
            labirinto.mapa[j][i] = parede;
            labirinto.caminho[j][i] = 1;
        }
        else if(valor >= 8 && valor < 10)
        {
            textcolor(corPremio);
            labirinto.mapa[j][i] = premio;
            labirinto.caminho[j][i] = 2;
            labirinto.premios += 10;
        }
        else if(valor >= 10 && aux < 2)
        {
            textcolor(corBuraco);
            labirinto.mapa[j][i] = buraco;
            labirinto.caminho[j][i] = 3;
        }
    }
    else if(dificuldade == 3)
    {
        aux = rand() % 2;
        if(valor >= 0 && valor < 2)
        {
            labirinto.mapa[j][i] = vazio;
            labirinto.caminho[j][i] = 0;
        }
        else if(valor >= 2 && valor < 4)
        {
            textcolor(corParede);
            labirinto.mapa[j][i] = parede;
            labirinto.caminho[j][i] = 1;
        }
        else if(valor == 4)
        {
            textcolor(corPremio);
            labirinto.mapa[j][i] = premio;
            labirinto.caminho[j][i] = 2;
            labirinto.premios += 10;
        }
        else if(valor == 5 && aux == 1)
        {
            textcolor(corBuraco);
            labirinto.mapa[j][i] = buraco;
            labirinto.caminho[j][i] = 3;
        }
    }
    else if(dificuldade == 4)
    {
        if(valor == 0)
        {
            labirinto.mapa[j][i] = vazio;
            labirinto.caminho[j][i] = 0;
        }
        else if(valor == 1)
        {
            textcolor(BLACK);
            labirinto.mapa[j][i] = parede;
            labirinto.caminho[j][i] = 1;
        }
        else if(valor == 2)
        {
            textcolor(corPremio);
            labirinto.mapa[j][i] = premio;
            labirinto.caminho[j][i] = 2;
            labirinto.premios += 10;
        }
        else if(valor == 3)
        {
            textcolor(corBuraco);
            labirinto.mapa[j][i] = buraco;
            labirinto.caminho[j][i] = 3;
        }
    }

    inimigo.caminho[j][i] = labirinto.caminho[j][i];

}

void OpcoesMenu(Configuracao cfg, int qnt)
{
    gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + qnt);
    textcolor(RED);
    cout << "[ESC]";
    textcolor(DARKGRAY);
    cout << " Voltar/Sair";
    gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + qnt + 1);
    textcolor(RED);
    cout << "[SPACE][ENTER]";
    textcolor(DARKGRAY);
    cout << " Acessar";
    gotoxy(cfg.posicao_menu[0], cfg.posicao_menu[1] + qnt + 2);
    textcolor(RED);
    cout << "[W][S]";
    textcolor(DARKGRAY);
    cout << " Navegar";
}

char PegaTecla(char &tecla)
{
    clearkeybuf();
    return tecla = tolower(_getch());
}

void GerarFrescura(char *nome)
{
    char letra;
    int color;

    for(int i = 1; i < 6; i++)
        for(int j = 1; j < 80; j++)
        {
            letra = rand() % 254 + 8;
            color = rand() % 7;
            gotoxy(j, i);
            textcolor(color);
            textbackground(color-1);
            cout << letra;
        }

    textbackground(7);
    textcolor(4);
    gotoxy(26, 3);
    cout << nome;

    textbackground(0);
    textcolor(8);
}

int Random(int dificuldade)
{
    int nivel;

    if(dificuldade == 0)
    {
        nivel = 10;
    }
    else if(dificuldade == 1)
    {
        nivel = 11;
    }
    else if(dificuldade == 2)
    {
        nivel = 13;
    }
    else if(dificuldade == 3)
    {
        nivel = 9;
    }
    else if(dificuldade == 4)
    {
        nivel = 4;
    }

    return rand() % nivel;
}
