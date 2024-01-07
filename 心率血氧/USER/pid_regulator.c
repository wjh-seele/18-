#include "pid_regulator.h"


void PID_Parameter_Init (void)
{
  /************PID_Left_Speed Regulator members*************/
  
  PID_Left_Speed.Kp = 0.0f;		  //0.3
  PID_Left_Speed.Ki = 0.0f; 		  //0.01
  PID_Left_Speed.Kd = 0.0f;		  //1
  PID_Left_Speed.PreviousError = 0;
  
  PID_Left_Speed.Lower_Limit_Output= -6800;   //电机输出限幅
  PID_Left_Speed.Upper_Limit_Output= 6800;    //电机输出限幅
  PID_Left_Speed.Lower_Limit_Integral = -6800;	//积分限幅
  PID_Left_Speed.Upper_Limit_Integral = 6800;
  PID_Left_Speed.Integral = 0;  // reset integral value 

  /**********END PID_Left_Speed Regulator members*********/

   /************PID_Right_Speed Regulator members*************/
  
  PID_Right_Speed.Kp = 0.f;		  //0.3
  PID_Right_Speed.Ki = 0.0f; 		  //0.01
  PID_Right_Speed.Kd = 0.0f;		  //1
  PID_Right_Speed.PreviousError = 0;
  
  PID_Right_Speed.Lower_Limit_Output= -6800;   //电机输出限幅
  PID_Right_Speed.Upper_Limit_Output= 6800;    //电机输出限幅
  PID_Right_Speed.Lower_Limit_Integral = -6800;	//积分限幅
  PID_Right_Speed.Upper_Limit_Integral = 6800;
  PID_Right_Speed.Integral = 0;  // reset integral value 

  /**********END PID_Right_Speed Regulator members*********/




  
}

/**
* @Description: PID计算输出
* @param Reference - 目标值
* @param resentFeedback - 目前反馈值（测量值）
* @return PID计算后的输出值
*/
float PID_Regulator(float Reference, float PresentFeedback, _pid *PID_Struct)
{
  float Error;
  float output; 
  /* 偏差计算  */
  Error= (Reference - PresentFeedback);
 
  /* 积分计算  */
  if (PID_Struct->Ki == 0)
  {
    PID_Struct->Integral = 0;
  }
  else
  { 
    PID_Struct->Integral += PID_Struct->Ki * Error;
    if (PID_Struct->Integral > PID_Struct->Upper_Limit_Integral)
    {
      PID_Struct->Integral = PID_Struct->Upper_Limit_Integral;
    }
    else if (PID_Struct->Integral < PID_Struct->Lower_Limit_Integral)
          { 
            PID_Struct->Integral = PID_Struct->Lower_Limit_Integral;
          }
          else
          {          
          }
  }

  output= (PID_Struct->Kp * Error +
  		   PID_Struct->Integral +
		   PID_Struct->Kd*(Error - PID_Struct->PreviousError) );
   
  PID_Struct->PreviousError = Error;    // store value 

    if (output >= PID_Struct->Upper_Limit_Output)
      {
      return(PID_Struct->Upper_Limit_Output);		  			 	
      }
    else if (output < PID_Struct->Lower_Limit_Output)
      {
      return(PID_Struct->Lower_Limit_Output);
      }
    else 
      {
        return output; 		
      }
}		   
