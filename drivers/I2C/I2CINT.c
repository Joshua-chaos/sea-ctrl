/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:I2CINT.c
	version:v1.0
	date:2013/10/30  y/m/d
	author��jiaxuechao
*/
//head files
#include  "I2CINT.h"

/* �������ں�I2C�жϴ�����Ϣ��ȫ�ֱ��� */
volatile uint8 	I2C_sla;				//I2C�����ӵ�ַ 					
volatile uint32	I2C_suba;				// I2C�����ڲ��ӵ�ַ
volatile uint8 	I2C_suba_num;		// I2C�ӵ�ַ�ֽ���
volatile uint8 	*I2C_buf;       // ���ݻ�����ָ��
volatile uint32 I2C_num;				// Ҫ��ȡ/д������ݸ���
volatile uint8 	I2C_end;				// I2C���߽�����־��������������1
volatile uint8 	I2C_suba_en;		
/* 	�ӵ�ַ���ơ�
0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
1--��ȡ����
2--д����*/

/* description:	�����ʱ���ṩ��I2C���ߵȴ���ʱʹ��
//		function:	Wait_I2c_End
//			@param: dly		��ʱ������ֵԽ����ʱԽ��
//return value:	
*/
uint8  Wait_I2c_End(uint32  dly)
{  uint32  i;

   if( I2C_end==1 ) return (1);
   for(; dly>0; dly--) 
      for(i=0; i<5000; i++)
      {
        if( I2C_end==1 ) return (1);
      }
   return (0);   
}

/* description:	�����ӵ�ַ��������1�ֽ����ݡ� 
//		function:	ISendByte()
//			@param: sla		������ַ
//						  dat		Ҫ���͵�����
//return value:	����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
//							ʹ��ǰҪ��ʼ����I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ
*/
uint8  ISendByte(uint8 sla, uint8 dat)
{  /* �������� */
   I2C_sla     = sla;		// д������������ַ
   I2C_buf     = &dat;		// �����͵�����
   I2C_num     = 1;			// ����1�ֽ�����
   I2C_suba_en = 0;		 	// ���ӵ�ַ
   I2C_end     = 0;
   
   I21CONCLR = 0x2C;
   I21CONSET = 0x60;             // ����Ϊ����������������
   
   return( Wait_I2c_End(20));
}

/* description:	�����ӵ�ַ������ȡ1�ֽ����ݡ� 
//		function:	IRcvByte()
//			@param: sla		������ַ
//						  dat		�������ݵı���ָ��
//return value:	����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
//							ʹ��ǰҪ��ʼ����I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ
*/
uint8  IRcvByte(uint8 sla, uint8 *dat)
{  /* �������� */
   I2C_sla     = (uint8)(sla+1);		// ��������������ַ
   I2C_buf     = dat;
   I2C_num     = 1;
   I2C_suba_en = 0;			// ���ӵ�ַ
   I2C_end     = 0;
   
   I21CONCLR = 0x2C;
   I21CONSET = 0x60;         // ����Ϊ����������������
   
   return( Wait_I2c_End(20));    
}

/* description:	�����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
//		function:	I2C_ReadNByte()
//			@param: sla			�����ӵ�ַ
//							suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	2��8+X�ṹ	2��˫�ֽڵ�ַ
//							suba		�����ӵ�ַ
//							s			���ݽ��ջ�����ָ��
//							num			��ȡ�ĸ���
//return value:	����ֵΪFALSEʱ��ʾ����ΪTRUEʱ��ʾ������ȷ��
*/
uint8 I2C_ReadNByte (uint8 sla, uint32 suba_type, uint32 suba, uint8 *s, uint32 num)
{
	if (num > 0)	//check num
	{	//parameter set
		if (suba_type == 1)
		{	/* �ӵ�ַΪ���ֽ� */
			I2C_sla     	= (uint8)(sla + 1);	/* �������Ĵӵ�ַ��R=1 	*/
			I2C_suba    	= suba;							/* �����ӵ�ַ 			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
		}
		if (suba_type == 2)
		{	/* �ӵ�ַΪ2�ֽ� */
			I2C_sla     	= (uint8)(sla + 1);	/* �������Ĵӵ�ַ��R=1 	*/
			I2C_suba   	 	= suba;							/* �����ӵ�ַ 			*/
			I2C_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
		}
		if (suba_type == 3)
		{	/* �ӵ�ַ�ṹΪ8+X*/
			I2C_sla			= (uint8)(sla + ((suba >> 7 )& 0x0e) + 1);	/* �������Ĵӵ�ַ��R=1	*/
			I2C_suba		= suba & 0x0ff;				/* �����ӵ�ַ	 		*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ8+x	 	*/
		}
		I2C_buf     = s;										/* ���ݽ��ջ�����ָ�� 	*/
		I2C_num     = num;									/* Ҫ��ȡ�ĸ��� 		*/
		I2C_suba_en = 1;										/* ���ӵ�ַ�� 			*/
		I2C_end     = 0;
		
		/* ���STA,SI,AA��־λ */
		I21CONCLR = 	(1 << 2)|	/* AA 		*/
					(1 << 3)|	/* SI 		*/
					(1 << 5);	/* STA 		*/
		
		/* ��λSTA,����I2C���� */
		I21CONSET = 	(1 << 5)|	/* STA 		*/
					(1 << 6);	/* I2CEN 	*/
		
		/* �ȴ�I2C������� */
		return( Wait_I2c_End(20));
	}
	return (FALSE);
}

/* description:	�����ӵ�ַ����д��N�ֽ�����
//		function:	I2C_WriteNByte()
//			@param: sla			�����ӵ�ַ
//							suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	2��8+X�ṹ	2��˫�ֽڵ�ַ
//							suba		�����ӵ�ַ
//							s			���ݽ��ջ�����ָ��
//							num			��ȡ�ĸ���
//return value:	����ֵΪFALSEʱ��ʾ����ΪTRUEʱ��ʾ������ȷ��
*/
uint8 I2C_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
	if (num > 0)/* �����ȡ�ĸ���Ϊ0���򷵻ش��� */
	{	/* ���ò��� */	
		if (suba_type == 1)
		{	/* �ӵ�ַΪ���ֽ� */
			I2C_sla     	= sla;							/* �������Ĵӵ�ַ	 	*/
			I2C_suba    	= suba;							/* �����ӵ�ַ 			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
		}
		if (suba_type == 2)
		{	/* �ӵ�ַΪ2�ֽ� */
			I2C_sla     	= sla;							/* �������Ĵӵ�ַ 		*/
			I2C_suba   	 	= suba;							/* �����ӵ�ַ 			*/
			I2C_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
		}
		if (suba_type == 3)
		{	/* �ӵ�ַ�ṹΪ8+X */
			I2C_sla			= (uint8)(sla + ((suba >> 7 )& 0x0e));		/* �������Ĵӵ�ַ		*/
			I2C_suba		= suba & 0x0ff;				/* �����ӵ�ַ			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ8+X	 	*/
		}

		I2C_buf     = s;										/* ���� 				*/
		I2C_num     = num;									/* ���ݸ��� 			*/
		I2C_suba_en = 2;										/* ���ӵ�ַ��д���� 	*/
		I2C_end     = 0;
		
		/* ���STA,SI,AA��־λ */
		I21CONCLR = 	(1 << 2)|	/* AA 	*/
					(1 << 3)|	/* SI 	*/
					(1 << 5);	/* STA 	*/
		
		/* ��λSTA,����I2C���� */
		I21CONSET = 	(1 << 5)|	/* STA 	*/
					(1 << 6);	/* I2CEN*/
		
		/* �ȴ�I2C������� */
        return( Wait_I2c_End(20));
	}
	return (FALSE);
}

/* description:	Ӳ��I2C�жϷ������  
//		function:	__irq IRQ_I2C
//			@param: 
//return value:	
//							ע�⴦���ӵ�ַΪ2�ֽڵ������ 
*/
void __irq IRQ_I2C(void)
{	/* ��ȡI2C״̬�Ĵ���I2DAT */
	/* ����ȫ�ֱ��������ý��в��������������־ */
	/* ����ж��߼�,�жϷ��� */
	
	switch (I21STAT & 0xF8)
	{	/* ����״̬�������Ӧ�Ĵ��� */
		case 0x08:	/* �ѷ�����ʼ���� */				/* �����ͺ������ն��� 		*/
			/* װ��SLA+W����SLA+R */
		 	if(I2C_suba_en == 1)/* SLA+R */				/* ָ���ӵ�ַ�� 			*/
		 	{	I21DAT = (unsigned long)I2C_sla & 0xFE; 				/* ��д���ַ 				*/
		 	}
            else	/* SLA+W */
            {  	I21DAT = I2C_sla;        	/* ����ֱ�ӷ��ʹӻ���ַ 	*/
            }
            /* ����SIλ */
            I21CONCLR =	(1 << 3)|			/* SI 						*/
            			(1 << 5);						/* STA 						*/
            break;
            
       	case 0x10:	/*�ѷ����ظ���ʼ���� */ 			/* �����ͺ������ն��� 		*/
       		/* װ��SLA+W����SLA+R */
       		I21DAT = I2C_sla;							/* �������ߺ��ط��ӵ�ַ 	*/
       		I21CONCLR = 0x28;							/* ����SI,STA */
       		break;

		case 0x18:
       	case 0x28:	/* �ѷ���I2DAT�е����ݣ��ѽ���ACK */
       		if (I2C_suba_en == 0)
       		{
	       		if (I2C_num > 0)
	       		{	I21DAT = *I2C_buf++;
	       			I21CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_num--;
	       		}
	       		else	/* û�����ݷ����� */
	       		{		/* ֹͣ���� */
	       		  	I21CONSET = (1 << 4);		/* STO 						*/
	       			I21CONCLR = 0x28;					/* ����SI,STA 				*/
	       		  	I2C_end = 1;						/* �����Ѿ�ֹͣ 			*/
	       		}
       		}
       		
            if(I2C_suba_en == 1)	/* ����ָ����ַ������������������ 				*/
            { 
            	if (I2C_suba_num == 2)
            	{	I21DAT = (unsigned long)((I2C_suba >> 8) & 0xff);
	       			I21CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_suba_num--;
	       			break;	
	       		} 
	       		
	       		if(I2C_suba_num == 1)
	       		{	I21DAT = (unsigned long)(I2C_suba & 0xff);
	       			I21CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_suba_num--;
	       			break;	
	       		}
	       		
            	if (I2C_suba_num == 0)
            	{	I21CONSET = 0x20;
               		I21CONCLR = 0x08;
               		I2C_suba_en = 0;     				/* �ӵ�ַ������ 			*/
               		break;
               	}
            }
            
            if (I2C_suba_en == 2)/* ָ���ӵ�ַд,�ӵ�ַ��δָ��,�����ӵ�ַ 		*/
       		{
       		 	if (I2C_suba_num > 0)
            	{	if (I2C_suba_num == 2)
            		{	I21DAT = (unsigned long)((I2C_suba >> 8) & 0xff);
            			I21CONCLR = 0x28;
            			I2C_suba_num--;
            			break;
            		}
            		if (I2C_suba_num == 1)
            		{	I21DAT    = (unsigned long)(I2C_suba & 0xff);
               			I21CONCLR = 0x28;
               			I2C_suba_num--;
               			I2C_suba_en  = 0;
               			break;
               		}
               	}
             }
       		break;
       		  
       case 0x40:	/* �ѷ���SLA+R,�ѽ���ACK */
       		if (I2C_num <= 1)	/* ��������һ���ֽ� */			
       		{	I21CONCLR = 1 << 2;      				/* �´η��ͷ�Ӧ���ź� 		*/
       		}
       		else
       		{ 	I21CONSET = 1 << 2;						/* �´η���Ӧ���ź� 		*/
       		}
       		I21CONCLR = 0x28;							/* ����SI,STA 				*/
       		break;

       	case 0x20:	/* �ѷ���SLA+W,�ѽ��շ�Ӧ��              */
       	case 0x30:	/* �ѷ���I2DAT�е����ݣ��ѽ��շ�Ӧ��     */
       	case 0x38:	/* ��SLA+R/W�������ֽ��ж�ʧ�ٲ�         */
   		case 0x48:	/* �ѷ���SLA+R,�ѽ��շ�Ӧ��              */
         	I21CONCLR = 0x28;
            I2C_end = 0xFF; 
       		break;   				
	
		case 0x50:	/* �ѽ��������ֽڣ��ѷ���ACK */
			*I2C_buf++ = (uint8)I21DAT;
			I2C_num--;
			if (I2C_num == 1)/* �������һ���ֽ� */
			{  	I21CONCLR = 0x2C;						/* STA,SI,AA = 0 			*/
			}
			else
			{  	I21CONSET = 0x04;						/* AA=1 					*/
			  	I21CONCLR = 0x28;
			}
			break;
		
		case 0x58:	/* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
			*I2C_buf++ = (uint8)I21DAT;     					/* ��ȡ���һ�ֽ����� 		*/
            I21CONSET = 0x10;        					/* �������� 				*/
            I21CONCLR = 0x28;
            I2C_end = 1; 
            break;
            
      	default:
      		break;
	}
   VICVectAddr = 0x00;              					/* �жϴ������ 			*/
}




