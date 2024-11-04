# get_next_line - Testes - 42.zip
Este repositório contém testes do projeto get_next_line presente no curso 42.zip. Abaixo você encontrará instruções detalhadas sobre como configurar, compilar e executar os testes para as funções criadas nele.

## Estrutura do Projeto

```
/
├── .vscode/
│   ├── launch.json
│   ├── tasks.json
│   ├── c_cpp_properties.json
│   └── settings.json
├── get_next_line/
│   └── [arquivos do projeto get_next_line]
├── lib/
│   └── [bibliotecas compiladas (mocks)]
├── tests/
│   ├── minunit.h
│   └── [arquivos de teste]
├── mocks/
│   └── [implementações de funções e dados falsos para testes]
├── build/
│   └── [binários compilados]
├── .gitignore
└── Makefile
```

## Pré-requisitos

Certifique-se de ter o compilador `gcc` ou `cc` instalado no seu sistema. Além disso, você precisará do `make` para construir os projetos.

## Compilação e Execução

### Compilação

Para compilar todos os exercícios, execute o comando:

```sh
make all
```

### Execução de Testes

Para executar os testes, utilize a regra `run` do Makefile:

```sh
make run
```

### Execução de Teste Único

Para executar um teste específico, utilize a variável `TEST` com a regra `run`:

```sh
make run TEST=get_next_line_utils
```

Substitua `get_next_line_utils` pelo nome da função que você deseja testar.

### Limpeza

Para limpar os arquivos compilados, execute:

```sh
make clean
```

Para limpar todos os arquivos compilados e binários, execute:

```sh
make fclean
```

### Recompilação

Para limpar e recompilar todos os exercícios, execute:

```sh
make re
```

### Uso de Variáveis

### TEST

A variável `TEST` é usada para especificar qual teste deve ser compilado. Por exemplo:

```sh
make TEST=get_next_line_utils
```

### TARGET

A variável `TARGET` é usada para especificar o diretório do projeto. A pasta pode ter qualquer nome. Por exemplo:

```sh
make TARGET=get_next_line
```

## Depuração

### Execução de Testes para Depuração

Para executar os testes em modo de depuração, utilize a regra `debug` do Makefile:

```sh
make debug
```

### Execução de Teste Único para Depuração

Para executar um teste específico em modo de depuração, utilize a regra `debug` com a variável `TEST`:

```sh
make debug TEST=get_next_line_utils
```

Substitua `get_next_line_utils` pelo nome da função que você deseja testar.

## Modelo para Criação de Novos Testes

Para criar novos testes para os exercícios, siga o modelo abaixo. Este modelo utiliza a biblioteca `minunit` para estruturar os testes.

### Estrutura do Arquivo de Teste

Cada arquivo de teste deve seguir a estrutura abaixo:

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exXX.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [seu_nome] <[seu_email]>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: [data_criacao]                           #+#    #+#             */
/*   Updated: [data_atualizacao]                       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minunit.h"

void	function_to_be_tested(void);

MU_TEST(test_function_name)
{
	// ARRANGE
	int	expected_result;
	int	actual_result;

	// ACT
	expected_result = expected_value;
	actual_result = function_to_test(arguments);

	// ASSERT
	mu_assert_int_eq(expected_result, actual_result);
}

MU_TEST_SUITE(test_suite_name)
{
	MU_RUN_TEST(test_function_name);
}

int main(void) {
	MU_RUN_SUITE(test_suite_name);
	MU_REPORT();
	return MU_EXIT_CODE;
}
```

### Passos para Criar um Novo Teste

1. **Crie um novo arquivo de teste**: O nome do arquivo deve seguir o padrão `test_exXX.c`, onde `XX` é o número do exercício.
2. **Inclua os cabeçalhos necessários**: Inclua os cabeçalhos padrão e o cabeçalho `minunit.h`.
3. **Declare a função a ser testada**: Declare a função que será testada.
4. **Implemente os testes**: Utilize as macros do `minunit` para criar os testes.
5. **Crie a suíte de testes**: Agrupe os testes em uma suíte de testes.
6. **Implemente a função `main`**: A função `main` deve executar a suíte de testes e gerar o relatório.

### Exemplo de Teste

Veja um exemplo de teste para a função `ft_strcmp`:

```c
MU_TEST(test_ft_strcmp_s1_a_s2_a)
{
	// ARRANGE
	int	expected_result;
	int	actual_result;

	// ACT
	expected_result = 0;
	actual_result = ft_strcmp("a", "a");

	// ASSERT
	mu_assert_int_eq(expected_result, actual_result);
}
```

Siga este modelo para criar testes consistentes e bem estruturados para os exercícios da Piscine Reloaded.

## Configuração do VSCode

### Tarefas

O arquivo `tasks.json` contém uma tarefa `build` que pode ser usada para compilar os exercícios diretamente do VSCode. A tarefa detecta automaticamente o exercício com base no diretório ou arquivo atual.

### Depuração

O arquivo `launch.json` está configurado para permitir a depuração dos exercícios. Certifique-se de que o binário `test_debug` foi gerado antes de iniciar a depuração.
