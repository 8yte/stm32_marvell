#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "test.h"
#include "dhcpd.h"
#include "webserver.h"
#include <cctype>
#include "delay.h"
#include "DHT11_BUS.h"
#include "oled.h"
#include "bmp.h"
#define VIRSION		"V8.8"

wait_event_t command_event = 0;		//�����ֽ������¼� usart.c					//�����ֽ�����
extern void clear_buf_uart1(void);	//��մ���1����
extern UART_BUF buf_uart1;			//���ڵĽ�������
SOCKET_STATUS i_socket = TCPSERVER;  //Ĭ�ϰ�����TCP������
extern test_create_adhoc(void);
uint8_t Data_Array[5];				//��ʪ������

void choose_socket(char * tmp)
{
	if(strcmp(tmp, "1001")==0) 
	{
		i_socket = TCPSERVER;
		test_close_tcp_server();
		test_tcp_server();
	}
	if(strcmp(tmp, "1003")==0) 
	{
		i_socket = UDPSERVER;
		test_close_tcp_server();
		test_udp_server();
	}
	if(strcmp(tmp, "1000")==0) 
	{
		i_socket = TCPCLIENT;
		test_tcp_unlink();
		test_tcp_link();
	}
	if(strcmp(tmp, "1002")==0) 
	{
		i_socket = UDPCLIENT;
		test_udp_link();
	}
}

void handle_cmd(char *cmd)
{
		p_dbg(cmd);
		if(strcmp(cmd, "AT+")==0)
			show_sys_info();

		if(strcmp(cmd, "AT+INFO")==0)
			show_sys_info();


		if(strcmp(cmd, "AT+SCAN")==0)
			test_scan();


		if(strcmp(cmd, "AT+CONN")==0) 
			test_wifi_connect();

		if(strcmp(cmd, "AT+DISCONN")==0) 
			test_wifi_disconnect();

		if(strcmp(cmd, "AT+AP")==0) 
			test_create_ap();

		if(strcmp(cmd, "AT+TCP")==0) 
			test_tcp_link();

		if(strcmp(cmd, "AT+DISTCP")==0) 
			test_tcp_unlink();

		if(strcmp(cmd, "AT+DNS")==0) 
			test_dns("www.baidu.com");
		
		if(strcmp(cmd, "AT+GETIP")==0) 
			test_auto_get_ip();

		if(strcmp(cmd, "AT+SLID")==0) 
#if USE_MEM_DEBUG	
			mem_slide_check(1);
#endif

		if(strcmp(cmd, "AT+STATUS")==0) 
			test_wifi_get_stats();


		if(strcmp(cmd, "AT+UDP")==0) 
			test_udp_link();


		if(strcmp(cmd, "AT+DBG")==0) 
			switch_dbg();
		
		if(strcmp(cmd, "AT+DISAP")==0) 
			test_stop_ap();

		if(strcmp(cmd, "AT+LIST")==0) 
			test_get_station_list();
		
		if(strcmp(cmd, "AT+ADHOC")==0) 
			test_wifi_join_adhoc();

		if(strcmp(cmd, "AT+DISADHOC")==0) 
			test_wifi_leave_adhoc();

		if(strcmp(cmd, "AT+PWR")==0) 
			test_power_save_enable();
		
		if(strcmp(cmd, "AT+DISPWR")==0) 
			test_power_save_disable();
			

		if(strcmp(cmd, "AT+MONITOR")==0) 
                        ;//monitor_switch();
		
		if(strcmp(cmd, "AT+FULL")==0) 
			test_full_speed_send();
		
		if(strcmp(cmd, "AT+DISFULL")==0) 
			test_full_speed_send_stop();
		
		if(strcmp(cmd, "AT+BACK")==0)  
			switch_loopback_test();
		
		if(strcmp(cmd, "AT+RST")==0) 
			soft_reset();
		

#ifndef FW_OUTSIDE
		if(strcmp(cmd, "AT+FIRMWARE")==0) 
			test_write_firmware_to_spi_flash();
#endif

		if(strcmp(cmd, "AT+TCPSERVER")==0) 
		{
			i_socket = TCPSERVER;
			test_close_tcp_server();
			test_tcp_server();
		}
		if(strcmp(cmd, "AT+UDPSERVER")==0) 
		{
			i_socket = UDPSERVER;
			test_close_tcp_server();
			test_udp_server();
		}
		if(strcmp(cmd, "AT+TCPCLIENT")==0) 
		{
			i_socket = TCPCLIENT;
			test_tcp_unlink();
			test_tcp_link();
		}
		if(strcmp(cmd, "AT+UDPCLIENT")==0) 
		{
			i_socket = UDPCLIENT;
			test_udp_link();
		}

		p_dbg("OK\r\n");
}


void main_thread(void *pdata)
{
	int ret;
	#ifdef DEBUG
	RCC_ClocksTypeDef RCC_ClocksStatus;
	#endif
	
	delay_init(72);				//��ʱ��ʼ��
	
   	OLED_Init();				//��ʼ��OLED  
	OLED_Clear(); 
	OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
	OLED_ShowCHinese(32,6,0);//��
	OLED_ShowCHinese(50,6,1);//˼
	OLED_ShowCHinese(68,6,2);//ͨ
	OLED_ShowCHinese(86,6,3);//��

	DHT11Read(Data_Array);		//��ȡ��ʪ��

	driver_misc_init(); 		//��ʼ��һЩ����(�������)
	usr_gpio_init(); 			//��ʼ��GPIO
    D2(0);						//��

	OSStatInit(); 				//��ʼ��UCOS״̬
	uart1_init(); 				//��ʼ������1

	//��ӡMCU����ʱ��
	#ifdef DEBUG
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	#endif
	p_dbg("SYSCLK_Frequency:%d,HCLK_Frequency:%d,PCLK1_Frequency:%d,PCLK2_Frequency:%d,ADCCLK_Frequency:%d\n", 
		RCC_ClocksStatus.SYSCLK_Frequency, 
		RCC_ClocksStatus.HCLK_Frequency, 
		RCC_ClocksStatus.PCLK1_Frequency, 
		RCC_ClocksStatus.PCLK2_Frequency, 
		0);
#if !defined(CHIP_F103) || defined(FW_OUTSIDE)
	//F103оƬ����ʱ�����û���ⲿflash����Ҫִ������
	m25p80_init(); //��ʼ��SPI-flash
#endif

	init_work_thread();	//��ʼ�������߳�

	ret = SD_Init();    //��ʼ��SDIO�豸
		
	if (ret == 0){
		ret = init_wifi();//��ʼ��WIFIоƬ
	}
	
	if(ret != 0)
	{
		p_err("init wifi faild!");  //wifi��ʼ��ʧ�ܣ�������������
		while(1);
	}

	init_monitor(); //��ʼ��monitorģ��,������init_wifi֮�����

	init_lwip(); //��ʼ��lwipЭ��ջ

	init_udhcpd(); //��ʼ��dhcp������

	enable_dhcp_server(); // ����dhcp������,���������staģʽ,���Բ�����dhcpserver

	misc_init(); //��ʼ��һЩ����(app���)

	command_event = init_event(); //��ʼ��һ���ȴ��¼�

/*
 *��������������Ԥ��Ĺ���
 *����web������, �˿ں�:80
 *����TCP���������˿ں�:1001
 *����AP���ܣ����ƣ�WIFIBORAD_AP�����룺12345678��
 *�����͹��ģ�ģ�鲻�ᷢ�ȣ�
*/
#if	SUPPORT_WEBSERVER
	web_server_init();
#endif
	switch(i_socket)
	{
		case TCPSERVER:
		test_tcp_server();
		break;
	    case TCPCLIENT:
		break;
	    case UDPSERVER:
		test_udp_server();
		break;
	    case UDPCLIENT:
		break;
		default :
		break;
	}
	OLED_Clear();

	D2(1);LED1(1);LED2(1);LED3(1);RELAY(1);			   //��
	
	p_dbg("CSIC����汾:%s", VIRSION);
	p_dbg("CSIC����ʱ��%d.%d S", os_time_get() / 1000, os_time_get() % 1000);
	p_dbg("sensen test");
	p_dbg("��������Э��ջ��ʼ�����");

	test_create_ap();
	test_power_save_enable();  //�����͹��ģ�ģ�鲻�ᷢ�ȣ�
	//test_wifi_connect();

	while (1)
	{
		//�ȴ��û�����
		wait_event(command_event);

		//ִ������
		if(strstr(buf_uart1.buf,"AT+") !=0 )
		{
        	handle_cmd(buf_uart1.buf);
			clear_buf_uart1();
		}

		if(strlen(buf_uart1.buf)>0)
		{
			if(socket_init_ok())
			{
				p_dbg("SEND %s ",buf_uart1.buf);
				switch(i_socket)
				{
					case TCPSERVER:
						test_send(buf_uart1.buf);
						clear_buf_uart1();
					break;
				    case TCPCLIENT:
						test_send(buf_uart1.buf);
						clear_buf_uart1();
					break;
				    case UDPSERVER:
						test_sendto(buf_uart1.buf);
						clear_buf_uart1();
					break;
				    case UDPCLIENT:
						test_send(buf_uart1.buf);
						clear_buf_uart1();
					break;
					default :
					break;
				}
			}
		}
	}
}

int main(void)
{	
	OSInit();
	_mem_init(); //��ʼ���ڴ����

	thread_create(main_thread, 0, TASK_MAIN_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");

	OSStart();
	return 0;
}
