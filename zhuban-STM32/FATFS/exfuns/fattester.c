#include "fattester.h"	 
#include "sdio_sdcard.h"
#include "gprs.h"
#include "usart.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
//#include "string.h"
#define READ_LEN 256
//为磁盘注册工作区
//drv:盘符
//返回值:执行结果
u8 mf_mount(u8 drv)
{
  return f_mount(drv,fs[drv]);
}
//打开路径下的文件
//path:路径+文件名
//mode:打开模式
//返回值:执行结果
u8 mf_open(u8*path,u8 mode)
{
  u8 res;	 
  res=f_open(file,(const TCHAR*)path,mode);//打开文件夹
  return res;
} 
//关闭文件
//返回值:执行结果
u8 mf_close(void)
{
  f_close(file);
  return 0;
}
//读出数据
//len:读出的长度
//返回值:执行结果
/*void send_SD_data(unsigned char num)
{
switch (num)
{
		case 1:
USART_Puts(USART1,fatbuf,br);
break;
		case 2:
//send_3gData(fatbuf,br);
break;
default:
break;
	}
}*/
u8 mf_read(u16 len)
{
  u16 i;
  u8 res=0;
  for(i=0;i<len/READ_LEN;i++)
  {
    res=f_read(file,fatbuf,READ_LEN,&br);
    if(!res)
    {
      USART_Puts(USART1,fatbuf,br);
      if(Is_Connect())
      {
        send_3gData(fatbuf,br);
      }
    }
  }
  if(len%READ_LEN)
  {
    res=f_read(file,fatbuf,len%READ_LEN,&br);
    if(!res)	//读数据出错了
    {
      USART_Puts(USART1,fatbuf,br);
      if(Is_Connect())
      {
        send_3gData(fatbuf,br);
      }
    }	 
  }
  return res;
}
//写入数据
//dat:数据缓存区
//len:写入长度
//返回值:执行结果
u8 mf_write(u8*dat,u16 len)
{			    
  return f_write(file,dat,len,&bw);
}

//打开文件夹
//path:路径
//返回值:执行结果
u8 mf_opendir(u8* path)
{
  return f_opendir(&dir,(const TCHAR*)path);	
}
//打读取文件夹
//返回值:执行结果
u8 mf_readdir(void)
{
  u8 res;
  char *fn;			 
#if _USE_LFN
  fileinfo.lfsize = _MAX_LFN * 2 + 1;
  fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
  res=f_readdir(&dir,&fileinfo);//读取一个文件的信息
  if(res!=FR_OK||fileinfo.fname[0]==0)
  {
    myfree(SRAMIN,fileinfo.lfname);
    return res;//读完了.
  }
#if _USE_LFN
  fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
  fn=fileinfo.fname;;
#endif	
  myfree(SRAMIN,fileinfo.lfname);
  return 0;
}			 

//遍历文件
//path:路径
//返回值:执行结果
u8 mf_scan_files(u8 * path)
{
  FRESULT res;	  
  char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
  fileinfo.lfsize = _MAX_LFN * 2 + 1;
  fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
  
  res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
  if (res == FR_OK) 
  {	
    //	printf("\r\n"); 
    while(1)
    {
      res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
      if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
      //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
      fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
      fn = fileinfo.fname;
#endif	                                              /* It is a file. */
    } 
  }	  
  myfree(SRAMIN,fileinfo.lfname);
  return res;	  
}
//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
u32 mf_showfree(u8 *drv)
{
  FATFS *fs1;
  u8 res;
  DWORD fre_clust=0, fre_sect=0, tot_sect=0;
  //得到磁盘信息及空闲簇数量
  res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
  if(res==0)
  {											   
    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512
    tot_sect*=fs1->ssize/512;
    fre_sect*=fs1->ssize/512;
#endif	  
  }
  return fre_sect;
}		    
//文件读写指针偏移
//offset:相对首地址的偏移量
//返回值:执行结果.
u8 mf_lseek(u32 offset)
{
  return f_lseek(file,offset);
}
//读取文件当前读写指针的位置.
//返回值:位置
u32 mf_tell(void)
{
  return f_tell(file);
}
//读取文件大小
//返回值:文件大小
u32 mf_size(void)
{
  return f_size(file);
} 
//创建目录
//pname:目录路径+名字
//返回值:执行结果
u8 mf_mkdir(u8*pname)
{
  return f_mkdir((const TCHAR *)pname);
}
//格式化
//drv:盘符
//mode:模式
//au:簇大小
//返回值:执行结果
u8 mf_fmkfs(u8 drv,u8 mode,u16 au)
{
  return f_mkfs(drv,mode,au);//格式化,drv:盘符;mode:模式;au:簇大小
}
//删除文件/目录
//pname:文件/目录路径+名字
//返回值:执行结果
u8 mf_unlink(u8 *pname)
{
  return  f_unlink((const TCHAR *)pname);
}

//修改文件/目录名字(如果目录不同,还可以移动文件哦!)
//oldname:之前的名字
//newname:新名字
//返回值:执行结果
u8 mf_rename(u8 *oldname,u8* newname)
{
  return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}

//从文件里面读取一段字符串
//size:要读取的长度
void mf_gets(u16 size)
{
  TCHAR* rbuf;
  rbuf=f_gets((TCHAR*)fatbuf,size,file);
  if(*rbuf==0)return  ;//没有数据读到
}
//需要_USE_STRFUNC>=1
//写一个字符到文件
//c:要写入的字符
//返回值:执行结果
u8 mf_putc(u8 c)
{
  return f_putc((TCHAR)c,file);
}
//写字符串到文件
//c:要写入的字符串
//返回值:写入的字符串长度
u8 mf_puts(u8*c)
{
  return f_puts((TCHAR*)c,file);
}

