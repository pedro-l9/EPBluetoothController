#include <CommandParser.h>
#include <ElectroPneumaticsToolkit.h>

Command cmdBuffer("");
Bancada bancada;
boolean shouldLoop = false;
Expression loopExpression;

void setup()
{
	pinMode(6, OUTPUT);

	Serial.begin(9600);

	delay(1000);
	Serial.println("Starting...");
}

void loop()
{
	if (shouldLoop)
	{
		doLoop();
	}

	if (Serial.available())
	{
		getComando();
	}
}

void doLoop()
{
	bancada.executaExpression(&loopExpression, 1);
}

void getComando()
{

	CommandParser::parseCommand(Serial.readString(), &cmdBuffer);
	executeCommand(&cmdBuffer);
}

void executeCommand(Command *cmd)
{
	//addAtuadorDigital(nome, porta, estadoInicial, tempoAtuacao)
	if (cmd->name == "addAtuadorDigital" && cmd->argCount == 4)
	{
		bancada.adicionaAtuador(new AtuadorDigital(cmd->args[0],
																							 cmd->args[1].toInt(),
																							 cmd->args[2] == "1",
																							 cmd->args[3].toInt()));
	}
	else if (cmd->name == "addServo" && cmd->argCount == 4)
	{
		//addServo(nome, porta, estadoInicial, tempoAtuacao)
		bancada.adicionaAtuador(new Servo(cmd->args[0],
																			cmd->args[1].toInt(),
																			cmd->args[2] == "1",
																			cmd->args[3].toInt()));
	}
	else if (cmd->name == "listAtuadores")
	{
		//listAtuadores
		bancada.listaAtuadores();
	}
	else if (cmd->name == "removeAtuador" && cmd->argCount == 1)
	{
		//removeAtuador(nome)
		bancada.removeAtuador(cmd->args[0]);
	}
	else if ((cmd->name == "loop" || cmd->name == "execute") && !shouldLoop && cmd->argCount >= 1)
	{
		//execute(expression) | loop(expression, repetitions) | loop(expression)
		CommandParser::parseExpression(cmd->args[0], &loopExpression);
		if (cmd->argCount == 2 || (cmd->argCount == 1 && cmd->name == "execute"))
		{
			bancada.executaExpression(&loopExpression, cmd->name == "loop" ? cmd->args[1].toInt() : 1);
		}
		else
		{
			shouldLoop = true;
			doLoop();
		}
	}
	else if (cmd->name == "atuar" && !shouldLoop && cmd->argCount == 2)
	{
		//atuar(nome, estado)
		bancada.atuar(cmd->args[0],
									cmd->args[1].toInt());
	}
	else if (cmd->name == "help" && !shouldLoop)
	{
		//help
		Serial.println("Comandos:\n");
		Serial.println("\n- addAtuadorDigital([nome do atuador], [porta], [estado inicial], [tempo de atuação])\n   Adiciona um atuador digital à bancada");
		Serial.println("\n- removeAtuador([nome do atuador])\n   Remove um atuador da bancada pelo nome");
		Serial.println("\n- listAtuadores()\n   Lista os atuadores já adicionados na bancada por nome");
		Serial.println("\n- atuar([nome do atuador], [estado desejado])\n   Coloca um atuador em um estado desejado");
		Serial.println("\n- execute([expressão])\n   Executa uma expressão");
		Serial.println("\n- loop([expressão], [número de repetições])\n   Executa uma expressão um número determinado de vezes");
		Serial.println("\n- loop([expressão])\n   Executa uma expressão até que seja interrompido");
		Serial.println("\n- parar()\n   Interrompe a execução de um loop");
	}
	else if (cmd->name == "parar" && shouldLoop)
	{
		//parar
		Serial.println("Parando loop");
		shouldLoop = false;
	}
	else
	{
		Serial.println("Comando não reconhecido");
	}
}
