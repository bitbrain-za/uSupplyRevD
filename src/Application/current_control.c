#define CURRENT_CONTROL_C_

/*
 * current_control.c
 *
 * Created: 2017/01/04 10:55:44 AM
 *  Author: Philip
 */ 

 #include "../system.h"

 /*
    ____       _____       _ __  _
   / __ \___  / __(_)___  (_) /_(_)___  ____  _____
  / / / / _ \/ /_/ / __ \/ / __/ / __ \/ __ \/ ___/
 / /_/ /  __/ __/ / / / / / /_/ / /_/ / / / (__  )
/_____/\___/_/ /_/_/ /_/_/\__/_/\____/_/ /_/____/

*/

/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

U16 interpret_current(U16 raw_value);

/*
   ________      __          __   _    __           _       __    __
  / ____/ /___  / /_  ____ _/ /  | |  / /___ ______(_)___ _/ /_  / /__  _____
 / / __/ / __ \/ __ \/ __ `/ /   | | / / __ `/ ___/ / __ `/ __ \/ / _ \/ ___/
/ /_/ / / /_/ / /_/ / /_/ / /    | |/ / /_/ / /  / / /_/ / /_/ / /  __(__  )
\____/_/\____/_.___/\__,_/_/     |___/\__,_/_/  /_/\__,_/_.___/_/\___/____/

*/

/*
    ______     __                        __   ______                 __  _
   / ____/  __/ /____  _________  ____ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / __/ | |/_/ __/ _ \/ ___/ __ \/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /____>  </ /_/  __/ /  / / / / /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/_/|_|\__/\___/_/  /_/ /_/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

void v_current_FSM(bool reset)
{
  SYS_MESSAGE message;
  static TIMER_HANDLE timer_current_limit_holdoff;
  
  if(reset)
  {
    b_queue_init(&queue_current_control, sizeof(SYS_MESSAGE), 1);
    timer_current_limit_holdoff = timer_new(1000);
  }  

  if(b_queue_read(&queue_current_control, &message))
  {
    eus_output_current_ua = interpret_current(message.value);
  }

  if(IsCurrentLimitEnabled() && (eus_output_current_ua >= eus_ram_current_setting))
  {
    timer_reset(timer_current_limit_holdoff);
    CLIM_ENABLE();
  }
  else if(timer_expired(timer_current_limit_holdoff))
  {
    CLIM_DISABLE();
  }
}

/*
    __                     __   ______                 __  _
   / /   ____  _________ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / /   / __ \/ ___/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /___/ /_/ / /__/ /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/\____/\___/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

U16 interpret_current(U16 raw_value)
{
  /* 1V / AMP 
  
  Vref = 3.3/1.48
  
  ADCraw = Vin*MAX/Vref
         = Vin * 0x0FFF * (1.48 / 3.3)
  
  Vin = ADC * Vref / MAX
  
  Iin (uA) = ADC * 544.5


  Iin (uA) = (ADC * 100*Vref/MAX) * 10000
 
  */

  U32 temp = raw_value * 1000;
  temp *= 330;  /* Boundary -> 1 351 350 000 */
  temp /= 148;
  temp /= 0xFFF;

  /* boundary -> 2230 */
  return (U16)temp;
}