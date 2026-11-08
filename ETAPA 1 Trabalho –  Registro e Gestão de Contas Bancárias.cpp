// ============================================================================
// Disciplina: INF101 - Programacao de Computadores I.
// Professor: Anderson R. Lamas.
// Aluno: [ Luan Antonio De Oliveira Silva. ] Matricula [ 28286. ]
// Trabalho Pratico - Etapa 1: Registro e Gestão de Contas Bancarias.
// ============================================================================
    #include <iostream>
    #include <string>
    // Inclui a biblioteca limits para usar numeric_limits.//
    #include <limits>
    using namespace std;
//numero maximo de contas que podem ser cadastradas.//
const int MAX_CONTAS = 5;
//variaveis globais para armazenar os dados das contas.//
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS], cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int totalContas = 0;

// Le um inteiro (repete ate ser valido).//
int lerInt(string msg, bool (*valido)(int), string erro) {
    int v;
    do {
        cout << msg;
        cin >> v;
        if (!valido(v)) cout << "[!] " << erro << endl;
    } while (!valido(v));
    return v;
}
// Funcoes de validacao.//
bool maiorQueZero(int v) { return v > 0; }
bool tipoValido(int v) { return v == 1 || v == 2; }
bool binario(int v) { return v == 0 || v == 1; }
// Busca o indice da conta pelo numero. Retorna -1 se nao encontrado.//
int buscarConta(int numero) {
    for (int i = 0; i < totalContas; i++)
        if (numeroConta[i] == numero) return i;
    return -1;
}
// Converte tipo e status para string.//
string strTipo(int t) { return t == 1 ? "Corrente" : "Poupanca"; }
string strStatus(bool a) { return a ? "Ativa" : "Inativa"; }
// Pede o numero da conta e retorna o indice correspondente. Retorna -1 se nao encontrado.//
int pedirIndiceExistente() {
        int num = lerInt("Digite o numero da conta: ", [](int v){ return true; }, "");
    int i = buscarConta(num);
    if (i == -1) cout << "\n[!] Conta nao encontrada!" << endl;
    return i;
}
// Funcoes de operacoes bancarias.//
void cadastrarConta() {
    if (totalContas >= MAX_CONTAS) {
        cout << "\n[!] Nao e possivel cadastrar mais contas.\nLimite de 5 contas atingido!" << endl;
        return;
    }
    int i = totalContas;
    cout << "\n--- CADASTRO DE CONTA ---" << endl;
    numeroConta[i] = lerInt("Digite o numero da conta (maior que 0): ", maiorQueZero, "O numero deve ser maior que 0!");
// Limpa o buffer do cin antes de ler strings.//
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nome do cliente: "; getline(cin, nomeCliente[i]);
    cout << "CPF do cliente: ";  getline(cin, cpf[i]);
// Lê o tipo da conta (1 ou 2).//
    tipoConta[i] = lerInt("Tipo de conta (1 - Corrente | 2 - Poupanca): ", tipoValido, "Digite apenas 1 ou 2!");
// Lê o saldo inicial (não negativo).//
    double s;
    do {
        cout << "Saldo inicial (R$): ";
        cin >> s;
        if (s < 0) cout << "[!] O saldo nao pode ser negativo!" << endl;
    } while (s < 0);
    saldo[i] = s;
// Define a conta como ativa e incrementa o total de contas.//
    contaAtiva[i] = true;
    totalContas++;
    cout << "\n[V] Conta cadastrada com sucesso!" << endl;
}
// Funcoes de consulta e alteracao de dados.//
void consultarConta() {
    if (totalContas == 0) { cout << "\n[!] Nenhuma conta cadastrada." << endl; return; }
    cout << "\n--- CONSULTA DE CONTA ---" << endl;
    int i = pedirIndiceExistente();
    if (i == -1) return;
    cout << "\n[ DADOS DA CONTA ]\n"
    // Exibe os dados da conta.//
         << "Numero: " << numeroConta[i] << "\n"
         << "Titular: " << nomeCliente[i] << "\n"
         << "CPF: " << cpf[i] << "\n"
         << "Tipo: " << strTipo(tipoConta[i]) << "\n"
         << "Status: " << strStatus(contaAtiva[i]) << endl;
}
// Funcoes de verificacao e alteracao de saldo, tipo e status.//
void verificarSaldo() {
    if (totalContas == 0) { cout << "\n[!] Nenhuma conta cadastrada." << endl; return; }
    cout << "\n--- CONSULTAR SALDO ---" << endl;
    int i = pedirIndiceExistente();
    if (i == -1) return;
    if (!contaAtiva[i]) { cout << "\n[!] Operacao nao permitida!\nA conta esta INATIVA." << endl; return; }
    cout << "\nSaldo da conta " << numeroConta[i] << ": R$ " << saldo[i] << endl;
}
// Funcoes de alteracao de tipo e status da conta.//
void alterarTipo() {
    if (totalContas == 0) { cout << "\n[!] Nenhuma conta cadastrada." << endl; return; }
    cout << "\n--- ALTERAR TIPO DE CONTA ---" << endl;
    int i = pedirIndiceExistente();
    if (i == -1) return;
    // Verifica se a conta está ativa antes de permitir a alteração do tipo.//
    if (!contaAtiva[i]) { cout << "\n[!] Operacao nao permitida!\nA conta esta INATIVA." << endl; return; }
    cout << "Tipo atual: " << strTipo(tipoConta[i]) << endl;
    tipoConta[i] = lerInt("Novo tipo (1 - Corrente | 2 - Poupanca): ", tipoValido, "Digite apenas 1 ou 2!");
    cout << "\n[V] Tipo de conta alterado com sucesso!" << endl;
}
// Função para alterar o status da conta d ativa para desativa.//
void alterarStatus() {
    if (totalContas == 0) { cout << "\n[!] Nenhuma conta cadastrada." << endl; return; }
    cout << "\n--- GERENCIAR STATUS DA CONTA ---" << endl;
    int i = pedirIndiceExistente();
    if (i == -1) return;
    cout << "Status atual: " << strStatus(contaAtiva[i]) << "\n1 - Ativar\n0 - Desativar" << endl;
    int esc = lerInt("Escolha: ", binario, "Opcao invalida! Digite 0 ou 1.");
    contaAtiva[i] = (esc == 1);
    cout << "\n[V] Conta " << (esc == 1 ? "ativada" : "desativada") << " com sucesso!" << endl;
}
// Funcao principal com menu interativo.//
int main() {
    int opcao;
    do {
        cout << "\n======================================\n"
             << "                BANCO.0.1               \n"
             << "======================================\n"
             << "1 - Cadastrar conta\n2 - Consultar conta\n3 - Verificar saldo\n"
             << "4 - Alterar tipo da conta\n5 - Ativar/Desativar conta\n6 - Sair\n"
             << "======================================\nEscolha uma opcao: ";
        cin >> opcao;
// Valida a entrada do usuário para garantir que seja um número inteiro.//
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            continue;
            cout << "\n[!] Entrada invalida! Digite um numero de 1 a 6." << endl;
;
        }// Executa a operação correspondente à opção escolhida.//

        switch (opcao) {
            // Chama a função correspondente à opção escolhida.//
            case 1: cadastrarConta(); break;
            case 2: consultarConta(); break;
            case 3: verificarSaldo(); break;
            case 4: alterarTipo(); break;
            case 5: alterarStatus(); break;
            case 6: cout << "\nSaindo do BANCO.0.1 ...Ate logo!" << endl; break;
            default: cout << "\n[!] Opcao invalida!\nEscolha uma opcao de 1 a 6." << endl; break;
        }
        // Limpa o buffer do cin para evitar problemas na próxima leitura.//
    } while (opcao != 6);
    //Finaliza o programa com sucesso.//
    return 0;
}
