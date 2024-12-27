//********************* Normal steering + Acceleration without diff **************************
//

#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "math.h"
#define maxPIXEL 128
//#define DEBUGLABV
//#define DEBUG

int main(void)

{

	int i=0;
	float deriv[maxPIXEL];
	float minDerivalue ;
	float maxDerivalue ;
	float averageDeriv ;
	float negDeriv_Thershold ;
	float posDeriv_Thershold ;
	float upper ;
	float lower ;
	float servo_pos = 0 ;
	int   error = 0, x = 0 ,y = 0 ;
	float A;
	float B;
	float m;
	float n;
	float Max_speed = 0.30;
	float Min_speed = 0.20 ;	
	
	
	TFC_Init();
	
	for(;;)
	{
		 minDerivalue = 0;
		 maxDerivalue = 0;
		 averageDeriv = 0;
		 negDeriv_Thershold = 0;
		 posDeriv_Thershold = 0;
		 upper = 0;
		 lower = 0;	
		 x = 0;
		 y = 0;
		 
		 	//TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
			TFC_Task();
			
			if(TFC_PUSH_BUTTON_0_PRESSED){//cette condition permet de demarrer les moteurs a 20% 
				TFC_HBRIDGE_ENABLE;			//au premier appui sur le switch 1
				TFC_SetMotorPWM(0.5,0.5);
			}
			if(TFC_PUSH_BUTTON_1_PRESSED){//cette condition permet de demarrer les moteurs a 20% 
				TFC_HBRIDGE_DISABLE;			//au premier appui sur le switch 1
				TFC_SetMotorPWM(0.0,0.0);
			}			
			
			
			//This Demo program will look at the middle 2 switch to select one of 4 demo modes.
			//Let's look at the middle 2 switches
			switch((TFC_GetDIP_Switch()>>1)&0x03)
			{
			default:
			case 0 :
				 Max_speed = 0.3 ;
				 Min_speed = 0.20 ;
				 A=1.2;
				 B=-1.2;
				 m=0.6;
				 n=-0.6;
				 TFC_SetBatteryLED_Level(1);
				
				break;
					
			case 1:				
				 Max_speed = 0.3 ;
				 Min_speed = 0.20 ;	
				 A=1.2;
				 B=-1.25;
				 m=0.6;
				 n=-0.6;
				 TFC_SetBatteryLED_Level(2);
				break;
				
			case 2 :
				 Max_speed = fabs (TFC_ReadPot(0)) ;
				 if (Max_speed > 0.45) Max_speed = 0.3; 
				 Min_speed = 0.18 ;	
				 A=1.2;
				 B=-1.2;
				 m=0.75;
				 n=-0.75;
				 TFC_SetBatteryLED_Level(4);
				break;
			
			case 3 :
				 Max_speed = 0.3 ;
				 Min_speed = 0.18 ;	
				 A=1.2;
				B=-1.2;
				m=0.6;
				n=-0.6;
				 TFC_SetBatteryLED_Level(3);
				break;
			}
			
			//Update servo pos every 20ms
			if(TFC_Ticker[1]>=20)
			{
				TFC_Ticker[1] = 0; //reset the Ticker
				//Every 20 mSeconds, update the Servos					
				TFC_SetServo(0,servo_pos);
			}			


			if(TFC_Ticker[0]> 30 && LineScanImageReady==1) // Only for debbugin: to receive data in tera term put instead 100 the value 400ms.
				{
				 TFC_Ticker[0] = 0;		
				 LineScanImageReady=0;
				 TERMINAL_PRINTF("\r\n");
				 TERMINAL_PRINTF("L:");
				 	 
				 						 
					#ifdef DEBUGLABV
					 for(i=0;i<128;i++)
					 {
							 TERMINAL_PRINTF("%X,",LineScanImage0[i]);
					 }
					
					 for(i=0;i<128;i++)
					 {
							 TERMINAL_PRINTF("%X",LineScanImage1[i]);
							 if(i==127)
								 TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
							 else
								 TERMINAL_PRINTF(",",LineScanImage1[i]);
					}
					#endif
					 
						// Calculate derivative
						for(i=0;i<128;i++)
						{
							if (i == 0)
							{
								upper = LineScanImage0[i+1];
								lower = LineScanImage0[i];
							}
							else if (i == 127)
							{
								upper = LineScanImage0[i];
								lower = LineScanImage0[i-1];
							}
							else
							{
								upper = LineScanImage0[i+1];
								lower = LineScanImage0[i-1];									
							}

							deriv[i] = (float) ((upper - lower)/2) ;
							
							if (deriv[i] > maxDerivalue) 
							{
								maxDerivalue = deriv[i];
							} 
							if (deriv[i] < minDerivalue) 
							{
							  	minDerivalue = deriv[i];
							}
							averageDeriv = averageDeriv + deriv[i];
								
						}
						
						// Calculate parameters
						averageDeriv       = (float) (averageDeriv / 128) ;
						negDeriv_Thershold = (float) ((minDerivalue - averageDeriv)/2);
						posDeriv_Thershold = (float) ((maxDerivalue - averageDeriv)/2);
						if (negDeriv_Thershold > (- 185))  negDeriv_Thershold = - 185 ;
						if (posDeriv_Thershold < 185)      posDeriv_Thershold =   185 ;
						
						// Scan left and Right
						for(i=63 ;i>=0 ;i--)
						{
							if (deriv[i] > posDeriv_Thershold)
							{
								break ;
							}
							else
							{
								x++ ;
							}
						}
						
						for(i=64;i<128;i++)
						{																
							if (deriv[i] < negDeriv_Thershold)
							{
								break ;
							}
							else
							{
								y++ ;
							}
						}
						// Calculate the error = x - y
						error = x - y ;	
												
						// When the car is not in the center we update new steering position
						// Otherwise, we kept the last values of steering			                
						if (error != 0)
						{
							// Steering processing
							//servo_pos = (float) (((error + 63)/126)- 0.5); (problem priority)
							// Change the domain from error[-63..63] to steering[-0,5..0.5]
							servo_pos = (float)  (error + 63) ;
							servo_pos = (float)  (servo_pos * A) ;
							servo_pos = (float)  ((servo_pos/126)- 0.8) ;							
							// because the steering action should be the opposite of the error
							servo_pos = servo_pos * (B) ; 
							
							// Control limits
							if (servo_pos > m) servo_pos = m;
							else if (servo_pos < n) servo_pos = n;
						}
						
						if (error < 0) { error = error * (-1) ;}
						if (error > 20 )
						{
							TFC_SetMotorPWM(Min_speed,Min_speed);
						}else
						{
							TFC_SetMotorPWM(Max_speed,Max_speed);
						}
						
						// Update the steering angle
						TFC_SetServo(0,servo_pos);

					#ifdef DEBUG
						TERMINAL_PRINTF("pot 0 =%2.2f\n\r;",TFC_ReadPot(0));
						TERMINAL_PRINTF("pot 1=%2.2f;",TFC_ReadPot(1));
					#endif	
						
				}// end case 3			
	}
	
	return 0;
}
