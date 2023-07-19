void configureAllSensors()
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Ultrasonic;
	wait1Msec(50);
}



void zeroAllMotors();
{
	motor[motorA] = motor[motorB] = motor[motorC] = 100;

	while(SensorValue[S1] == 0 && SensorValue[S2] == 0 && SensorValue[S3] != 2) //change "2" to appropriate value.
	{}
	motor[motorA] = motor[motorB] = motor[motorC] = 0;
}

void moveXY(float x, float y, int coord[][]) //determine where whcih side is 0,0 and what that nMotorEncoderValue is.
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = 100;
	while (nMotorEncoder[motorA] < x*(number)*(conversion))
	{}
	motor[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 100;
	while(nMotorEncoder[motorB] < y*(number)*(conversion))
	{}
	motor[motorB] = 0;
}

task main()
{
	configureAllSensors();
	zeroAllMotors();


}
