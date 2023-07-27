#include "PC_FileIO.c"

/*
Motors:
A; y
B: z
C: claw
D: x

*/
const int X_COORD[8] ={4400,8300,13575,18150,22950,27200,32000,37000};
const int Y_COORD[8] = {14280,12350,10430,8500,6150,4250,1960,80}; //subject to change due to y-zeroing

const int X_ZERO_DIST = 6;
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

bool zeroAllMotors()
{
	motor[motorA] = -100;
	motor[motorB] = -70;
	motor[motorD] = -100;

	while (!(SensorValue[Z_ZERO] == 1 && SensorValue[Y_ZERO] == 1 && SensorValue[X_ZERO] < X_ZERO_DIST)) //change "2" to appropriate value.
	{
		if(SensorValue[Z_ZERO] == 1)
			motor[motorB] = 0;
		if(SensorValue[Y_ZERO] == 1)
			motor[motorA] = -0.5;
		if(SensorValue[X_ZERO] < X_ZERO_DIST)
			motor[motorD] = 0;
		if(SensorValue[X_ZERO] < 10 && !(SensorValue[X_ZERO] < X_ZERO_DIST))
			motor[motorD] = -75;
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorA] = motor[motorB] = motor[motorD] = 0;
	nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorD] = 0;
	return true;
}






bool moveXY(int x, int y) //a1 is (0,0), will take a bool value and return 1 normally, if 0 is returned end the game
{
  nMotorEncoder[motorA] = nMotorEncoder[motorD] = 0;
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

bool moveFromXY(int x, int y)
{
	//D=x-axis A=y-axis


	//int distX = Y_COORD[y2] - Y_COORD[y];
	//int distY = X_COORD[x2] - X_COORD[x];
	int initDistX = nMotorEncoder[motorD];
	int initDistY = nMotorEncoder[motorA];

	int distX = X_COORD[x] - initDistX;
	int distY = Y_COORD[y] - initDistY;

	if(distX > 0)
		motor[motorD] = 75;
	else
		motor[motorD] = -75;

	if(distY> 0)
		motor[motorA] = 75;
	else
		motor[motorA] = -75;

	while (abs(nMotorEncoder[motorD] - initDistX) < abs(distX) || abs(nMotorEncoder[motorA]-initDistY) < abs(distY))
	{
		if(abs(nMotorEncoder[motorD] - initDistX) > abs(distX))
			motor[motorD] = 0;
		if(abs(nMotorEncoder[motorA]-initDistY) > abs(distY))
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

	moveXY(0,7);

	nMotorEncoder[motorB] = 0;
	motor[motorB] = 30;
	while(nMotorEncoder[motorB] < 1080)
	{}
	motor[motorB]=0;


	displayString(3, "Press and release Enter to confirm board calibration");

	while(!getButtonPress(buttonEnter))
	{}
	while(getButtonPress(buttonEnter))
	{}
	zeroAllMotors();
}

bool pickUpPiece() // same return as moveXY Calum
{
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 30;
	while(nMotorEncoder[motorB] < 1100)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB]=0;
	nMotorEncoder[motorC] = 0;
	wait1Msec(2000);
	motor[motorC] = 10;
	wait1Msec(2500);
	motor[motorB] = -40;
	while(SensorValue[Z_ZERO] == 0)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB] =0;
	return true;
}

bool dropPiece()
{
	motor[motorB] = 50;
	while(nMotorEncoder[motorB] < 1100 )
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB]=0;
	wait1Msec(2000);
	motor[motorC] = -10;
	while(nMotorEncoder[motorC] > 0)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorC] = 0;

	return true;
}

bool removePiece(int &counter) //same return as moveXY
{
	int y =400+ counter*(1000);
	zeroAllMotors();
	nMotorEncoder[motorD] = 0;
	motor[motorA] = 100;
	motor[motorD]= -30;

	while (nMotorEncoder[motorA] < y || SensorValue[X_ZERO] > X_ZERO_DIST)
	{
		if(SensorValue[X_ZERO] < X_ZERO_DIST)
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > y)
			motor[motorA] = 0;
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorA] = 0;
	dropPiece();
	motor[motorB] = -40;
	while(SensorValue[Z_ZERO] == 0)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB] =0;
	counter++;
	return true;
}

int chessMoves(TFileHandle & fin)//reads in the files and generates move
{
	int numMoves = 0, statPiece = 10, x = 0, y = 0, x2 = 0, y2 = 0, counter = 0;
	string startColor, piece;
	bool movePiece = true;

	/*
	if(removepiece || pickUpPiece || dropPiece == !true )
	{
		return false */

	readTextPC(fin, startColor);

	while(readTextPC(fin, piece) && readIntPC(fin, statPiece))
	{
		numMoves+=1;

		displayString(9, "Status: %d", statPiece);
		readIntPC(fin, y);
		readIntPC(fin, x);
		readIntPC(fin, y2);
		readIntPC(fin , x2);

		displayString(10, "Move %s from %d,%d to %d,%d", piece ,x,y,x2,y2);
		zeroAllMotors();

		if(statPiece == 0)
		{
			//movePiece = moveXY(x,y)&& pickUpPiece()&&zeroAllMotors()&&moveXY(x2,y2) && dropPiece();
			movePiece = moveXY(x,y)&& pickUpPiece()&& moveFromXY(x2,y2) && dropPiece();
			if(!movePiece)
			{
				return numMoves;
			}

		}
		else
		{
			/*movePiece = moveXY(x2,y2) && pickUpPiece() && removePiece(counter) && zeroAllMotors()
									&& moveXY(x,y) && pickUpPiece()&& zeroAllMotors() && moveXY(x2,y2)
									&& dropPiece();*/
			movePiece = moveXY(x2,y2) && pickUpPiece() && removePiece(counter) && zeroAllMotors()
									&& moveXY(x,y) && pickUpPiece()&& moveFromXY(x2,y2)
									&& dropPiece();

			//moveFromXY(x,y,x2,y2);
			if(!movePiece)
			{
				return numMoves;
			}
		}
	}

	if (piece == "Checkmate")
	{
		displayString(8, "Game Over!");
		return numMoves;
	}

	return numMoves;
}



task main()
{
	int movesPlayed = 0;
	configureAllSensors();

	callibrateBoard();

	displayString(5,"Press Any Button To Start!");		//initialize game
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
	clearTimer(T1);

	TFileHandle fin;
	bool fileCheck = openReadPC(fin , "Chess_Game_1.txt");



	if(!fileCheck)
	{
		displayString(5, "Error, file not found");
	}
	else
	{
		movesPlayed = chessMoves(fin);
		int time = time1[T1] / 1000;
		displayString(10, "Game finished in %d moves and in %d seconds.", movesPlayed, time);
	}
	/*
	zeroAllMotors();
	moveXY(0,0);
	pickUpPiece();
	moveFromXY(0,0,1,2);
	dropPiece();*/
}
