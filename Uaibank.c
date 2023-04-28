#include <stdlib.h>
#include <stdio.h>

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


void MostrarUsuario(Banco *b, int id )
{
	int i = 0;
	Usuario u;
	u.id = 0;

	for(i = 0; i < (b->size - 1); i++){		
		if( b->clientes[i].id == id ){
			u = b->clientes[i];
			break;
		}
	}

	if(u.id != 0) {
		printf("Usuário %d tem saldo de R$ %.2f .\n", u.id, u.saldo);
	}
	else{
		printf("ERRO: Usuário %d não encontrado .");
	}
	
}

void AdicionarUsuario(Banco *b, Usuario u )
{
	u.id = b->gen_id;
	b->gen_id += 1;

	b->clientes[(b->size - 1)] = u;
	b->clientes = (Usuario *) realloc( b->clientes, (b->size + 1) * sizeof(Usuario));

	// Deu erro na alocação de memória
	if(b->clientes == NULL){
		b->size = 0;
	}
	else{
		b->size += 1;
		
	}
}

void DeletarUsuario(Banco *b, int id)
{
	int i = 0;
	int indice = -1;
	for(i = 0; i < (b->size - 1); i++){
		if( b->clientes[i].id == id ){
			indice = i;
			break;
		}
	}

	if(indice > -1){

		int j;
		for(j = (indice + 1); j < (b->size - 1); j++){
			b->clientes[j - 1] = b->clientes[j];
		}

		b->size -= 1;
		b->clientes = (Usuario *) realloc( b->clientes, (b->size + 1) * sizeof(Usuario));
	}
	else{

	}

}

void Transferencia(Banco *b, int id_solicitante, int id_beneficiario)
{
	Usuario *solicitante;
	Usuario *beneficiario;
	
}

void ExportarDados(Banco *b){

	FILE *file;
	file = fopen("UAIBANK.txt", "w");

	int i = 0;
	for(i = 0; i < (b->size - 1); i++){		
		fprintf(file, "%s, %d, %.2f", b->clientes[i].nome, b->clientes[i].idade, b->clientes[i].saldo );
	}

	fclose(file);

}

int main()
{

	Banco Uaibank;	

	// Inicia o Uaibank com um cliente
	Uaibank.clientes = (Usuario *) malloc(1 * sizeof(Usuario));
	Uaibank.size = 1;
	Uaibank.gen_id = 1;

	int opcao = 0;
	int id = 0;
	int i = 0;
	int n = 0;
	Usuario u;



	// Imprime o menu do opções para o usuário
	printf("\t\t _     _       _ _                 _     \n");
	printf("\t\t| |   | |     (_) |               | |    \n");
	printf("\t\t| |   | | ____ _| | _   ____ ____ | |  _ \n");
	printf("\t\t| |   | |/ _  | | || \\ / _  |  _ \\| | / )\n");
	printf("\t\t| |___| ( ( | | | |_) | ( | | | | | |< ( \n");
	printf("\t\t \\______|\\_||_|_|____/ \\_||_|_| |_|_| \\_)\n");

	printf("\nBem vindo ao UaiBank !!\n");
	
	do {
		printf("\n\n\nEm que podemos ajudá-lo hoje ?\n\n");
		printf("\t - Digite 1 para cadastrar um usuário\n");
		printf("\t - Digite 2 para cadastrar vários usuários\n");
		printf("\t - Digite 3 para buscar por um usuário\n");
		printf("\t - Digite 4 para excluir um usuário\n");
		printf("\t - Digite 5 para exportar a base de dados para um arquivo\n");
		printf("\t - Digite 0 para sair\n");

		scanf("%d", &opcao);

		switch (opcao)
		{
			case 1: 
				printf("Digite o nome completo, a idade e o saldo do usuário para cadastrar\n");				
				u.id = 0;
				//u.nome = "\0";
				u.idade = 0;
				u.saldo = 0.f;
				scanf("%[^,], %d, %f", u.nome, &u.idade, &u.saldo);
				AdicionarUsuario(&Uaibank, u);
				
				break;
			case 2:
				i = 0;
				n = 0;

				printf("Qual o número de usuário que serão cadastrados ? ");
				scanf("%d", &n);

				while(i < n){
					u.id = 0;
					//u.nome = "\0";
					u.idade = 0;
					u.saldo = 0.f;

					u.id = 0;
					scanf("%[^,], %d, %f", u.nome, &u.idade, &u.saldo);
					AdicionarUsuario(&Uaibank, u);

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
				ExportarDados(&Uaibank);
				break;							
			default: ;
				opcao = 0;
				break;
		}
		
	} while( opcao != 0);


	printf("\n\nAté logo !\n\n");     
	free(Uaibank.clientes);

	return 0;
}