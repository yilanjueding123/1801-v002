#include "program_03.h"
#include "application.h"

#if LED_1_CONFIG == USED
void led_1_on(void)
{
  	#if LED_1_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_1_PIN, DATA_HIGH);
  	#else
  	gpio_write_io(LED_1_PIN, DATA_LOW);
	#endif
}
void led_1_off(void)
{
  	#if LED_1_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_1_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_1_PIN, DATA_HIGH);
	#endif
}
#endif

#if LED_2_CONFIG == USED
void led_2_on(void)
{
  	#if LED_2_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_2_PIN, DATA_HIGH);
  	#else
  	gpio_write_io(LED_2_PIN, DATA_LOW);
	#endif
}
void led_2_off(void)
{
  	#if LED_2_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_2_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_2_PIN, DATA_HIGH);
	#endif
}
#endif

#if LED_3_CONFIG == USED
void led_3_on(void)
{
  	#if LED_3_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_3_PIN, DATA_HIGH);
  	#else
  	gpio_write_io(LED_3_PIN, DATA_LOW);
	#endif
}
void led_3_off(void)
{
  	#if LED_3_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_3_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_3_PIN, DATA_HIGH);
	#endif
}
#endif

#if LED_4_CONFIG == USED
void led_4_on(void)
{
  	#if LED_4_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_4_PIN, DATA_HIGH);
  	#else
  	gpio_write_io(LED_4_PIN, DATA_LOW);
	#endif
}
void led_4_off(void)
{
  	#if LED_4_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_4_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_4_PIN, DATA_HIGH);
	#endif
}
#endif
//===============================================================================================
//===============================================================================================
