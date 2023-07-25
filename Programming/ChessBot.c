//#include "PC_FileIO.c"

/*
Motors:
A; y
B: z
C: claw
D: x

*/
const int X_COORD[] ={4950,9200,13581,18200,23100,27200,32000,37000};
const int Y_COORD[] = {14230,12350,10450,8550,6150,4250,1960,80}; //subject to change due to y-zeroing
const int zeroDist = 6;
const tSensors X_ZERO = S4;
const tSensors Y_ZERO = S2;
const tSensors Z_ZERO = S3;

void configureAllSensors()
{
	SensorType[Z_ZERO] = sensorEV3_Touch; //z
	SensorType[Y_ZERO] = sensorEV3_Touch; //y
	SensorType[X_ZERO] = sensorEV3_Ultrasonic; //x
	wait1Msec(100);
}

void zeroAllMotors()
{
	motor[motorA] = -100;
	motor[motorB] = -70;
	motor[motorD] = -100;

	while (!(SensorValue[Z_ZERO] == 1 && SensorValue[Y_ZERO] == 1 && SensorValue[X_ZERO] < zeroDist)) //change "2" to appropriate value.
	{
		if(SensorValue[Z_ZERO] == 1)
			motor[motorB] = 0;
		if(SensorValue[Y_ZERO] == 1)
			motor[motorA] = -0.5;
		if(SensorValue[X_ZERO] < zeroDist)
			motor[motorD] = 0;
	}
	motor[motorA] = motor[motorB] = motor[motorD] = 0;
	nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorD] = 0;
}






bool moveXY(int x, int y) //a1 is (0,0), will take a bool value and return 1 normally, if 0 is returned end the game
{
    nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorD] = 0;
	motor[motorD] = motor[motorA] = 100;

	while (nMotorEncoder[motorD] < X_COORD[x] || nMotorEncoder[motorA] < Y_COORD[y])
	{
		if(nMotorEncoder[motorD] > X_COORD[x])
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > Y_COORD[y])
			motor[motorA] = 0;
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorD] = motor[motorA] = 0;
	return true;
}

void callibrateBoard()
{
	zeroAllMotors();
	nMotorEncoder[motorD] = nMotorEncoder[motorA] = 0;
	motor[motorD] = motor[motorA] = 100;

	while (nMotorEncoder[motorD] < X_COORD[0] || nMotorEncoder[motorA] < Y_COORD[0])
	{
		if(nMotorEncoder[motorD] > X_COORD[0])
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > Y_COORD[0])
			motor[motorA] = 0;
	}

	//moveXY(0,0);

	displayString(3, "Press and release Enter to confirm board calibration");

	while(!getButtonPress(buttonEnter))
	{}
	while(getButtonPress(buttonEnter))
	{}
	zeroAllMotors();
}


bool removePiece(int &counter) //same return as moveXY
{
	int y =400+ counter*(400);
	zeroAllMotors();
	nMotorEncoder[motorD] = 0;
	motor[motorA] = 100;
	motor[motorD]= -30;

	while (nMotorEncoder[motorA] < y || SensorValue[X_ZERO] < 5)
	{
		if(SensorValue[X_ZERO] > 5)
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > y)
			motor[motorA] = 0;
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorA] = 0;
	return true;
	counter++;
}

bool pickUpPiece() // same return as moveXY Calum
{
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 30;
	while(nMotorEncoder[motorB] < 1050)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB]=0;
	nMotorEncoder[motorC] = 0;
	wait1Msec(2000);
	motor[motorC] = 20;
	while(nMotorEncoder[motorC] < 120)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorC] = 0;
	wait1Msec(2000);
	motor[motorB] = -40;
	while(!(nMotorEncoder[motorB] == 0))
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB] =0;
	return true;
}

void dropPiece()
{
	motor[motorB] = 50;
	while(nMotorEncoder[motorB] < 1000 )
	{}
	motor[motorB]=0;
	wait1Msec(2000);
	motor[motorC] = -20;
	while(nMotorEncoder[motorC] <80)
	{}
	motor[motorC] = 0;

}

/*
int chessMoves(TFileHandle & fin, TFileHandle & fout) //reads in the files and generates move
{
	/*int moves = 1, retractClaw = 1;
	if (moves == 0 || retractClaw == 0)
	{
		return 0;
	}
	return 1;

	int numValue = 0;
	readIntPC(fin,numValue);

	}
*/


task main()
{

	int movesPlayed = 0;
	int counter =0;
	configureAllSensors();

	displayString(5,"Press Any Button To Start!");
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{
		clearTimer(T1);
	}


//	TFileHandle fin;
//	bool fileCheck = openReadPC(fin , "chess.txt");
//=======
//>>>>>>> 8027879e7659723e964c945c41774c189fcb7b26

	//int moveTest =moveXY(7,7);
	zeroAllMotors();
	//callibrateBoard();

	int te = moveXY(1,3);
	int test = pickUpPiece();

	int rem = removePiece(counter);
	dropPiece();





	//callibrateBoard();
/*
	TFileHandle fin;
	bool fileCheck = openReadPC(fin , "chess.txt");
	TFileHandle fout;
	bool fileOut = openWritePC(fout, "chess_output.txt");




	TFileHandle fin;
	bool fileCheck = openReadPC(fin , "chess.txt");
	TFileHandle fout;
	bool fileOut = openWritePC(fout, "chess_output.txt");


	if(!fileCheck)
	{
		displayString(10, "Error");
	}
	else
	{
		chessMoves(fin, fout);
	}
;

	//moveXY(5,5);

	//chessMoves(movesPlayed);

*/
}
