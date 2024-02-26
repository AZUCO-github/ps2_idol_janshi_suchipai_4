#include	<windows.h>
#include	<stdio.h>

extern	unsigned long	ps2_lzss(	unsigned char *,
									unsigned char *,
									unsigned long,
									unsigned long);
extern	HANDLE	NonOverWriteCreateFile(	LPCSTR);
extern	void	decode_img(char *,unsigned long,unsigned char *);
extern	void	decode_tx8(unsigned char *,char *);
extern	void	decode_tx4(unsigned char *,char *);



void	decode_lzsspak(char *name,unsigned long ssrc,unsigned char *src)
{	unsigned char	*buf;
	unsigned long	imx=*(unsigned long *)(src+0);	//Item MaX
	unsigned long	ict;							//Item CounT
	unsigned char	*iad;							//Item ADdress
	unsigned long	isz;							//Item SiZe
	unsigned long	usz;							//Uncompress SiZe
	char			imgname[MAX_PATH];



	buf=new unsigned char[0x2000000];				//PS2 main memory 32MB

	for(ict=0;ict<imx;ict++){
		iad=					src+ *(unsigned long *)(src+(ict+1)*4) +4;
		isz=*(unsigned long *)(	src+ *(unsigned long *)(src+(ict+1)*4)	);

		usz=ps2_lzss(iad,buf,isz,0x2000000);

		switch((*(unsigned long *)buf)&0x00ffffff){
			case	0x00544f4d:	//MOT
				break;

			case	0x00385854:	//TX8
				sprintf(imgname,"%s_%04X",name,ict);
				decode_tx8(buf,imgname);
				break;

			case	0x00345854:	//TX4
				sprintf(imgname,"%s_%04X",name,ict);
				decode_tx4(buf,imgname);
				break;

			default:
				if(	(	(	*(unsigned short *)(buf+0)+
							*(unsigned short *)(buf+2)	)==
							*(unsigned short *)(buf+4)		)	&&
						(	*(unsigned short *)(buf+0)	!=0	)		){
					sprintf(imgname,"%s_%04X",name,ict);
					decode_img(imgname,usz,buf);
					break;
				}
		}
	}
	delete[]	buf;
}
