#include <stdio.h>
#include <scws.h>
#define SCWS_PREFIX     "/usr/local/scws"

main(int argc, char *argv[])
{
   // char *text;
    
    if (argc == 2) {
	if (!strcmp(argv[1], "--text")) {
	    printf("text\n\n");
 	    

        } else if(!strcmp(argv[1], "--version")) {
	    printf("version 0.1\n\n");
	}
    } else if (argc == 3) {
	if (!strcmp(argv[1], "--file")){
	    printf("file: %s\n\n",argv[2]);

        }else if(!strcmp(argv[1], "--url")) {
	    printf("url: %s\n\n",argv[2]);

	}
    }


  scws_t s;
  scws_res_t res, cur;
  char *text = "（中央社記者陳淑芬台中28日電）洪仲丘姐姐洪慈庸今天晚間回應外界批評表示，不要求社會都站在家屬角度看待全案，但盼能體諒被害人家屬心情，給多點空間。";

  if (!(s = scws_new())) {
    printf("ERROR: cann't init the scws!\n");
    exit(-1);
  }
  scws_set_charset(s, "utf8");
  scws_set_dict(s, "/usr/local/scws/etc/dict_cht.utf8.xdb", SCWS_XDICT_XDB); //traditional Chinese
  scws_set_rule(s, "/usr/local/scws/etc/rules_cht.utf8.ini");
  scws_set_ignore(s, 1); //set ignore punctuation

  scws_send_text(s, text, strlen(text));
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
