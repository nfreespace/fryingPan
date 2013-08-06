#include <stdio.h>
#include <scws.h>
#include <md5.h>
#define SCWS_PREFIX     "/usr/local/scws"
#define PAGESIZE 512
#define MAXSIZE 512*256
static const char *const usage = "\
Usage:\n\
    simpleSeg --demo                 # run the demo\n\
    simpleSeg --help                 # print help info of the package\n\
    simpleSeg --version              # print the version of the package\n\
    simpleSeg --fileall              # do segmentation on all files in \\news directory\n\
    simpleSeg --file <file_path>     # do segmentation on a file\n\
    simpleSeg --text <String>        # do segmentation on a string\n\
";


int readfile(char *file_name, char **strToReturn)
{

  
  size_t bufSiz = 0;
  char message[MAXSIZE] = "\0";
char buffer[PAGESIZE];
  FILE *in;

  printf("begin read file:%s\n",file_name);
  in = fopen(file_name, "r");
  int position = 0;
  if (in)
  {
 	while(fgets(buffer,PAGESIZE,in)){
		strcat(message,buffer);
		position = position + sizeof(buffer)-sizeof(char)*4;
		printf("%s\n", buffer);
	} 
        fclose(in);
}
 printf("whole txt: %s\n",message);
 *strToReturn = message;
  return 0;
}

main(int argc, char *argv[])
{
    char *text;
    
    char *demo  = "（中央社記者陳淑芬台中28日電）洪仲丘姐姐洪慈庸今天晚間回應外界批評表示洪仲丘，不要求社會都站在家>屬角度看待全案，但盼能體諒被害人家屬心情，給多點空間。";
    if (argc == 2) {
	if (!strcmp(argv[1], "--demo")) {
	    printf("demo\n\n");
 	    text = demo;	    

        } else if(!strcmp(argv[1], "--version")) {
	    printf("version 0.1\n\n");
	}else if(!strcmp(argv[1], "--fileall")) {
	/*	for
		readfile();
        */  
	  printf("fileall\n\n");
        }else if(!strcmp(argv[1], "--help")){
		puts(usage);
		return;
	}else{
		puts(usage);
		return;
	}
    } else if (argc == 3) {
	if (!strcmp(argv[1], "--file")){
	    printf("\nFile: %s\n\n",argv[2]);
	     readfile(argv[2],&text);
		printf("File read end\n");
		printf("in main: %s\n",text);
        }else if(!strcmp(argv[1], "--text")) {
	    printf("text: %s\n\n",argv[2]);
	    text = argv[2];
	}else{
		puts(usage);
		return;
	}
	
    }else {
	puts(usage);
	return;
	}



  scws_t s;
  scws_res_t res, cur;
  
  if (!(s = scws_new())) {
    printf("ERROR: cann't init the scws!\n");
    exit(-1);
  }
  scws_set_charset(s, "utf8");
  scws_set_dict(s, "/usr/local/scws/etc/dict_cht.utf8.xdb", SCWS_XDICT_XDB); //traditional Chinese
  scws_set_rule(s, "/usr/local/scws/etc/rules_cht.utf8.ini");
  scws_set_ignore(s, 1); //set ignore punctuation

  scws_send_text(s, text, strlen(text));

  scws_top_t topres, topcur; 
  topres = topcur = scws_get_tops(s,50,"an,j,n,nr,ns,nt,nz,Ng,s,vn");
       
  md5_state_t state; 
  md5_byte_t digest[16];
  char hex_output[16*2 + 1];
  int di;  

  int counter = 1;
    while (topcur != NULL)
    {
      printf("%dth WORD: %s  weight:%f  time:%d  attr:%s \n", 
	counter,topcur->word, topcur->weight, topcur->times, topcur->attr);

  //      md5_state_t state;
  //      md5_byte_t digest[16];
  //      char hex_output[16*2 + 1];
  //      int di;

        md5_init(&state);
        md5_append(&state, (const md5_byte_t *)(topcur->word), strlen(topcur->word));
        md5_finish(&state, digest);
        for (di = 0; di < 16; ++di){
            sprintf(hex_output + di * 2, "%02x", digest[di]);

        }
	printf("MD5 of the word is: %s\n",hex_output);	
      topcur = topcur->next;
	counter++;
    }
    scws_free_tops(topres);
  
printf("\n=============================================================\n");  

  while (res = cur = scws_get_result(s))
  {
    while (cur != NULL)
    {
      printf("WORD: %.*s/%s (IDF = %4.2f)\n", cur->len, text+cur->off, cur->attr, cur->idf);
      cur = cur->next;
    }
    scws_free_result(res);
  }
  scws_free(s);
}
