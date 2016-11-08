#ifndef _DHT11_BUS_H_
#define _DHT11_BUS_H_
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
//����IO��ѡ��
#define GPIO           GPIOA
#define RCC_IO         RCC_APB2Periph_GPIOA
#define PIN            GPIO_Pin_1
//��ȡIO����
#define Read           GPIO_ReadInputDataBit(GPIO,PIN)
//a=1:����ߵ�ƽ
//a=0:����͵�ƽ
#define OutPut(a)  if(a)\
						GPIO_SetBits(GPIO,PIN); \
					else \
						GPIO_ResetBits(GPIO,PIN)			

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<															
/*������ʾ��ʽ��
*             ʪ�ȣ�byte4.byte3=������%RH��
*             �¶ȣ�byte2.byte1=�������棩
*             У�飺byte4+byte3+byte2+byte1=���¶�+ʪ�ȣ�
*����һ��һά����ָ�룬
*������ı�������ֵ
*             ֵһ��ʪ������
*             ֵ����ʪ��С��
*             ֵ�����¶�����
*             ֵ�ģ��¶�С��
*             ֵ�壺У���
*/															
void DHT11Read(uint8_t*Data_Array);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>			
																						
//�ڲ�����
static void DHT11WriteStart(void);//�����ź�
static uint8_t  Byte_Receive(void);//���ݽ���
static void Input_GPIO_Config(void);//IO�ڽ�������
static void Output_GPIO_Config(void);//IO�ڷ�������
#endif
