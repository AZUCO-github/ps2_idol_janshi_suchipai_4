///////////////////////////////////////////////////////////////////////////////
//	PS2	スーチパイIV converter
//		071017
//			by	AZUCO
///////////////////////////////////////////////////////////////////////////////
#include	<windows.h>
#include	<stdio.h>

extern	void	decode_img	(char *,unsigned long,unsigned char *);
extern	void	decode_lzsspak(char *,unsigned long,unsigned char *);
extern	void	overlay_bmp(unsigned char *,char *);

int	main(int argc,char *argv[])
{
	unsigned char	*buff=NULL;

	HANDLE			hndl;

	unsigned long	file_size;

	unsigned long	tul;
	unsigned long	fnargc=0;

	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];

	printf("PS2 スーチーパイIV Converter\n");
	printf("\t\tProg. By AZUCO\n\n");

	if(argc==1){
		printf("milk <option ...> <file ...>\n");
		printf("<type>\t<option>\n");
		exit(0);
	}

///////	option	///////////////////////////////////////////////////////////////
	for(tul=1;tul<argc;tul++){
		if(argv[tul][0]=='/'){
			if(	argv[tul][1]=='a' &&
				argv[tul][2]=='f'	)
				NULL;
		}else{
			if(fnargc==0)	fnargc=tul;
		}
	}

//strcpy(argv[fnargc],"F:\\DT\\IMG\\K.IMG");

	hndl=CreateFile(argv[fnargc],
					GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hndl==(HANDLE)-1){
		printf("\tError\tsource file not found\n");
		exit(-1);	}

	file_size=GetFileSize(hndl,NULL);

	_splitpath(argv[fnargc],drive,dir,fname,ext);

	if(	strcmp(_strlwr(ext),".img")==0){
		buff=new unsigned char[file_size];
		ReadFile(hndl,buff,file_size,&tul,NULL);
		decode_img(fname,file_size,buff);
		goto esc;	}

	if(	strcmp(_strlwr(ext),".dat")==0){
		buff=new unsigned char[file_size];
		ReadFile(hndl,buff,file_size,&tul,NULL);
		decode_lzsspak(fname,file_size,buff);
		goto esc;	}

	if(	strcmp(_strlwr(ext),".bmp")==0){
		buff=new unsigned char[file_size];
		ReadFile(hndl,buff,file_size,&tul,NULL);
		overlay_bmp(buff,argv[fnargc]);
		goto esc;	}

esc:
	CloseHandle(hndl);
	if(buff!=NULL){	delete[]	buff;	buff=NULL;	}
}
