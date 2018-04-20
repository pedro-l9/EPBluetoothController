#include <CommandParser.h>
#include <ElectroPneumaticsToolkit.h>

Command cmdBuffer ("");
Bancada bancada;

void setup() {
	Serial.begin(9600);
	
	delay(1000);
	Serial.println("Starting...");
}

void loop() {
	if(Serial.available()){
		Serial.println("Interpretando comando...");
		CommandParser::parseCommand(Serial.readString(), &cmdBuffer);
		Serial.println("Encontrado: " + cmdBuffer.name);
		cmdBuffer.printArgs();
		commandExecuter(&cmdBuffer);
	}
}

void commandExecuter(Command* cmd){
	if(cmd->name == "addAtuadorDigital"){
		AtuadorDigital atuador ( cmd->args[0],
								 (int)cmd->args[1].toInt(),
								 cmd->args[ 2] == "1",
								 (int)cmd->args[4].toInt());

		bancada.adicionaAtuador(&atuador);		
	}else if(cmd->name == "addServo"){
		Servo atuador ( cmd->args[0],
								 (int)cmd->args[1].toInt(),
								 cmd->args[ 2] == "1",
								 (int)cmd->args[4].toInt());

		bancada.adicionaAtuador(&atuador);
	}else if(cmd->name == "listAtuadores"){
		for(int i = 0; i < bancada.qtdAtuadores; i++){
			Serial.println(bancada.atuadores[i].nome);
		}
	}else if(cmd->name == "removeAtuador"){
		bancada.removeAtuador(cmd->args[0]);
	}else if(cmd->name == "addEvento"){

	}else if(cmd->name == "execute"){

	}else if(cmd->name == "loop"){

	}
}

void executeExpression(String expression, int repetitions){
	do{

		repetitions--;
	}while(repetitions > 0);
}


