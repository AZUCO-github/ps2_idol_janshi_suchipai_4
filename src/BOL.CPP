/*
	似非アルファ付きBMPオーバーレイ まじめに計算版(060122)

	RGBA
		A	0x80	不透過
			0x00	　透過
*/
#include	<windows.h>
#include	<stdio.h>

extern	HANDLE				NonOverWriteCreateFile(LPCSTR);

		void	overlay_bmp(unsigned char *,char *);

void	overlay_bmp(unsigned char *ap,char *an)
{	HANDLE				bh;
	HANDLE				ch;
	BITMAPFILEHEADER	*tf;
	BITMAPINFOHEADER	*ti;

	unsigned char		*bp;
	unsigned char		*cp;
	char				*tp;
	unsigned short		ta[4];
	unsigned short		tb[4];

	unsigned long		ax,ay,az;
	unsigned long		bx,by,bz;
	unsigned long		cx,cy,cz;
	unsigned long		dx,dy;
	unsigned long		tx,ty,tz;

	unsigned long		tul1;

	char				bn[MAX_PATH];
	char				cn[MAX_PATH];
	char				tn[MAX_PATH];

	char				af[MAX_PATH];
	char				bf[MAX_PATH];

	char				drive[MAX_PATH];
	char				dir[MAX_PATH];
	char				fname[MAX_PATH];
	char				ext[MAX_PATH];



//	strcpy(tn,GetCommandLine());
//	strcpy(an,strrchr(tn,' ')+1);
//	tp=strrchr(an,'"');	if(tp!=NULL)	*tp=0;
//	tp=strrchr(an,'>');	if(tp!=NULL)	*tp=0;
	_splitpath(an,drive,dir,fname,ext);
	strcpy(af,fname);

	strcpy(tn,GetCommandLine());

	tp=strstr(tn,"/x");
	if(tp!=NULL)	sscanf(tp,"/x%d\n",&dx);	
	else			dx=0;

	tp=strstr(tn,"/y");
	if(tp!=NULL)	sscanf(tp,"/y%d\n",&dy);
	else			dy=0;

	tp=strstr(tn,"/b");
	if(tp==NULL){
		printf("\tError\tcan't found add bmp file\n");
		exit(-1);	}
	*strrchr(tp+2,' ')=0;
	strcpy(bn,tp+2);

	_splitpath(bn,drive,dir,fname,ext);
	if(	(strcmp(ext,".bmp")==0)	&&
		(strcmp(ext,".BMP")==0)		){
		printf("\tError\tcan't found add bmp file\n");
		exit(-1);	}
	strcpy(bf,fname);

	bh=CreateFile(bn,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(bh==(HANDLE)-1){
		printf("\tError\tcan't open add bmp file\n");
		exit(-1);	}

	bz=GetFileSize(bh,NULL);
	bp=new unsigned char[bz];
	tp=(char *)bp;
	ReadFile(bh,bp,bz,&tul1,NULL);
	CloseHandle(bh);



	tf=(BITMAPFILEHEADER *)bp;
	ti=(BITMAPINFOHEADER *)(bp+sizeof(BITMAPFILEHEADER));
	bx=ti->biWidth;
	by=ti->biHeight;

	tf=(BITMAPFILEHEADER *)ap;
	ti=(BITMAPINFOHEADER *)(ap+sizeof(BITMAPFILEHEADER));
	ax=ti->biWidth;
	ay=ti->biHeight;

	cx=max(ax,bx);
	cy=max(ay,by);

	cp=new unsigned char[cx*cy*4];

	sprintf(cn,"%s-%s.bmp",af,bf);
	ch=NonOverWriteCreateFile((LPCSTR)cn);
	if(ch==(HANDLE)-1){
		printf("\tError\tcan't create bmp file\n");
		exit(-1);	}

	tf->bfSize		=sizeof(BITMAPFILEHEADER)+
					 sizeof(BITMAPINFOHEADER)+
					 cx*cy;						//image
	ti->biWidth		=cx;
	ti->biHeight	=cy;
	WriteFile(ch,tf,sizeof(BITMAPFILEHEADER),&tul1,NULL);
	WriteFile(ch,ti,sizeof(BITMAPINFOHEADER),&tul1,NULL);

	ap+=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bp+=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	for(ty=0;ty<cy;ty++){
		for(tx=0;tx<cx;tx++){
			if(	(ty>=dy)		&&
				(tx>=dx)		&&
				((ty-dy)<by)	&&
				((tx-dx)<bx)	){
				bz=(((ty-dy)*bx)+(tx-dx))*4;
				tb[0]=*(bp+bz+3);
				tb[1]=*(bp+bz+2);
				tb[2]=*(bp+bz+1);
				tb[3]=*(bp+bz+0);
			}else{
				tb[0]=0;
				tb[1]=0;
				tb[2]=0;
				tb[3]=0;			}

			if((ty<ay)&&(tx<ax)){
				az=((ty*ax)+tx)*4;
				ta[0]=0x80-tb[0];
				ta[1]=*(ap+az+2);
				ta[2]=*(ap+az+1);
				ta[3]=*(ap+az+0);
			}else{
				ta[0]=0x80-tb[0];
				ta[1]=0;
				ta[2]=0;
				ta[3]=0;			}

			tb[1]*=tb[0];
			tb[2]*=tb[0];
			tb[3]*=tb[0];

			ta[1]*=ta[0];
			ta[2]*=ta[0];
			ta[3]*=ta[0];

			ta[1]+=tb[1];
			ta[2]+=tb[2];
			ta[3]+=tb[3];

			ta[1]/=0x80;
			ta[2]/=0x80;
			ta[3]/=0x80;

			cz=((ty*cx)+tx)*4;
			*(cp+cz+3)=tb[0];
			*(cp+cz+2)=ta[1];
			*(cp+cz+1)=ta[2];
			*(cp+cz+0)=ta[3];
		}
	}

	WriteFile(ch,cp,cx*cy*4,&tul1,NULL);
	CloseHandle(ch);

	if(tp!=0)	delete[]	tp;
	if(cp!=0)	delete[]	cp;
}
