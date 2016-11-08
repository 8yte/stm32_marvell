#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include <absacc.h>
extern char Image$$RW_IRAM1$$ZI$$Limit[];
void _mem_init(void)
{
	uint32_t malloc_start, malloc_size;

	malloc_start = (uint32_t) &Image$$RW_IRAM1$$ZI$$Limit; //ȡ�����ڴ���͵�ַ
	malloc_start = (malloc_start + 3) & (~0x03);// ALIGN TO word

//0x20000Ϊ�����ڴ��ܴ�С������MCUʱ��ע��
	malloc_size = 0x20000000 + 0x10000 - malloc_start;

	sys_meminit((void*)malloc_start, malloc_size);
}

void init_rng()
{

}

void init_systick()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Config(SystemFrequency / (OS_TICKS_PER_SEC *10));
}

/**
 *��һ�������
 */
uint32_t get_random(void)
{
	return os_time_get();
}

void gpio_cfg(uint32_t group, uint32_t pin, uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef) mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef*)group, &GPIO_InitStructure); 
}

void driver_gpio_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
}


int check_rst_stat()
{
	uint32_t stat;
	stat = RCC->CSR;
	RCC->CSR = 1L << 24; //�����λ��־

	p_err("reset:");
	if (stat &(1UL << 31))
	// �͹��ĸ�λ
	{
		p_err("low power\n");
	}
	if (stat &(1UL << 30))
	//���ڿ��Ź���λ
	{
		p_err("windw wdg\n");
	}
	if (stat &(1UL << 29))
	//�������Ź���λ
	{
		p_err("indep wdg\n");
	}
	if (stat &(1UL << 28))
	//�����λ
	{
		p_err("soft reset\n");
	}
	if (stat &(1UL << 27))
	//���縴λ
	{
		p_err("por reset\n");
	}
	if (stat &(1UL << 26))
	//rst��λ
	{
		p_err("user reset\n");
	}

	return 0;
}

void driver_misc_init()
{
	driver_gpio_init();
	init_rng();
	init_systick();
}

