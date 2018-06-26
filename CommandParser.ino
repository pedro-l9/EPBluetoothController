#include <CommandParser.h>
#include <ElectroPneumaticsToolkit.h>

Command cmdBuffer("");
Bancada bancada;
boolean shouldLoop = false;
Expression loopExpression;

void setup() {
  pinMode(6, OUTPUT);
  
	Serial.begin(9600);
	
	delay(1000);
	Serial.println("Starting...");
}

void loop() {
	if(shouldLoop){
		doLoop();
	}

	if(Serial.available()){
		getComando();
	}
}

void doLoop(){
	bancada.executaExpression(&loopExpression, 1);
}

void getComando(){
	Serial.println("Interpretando comando...");

	CommandParser::parseCommand(Serial.readString(), &cmdBuffer);

	Serial.println("Encontrado: " + cmdBuffer.name);
	if(cmdBuffer.argCount > 0) cmdBuffer.printArgs();

	executeCommand(&cmdBuffer);
}

void executeCommand(Command* cmd){
	//addAtuadorDigital(nome, porta, estadoInicial, tempoAtuacao)
	if(cmd->name == "addAtuadorDigital" && cmd->argCount == 4){
		bancada.adicionaAtuador(new AtuadorDigital (cmd->args[0],
													cmd->args[1].toInt(),
													cmd->args[2] == "1",
													cmd->args[3].toInt()));		
	}else if(cmd->name == "addServo" && cmd->argCount == 4){
		//addServo(nome, porta, estadoInicial, tempoAtuacao)
		bancada.adicionaAtuador(new Servo (cmd->args[0],
										   cmd->args[1].toInt(),
										   cmd->args[2] == "1",
										   cmd->args[3].toInt()));
	}else if(cmd->name == "listAtuadores"){
		//listAtuadores
		bancada.listaAtuadores();
	}else if(cmd->name == "removeAtuador" && cmd->argCount == 1){
		//removeAtuador(nome)
		bancada.removeAtuador(cmd->args[0]);
	}else if((cmd->name == "loop" || cmd->name == "execute") && !shouldLoop && cmd->argCount >= 1){
    	//execute(expression) | loop(expression, repetitions) | loop(expression)
		CommandParser::parseExpression(cmd->args[0], &loopExpression);
    	if(cmd->argCount == 2){
    		bancada.executaExpression(&loopExpression, cmd->name == "loop" ? cmd->args[1].toInt() : 1);
    	}else{
    		shouldLoop = true;
    		doLoop();
    	}
    }else if(cmd->name == "atuar" && !shouldLoop && cmd->argCount == 2){
    	//atuar(nome, estado)
    	bancada.atuar(cmd->args[0],
                	  cmd->args[1].toInt());
	}else if(cmd->name == "parar" && shouldLoop){
		//parar
		Serial.println("Parando loop");
    	shouldLoop = false;
    }
    
}



