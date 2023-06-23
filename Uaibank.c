#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
	int id;
	char nome[100];
	int idade;

	float saldo;

} Usuario;

typedef struct
{
	Usuario *clientes;
	int unsigned size;
	int unsigned gen_id;
} Banco;

int GetIndexDoUsuarioComId(Banco *b, int id)
{
	int i = 0;
	int indice = -1;
	for (i = 0; i < (b->size - 1); i++)
	{
		if (b->clientes[i].id == id)
		{
			indice = i;
			break;
		}
	}

	return indice;
}

void MostrarUsuario(Banco *b, int id)
{
	int i = GetIndexDoUsuarioComId(b, id);
	Usuario u = b->clientes[i];

	if (u.id != 0)
	{
		printf("ID do Usuário: %d\nNome: %s\nIdade: %d\nSaldo: %.2f\n", u.id, u.nome, u.idade, u.saldo);
	}
	else
	{
		printf("ERRO: Usuário %d não encontrado.", id);
	}
}

void AdicionarUsuario(Banco *b, Usuario u)
{
	// Antes do cadastro a sequência de id's deve ser incrementada para garantir a unicidade
	b->gen_id += 1;
	u.id = b->gen_id;
	printf("Usuário cadastrado com sucesso. ID de Usuário: %d\n", u.id);

	// Para adicionar um novo usuário o sistema irá abrir o arquivo de banco de dados e adicionar o novo usuário no final do arquivo
	// portanto a flag utilziada será ab
	FILE *db = fopen("DATABASE.bin", "ab");

	b->clientes[(b->size - 1)] = u;
	b->clientes = (Usuario *)realloc(b->clientes, (b->size + 1) * sizeof(Usuario));

	// Deu erro na alocação de memória
	if (b->clientes == NULL)
	{
		b->size = 0;
	}
	else
	{
		b->size += 1;
		// fwrite(db, "%d, %s, %d, %.2f\n", u.id, u.nome, u.idade, u.saldo);
		fwrite(&u, sizeof(Usuario), 1, db);
	}

	fclose(db);
}

void DeletarUsuario(Banco *b, int id)
{
	int i = 0;
	int indice = GetIndexDoUsuarioComId(b, id);

	if (indice > -1)
	{

		int j;
		for (j = (indice + 1); j < (b->size - 1); j++)
		{
			b->clientes[j - 1] = b->clientes[j];
		}

		b->size -= 1;
		b->clientes = (Usuario *)realloc(b->clientes, (b->size + 1) * sizeof(Usuario));
		printf("Usuário %d excluído com sucesso.\n", id);

		// Para o caso de exclusão de um usuário o sistema irá recriar o arquivo de banco de dados e adicionar todos os usuários novamente
		// portanto a flag utilizada será wb
		FILE *db = fopen("DATABASE.bin", "wb");
		for (i = 0; i < (b->size - 1); i++)
		{
			fwrite(&b->clientes[i], sizeof(Usuario), 1, db);
		}
		fclose(db);
	}
	else
	{
		printf("ERRO: Usuário %d não encontrado.\n", id);
	}
}

void Transferencia(Banco *b, int id_solicitante, int id_beneficiario, float quantidade)
{
	int i = GetIndexDoUsuarioComId(b, id_solicitante);
	int j = GetIndexDoUsuarioComId(b, id_beneficiario);

	Usuario *solicitante = &(b->clientes[i]);
	Usuario *beneficiario = &(b->clientes[j]);

	if (solicitante->saldo >= quantidade)
	{
		beneficiario->saldo += quantidade;
		solicitante->saldo -= quantidade;
		printf("Transferência realizada com sucesso.\n");

		// Para o caso de transferencia bancário, o sistema irá recriar o arquivo de banco de dados e adicionar todos os usuários novamente
		// portanto a flag utilizada será wb
		FILE *db = fopen("DATABASE.bin", "wb");
		for (i = 0; i < (b->size - 1); i++)
		{
			fwrite(&b->clientes[i], sizeof(Usuario), 1, db);
		}
		fclose(db);
	}
	else
	{
		printf("ERRO: Usuário %d não tem saldo suficiente para transferir.\n", solicitante->id);
	}
}

void ExportarDados(Banco *b)
{
	// A exportação de dados deve ser feita por meio de um arquivo de texto, neste arquivo deve conter o nome, idade e saldo de cada usuário
	// a id não será mostrada ao usuário final
	FILE *file = fopen("UAIBANK.txt", "w");

	int i = 0;
	for (i = 0; i < (b->size - 1); i++)
	{
		fprintf(file, "%d, %s, %d, %.2f\n", b->clientes[i].id, b->clientes[i].nome, b->clientes[i].idade, b->clientes[i].saldo);
	}

	printf("Dados exportados com sucesso.\n");

	fclose(file);
}

int main()
{

	Banco Uaibank;

	printf("\t\t _     _       _ _                 _     \n");
	printf("\t\t| |   | |     (_) |               | |    \n");
	printf("\t\t| |   | | ____ _| | _   ____ ____ | |  _ \n");
	printf("\t\t| |   | |/ _  | | || \\ / _  |  _ \\| | / )\n");
	printf("\t\t| |___| ( ( | | | |_) | ( | | | | | |< ( \n");
	printf("\t\t \\______|\\_||_|_|____/ \\_||_|_| |_|_| \\_)\n");

	printf("\nBem vindo ao UaiBank !!\n\n");

	// Inicia o Uaibank sem nenhum cliente
	Uaibank.clientes = (Usuario *)malloc(1 * sizeof(Usuario));
	Uaibank.size = 1;
	Uaibank.gen_id = 0;

	// O projeto contará com uma pequena base de dados que será feita em um arquivo binário, para evitar que o usuário possa alterar diretamente os dados
	// a flag WB+ cria um arquivo binário para escrita e leitura
	// Como padrão, este arquivo está no formato: id, nome, idade, saldo
	FILE *db = fopen("DATABASE.bin", "rb");
	// A primeira vez que o programa é executado o arquivo DATABASE pode não exisir.
	// Contudo, caso exista, o programa irá carregar os dados dos usuários para a memória
	if (db != NULL)
	{
		//  Lê o arquivo de texto e carrega os dados para a memória
		Usuario r;
		while (fread(&r, sizeof(Usuario), 1, db) == 1)
		{
			Uaibank.clientes = (Usuario *)realloc(Uaibank.clientes, (Uaibank.size + 1) * sizeof(Usuario));
			Uaibank.clientes[Uaibank.size - 1] = r;
			Uaibank.size += 1;
			// Para garantir a unicidade das IDs, o sistema irá utilizar o valor salvo no banco de dados
			// sem tratar possíveis gaps entre os números
			Uaibank.gen_id = r.id;
		}

		fclose(db);
	}

	int id_solicitante = 0;
	int id_beneficiario = 0;
	float quantidade = 0.0;

	int isInputOK = 0;

	int opcao = 0;
	int subOpcao = 0;

	int id = 0;

	int i = 0;
	int n = 0;

	Usuario u;

	char feedback[100];
	int index = 0;

	// Imprime o menu do opções para o usuário
	do
	{
		printf("\nEm que podemos ajudá-lo hoje ?\n\n");
		printf("\t - Digite 1 para cadastrar um usuário\n");
		printf("\t - Digite 2 para cadastrar vários usuários\n");
		printf("\t - Digite 3 para buscar por um usuário\n");
		printf("\t - Digite 4 para excluir um usuário\n");
		printf("\t - Digite 5 para executar uma transferência entre dois clientes\n");
		printf("\t - Digite 6 para exportar a base de dados para um arquivo\n");
		printf("\t - Digite 0 para sair\n");

		scanf("%d", &opcao);

		// Como o primeiro parâmetro para adicionar um cliente é uma string, é preciso limpar o buffer do teclado
		// caso contrário o \n será inserido no começo do nome do usuário
		getchar();

		switch (opcao)
		{
		case 1:

			do
			{
				isInputOK = 1;
				printf("Digite o nome completo, a idade e o saldo do usuário para cadastrar separados por vírgulas\n");
				u.id = 0;
				u.idade = 0;
				u.saldo = 0.f;

				scanf("%[^,], %d, %f", u.nome, &u.idade, &u.saldo);

				if (u.idade < 18)
				{
					printf("ERRO: Usuário %s não pode ser cadastrado, pois é menor de idade.\n", u.nome);
					isInputOK = -1;
				}

				else if (u.saldo < 0)
				{
					printf("ERRO: Usuário %s não pode ser cadastrado, pois tem saldo negativo.\n", u.nome);
					isInputOK = -1;
				}

				if (isInputOK < 0)
				{
					printf("\nSe deseja cadastrar um novo usuário, digite 1.\nCaso deseje voltar ao menu anterior digite 0\n");
					scanf("%d", &subOpcao);

					// Como o primeiro parâmetro para adicionar um cliente é uma string, é preciso limpar o buffer do teclado
					// caso contrário o \n será inserido no começo do nome do usuário
					getchar();
					if (subOpcao == 0)
					{
						break;
					}
					printf("\n\n");
				}

				else if (isInputOK == 1)
				{
					index = Uaibank.size;
					AdicionarUsuario(&Uaibank, u);
				}
			} while (isInputOK < 0);
			break;
		case 2:
			i = 0;
			n = 0;

			printf("Qual o número de usuário que serão cadastrados ? ");
			scanf("%d", &n);
			// Como o primeiro parâmetro para adicionar um cliente é uma string, é preciso limpar o buffer do teclado
			// caso contrário o \n será inserido no começo do nome do usuário
			getchar();
			while (i < n)
			{
				do
				{
					isInputOK = 1;
					printf("Digite o nome completo, a idade e o saldo do usuário para cadastrar\n");
					u.id = 0;
					u.idade = 0;
					u.saldo = 0.f;

					scanf("%[^,], %d, %f", u.nome, &u.idade, &u.saldo);

					if (u.idade < 18)
					{
						printf("ERRO: Usuário %s não pode ser cadastrado, pois é menor de idade.\n", u.nome);
						isInputOK = -1;
					}

					else if (u.saldo < 0)
					{
						printf("ERRO: Usuário %s não pode ser cadastrado, pois tem saldo negativo.\n", u.nome);
						isInputOK = -1;
					}

					if (isInputOK < 0)
					{
						printf("\nSe deseja cadastrar um novo usuário, digite 1.\nCaso deseje voltar ao menu anterior digite 0\n");
						scanf("%d", &subOpcao);

						// Como o primeiro parâmetro para adicionar um cliente é uma string, é preciso limpar o buffer do teclado
						// caso contrário o \n será inserido no começo do nome do usuário
						getchar();
						if (subOpcao == 0)
						{
							break;
						}
						printf("\n\n");
					}

					else if (isInputOK == 1)
					{

						AdicionarUsuario(&Uaibank, u);
					}
				} while (isInputOK < 0);
				i += 1;
			}

			break;
		case 3:
			id = 0;
			printf("Digite a ID do usuário que deseja consultar \n");
			scanf("%d", &id);
			MostrarUsuario(&Uaibank, id);

			break;
		case 4:
			id = 0;
			printf("Digite a ID do usuário que deseja excluir \n");
			scanf("%d", &id);
			DeletarUsuario(&Uaibank, id);

			break;
		case 5:
			id_solicitante = 0;
			id_beneficiario = 0;
			quantidade = 0.0;
			printf("Digite a ID do usuário que fará a transferência : \n");
			scanf("%d", &id_solicitante);

			printf("Digite a ID do usuário que receberá a transferência : \n");
			scanf("%d", &id_beneficiario);

			printf("Digite a quantidade que deve ser transferida em R$: \n");
			scanf("%f", &quantidade);

			Transferencia(&Uaibank, id_solicitante, id_beneficiario, quantidade);
			break;
		case 6:
			ExportarDados(&Uaibank);
			break;
		default:;
			opcao = 0;
			break;
		}

	} while (opcao != 0);

	printf("\n\nAté logo!\n\n");

	free(Uaibank.clientes);
	return 0;
}