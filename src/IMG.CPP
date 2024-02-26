#include	<windows.h>
#include	<stdio.h>

extern	HANDLE	NonOverWriteCreateFile(	LPCSTR);
		void	decode_img(char *,unsigned long,unsigned char *);
extern	void	decode_tx8(unsigned char *,char *);
extern	void	decode_tx4(unsigned char *,char *);
extern	void	decode_lzsspak(char *,unsigned long,unsigned char *);



void	decode_img(char *name,unsigned long ssrc,unsigned char *src)
{	unsigned long	tul1;
	unsigned long	lsa,lss;
	unsigned char	*iad;
	unsigned long	imgk;
	char			imgname[MAX_PATH];

	for(tul1=0;	*(unsigned short *)(src+(tul1)*4+2)!=0;tul1++){
		lsa=	*(unsigned short *)(src+(tul1)*4+0)	*0x800;
		lss=	*(unsigned short *)(src+(tul1)*4+2)	*0x800;

		iad=src+lsa;
		imgk=(*(unsigned long *)iad)&0x00ffffff;

		switch(imgk){
			case	0x00544f4d:	//MOT
				break;

			case	0x00385854:	//TX8
				sprintf(imgname,"%s_%04X",name,tul1);
				decode_tx8(iad,imgname);
				break;

			case	0x00345854:	//TX4
				sprintf(imgname,"%s_%04X",name,tul1);
				decode_tx4(iad,imgname);
				break;

			default:
				if(	(	(	*(unsigned short *)(iad+0)+
							*(unsigned short *)(iad+2)	)==
							*(unsigned short *)(iad+4)		)	&&
						(	*(unsigned short *)(iad+0)	!=0	)		){
					sprintf(imgname,"%s_%04X",name,tul1);
					decode_img(imgname,lss,iad);
					break;
				}
				
				if(	(	*(unsigned long *)(iad+0) *4 +8	)==
						*(unsigned long *)(iad+4)		){
					sprintf(imgname,"%s_%04X",name,tul1);
					decode_lzsspak(imgname,lss,iad);
					break;
				}
		}
	}
}



