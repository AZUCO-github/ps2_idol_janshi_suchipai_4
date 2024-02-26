#include	<windows.h>
#include	<stdio.h>

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
extern	void	decode_palette_4bit(unsigned char *);
		void	decode_tx4(unsigned char *,char *);



void	decode_tx4(unsigned char *src,char *name)
{	unsigned long		tul1,tul2;
	unsigned long		sx,sy,sz;
	unsigned char		*pal;
	unsigned char		*gfx;
	unsigned char		*pos;		//使わないけど
	unsigned char		*bmp;
	char				tfn[0x100];
	HANDLE				hbmp;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;

	sx =     *(unsigned short *)(src+0x04);	//テクスチャなのでX=Y
	sy = sx;
	sz = sx * sy;
	pal=src+ *(unsigned long  *)(src+0x08);
	gfx=src+ *(unsigned long  *)(src+0x0c);
	pos=src+ *(unsigned long  *)(src+0x10);
	bmp=new unsigned char[sx*sy*4];

	if(sx==0)	return;
	if(sy==0)	return;

	sprintf(tfn,"%s.bmp",name);
	hbmp=NonOverWriteCreateFile((LPCSTR)tfn);

	*((char *)&bf.bfType+0)='B';
	*((char *)&bf.bfType+1)='M';
	bf.bfSize=			sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER)+
						sz;								//image
	bf.bfOffBits=		sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER);
	bi.biSize=			sizeof(BITMAPINFOHEADER);
	bi.biWidth=			sx;
	bi.biHeight=		sy;
	bi.biPlanes=		1;
	bi.biBitCount=		32;
	bi.biCompression=	0;
	bi.biSizeImage=		0;
	bi.biXPelsPerMeter=	0;
	bi.biYPelsPerMeter=	0;
	bi.biClrUsed=		0;
	bi.biClrImportant=	0;

		WriteFile(hbmp,&bf,sizeof(BITMAPFILEHEADER),&tul1,NULL);
		WriteFile(hbmp,&bi,sizeof(BITMAPINFOHEADER),&tul1,NULL);

	decode_palette_4bit(pal);
	for(tul1=0;tul1<sz;tul1+=2){						//PAL to RGBA
		memcpy(bmp+tul1*4+4,pal+((gfx[tul1>>1]>>4)&0x0f)*4,4);
		memcpy(bmp+tul1*4+0,pal+((gfx[tul1>>1]>>0)&0x0f)*4,4);	}

	for(tul2=((sz-sx)*4);tul2<(sz*4);tul2-=(sx*4))
		WriteFile(hbmp,bmp+tul2,sx*4,&tul1,NULL);
	CloseHandle(hbmp);
	delete[]	bmp;
}
