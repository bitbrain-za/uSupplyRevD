/*
 * cdc.c
 *
 * Created: 2016/12/23 12:24:10 PM
 *  Author: philb
 */ 
#include "../system.h"
#include "cdc.h"

#include "conf_board.h"
#include "conf_usb.h"

static volatile bool main_b_cdc_enable = false;

void main_suspend_action(void)
{
}

void main_resume_action(void)
{
}

void main_sof_action(void)
{
}

bool main_cdc_enable(uint8_t port)
{
	UNUSED(port);
  main_b_cdc_enable = true;
  return true;
}

void main_cdc_disable(uint8_t port)
{
	UNUSED(port);
  main_b_cdc_enable = false;
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	UNUSED(port);
  if (b_enable) 
  {
  }
  else
  {
  }
}

#ifdef USB_DEVICE_LPM_SUPPORT
void main_suspend_lpm_action(void)
{
}

void main_remotewakeup_lpm_disable(void)
{
}

void main_remotewakeup_lpm_enable(void)
{
}
#endif


void cdc_rx_notify(uint8_t port)
{
  static console_message rx_msg;
  static char *ptr_str = rx_msg.command;
  char ch;

	UNUSED(port);
	
#if 1
  while (udi_cdc_is_rx_ready()) 
  {
	  ch = udi_cdc_getc() & 0x7f;

    if(ch == '\r')
    {
      b_queue_send(&queue_cdc_rx, &rx_msg);
      ptr_str = rx_msg.command;
      memset(&rx_msg, 0, sizeof(console_message));
    }
    else if(ch == ' ')
    {
      ptr_str = rx_msg.param;
    }
    else
    {
      *ptr_str++ = ch;
    }
  }
#else
  while (udi_cdc_is_rx_ready()) 
  {
	  ch = udi_cdc_getc() & 0x7f;
    if (!udi_cdc_is_tx_ready()) 
    {
      /* Fifo full */
      udi_cdc_signal_overrun();
    } 
    else 
    {
      udi_cdc_putc(ch);
    }
  }
#endif
}