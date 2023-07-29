#include "PC_FileIO.c"

// Chess Bot Code By: Alex, Calum, Collin, Nathan

/*
Corresponding Motor Identification:
motorA = Y-Axis
motorB = Z-Axis
motorC = Claw
motorD = X-Axis
*/

const int X_COORD[8] ={4400,8300,13575,18150,22950,27200,32000,37000}; // x-coordinates for chess board.
const int Y_COORD[8] = {14280,12350,10430,8500,6150,4250,1960,80}; // y-coordinates for chess board.

// Corresponding Sensor Identification:
const int X_ZERO_DIST = 6;
const tSensors X_ZERO = S4;
const tSensors Y_ZERO = S2;
const tSensors Z_ZERO = S3;

void configureAllSensors() //initializes sensors.
{
	SensorType[Z_ZERO] = sensorEV3_Touch; 
	SensorType[Y_ZERO] = sensorEV3_Touch; 
	SensorType[X_ZERO] = sensorEV3_Ultrasonic; 
	wait1Msec(100);
}

bool zeroAllMotors() // Homes the gantry (moves gantry to where the motors are).
{
	motor[motorA] = -100;
	motor[motorB] = -70;
	motor[motorD] = -100;

	while (!(SensorValue[Z_ZERO] == 1 && SensorValue[Y_ZERO] == 1 && SensorValue[X_ZERO] < X_ZERO_DIST)) //Checks for touch.
	{
		if(SensorValue[Z_ZERO] == 1)
			motor[motorB] = 0;
		if(SensorValue[Y_ZERO] == 1)
			motor[motorA] = -0.5; //to make sure the touch sensor will still be touched (too much shaking moves touch).
		if(SensorValue[X_ZERO] < X_ZERO_DIST)
			motor[motorD] = 0;
		if(SensorValue[X_ZERO] < 10 && !(SensorValue[X_ZERO] < X_ZERO_DIST))
			motor[motorD] = -75; //slows down motors to be smooth
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

bool moveFromXY(int x, int y) //moves chess piece from inital position to final position;
//will take a bool value and return 1 normally, if 0 is returned end the game
{
	int initDistX = nMotorEncoder[motorD]; //x-axis
	int initDistY = nMotorEncoder[motorA]; //y-axis

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

void callibrateBoard().//moves gantry to h1(7,7) location; move chess board to align
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

bool pickUpPiece() // picks up chess piece; will take a bool value and return 1 normally, if 0 is returned end the game
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
	wait1Msec(2000);
	motor[motorB] = -40;
	while(SensorValue[Z_ZERO] == 0)
	{
		if(getButtonPress(buttonAny) == 1)
			return false;
	}
	motor[motorB] =0;
	return true;
}

bool dropPiece() // drops chess piece; will take a bool value and return 1 normally, if 0 is returned end the game
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

bool removePiece(int &counter) //if piece is being eaten it will move to edge of board; 
//will take a bool value and return 1 normally, if 0 is returned end the game
{
	int die =400+ counter*(1000); //calculates nMotorEncoder values for chess piece to be dropped 
								 //based on number of times eaten
	zeroAllMotors();
	nMotorEncoder[motorD] = 0;
	motor[motorA] = 100;
	motor[motorD]= -30;

	while (nMotorEncoder[motorA] < die || SensorValue[X_ZERO] > X_ZERO_DIST)
	{
		if(SensorValue[X_ZERO] < X_ZERO_DIST)
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > die)
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

int chessMoves(TFileHandle & fin)//reads in the files and generates move, returns number of chess moves
{
	int numMoves = 0, statPiece = 10, x = 0, y = 0, x2 = 0, y2 = 0, counter = 0;
	string startColor, piece;
	bool movePiece = true;
	
	readTextPC(fin, startColor);

	while(readTextPC(fin, piece) && readIntPC(fin, statPiece))
	{
		numMoves+=1;

		displayString(9, "Status: %d", statPiece);
		readIntPC(fin, y);
		readIntPC(fin, x);
		readIntPC(fin, y2);
		readIntPC(fin , x2);

		displayString(6, "Move %s from %d,%d to %d,%d", piece ,x,y,x2,y2);
		zeroAllMotors();

		if(statPiece == 0)
		{
			movePiece = moveXY(x,y)&& pickUpPiece()&& moveFromXY(x2,y2) && dropPiece();
			if(!movePiece)
			{
				return numMoves;
			}

		}
		else
		{
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
	int movesPlayed = 0; //counter for number of moves
	configureAllSensors();
	callibrateBoard();

	displayString(5,"Press Any Button To Start!");	//initializes game
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
	clearTimer(T1); //timer for duration of game

	TFileHandle fin;
	bool fileCheck = openReadPC(fin , "Chess_Game_2.txt");

	if(!fileCheck)
	{
		displayString(5, "Error, file not found");
	}
	else
	{
		movesPlayed = chessMoves(fin);
		int time = time1[T1] / 1000;
	}
	displayString(9, "Game finished in %d moves.", movesPlayed);
	displayString(10,"Elapsed time: %d seconds", time);

	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
}
