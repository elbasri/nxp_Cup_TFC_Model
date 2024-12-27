#include "derivative.h"
#include "TFC\TFC.h"
#include "math.h"
#define COURSE

/**************************
 * Main
 **************************/
int main(void) {

  float Max_speed = 0.6;
  float last_err = 0.0;
  float pos_serv = 0.0;
  float KP = 0.49;
  float KI = 0.00;
  float KD = 0.43;
  float servo = -1.3;
  float nbrScan = 10;
  float virage = 0.70;
  float frn = 1.2;
  TFC_Init();

  for (;;) {

    //TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
    TFC_Task();

    // Change exposure time of camera using switch
    switch ((TFC_GetDIP_Switch() >> 1) & 0x03) {
    default:

    case 0: //1.7cm 2.2cm
      //speed 0.63
      Max_speed = 0.45;
      TFC_SetLineScanExposureTime(1500);
      TFC_SetBatteryLED_Level(1);
      KP = 0.50;
      KI = 0.00;
      KD = 0.42;
      servo = -1.26;
      nbrScan = 8;
      virage = 0.50;

      break;

    case 1:
      //speed 0.63
      Max_speed = 0.45;
      TFC_SetLineScanExposureTime(11000);
      TFC_SetBatteryLED_Level(2);
      KP = 0.50;
      KI = 0.00;
      KD = 0.42;
      servo = -1.26;
      nbrScan = 7;
      virage = 0.50;

      break;

    case 2:
      Max_speed = 0.45;
      TFC_SetLineScanExposureTime(9000);
      TFC_SetBatteryLED_Level(3);
      KP = 0.50;
      KI = 0.00;
      KD = 0.42;
      servo = -1.26;
      nbrScan = 9;
      virage = 0.50;
      break;

      //ça marche
    case 3:
      Max_speed = 0.45;
      TFC_SetLineScanExposureTime(9000);
      TFC_SetBatteryLED_Level(4);
      KP = 0.50;
      KI = 0.00;
      KD = 0.42;
      servo = -1.26;
      nbrScan = 7;
      virage = 0.50;
      break;

    }

    //Max_speed = fabs (TFC_ReadPot(0));
    // if (Max_speed > 0.6) Max_speed = 0.6;
    if (TFC_PUSH_BUTTON_0_PRESSED) { //cette condition permet de demarrer les moteurs a 20%
      TFC_HBRIDGE_ENABLE; //au premier appui sur le switch 1
      TFC_SetMotorPWM(0.3, 0.3);
    } else if (TFC_PUSH_BUTTON_1_PRESSED) // Cette condition arrete le moteur
    {

      TFC_HBRIDGE_DISABLE;
      TFC_SetMotorPWM(0.0, 0.0);

    }

    // Update servo position every 10ms
    if (TFC_Ticker[1] >= nbrScan) {
      TFC_Ticker[1] = 0; //reset the Ticker
      //Every nbrScan mSeconds, update the Servos
      TFC_SetServo(0, pos_serv);
    }

    // Only for terateam: to receive data in tera-team put 400ms.
    // Only for labview: to receive data in tera-team put 100ms.
    // for racing put 20ms or don't check time

    #ifdef DEBUG

    if (TFC_Ticker[0] > 400 && LineScanImageReady == 1)
      #endif

    #ifdef DEBUGLABV
    if (TFC_Ticker[0] > 100 && LineScanImageReady == 1)
      #endif

    #ifdef COURSE
    if (LineScanImageReady == 1)
      #endif

    {
      TFC_Ticker[0] = 0;
      LineScanImageReady = 0;

      // déclaration des variables
      int i;
      float deriv[128];
      float max, min;
      int x, y, err;
      float new_err, sum_err, var_err;
      float SP, speed_R, speed_L;

      //debut du code
      deriv[0] = ((LineScanImage0[1] - LineScanImage0[0]) / 2.0);
      deriv[127] = ((LineScanImage0[127] - LineScanImage0[126]) / 2.0);
      max = 200.0;
      min = -200.0;
      for (i = 1; i < 127; i++) {
        deriv[i] = ((LineScanImage0[i + 1] - LineScanImage0[i - 1]) / 2.0);
        if (max < deriv[i]) {
          max = deriv[i];
        }
        if (min > deriv[i]) {
          min = deriv[i];
        }
      }

      if (max > 1750) {
        max = 1750;
      } // max sonde avec 40ms zone eclairé

      #ifdef DEBUG
      /* TERMINAL_PRINTF("\r\n");
      TERMINAL_PRINTF("L:");
      TERMINAL_PRINTF("\r\n");
      for(i=0;i<128;i++)
      {
      TERMINAL_PRINTF("%d;",LineScanImage0[i]);
      }

      TERMINAL_PRINTF("\r\n");
      TERMINAL_PRINTF("D:");
      TERMINAL_PRINTF("\r\n");
      for(i=0;i<128;i++)
      {/// affiche tableau de la dérivé
      TERMINAL_PRINTF("%4.0f;",deriv[i]);
      }*/

      #endif

      // display data on labview
      #ifdef DEBUGLABV
      TERMINAL_PRINTF("\r\n");
      TERMINAL_PRINTF("L:");

      uint16_t deriv_tmp[128];
      for (i = 0; i < 128; i++) {
        deriv_tmp[i] = deriv[i] + max + 10;
      }

      for (i = 0; i < 128; i++) {
        TERMINAL_PRINTF("%X,", LineScanImage0[i]);
      }

      for (i = 0; i < 128; i++) {
        TERMINAL_PRINTF("%X", deriv_tmp[i]);
        if (i == 127)
          TERMINAL_PRINTF("\r\n");
        else
          TERMINAL_PRINTF(",");
      }
      #endif

      x = 0;
      y = 0;
      for (i = 63; i >= 0; i--) {
        if (deriv[i] < (max / 2.5)) {
          x++;
        } else {
          break;
        }
      }
      for (i = 64; i < 128; i++) {
        if (deriv[i] > (min / 2.5)) {
          y++;
        } else {
          break;
        }
      }
      /*
      if((x<20&&y<20)||(y<20&&x<20)){ //The finish line stop

      //for(i=0;i<1000;i++)
      //   TFC_SetMotorPWM(-1*(Max_speed/frn),-1*(Max_speed/frn));
      TFC_SetMotorPWM(0.0,0.0);
      TFC_HBRIDGE_DISABLE;
      }*/

      err = x - y;
      if (err != 0) {
        pos_serv = err / 128.0;
        new_err = err / 64.0;
        // PID control of servo position
        sum_err += new_err;
        var_err = new_err - last_err;
        pos_serv = KP * new_err + KI * sum_err + KD * var_err;
        last_err = new_err;
        if (pos_serv > 0.5) {
          pos_serv = 0.5;
        }
        if (pos_serv < -0.5) {
          pos_serv = -0.5;
        }

        pos_serv = (servo) * pos_serv; // inverser le sens pour compenser l'erreur

        if (fabs(new_err) < 0.20) {
          speed_R = Max_speed;
          speed_L = Max_speed;
        } else {
          SP = (x + y) * 0.5;
          if (Max_speed == 0.5) {
            if (x > y) {
              speed_L = 0.3;
              speed_R = Max_speed;
            }
            if (x < y) {
              speed_L = Max_speed;
              speed_R = 0.3;
            }
          } else {
            speed_L = ((y * Max_speed) / SP) * virage;
            speed_R = ((x * Max_speed) / SP) * virage;
          }

          if (speed_R > Max_speed) {
            speed_R = Max_speed;
          }
          if (speed_L > Max_speed) {
            speed_L = Max_speed;
          }
        }
        TFC_SetMotorPWM(speed_L, speed_R);
      }

    }
  }
  return 0;
}
