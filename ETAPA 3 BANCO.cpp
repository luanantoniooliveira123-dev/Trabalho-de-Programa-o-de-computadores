// Disciplina: INF101 - Programacao de Computadores I
// Professores: Anderson R. Lamas e Vanderlea Queiroz
// Aluno: Luan Antonio De Oliveira Silva | Matricula: 28286
// Trabalho Pratico - Etapa 3: Registro e Gestao de Contas Bancarias (com arquivos)
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <iomanip>
using namespace std;

const int MAX_CONTAS = 100, MAX_MOVIMENTACOES = 500;
int numeroConta[MAX_CONTAS], tipoConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS], cpf[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;

int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];   // D = deposito | S = saque
double valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;

// ---------- Arquivos (Etapa 3) ----------
void salvarContas() {
    ofstream arquivo("contas.txt");
    if (!arquivo.is_open()) { cout << "[!] Erro ao salvar contas.txt!\n"; return; }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++)
        arquivo << numeroConta[i] << ";" << nomeCliente[i] << ";" << cpf[i] << ";"
                << tipoConta[i] << ";" << saldo[i] << ";" << contaAtiva[i] << endl;
    arquivo.close();
}
void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) { cout << "[!] Erro ao salvar movimentacoes.txt!\n"; return; }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++)
        arquivo << contaMovimentacao[i] << ";" << tipoMovimentacao[i] << ";" << valorMovimentacao[i] << endl;
    arquivo.close();
}
void carregarContas() {   // se o arquivo nao existir, comeca vazio
    ifstream arquivo("contas.txt");
    string linha, c;
    while (arquivo.is_open() && quantidadeContas < MAX_CONTAS && getline(arquivo, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        int i = quantidadeContas;
        getline(ss, c, ';'); numeroConta[i] = atoi(c.c_str());
        getline(ss, nomeCliente[i], ';');
        getline(ss, cpf[i], ';');
        getline(ss, c, ';'); tipoConta[i] = atoi(c.c_str());
        getline(ss, c, ';'); saldo[i] = atof(c.c_str());
        getline(ss, c, ';'); contaAtiva[i] = (atoi(c.c_str()) == 1);
        if (numeroConta[i] > 0 && (tipoConta[i] == 1 || tipoConta[i] == 2)) quantidadeContas++;   // ignora linha invalida
    }
}
void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    string linha, c;
    while (arquivo.is_open() && quantidadeMovimentacoes < MAX_MOVIMENTACOES && getline(arquivo, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        int i = quantidadeMovimentacoes;
        getline(ss, c, ';'); contaMovimentacao[i] = atoi(c.c_str());
        getline(ss, c, ';'); tipoMovimentacao[i] = c[0];
        getline(ss, c, ';'); valorMovimentacao[i] = atof(c.c_str());
        if (contaMovimentacao[i] > 0 && (tipoMovimentacao[i] == 'D' || tipoMovimentacao[i] == 'S')) quantidadeMovimentacoes++;
    }
}

// ---------- Utilitarios ----------
// Le um numero sem travar se o usuario digitar letras
double lerNum(string msg) {
    double v;
    cout << msg;
    while (!(cin >> v)) {
        if (cin.eof()) { cout << "\nEntrada encerrada. Saindo...\n"; exit(0); }   // evita laco infinito
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Entrada invalida! " << msg;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return v;
}
int buscarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++)
        if (numeroConta[i] == numero) return i;
    return -1;
}
// Pede o numero da conta; retorna o indice ou -1 (nao existe / inativa, se exigida)
int pedirConta(string msg, bool exigeAtiva) {
    int i = buscarConta((int)lerNum(msg));
    if (i == -1) cout << "[!] Conta nao encontrada!\n";
    else if (exigeAtiva && !contaAtiva[i]) { cout << "[!] Conta INATIVA!\n"; return -1; }
    return i;
}
// Valida o valor e o espaco para n novas movimentacoes
bool valorValido(double v, int n) {
    if (v <= 0) { cout << "[!] O valor deve ser maior que zero!\n"; return false; }
    if (quantidadeMovimentacoes + n > MAX_MOVIMENTACOES) { cout << "[!] Limite de movimentacoes atingido!\n"; return false; }
    return true;
}
void registrarMovimentacao(int conta, char tipo, double valor) {
    contaMovimentacao[quantidadeMovimentacoes] = conta;
    tipoMovimentacao[quantidadeMovimentacoes] = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;
    quantidadeMovimentacoes++;
}
void salvarTudo() { salvarContas(); salvarMovimentacoes(); }

// ---------- Operacoes ----------
void cadastrarConta() {
    if (quantidadeContas >= MAX_CONTAS) { cout << "[!] Limite de contas atingido!\n"; return; }
    int i = quantidadeContas, num;
    do {
        num = (int)lerNum("Numero da conta (maior que 0): ");
        if (num <= 0) cout << "[!] O numero deve ser maior que 0!\n";
        else if (buscarConta(num) != -1) cout << "[!] Ja existe uma conta com esse numero.\n";
    } while (num <= 0 || buscarConta(num) != -1);
    numeroConta[i] = num;
    cout << "Nome do cliente: "; getline(cin, nomeCliente[i]);
    cout << "CPF do cliente: ";  getline(cin, cpf[i]);
    do tipoConta[i] = (int)lerNum("Tipo (1 - Corrente | 2 - Poupanca): ");
    while (tipoConta[i] != 1 && tipoConta[i] != 2);
    do {
        saldo[i] = lerNum("Saldo inicial (R$): ");
        if (saldo[i] < 0) cout << "[!] O saldo nao pode ser negativo!\n";
    } while (saldo[i] < 0);
    contaAtiva[i] = true;
    quantidadeContas++;
    salvarContas();
    cout << "[V] Conta cadastrada com sucesso!\n";
}
void mostrarConta(int i) {
    cout << "--------------------------------------\n"
         << "Numero: " << numeroConta[i] << " | Titular: " << nomeCliente[i] << " | CPF: " << cpf[i] << "\n"
         << "Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca") << " | Saldo: R$ " << saldo[i]
         << " | " << (contaAtiva[i] ? "Ativa" : "Inativa") << endl;
}
void listarContas() {
    if (quantidadeContas == 0) cout << "[!] Nenhuma conta cadastrada.\n";
    for (int i = 0; i < quantidadeContas; i++) mostrarConta(i);
}
void consultarConta() {
    int i = pedirConta("Numero da conta: ", false);
    if (i != -1) mostrarConta(i);
}
// Deposito (dep = true) ou saque (dep = false)
void movimentar(bool dep) {
    int i = pedirConta("Numero da conta: ", true);
    if (i == -1) return;
    double valor = lerNum(dep ? "Valor do deposito (R$): " : "Valor do saque (R$): ");
    if (!valorValido(valor, 1)) return;
    if (!dep && saldo[i] < valor) { cout << "[!] Saldo insuficiente!\n"; return; }
    saldo[i] += dep ? valor : -valor;
    registrarMovimentacao(numeroConta[i], dep ? 'D' : 'S', valor);
    salvarTudo();
    cout << "[V] " << (dep ? "Deposito" : "Saque") << " realizado! Novo saldo: R$ " << saldo[i] << endl;
}
void transferir() {
    int o = pedirConta("Conta de ORIGEM: ", true);
    if (o == -1) return;
    int d = pedirConta("Conta de DESTINO: ", true);
    if (d == -1) return;
    if (o == d) { cout << "[!] Origem e destino nao podem ser a mesma conta!\n"; return; }
    double valor = lerNum("Valor da transferencia (R$): ");
    if (!valorValido(valor, 2)) return;   // espaco para os dois registros
    if (saldo[o] < valor) { cout << "[!] Saldo insuficiente na origem!\n"; return; }
    saldo[o] -= valor;
    saldo[d] += valor;
    registrarMovimentacao(numeroConta[o], 'S', valor);
    registrarMovimentacao(numeroConta[d], 'D', valor);
    salvarTudo();
    cout << "[V] Transferencia realizada!\nSaldo conta " << numeroConta[o] << ": R$ " << saldo[o]
         << "\nSaldo conta " << numeroConta[d] << ": R$ " << saldo[d] << endl;
}
void exibirExtrato() {
    int i = pedirConta("Numero da conta: ", false);
    if (i == -1) return;
    cout << "===== EXTRATO - Conta " << numeroConta[i] << " | " << nomeCliente[i] << " =====\n";
    bool achou = false;
    for (int j = 0; j < quantidadeMovimentacoes; j++)
        if (contaMovimentacao[j] == numeroConta[i]) {
            achou = true;
            cout << (tipoMovimentacao[j] == 'D' ? "Deposito: R$ " : "Saque: R$ ") << valorMovimentacao[j] << endl;
        }
    if (!achou) cout << "Nenhuma movimentacao registrada.\n";
    cout << "Saldo atual: R$ " << saldo[i] << endl;
}

int main() {
    cout << fixed << setprecision(2);
    carregarContas();
    carregarMovimentacoes();
    int opcao;
    do {
        cout << "\n======== BANCO.0.3 ========\n"
             << "1 - Cadastrar conta\n2 - Listar contas\n3 - Consultar conta\n4 - Depositar\n"
             << "5 - Sacar\n6 - Transferir\n7 - Extrato\n8 - Sair\n";
        opcao = (int)lerNum("Escolha uma opcao: ");
        switch (opcao) {
            case 1: cadastrarConta(); break;
            case 2: listarContas(); break;
            case 3: consultarConta(); break;
            case 4: movimentar(true); break;
            case 5: movimentar(false); break;
            case 6: transferir(); break;
            case 7: exibirExtrato(); break;
            case 8: cout << "Saindo... Ate logo!\n"; break;
            default: cout << "[!] Opcao invalida!\n";
        }
    } while (opcao != 8);
    return 0;
}