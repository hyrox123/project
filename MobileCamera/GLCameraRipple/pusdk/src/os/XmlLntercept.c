#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ossys.h"
#if 1
extern char Replacement_parts[128];

extern char Replacement_parts_preset[128];


char *XmlLntercept(char *SrcBuf)
{
    char *original = SrcBuf ;
	char *src_string = NULL;
	src_string = original;
	char tmp_data[128] = "";
	char *p1 ,*p2;
	int flag1 = 1;
	int flag2 = 1;
	char *dest_string = "@#$";
	char *last_data = (char *)os_sys_malloc(1024, __FILE__, __LINE__);
	if(strstr(src_string , "<bitmapText>") == NULL)
		return SrcBuf ;
	else

		while( flag1 ){		
			p1 = strstr(src_string,"<") ;
			memcpy(&tmp_data[0],p1,strlen("<bitmapText>"));
			if(strcmp(tmp_data,"<bitmapText>") == 0){
				flag1 = 0;
				src_string = p1 + 1;			
				//printf("we found it.\n");
			}else{
				src_string = p1 + 1;
				memset(tmp_data, 0 ,128);
			}
		}

		while( flag2 ){ 	
			p2 = strstr(src_string,"<") ;
			memcpy(&tmp_data[0],p2,strlen("</bitmapText>"));
			if(strcmp(tmp_data,"</bitmapText>") == 0){
				flag2 = 0;
				//printf("string len is %d.\n",p2 - p1- strlen("<bitmapText>"));
			}else{
				src_string = p2 + 1;
				memset(tmp_data, 0 ,128);
			}
		}
	
		memcpy( Replacement_parts , p1+12 , p2-p1-12) ;
		strncat(last_data,original,strlen(original)-strlen(p1)+strlen("<bitmapText>"));
		strncat(last_data,dest_string,strlen(dest_string));
		strncat(last_data,p2,strlen(p2));
	
	return last_data ;
}


char *Contrary_XmlLntercept(char *SrcBuf)
{
	char *original = SrcBuf ;
	char *src_string = NULL;
	src_string = original;
	char tmp_data[128] = "";
	char *p1 ,*p2;
	int flag1 = 1;
	int flag2 = 1;
	char *dest_string = "@#$";
	char last_data[1024] = "";

	memset(tmp_data, 0 ,sizeof(tmp_data));
	while( flag1 ){		
		p1 = strstr(src_string,"<") ;
		memcpy(tmp_data,p1,strlen("<presetName>"));
		if(strcmp(tmp_data,"<presetName>") == 0){
			flag1 = 0;
			src_string = p1 + 1;			
		}else{
			src_string = p1 + 1;
			memset(tmp_data, 0 ,128);
		}
	}

	while( flag2 ){ 	
		p2 = strstr(src_string,"<") ;
		memcpy(&tmp_data[0],p2,strlen("</presetName>"));
		if(strcmp(tmp_data,"</presetName>") == 0){
			flag2 = 0;
			
		}else{
			src_string = p2 + 1;
			memset(tmp_data, 0 ,128);
		}
	}
	
	memcpy(Replacement_parts_preset, p1+12 , p2-p1-12) ;

	strncat(last_data,original,strlen(original)-strlen(p1)+strlen("<presetName>"));
	strncat(last_data,dest_string,strlen(dest_string));
	strncat(last_data,p2,strlen(p2));
	
	return last_data ;	
}
#endif
char *prase_oper_query_storage_pack_fromDate(char *Srcbuf)
{
	char *pbuf = Srcbuf;
	char *datetest = strstr(pbuf,"<fromDate>");
	datetest += strlen("<fromDate>");
	printf("datetest is %s\n",datetest);
	char *tmp = (char *)malloc(30);
	char *test = tmp;
    memset(tmp,0,30);
	memcpy(test, datetest, 19);
	printf("the last xml is %s \n\n" , test) ;
	return test ;
}

char *prase_oper_query_storage_pack_toDate(char *Srcbuf)
{
	char *pbuf = Srcbuf;
	char *datetest = strstr(pbuf,"<toDate>");
	datetest += strlen("<toDate>");
//	printf("datetest is %s\n",datetest);
	char *tmp = (char *)malloc(30);
	char *test = tmp;
      memset(tmp,0,30);
	memcpy(test, datetest, 19);
//	printf("the last xml is %s \n\n" , test) ;
	return test ;
}

