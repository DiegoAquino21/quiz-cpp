#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <windows.h>
#include <conio.h>
#include <chrono>


using namespace std;

struct Pergunta
{
	string pergunta;
	string respostas[4];
	int correta;
};


	// Função 1

vector<Pergunta> carregarPerguntas(string nomeFicheiro)
{
	vector<Pergunta> perguntas;
	Pergunta p;

	ifstream ficheiro(nomeFicheiro);

	if (!ficheiro) 
	{
		cout << "Erro ao abrir o ficheiro: " << nomeFicheiro << endl;
		return perguntas;
	}

	while (getline(ficheiro, p.pergunta))
	{
		getline(ficheiro, p.respostas[0]);
		getline(ficheiro, p.respostas[1]);
		getline(ficheiro, p.respostas[2]);
		getline(ficheiro, p.respostas[3]);

		ficheiro >> p.correta;
		ficheiro.ignore();

		perguntas.push_back(p);
	}

	ficheiro.close();

	return perguntas;
}


	// Função 2

vector<Pergunta> selecionarPerguntas(vector<Pergunta> perguntas, int quantidade)
{
	vector<Pergunta> selecionadas;

	// Evita escolher mais perguntas do que existem no ficheiro
	if (quantidade > perguntas.size())
	{
		quantidade = perguntas.size();
	}

	random_device rd;
	mt19937 g(rd());

	shuffle(perguntas.begin(), perguntas.end(), g);

	for (int i = 0; i < quantidade; i++)
	{
		selecionadas.push_back(perguntas[i]);
	}

	return selecionadas;
}

	// Passo 1 Mostrar perguntas

void mostrarPergunta(Pergunta p)
{
	cout << "____________________________________" << endl;
	cout << p.pergunta << endl << endl;

	for (int i = 0; i < 4; i++)
	{
		cout << i + 1 << " - " << p.respostas[i] << endl;
	}

	cout << "____________________________________" << endl;
}



// Função de tempo limite para cada pergunta

int lerRespostaComTempo(int segundos)
{
	auto inicio = chrono::steady_clock::now();

	while (true)
	{
		// Verificar se o utilizador pressinou alguma tecla

		if (_kbhit())
		{
			char tecla = _getch();

			// Só aceita respostas de 1 a 4

			if (tecla >= '1' && tecla <= '4')
			{
				cout << tecla << endl;
				return tecla - '0';
			}
		}

		auto agora = chrono::steady_clock::now();

		int tempoPassado =
			chrono::duration_cast<chrono::seconds>(agora - inicio).count();

		if (tempoPassado >= segundos)
		{
			cout << "\nTempo esgotado!" << endl;
			return 0;
		}

		Sleep(100);
	}
}

	// Jogar o QUIZ

	int jogarQuiz(vector<Pergunta>perguntas)
	{

		int pontos = 0;
		int resposta;
		int certas = 0;

		for (int i = 0; i < perguntas.size(); i++)
		{
			cout << "\nPergunta " << i + 1 << " de "
				<< perguntas.size() << endl << endl;

			mostrarPergunta(perguntas[i]);

			cout << "\nTem 20 segundos para responder. " << endl;
			cout << "Resposta:";

			resposta = lerRespostaComTempo(20);

			if (resposta == 0)
			{
				cout << "Pergunta não respondida." << endl;
			}

			else if (resposta == perguntas[i].correta)
			{
				cout << "\n✅ Resposta correta!" << endl;
				pontos += 10;
				certas++;
			}
			else
			{
				cout << "\n❌ Resposta errada!" << endl;

				cout << "Resposta correta: "
					<< perguntas[i].correta << endl;
			}

			cout << "\nPontuação: "
				<< pontos << " pontos" << endl;

			cout << "\n-------------------------------\n";
		}

		
		double percentagem;

		percentagem = (certas * 100.0) / perguntas.size();

		cout << "\nPercentagem de respostas certas: "
			<< percentagem << "%" << endl;

		return pontos;
	}
		
	

	// Guardas as pontuações

void guardarPontuacao(int pontos) 
{
	ofstream ficheiro("pontuacoes.txt", ios::app);

	if (ficheiro)
	{
		ficheiro << pontos << endl;
		ficheiro.close();
	}
	else
	{
		cout << "Erro ao guardar a pontuação." << endl;
	}
}

		



int main() 
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	

	

	//cout << "Número de perguntas carregadas: "
		//<< perguntas.size() << endl;

	//cout << "Perguntas selecionadas: "
		//<< perguntasSelecionadas.size() << endl << endl;

	int opcao;

	cout << "\t==================================" << endl;
	cout << "\t\tJOGO DE QUIZ 		   " << endl;
	cout << "\t==================================" << endl;

	cout << "\nEscolha o nivel de difculdade: \n" << endl;
	cout << "1 - Iniciante" << endl;
	cout << "2 - Intermedio" << endl;
	cout << "3 - Avançado" << endl;
	cout << "0 - Sair" << endl;

	cout << "\nOpção: ";
	cin >> opcao;

	switch (opcao)
	{
	case 1:
	{
		cout << "\nNível Iniciante selecionado." << endl;

		vector<Pergunta>perguntas =
			carregarPerguntas("Iniciante.txt");

		vector<Pergunta>perguntasSelecionadas =
			selecionarPerguntas(perguntas, 20);

		if (!perguntasSelecionadas.empty())
		{
			int pontos = jogarQuiz(perguntasSelecionadas);

			guardarPontuacao(pontos);

			cout << "\n====================================" << endl;
			cout << "Fim do nível Iniciante!" << endl;
			cout << "Pontuação final: "
				<< pontos << " ponto" << endl;
			cout << "\n====================================" << endl;
		}
		break;
	}

	case 2:
	{
		cout << "\nNível Intermedio selecionado." << endl;

		vector<Pergunta> perguntas =
			carregarPerguntas("Intermedio.txt");

		vector<Pergunta> perguntasSelecionadas =
			selecionarPerguntas(perguntas, 20);

		if (!perguntasSelecionadas.empty())
		{
			int pontos = jogarQuiz(perguntasSelecionadas);

			guardarPontuacao(pontos);

			cout << "\n====================================" << endl;
			cout << "Fim do nível Intemédio!" << endl;
			cout << "Pontuação final: "
				<< pontos << " ponto" << endl;
			cout << "\n====================================" << endl;

		}
		break;
	}

	case 3:
		cout << "\nNivel Avançado selecionado." << endl;
		break;

	case 0:
		cout << "\nPrograma terminado!" << endl;
		break;

	default:
		cout << "\nOpção Invalida!" << endl;

	}

	return 0;

}